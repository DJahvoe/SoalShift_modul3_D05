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
	printf("Choices\n");
	printf("1. Restock\n");
	printf("2. Exit\n");

	int temp = *value;
	switch(Choices)
	{
		case 1:
		temp++;
		*value = temp;
		break;
		case 2:
		exit(EXIT_SUCCESS);
		break;
	}
	shmdt(value);
	//Reset
	Choices = 0;
}

int main()
{

	pthread_create(&inputid, NULL, &input, NULL);
		while(1)
		{	//Shop Mode
			Shop();
			sleep(1);
		}


}
