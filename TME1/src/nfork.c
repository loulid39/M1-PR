#define _POSIX_SOURCE 1
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int nfork(int nb_fils);

int main(int argc, char** argv){
	if(argc < 2){
		printf("introduire le nombre des fils a creer\n");
		exit(1);
		}
		
	int nb_fils_creer =	nfork(atoi(argv[1]));
	printf("le nombre des processus creer : %d\n",nb_fils_creer);
	


return EXIT_SUCCESS;
}


int nfork(int nb_fils){
	int i,pid;
	if(nb_fils < 0) return -1;
	for(i=0;i<nb_fils;i++){
		pid = fork();
		if(pid == 0){
		exit(0);
		} else if(pid == -1) {
			return i=0 ? -1 : i;
		} 
	}	
	return nb_fils;

}



