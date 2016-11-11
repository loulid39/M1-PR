#define _XOPEN_SOURCE 700
# include <unistd.h>
# include <sys/types.h>
# include <stdio.h>
#include<stdlib.h>
# include <sys/wait.h>
# include <signal.h>


void sig_hand_f(int sig){
	printf("[%d]\n",getpid());
}


void main(void){
 sigset_t sig_proc1;
 struct sigaction action1;


 action1.sa_handler = sig_hand_f;
 sigaction(SIGCHLD, &action1,NULL);
	if(fork() == 0){
		printf("fils %d\n",getpid());
		exit(0);
	}
	printf("pere\n");
	sigsuspend(&sig_proc1);
	printf("Fin du progamme\n");
	
}
