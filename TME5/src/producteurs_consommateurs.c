#define _POSIX_SOURCE 1
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

/*

condition pour les consommateurs
(sommet non negative)
*/

char pile[100];
int sommet = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void push(int c){	
	pthread_mutex_lock(&mutex);
	pile[sommet] = (char) c;
	sommet++;
	pthread_mutex_unlock(&mutex);
}

int pop(){
	int c;
	pthread_mutex_lock(&mutex);	
	if(sommet < 0) {
	pthread_mutex_unlock(&mutex);
		return -1;
	}
	c = pile[sommet-1];
	sommet--;
	pthread_mutex_unlock(&mutex);
return c;
}

void* producteur(void* arg){
	int c;  
	while((c = getchar()) != EOF){
	 push(c); 
	}
	 pthread_exit((void*)0);
}

void* consommateur(void* arg){
int c = 0;
	while(c != EOF) {
	c = pop();
	 if(c != -1){
	 	putchar(c);
		 fflush(stdout); 
		}
	}
 pthread_exit((void*)0);
}

void main(int argc,char **argv){

if(argc < 3){
	printf("manque des arguments\n");
	exit(0);
}
int n_p = atoi(argv[1]),n_c = atoi(argv[2]),i;
pthread_t t_prod[n_p];
pthread_t t_cons[n_c];

for(i=0;i<n_p;i++){
	pthread_create(&t_prod[i],NULL,producteur,NULL);
	}
for(i=0;i<n_c;i++){
	pthread_create(&t_cons[i],NULL,consommateur,NULL);
}

for(i=0;i<n_c;i++)
pthread_join(t_prod[i],NULL);

for(i=0;i<n_c;i++)
pthread_join(t_cons[i],NULL);

sleep(1);
printf("\nFin\n");
	
}
