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


int N;
void init_syn(sem_t *smutex[]){
int fd,i;

if((fd = shm_open("/monsem",O_RDWR | O_CREAT, 0600)) == -1){
	perror("shm_open");
	exit(1);
}
	
if(ftruncate(fd,sizeof(sem_t)) == -1){
	perror("ftruncate");
	exit(0);
}
for(i=0;i<(N-1);i++){
if((smutex[i] = mmap(NULL,sizeof(sem_t),PROT_READ | PROT_WRITE, MAP_SHARED,fd,0)) == MAP_FAILED){
	perror("mmap2");
	exit(0);
}
}
	
/*initialisation mutex, tab -> boucle*/
for(i=0;i<(N-1);i++){
	sem_init(smutex[i], 1, 0);//toujours par pour que proc demandeur se bloque !!!
	}

}

void free_syn(sem_t *smutex[]){
int i;
for(i=0;i<(N-1);i++)
	sem_close(smutex[i]);
for(i=0;i<(N-1);i++)
munmap(smutex[i],N * sizeof(sem_t));
shm_unlink("/monsem");
}

void wait_synchro(int i,sem_t *smutex[]){
	if(i == 1){
		printf("[%d] pid = %d\n",i,getpid());
		sem_post(smutex[i]);
	} else {
	sem_wait(smutex[i-1]);
	printf("[%d] pid = %d\n",i,getpid());
	sem_post(smutex[i]);
	}
}

int main(int argc, char** argv){
	if(argc != 2){
		printf("manque des arguments\n");
		exit(0);
	}
	N = atoi(argv[1]);
	sem_t *smutex[N];
	init_syn(smutex);
	int i;
	pid_t *pid,pb;
	pid = malloc(N * sizeof(int));
	for(i=0;i<N;i++){
		sleep(N % (i+1));
		pb = fork();
		sleep(1);
		if(pb == 0){
			wait_synchro((i+1),smutex);
			exit(0);
		}
	}
	for(i=0;i<N;i++) wait(NULL);
	free_syn(smutex);
return EXIT_SUCCESS;
}
