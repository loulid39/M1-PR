#define _POSIX_SOURCE 1

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include <sys/wait.h>


void main(void){
	pid_t pid;
	
	pid = fork();
	
	if(pid == 0){
		printf("pid : %d, ppid : %d\n",getpid(),getppid());
		if(fork() == 0){
			printf("pid : %d, ppid : %d\n",getpid(),getppid());
			exit(0);
		} else wait(NULL);
		exit(0);
	} else {
		if(fork() == 0){
			printf("pid : %d, ppid : %d et mon frere : %d\n",getpid(),getppid(),pid);
			if(fork() == 0){
				printf("pid : %d, ppid : %d\n",getpid(),getppid());
				exit(0);
			}
			wait(NULL);
		} else{
			wait(NULL);
			exit(0);
		}
	
	}
	wait(NULL);
	printf("Fin\n");
}
