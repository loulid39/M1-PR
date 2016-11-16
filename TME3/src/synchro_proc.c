#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include	<stdlib.h>
#include <sys/wait.h>
#include <signal.h>

void handler(int sig) {

}

void sig_hand_f(int sig) {

}

int main(int argc, char* argv[]) {

	int status;
	pid_t pid = getpid();
	sigset_t pp,sig;

	struct sigaction act,action;


	sigemptyset(&sig);
	action.sa_mask=sig;
	action.sa_flags=0;
	action.sa_handler = sig_hand_f;
	sigaction(SIGUSR1, &action,NULL);
	sigaddset (&sig, SIGUSR1);
	sigprocmask (SIG_SETMASK,&sig, NULL);
	sigfillset (&sig);
	sigdelset (&sig, SIGUSR1);
	
	act.sa_handler=handler;
	sigaction(SIGCHLD,&act,NULL);
	
	if(fork() == 0) {
		if(fork() == 0){
			/*P3*/
			kill(pid,SIGUSR1);
			exit(0);
		}
		/*P2*/
		sigsuspend(&pp);
		kill(getppid(),SIGUSR1);
		exit(0);
	}  
	/*P1*/
	sigsuspend(&sig);
	printf("Processus P3 creer\n");
	sigsuspend(&sig);
	printf("Processus P3 terminer\n");
	sigsuspend(&pp);
	printf("Processus P2 terminer\n");
return EXIT_SUCCESS;
}
