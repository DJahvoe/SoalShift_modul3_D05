#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

//Shared memory
#include <sys/ipc.h>
#include <sys/shm.h>

  
int main(int argc, char const *argv[]) {

	//Shared Memory
	key_t key = 42;
	int *stok;

	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	stok = shmat(shmid, NULL, 0);

	*stok = 0;

	printf("%d\n", *stok);
	shmdt(stok);
	shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
