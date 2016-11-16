#define _POSIX_SOURCE 1
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdint.h>
#include<string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void wait_barrier(int N){
	static int cpt = 0;
	pthread_mutex_lock(&mutex);
	cpt++;
	if(cpt == N){
		pthread_cond_broadcast(&cond);
	} else{
		pthread_cond_wait(&cond,&mutex);
	}
	pthread_mutex_unlock(&mutex);
}


void* thread_func (void *arg) {
   printf ("avant barriere\n");
   wait_barrier (((int *)arg)[0]);
   printf ("après barriere\n");
   pthread_exit ( NULL);
}

int main(int argc, char* argv[]){
	int i,N = atoi(argv[1]);
	pthread_t tid[N];
	for(i=0;i<N;i++){
		pthread_create(&tid[i],NULL,thread_func,&N);
		sleep(1);
	}
	for(i=0;i<N;i++){
		pthread_join(tid[i],NULL);
	}

return EXIT_SUCCESS;
}
