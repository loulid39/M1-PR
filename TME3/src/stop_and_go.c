#define _POSIX_SOURCE 1
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

void sig_hand(int sig){

}

void sig_hand_f(int sig){
	signal(SIGCHLD,sig_hand_f);
}

pid_t *pids;

int main(int argc, char **argv) {
	pid_t pid,prem = getpid();
	int i,y,z,np;
 sigset_t sig_proc,sig_proc1;
 struct sigaction action,action1;
 
 	if(argc < 2){
		perror("manque des arguments");
		return EXIT_FAILURE;
	}
	
	np = 1 + atoi(argv[1]);
	pids = malloc(np * sizeof(pid_t));
	pids[0] = getpid();

 sigemptyset(&sig_proc);
 sigemptyset(&sig_proc1);
 /* changer le traitement */
 action.sa_mask=sig_proc;
 action.sa_flags=0;
 action.sa_handler = sig_hand;
 sigaction(SIGUSR1, &action,NULL);
 
 action1.sa_handler = sig_hand_f;
 sigaction(SIGCHLD, &action1,NULL);

/* masquer SIGINT */
 sigaddset (&sig_proc, SIGUSR1);
 sigprocmask (SIG_SETMASK,
&sig_proc, NULL);

 /* attendre le signal SIGINT */
 sigfillset (&sig_proc);
 sigdelset (&sig_proc, SIGUSR1);

for(i=1;i<np;i++){

		pid = fork();
		if(pid != 0){			
			if(prem == getpid()){
				 sigsuspend(&sig_proc); 
					printf("Tous les descendants sont suspendus\n");
					for(z=1;z<np;z++) kill(pids[z],SIGCONT);
					sigsuspend(&sig_proc1);
					sleep(1);
					printf("Fin du programme\n");
					return EXIT_SUCCESS;
				}else {
				kill(getpid(),SIGSTOP);
			printf("[%d] fils de %d et son fils %d\n",getpid(),getppid(),pid);
			sigsuspend(&sig_proc1);
		exit(0);
		}
		} else {
		pids[i] = getpid();
			if((i + 1) == np) {
			kill(pids[0],SIGUSR1);
			kill(getpid(),SIGSTOP);
			printf("dernier fils : %d\n",getpid());
			printf("voici la liste des processus:\n");	
			for(y=0;y<np;y++){
				printf("%d > %d\n",y,pids[y]);
			}						
			exit(0);}
		}

}
 return EXIT_SUCCESS;
}
