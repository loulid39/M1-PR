#define _POSIX_SOURCE 1
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void interrupt_signal(int signo, siginnfo_t si, void *context){

}

int main(int argc, char **argv){
	if(argc != 2){
		printf("manque des arguments\n");
		exit(0);
	}
	int i,N = atoi(argv[1]);
	int mysig;
	union sigval val;
	sigset_t block_mask;
	struct sigaction action;

	action.sa_sigaction = interrupt_signal;
	action.sa_flags = SA_SIGINFO;
	sigfillset(&block_mask);
	action.sa_mask = block_mask;
	sigaction(SIGRTMIN, &action, 0);
	sigprocmask(SIG_SETMASK, &block_mask, 0);
	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGRTMIN);

	for(i=0;i<N;i++){
		if(fork() == 0){
			/**/
			printf("[%d] : \n",getpid());
			if((i + 1) == N) printf("*%d*\n",getpid());
			exit(0);
		}
	}

	return EXIT_SUCCESS;
}