#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include<signal.h>

void sig_hand(int sig){

}


pid_t pfils;

int main(void){

	sigset_t sig;
	struct sigaction action;
	
	sigemptyset(&sig);
	action.sa_mask=sig;
	action.sa_flags=0;
	action.sa_handler = sig_hand;

	sigaction(SIGUSR1, &action,NULL);
	sigaction(SIGUSR2, &action,NULL);

	sigaddset (&sig, SIGUSR1);
	sigaddset (&sig, SIGUSR2);
	sigprocmask (SIG_SETMASK,&sig, NULL);
	sigfillset (&sig);
	
	
	

printf("%d pere\n",getpid());
if(fork() == 0){
pfils = getpid();
printf("fils numero 1 : %d\n",pfils);
if(fork() == 0){
	printf("%d fils de %d\n",getpid(),getppid());
	kill(getppid(),SIGUSR1);
	exit(0);
}
	sigdelset (&sig, SIGUSR1);
	sigsuspend(&sig);
	sigdelset (&sig, SIGUSR2);
	sigsuspend(&sig);
	printf("%d fils de %d\n",getpid(),getppid());
	kill(getppid(),SIGUSR1);
	exit(0);
}
	else {
		if(fork() == 0){
		if(fork() == 0){
			printf("%d fils de %d\n",getpid(),getppid());
			kill(getppid(),SIGUSR1);
			exit(0);			
		}
			sigdelset (&sig, SIGUSR1);
			sigsuspend(&sig);
			printf("%d fils de %d\n",getpid(),getppid());
			kill(pfils,SIGUSR2);
			kill(getppid(),SIGUSR1);
			exit(0);
		}
	}
sigdelset (&sig, SIGUSR1);
sigsuspend(&sig);
sigsuspend(&sig);
	
return EXIT_SUCCESS;
}
