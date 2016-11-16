#define _POSIX_SOURCE 1
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdint.h>
#include<string.h>

int SomGlob,N;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* thread1(void* arg){
pthread_mutex_lock(&mutex);
pthread_cond_wait(&cond,&mutex);
printf("Thread dit la somme est : %d\n",SomGlob);
pthread_mutex_unlock(&mutex);
return (void*)0;
}

void* func(void* arg){
	static int in = 0;
		
	int *val = malloc(sizeof(int));
	memcpy(val,arg,1);
	srand(time(NULL));
	int nb = (int) (10*((double)rand())/ RAND_MAX);
	printf("%u : Mon numero de creation : %d, ValAleat : %d\n",(unsigned int)pthread_self(),*val,nb);
	 
	pthread_mutex_lock(&mutex);
	in++;
	SomGlob += nb;
	if(in == N) pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	return (void*) val;
}

int main(int argc, char* argv[]){
	int y,i,*x;
	SomGlob = 0;
	if(argc < 2){
		printf("manque des arguments\n");
		exit(0);
	}
	N = atoi(argv[1]);
	pthread_t tid[N],ttid;
	
	if(pthread_create(&ttid,NULL,thread1,(void*)0) != 0){
		perror("pthread_create");
		exit(0);
	}
	
	for(i=0;i<N;i++){
		if(pthread_create(&tid[i],NULL,func,&i) != 0){
			perror("pthread_create");
			exit(0);
		}
		sleep(1);
	}
	
		pthread_join(ttid,NULL);
		printf("Thread %u terminer\n",(unsigned int) ttid);

	printf("Fin !!\n");
return EXIT_SUCCESS;
}
