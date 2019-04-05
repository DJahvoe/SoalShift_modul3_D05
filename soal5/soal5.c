#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

//Handle Button Pressed Header
#include <termios.h>

//Function Button Pressed Handler
char getch()
{
	char buf = 0;
	struct termios old = {0};
	if(tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if(tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");
	if(read(0, &buf, 1) < 0)
		perror("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if(tcsetattr(0, TCSADRAIN, &old) < 0)
		perror("tcsetattr ~ICANON");
	return (buf);
}

pthread_t tid[2];
//Mode
int GameOver = 0; //0 = false, 1 = true
int Mode = 0; //Standby : 0, Battle : 1, Shop : 2
int Choices = 0;

//Cooldown
int BathCooldown = 0;

//Pet
char pet_name[50];
	//Status
int Hunger = 200;
int Health = 300;
int Hygiene = 100;
int FoodStock = 10;

//Enemy
int Enemy_Health;

pthread_t inputid;
void* input(void *arg)
{
	int character;
	while(character = getch())
	{
		switch(character)
		{
			case '1':
			Choices = 1;
			break;
			case '2':
			Choices = 2;
			break;
			case '3':
			Choices = 3;
			break;
			case '4':
			Choices = 4;
			break;
			case '5':
			Choices = 5;
			break;
		}
	}
}

pthread_t debuffid[3];
void* debuff(void *arg)
{
	pthread_t id = pthread_self();
	if(pthread_equal(id, debuffid[0]))
	{
		//Health Debuff
		while(1)
		{
			if(Mode != 1)
			{
				sleep(10);
				Health+=5;
			}
		}
	}
	else if(pthread_equal(id, debuffid[1]))
	{
		//Hunger Debuff
		while(1)
		{
			if(Mode != 1)
			{
				sleep(10);
				Hunger-=5;
				if(Hunger <= 0)
				{
					printf("Game Over (Hunger)\n");
					exit(EXIT_SUCCESS);
				}
			}
		}
	}
	else if(pthread_equal(id, debuffid[2]))
	{
		//Hygiene Debuff
		while(1)
		{
			if(Mode != 1)
			{
				sleep(30);
				Hygiene-=10;
				if(Hygiene <= 0)
				{
					printf("Game Over (Dirty)\n");
					exit(EXIT_SUCCESS);
				}
			}
		}
	}
}

void Eat()
{
	if(Hunger < 200)
	{
		if(FoodStock > 0)
		{
			FoodStock--;
			Hunger+=15;
			if(Hunger>200)
			{
				Hunger = 200;
			}
		}
	}
}

pthread_t hygid;
void* Bath(void *arg)
{
	pthread_join(hygid, NULL);
	if(Hygiene < 100 && BathCooldown <= 0)
	{
		Hygiene+=30;
		if(Hygiene > 100)
		{
			Hygiene = 100;
		}
		//Cooldown
		BathCooldown = 20;
		while(BathCooldown > 0)
		{
			sleep(1);
			BathCooldown--;
		}
	}
}

void StandBy()
{
	system("clear");
	printf("Standby Mode\n");
	printf("Name : %s\n", pet_name);
	printf("Health : %d\n", Health);
	printf("Hunger : %d\n", Hunger);
	printf("Hygiene : %d\n", Hygiene);
	printf("Food left : %d\n", FoodStock);

	if(BathCooldown > 0)
	{
		printf("Bath will be ready in %ds\n", BathCooldown);
	}
	else
	{
		printf("Bath is ready\n");
	}
	printf("Choices\n");
	printf("1. Eat\n");
	printf("2. Bath\n");
	printf("3. Battle\n");
	printf("4. Shop\n");
	printf("5. Exit\n");

	switch(Choices)
	{
		case 1:
		//Eat
		Eat();
		break;

		case 2:
		//Bath
		pthread_create(&hygid, NULL, &Bath, NULL);
		break;
		case 3:
		//Battle
		Mode = 1;
		break;
		case 4:
		//Shop
		Mode = 2;
		break;
		case 5:
		exit(EXIT_SUCCESS);
		break;
	}
	//Reset
	Choices = 0;
}

void Battle()
{
	Enemy_Health = 100;
	int flag = 0; //keluar
	while(Health > 0 && Enemy_Health > 0)
	{
		system("clear");
		printf("Battle Mode\n");
		printf("Monster's Health : %d\n", Health);
		printf("Enemy's Health : %d\n", Enemy_Health);
		printf("Choices\n");
		printf("1. Attack\n");
		printf("2. Run\n");

		switch(Choices)
		{
			case 1:
			printf("Attack!\n");
			printf("Enemy Health -20\n");
			Enemy_Health-=20;
			sleep(1);
			printf("Enemy Counter-Attack!\n");
			printf("Monster Health -20\n");
			Health-=20;
			Choices = 0;
			break;

			case 2:
			Mode = 0;
			flag = 1; //run battle
			break;
		}
		if ( flag == 1 ) break;
		sleep(1);
	}
	if(Health <= 0)
	{
		printf("Game Over (Dead)\n");
		exit(EXIT_SUCCESS);
	}
	else if(Enemy_Health <= 0)
	{
		Mode = 0;
	}
}

void Shop()
{
	system("clear");
	printf("Shop Mode\n");
	//Shared Memory
	key_t key = 69;
	int *value;
	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	value = shmat(shmid, NULL, 0);
	printf("Shop food stock : %d\n", *value);
	printf("Your food stock : %d\n", FoodStock);
	printf("Choices\n");
	printf("1. Buy\n");
	printf("2. Back\n");

	int temp = *value;
	switch(Choices)
	{
		case 1:
		if(temp > 0)
		{
			FoodStock++;
			temp--;
			*value = temp;
		}
		break;
		case 2:
		Mode = 0;
		break;
	}
	shmdt(value);
	//Reset
	Choices = 0;
}

int main()
{

	printf("Input your pet name: ");
	scanf("%s", pet_name);

	pthread_create(&inputid, NULL, &input, NULL);
	pthread_create(&(debuffid[0]), NULL, &debuff, NULL);
	pthread_create(&(debuffid[1]), NULL, &debuff, NULL);
	pthread_create(&(debuffid[2]), NULL, &debuff, NULL);

	while(GameOver != 1)
	{
		if(Mode == 0)
		{
			//StandBy Mode
			StandBy();
		}
		else if(Mode == 1)
		{
			//Battle Mode
			Battle();
		}
		else if(Mode == 2)
		{
			//Shop Mode
			Shop();
		}
		sleep(1);
	}

	


}
