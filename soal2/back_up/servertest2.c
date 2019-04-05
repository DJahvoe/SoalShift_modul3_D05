//Socket
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#define PORT 8080

//Thread
#include <pthread.h>
pthread_t tid[2];

//Shared Memory
#include <sys/ipc.h>
#include <sys/shm.h>

void checkstring();

int end = 0;


int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);

int flag;
char buffer[1024] = {0};
char *message = "";

void* opensocketserver(void *arg)
{

	pthread_t id=pthread_self();
	if(pthread_equal(id,tid[0]))
	{
	//printf("Socket Thread\n");
	    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
	        perror("socket failed");
	        exit(EXIT_FAILURE);
	    }
	      
	    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
	        perror("setsockopt");
	        exit(EXIT_FAILURE);
	    }

	    address.sin_family = AF_INET;
	    address.sin_addr.s_addr = INADDR_ANY;
	    address.sin_port = htons( PORT );
	      
	    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
	        perror("bind failed");
	        exit(EXIT_FAILURE);
	    }

		// Server hanya menerima 1 client
	    if (listen(server_fd, 1) < 0) {
	        perror("listen");
	        exit(EXIT_FAILURE);
	    }
		else
		{
			//printf("ServerJual listening. .\n");
		}

	    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
	        perror("accept");
	        exit(EXIT_FAILURE);
	    }
		else
		{
			//printf("Server accepting client. .\n");
		}

		//Communication

		//do
		//{
		while(valread = read( new_socket , buffer, 1024))
		{
			checkstring();
			if(strcmp(buffer,"stop") == 0)
			{
				break;
				end = 1;
			}
			//strcpy(buffer,"");

		}
		//}while(strcmp(buffer,"stop") != 0);
	}
	else if(pthread_equal(id,tid[1]))
	{
	//printf("Print Thread\n");
		while(end != 1)
		{
			sleep(5);
			//Shared memory access
			key_t key = 42;
			int *stok;
			int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
			stok = shmat(shmid, NULL, 0);

			printf("Stok : %d\n", *stok);
			shmdt(stok);
		}
	}
}

void checkstring()
{
	printf("%s\n",buffer );
	message = "\n";
	if(strcmp(buffer, "beli") == 0)
	{

		//message = "Transaksi Berhasil";

		//Shared memory access
		key_t key = 42;
		int *stok;
		int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
		stok = shmat(shmid, NULL, 0);

		int temporary = *stok;
		if(temporary > 0)
		{
			message = "transaksi berhasil";
			temporary--;
			//printf("%d\n", temporary);

			*stok = temporary;
			shmdt(stok);
			//shmctl(shmid, IPC_RMID, NULL);
		}
		else
		{
			message = "transaksi gagal";
		}
	}
	//printf("Send Message\n");
	send(new_socket , message , strlen(message) , 0 );


}


int main(int argc, char const *argv[]) {



	//while(end != 1)
	//{
	pthread_create(&(tid[0]), NULL, opensocketserver, NULL);
	pthread_create(&(tid[1]), NULL, opensocketserver, NULL);

	pid_t child = fork();
	if(child > 0)
	{
		pthread_join(tid[0], NULL);
		kill(child, SIGKILL);
	}
	else if(child == 0)
	{
		pthread_join(tid[1], NULL);
	}

	//}
   return 0;
}
