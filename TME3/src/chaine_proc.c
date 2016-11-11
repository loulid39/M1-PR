#define _POSIX_SOURCE 1
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pid_t *pids;
int main(int argc, char* argv[]){
	srand(getpid());
	int i,y,np,status;
	pid_t pere = getpid();
	if(argc < 2){
		perror("manque des arguments");
		return EXIT_FAILURE;
	}
	np = atoi(argv[1]);
	pid_t pid;
	pids = malloc(np * sizeof(pid_t));
	pids[0] = getpid();
	for(i=1;i<np;i++){
		pid = fork();
		if(pid != 0){			
			wait(&status);
			printf("[%d] fils de %d et son fils %d\n",getpid(),getppid(),pid);
			if(getpid() == pere) printf("valeur generer : %d\n",status);
			exit(WEXITSTATUS(status));
		} else {
		pids[i] = getpid();
		if((i+1) == np){
			printf("dernier fils : %d\n",getpid());
			printf("voici la liste des processus:\n");
			for(y=0;y<np;y++){
				printf("%d > %d\n",y,pids[y]);
			}
			int val = (int)(rand () /(((double) RAND_MAX +1) /100));
			
			exit(val);
		}
		}
	}
	

return EXIT_SUCCESS;
}
