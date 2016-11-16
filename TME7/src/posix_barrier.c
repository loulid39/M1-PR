#define  _XOPEN_SOURCE 700
/*
semaphore anonyme
*/

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

int *cpt;
sem_t *smutex;
sem_t *sbarrier;

void init_barrier(){
int fd,fd1,fd2;

if((fd2 = shm_open("/monbsem",O_RDWR | O_CREAT, 0600)) == -1){
	perror("shm_open");
	exit(1);
}

if(ftruncate(fd2,sizeof(sem_t)) == -1){
	perror("ftruncate");
	exit(0);
}

if((sbarrier = mmap(NULL,sizeof(sem_t),PROT_READ | PROT_WRITE, MAP_SHARED ,fd2,0)) == MAP_FAILED){
	perror("mmap3");
	exit(0);
}

if((fd1 = shm_open("/monsem",O_RDWR | O_CREAT, 0600)) == -1){
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

/*initialisation mutex*/
sem_init(sbarrier, 1, 0);//toujours par 0
sem_init(smutex, 1, 1);//toujours par 1 !!!

if((fd = shm_open("/monshm",O_RDWR | O_CREAT, 0600)) == -1){
	perror("shm_open");
	exit(1);
}

if(ftruncate(fd,sizeof(int)) == -1){
	perror("ftruncate");
	exit(0);
}

if((cpt = mmap(NULL,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED,fd,0)) == MAP_FAILED){
	perror("mmap1");
	exit(0);
}

*cpt = 0;
}

void free_barrier(){
sem_close(smutex);
sem_close(sbarrier);

munmap(cpt,sizeof(int));
shm_unlink("/monshm");
munmap(smutex,sizeof(sem_t));
shm_unlink("/monsem");
munmap(sbarrier,sizeof(sem_t));
shm_unlink("/monbsem");
}

void wait_barrier(int n){
	sem_wait(smutex);
	*cpt+=1;
	if(*cpt == n){
		sem_post(sbarrier);
	}
	sem_post(smutex);
	sem_wait(sbarrier);
	sem_post(sbarrier);
}

int main(int argc, char **argv){

if(argc < 2){
	printf("manque des arguments\n");
	exit(0);
}

int N = atoi(argv[1]);
int i;

pid_t *pid,pd;
pid = malloc(sizeof(pid_t) * N);
init_barrier();

for(i=0;i<N;i++){
	 pd = fork();
	if(pd == 0){

	printf("avant barriere\n");
	wait_barrier(N);
	printf("apres barriere\n");
	

	exit(0);
	} else pid[i];
}

for(i=0;i<N;i++) wait(NULL);
free_barrier();
return EXIT_SUCCESS;
}
