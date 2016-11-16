#define _POSIX_SOURCE 1 
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int pos = 0;

int max;

int fact(int n){
int i = 1,f = 1;
        while(i<=n){
                f *= i;
                i++;
        }
        return f;
}

int calcul(int n){
int i = 1,k = 0;
while(i<= n){
	k += fact(i);
	i++;
}
return k;
}

void* thread_func(void* arg) {

 int i, nb;
 int *param;
 int *lvl = (int*)arg;

 pthread_t *tid;

 nb = (*lvl)+1;
                
 if (*lvl < max) {
 pthread_mutex_lock(&mutex);
 
 if(pos == calcul(nb - 1)){
 //dernier thread -> liberer niveau suivant
 printf("******\n");
         pthread_cond_broadcast(&cond);
         pthread_mutex_unlock(&mutex);
 } else {
 //attendre la terminaison du niveau precedent
         pthread_cond_wait(&cond,&mutex);
         pthread_mutex_unlock(&mutex);
 }
   param = (int*)malloc(sizeof(int));
   *param = nb;
   tid = calloc(nb, sizeof(pthread_t));
   printf("%d cree %d fils\n", (int)pthread_self(), nb);
   for (i = 0; i < nb; i++) {
     pthread_create((tid+i), 0, thread_func, param);
     pos++;
   }
   for (i = 0; i < nb; i++)
     pthread_join(tid[i], NULL);
 }

 if (*lvl > 1)
   pthread_exit ( (void*)0);
        
 return (void*)0;
}

int main(int argc, char** argv){
if(argc > 1) max = atoi(argv[1]);
	else exit(0);
pthread_t tid;
int t = 1;
pthread_create(&tid,NULL,thread_func,&t);
pos++;
pthread_join(tid,NULL);
printf("nombre des threads : %d\n",pos);
printf("FIN\n");
return EXIT_SUCCESS;
}
