#define _POSIX_SOURCE 1 
#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER,mutexf = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER,condf = PTHREAD_COND_INITIALIZER;
int N,pos = 0;

void sig_hand(int sig){
        printf("\nTmain je vais liberer les autres\n");
        pthread_mutex_lock(&mutexf);
        pthread_cond_broadcast(&condf);
        pthread_mutex_unlock(&mutexf);
}
void* thread(void* arg){
pthread_t tid;
pthread_mutex_lock(&mutex);
pthread_create(&tid,NULL,thread,NULL);

if(pos == N){
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        pthread_exit((void*)0);
}
pos++;
pthread_mutex_unlock(&mutex);
pthread_mutex_lock(&mutexf);
pthread_cond_wait(&condf,&mutexf);

pthread_mutex_unlock(&mutexf);
pthread_join(tid,NULL);

pthread_exit((void*)0);
}

int main(int argc, char** argv){
        N = atoi(argv[1]);
        pthread_t tid,tid_cont;
        sigset_t ens;
        struct sigaction action;
        sigfillset(&ens);
        sigprocmask(SIG_BLOCK,&ens,NULL);
        
        if(pthread_create(&tid,NULL,thread,NULL) != 0){
                perror("pthread_create");
                exit(0);
        }
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond,&mutex);
        printf("Tous mes descendants sont créés\n");
        pthread_mutex_unlock(&mutex);
        printf("attente d'un signal SIGINT\n");
        action.sa_mask = ens;
        action.sa_flags = 0;
        action.sa_handler = sig_hand;
        sigaction(SIGINT,&action,NULL);
        sigdelset(&ens,SIGINT);        
        sigsuspend(&ens);

        if(pthread_join(tid,NULL) != 0){
                perror("pthread_join");
                exit(0);
        }
        printf("Tous mes descendants se sont terminés\n");
        printf("Fin\n");

return EXIT_SUCCESS;
}
