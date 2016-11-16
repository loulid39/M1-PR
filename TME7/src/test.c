#define  _XOPEN_SOURCE 700

#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<semaphore.h>
#include<unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/wait.h>

void main(void){

sem_t *smutex;
int *cpt;
int fd1,fd;

if((fd1 = shm_open("/ssem",O_RDWR | O_CREAT, 0600)) == -1){
	perror("shm_open");
	exit(1);
}

if(ftruncate(fd1,sizeof(sem_t)) == -1){
	perror("ftruncate");
	exit(0);
}

if((smutex = mmap(NULL,sizeof(sem_t),PROT_READ | PROT_WRITE, MAP_SHARED,fd1,0)) == MAP_FAILED){
	perror("mmap2");
	exit(0);
}

if((fd = shm_open("/kkkkkk",O_RDWR | O_CREAT, 0600)) == -1){
	perror("shm_open");
	exit(1);
}

if(ftruncate(fd,sizeof(int)) == -1){
	perror("ftruncate");
	exit(0);
}

if((cpt = mmap(NULL,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED,fd,0)) == MAP_FAILED){
	perror("mmap4");
	exit(0);
}
*cpt += 1;
printf("%d\n",*cpt);

munmap(cpt,sizeof(int));
shm_unlink("/kkkkkk");

printf("Fin\n");

}
