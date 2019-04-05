#include    <stdio.h>
#include    <string.h>
#include    <pthread.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <string.h>
#include    <math.h>

int A[10000];
pthread_t tid[50];

struct grep10{
	int val;
	int batas_atas;
	int batas_bawah;
};

void* proses1(void* args){
    struct grep10*extract =(struct grep10*)args;
    char command[200];
    //mkdir folderproses
    sprintf(command,"mkdir ~/Documents/FolderProses%d",extract->val);
    system(command);

    //buat file simpanproses*.txt
    strcpy(command,"");
    sprintf(command,"ps aux --no-heading | head -%d  | tail -%d > ~/Documents/FolderProses%d/SimpanProses%d.txt",extract->batas_atas,extract->batas_bawah,extract->val,extract->val);

    system(command);

    //buat file zip KompresProses*.zip
    strcpy(command,"");
    sprintf(command,"zip -qmj ~/Documents/FolderProses%d/KompresProses%d ~/Documents/FolderProses%d/SimpanProses%d.txt",extract->val,extract->val,extract->val,extract->val);
    system(command);	
}

void* proses2(void* args){
    char command[100];
    struct grep10*extract =(struct grep10*)args;
    strcpy(command,"");
    sprintf(command,"unzip -qd ~/Documents/FolderProses%d ~/Documents/FolderProses%d/KompresProses%d.zip",extract->val,extract->val,extract->val);
    system(command);
}  



int main(){
  int n=2;
  struct grep10 structt;
  structt.val=0;
  structt.batas_bawah=10;
  structt.batas_atas=10;

  //hapus file yg sudah ada
  system("rm -rf ~/Documents/FolderProses*");
  //thread blablabla sampe zip
  for(int i=1; i<=n;i++){
	structt.val=i;
	pthread_create(&tid[i], NULL, &proses1,(void *)&structt);
    pthread_join(tid[i],NULL);
	structt.batas_atas += 10;
  }

  printf("Tunggu 15 Detik Untuk Unzip\n");
  sleep(15);
  //thread unzip
  for(int i=1; i<=n;i++){
	structt.val=i;
	pthread_create(&tid[i], NULL, &proses2,(void *)&structt);
    pthread_join(tid[i],NULL);
  }
}
