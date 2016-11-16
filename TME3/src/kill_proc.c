#define _POSIX_SOURCE 1
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

void sig_hand(int sig){

}

pid_t *pids;

int main(int argc, char **argv) {
	pid_t pid,prem = getpid();
	int i,y,np;
 sigset_t sig_proc;
 struct sigaction action;
 
 	if(argc < 2){
		perror("manque des arguments");
		return EXIT_FAILURE;
	}
	
	np = 1 + atoi(argv[1]);
	pids = malloc(np * sizeof(pid_t));
	pids[0] = getpid();

 sigemptyset(&sig_proc);
 /* changer le traitement */
 action.sa_mask=sig_proc;
 action.sa_flags=0;
 action.sa_handler = sig_hand;
 sigaction(SIGUSR1, &action,NULL);

/* masquer SIGUSR1 */
 sigaddset (&sig_proc, SIGUSR1);
 sigprocmask (SIG_SETMASK,
&sig_proc, NULL);

 /* attendre le signal SIGINT */
 sigfillset (&sig_proc);
 sigdelset (&sig_proc, SIGUSR1);
 
for(i=1;i<np;i++){
		pid = fork();
		if(pid != 0){			
			 sigsuspend(&sig_proc);
			printf("[%d] fils de %d et son fils %d\n",getpid(),getppid(),pid);
			if(prem != getpid()) kill(getppid(),SIGUSR1);
		exit(0);
		} else {
		pids[i] = getpid();
			if((i + 1) == np) {
			printf("dernier fils : %d\n",getpid());
			printf("voici la liste des processus:\n");	
			for(y=0;y<np;y++){
				printf("%d > %d\n",y,pids[y]);
			}						
			kill(getppid(),SIGUSR1);
			exit(0);}
		}

}
 return EXIT_SUCCESS;
}
