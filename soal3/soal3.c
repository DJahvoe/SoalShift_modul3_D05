#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pthread_t tid[2];

int pilihan;
int agmalfunc = 1;
int irajfunc = 1;

int WakeUp_Status = 0;
int Spirit_Status = 100;

void* handlefunc(void *arg)
{
	pthread_t id = pthread_self();
	if(pthread_equal(id,tid[0]))
	{
		printf("Fitur Iraj Ayo Tidur disabled 10s\n");
		irajfunc = 0;
		sleep(10);
		printf("Iraj Enabled\n");
		irajfunc = 1;
	}
	else if(pthread_equal(id,tid[1]))
	{
		printf("Fitur Agmal Ayo Bangun disabled 10s\n");
		agmalfunc = 0;
		sleep(10);
		printf("Agmal Enabled\n");
		agmalfunc = 1;
	}
}

int main(void)
{
	int agmal = 0;
	int iraj = 0;
	int before = 0;
	printf("Menu\n");
	printf("1. All Status\n");
	printf("2. Agmal Ayo Bangun\n");
	printf("3. Iraj Ayo Tidur\n");
	while(WakeUp_Status < 100 && Spirit_Status > 0)
	{
		//printf("Input : ");
		scanf("%d", &pilihan);
		switch(pilihan)
		{
			case 1:
			printf("Agmal WakeUp_Status = %d\n", WakeUp_Status);
			printf("Iraj Spirit_Status = %d\n", Spirit_Status);
			break;
			case 2:
			if(agmalfunc == 1)
			{
				printf("Agmal Ayo Bangun\n");
				//if(before == pilihan)
				//{
					agmal++;
				//}
				//else
				//{
				//	agmal = 1;
				//	iraj = 0;
				//}
				WakeUp_Status += 15;
			}
			break;
			case 3:
			if(irajfunc == 1)
			{
				printf("Iraj Ayo Tidur\n");
				//if(before == pilihan)
				//{
					iraj++;
				//}
				//else
				//{
				//	agmal = 0;
				//	iraj = 1;
				//}
				Spirit_Status -= 20;
			}
			break;
		}
		before = pilihan;


		if(agmal == 3)
		{
			agmal = 0;
			pthread_create(&(tid[0]), NULL, &handlefunc, NULL);
			//pthread_join(tid[0], NULL);
		}
		else if(iraj == 3)
		{
			iraj = 0;
			pthread_create(&(tid[1]), NULL, &handlefunc, NULL);
			//pthread_join(tid[1], NULL);
		}
	}

	if(WakeUp_Status >= 100)
	{
		printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
	}
	else if(Spirit_Status <= 0)
	{
		printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
	}
}
