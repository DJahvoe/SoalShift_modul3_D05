#include    <stdio.h>
#include    <pthread.h>
#include    <string.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <math.h>

int A[10000];
pthread_t tid[50];

struct structt{
	int value;
};

//fungsi cari faktorial dan print nilai
void* cari(void* args){
    unsigned long long nilai=1;
    struct structt*fact =(struct structt*)args;
    //faktorial n
    for(int i=1; i<=fact->value; ++i) {
        nilai *= i;          
    }
    printf("%d!\t= %llu\n",fact->value,nilai);
}



int main(){
  int i = 0, j = 0, hitung,tukar;
  char enter;

  do {
      scanf("%d%c", &A[i], &enter);
      i++;
  } while(enter != '\n');
 
 for (i = 0; A[i]!='\0'; ++i) {
            for (j = i + 1; A[j]!='\0'; ++j) {   
                if (A[i] > A[j]) 
                {
                    tukar =  A[i];
                    A[i] = A[j];
                    A[j] = tukar;
                }
            }     
        }
//cari faktorial dengan thread fungsi cari
  i=0;
  struct structt fac;
  while(A[i]!='\0') {
	fac.value = A[i];
	hitung=A[i];
	pthread_create(&tid[hitung], NULL, &cari,(void *)&fac);
    pthread_join(tid[hitung],NULL);
    i++;
  }
}