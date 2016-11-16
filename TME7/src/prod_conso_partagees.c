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

#define STACK_MAX_SIZE 100

	sem_t *prod;
	sem_t *cons;
	sem_t *mutex;
	int *pile;
	int *sp;
 
void init_sync(){
	int fd,fd1,fd2,fd3,fd4;
	
if((fd = shm_open("/monsem",O_RDWR | O_CREAT, 0600)) == -1){
	perror("shm_open");
	exit(1);
}
	
if(ftruncate(fd,sizeof(sem_t)) == -1){
	perror("ftruncate");
	exit(0);
}
if((mutex = mmap(NULL,sizeof(sem_t),PROT_READ | PROT_WRITE, MAP_SHARED,fd,0)) == MAP_FAILED){
	perror("mmap2");
	exit(0);
}

if((fd1 = shm_open("/sem_p",O_RDWR | O_CREAT, 0600)) == -1){
	perror("shm_open");
	exit(1);
}
	
if(ftruncate(fd1,sizeof(sem_t)) == -1){
	perror("ftruncate");
	exit(0);
}
if((prod = mmap(NULL,sizeof(sem_t),PROT_READ | PROT_WRITE, MAP_SHARED,fd1,0)) == MAP_FAILED){
	perror("mmap2");
	exit(0);
}

if((fd2 = shm_open("/sem_c",O_RDWR | O_CREAT, 0600)) == -1){
	perror("shm_open");
	exit(1);
}
	
if(ftruncate(fd2,sizeof(sem_t)) == -1){
	perror("ftruncate");
	exit(0);
}
if((cons = mmap(NULL,sizeof(sem_t),PROT_READ | PROT_WRITE, MAP_SHARED,fd2,0)) == MAP_FAILED){
	perror("mmap2");
	exit(0);
}

if((fd3 = shm_open("/monsp",O_RDWR | O_CREAT, 0600)) == -1){
	perror("shm_open");
	exit(1);
}

if(ftruncate(fd3,sizeof(int)) == -1){
	perror("ftruncate");
	exit(0);
}

if((sp = mmap(NULL,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED,fd3,0)) == MAP_FAILED){
	perror("mmap1");
	exit(0);
}

if((fd4 = shm_open("/monst",O_RDWR | O_CREAT, 0600)) == -1){
	perror("shm_open");
	exit(1);
}

if(ftruncate(fd4,sizeof(int)) == -1){
	perror("ftruncate");
	exit(0);
}

if((pile = mmap(NULL,STACK_MAX_SIZE * sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED,fd4,0)) == MAP_FAILED){
	perror("mmap1");
	exit(0);
}

sem_init(mutex,1,1);
sem_init(prod,1,STACK_MAX_SIZE);
sem_init(cons,1,0);
*sp = 0;
}

char pop(){
char c;
	sem_wait(cons);
	sem_wait(mutex);
	c = pile[(*sp)];
	*sp-=1;
	sem_post(mutex);
	sem_post(prod);
	
return c;
}

void push(char c){
	sem_wait(prod);
	sem_wait(mutex);
	pile[*sp] = c;
	*sp+=1;
	sem_post(mutex);
	sem_post(cons);
}

void free_pile(){
	sem_close(mutex);
	sem_close(cons);
	sem_close(prod);
	

	
	munmap(sp,sizeof(int));
	shm_unlink("/monsp");
	munmap(sp,sizeof(int));
	shm_unlink("/monst");		
	munmap(mutex,sizeof(sem_t));
	shm_unlink("/monsem");
	munmap(cons,sizeof(sem_t));
	shm_unlink("/sem_c");
	munmap(prod,sizeof(sem_t));
	shm_unlink("/sem_p");

}

void production(){
	char c;  
	while(1){
		c = getchar();
		if(c == EOF){break;}
	 push(c); 
	}
}

void consommation(){
char c;
	while(1) {
		c = pop();
		if(c == EOF){break;}	 
	 		putchar(c);
		 fflush(stdout); 
		}
	}

int main(int argc, char **argv){
	if(argc != 3){
		printf("erreur argument\n");
		exit(0);
	}
	int i;
	int n_p = atoi(argv[1]);
	int n_c = atoi(argv[2]);
	init_sync();
	for(i=0;i<n_p;i++){
		if(fork() == 0){
			production();
			exit(0);
		}
	}
	
	for(i=0;i<n_c;i++){
		if(fork() == 0){
			consommation();
			exit(0);
		}
	}
	
	for(i=0;i<n_c;i++){
		if(fork() == 0){
			exit(0);
		}
	}	
	
for(i=0;i<(n_c + n_p);i++)
	wait(NULL);
free_pile();
return EXIT_SUCCESS;
}


