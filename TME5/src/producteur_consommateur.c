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

pthread_t t_prod;
pthread_t t_cons;

pthread_create(&t_prod,NULL,producteur,NULL);
pthread_create(&t_cons,NULL,consommateur,NULL);

pthread_join(t_prod,NULL);
pthread_join(t_cons,NULL);
sleep(1);
printf("\nFin\n");
	
}
