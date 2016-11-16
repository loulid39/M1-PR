#define _POSIX_SOURCE 1
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdint.h>
#include<string.h>

void* func(void* arg){
	int *val = malloc(sizeof(int));
	memcpy(val,arg,1);
	printf("%u : Mon numero de creation : %d\n",(unsigned int)pthread_self(),*val);
	(*val) *=2; 
	return (void*) val;
}

int main(int argc, char* argv[]){
	int N,y,i,*x;
	
	if(argc < 2){
		printf("manque des arguments\n");
		exit(0);
	}
	N = atoi(argv[1]);
	pthread_t tid[N];
	for(i=0;i<N;i++){
		if(pthread_create(&tid[i],NULL,func,&i) != 0){
			perror("pthread_create");
			exit(0);
		}
		sleep(1);
	}
	
	for(y=0;y<N;y++){
		pthread_join(tid[y],(void**)&x);
		printf("%u terminer avec %d\n",(unsigned int) tid[y],*(int*)x);
	}
	printf("Fin !!\n");
return EXIT_SUCCESS;
}
