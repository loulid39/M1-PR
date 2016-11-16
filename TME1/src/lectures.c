#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
/*
la différence entre POSIX et libC
en mode POSIX il y a la notion de concurrence 
qu'on le trouve pas dans le mode la libC
*/

#define SIZE 1


void c_reader(int fd){
int c;
FILE* steam = fdopen(fd,"r");
while(c = fgetc(steam) != EOF){
		printf("[%d] %c\n",getpid(),c);
	}
}

void p_reader(int fd){
int n;
char tambon[SIZE];
	while((n = read(fd, tambon, SIZE)) >0){
	tambon[n] = '\0';
		printf("[%d] %s\n",getpid(),tambon);
	}
}


int main(int argc, char* argv[]){

	int i,fd;
	fd = open(argv[2],O_RDWR,0600);

	for(i=0;i<2;i++){
		if(fork() == 0){ 
			if(i == 0)p_reader(fd);
			if(i==1) c_reader(fd);
			exit(0);
		}
	}
	if(strcmp(argv[1], "-p") == 0) p_reader(fd);
	if(strcmp(argv[1], "-C") == 0) c_reader(fd);
	printf("Fin\n");
return EXIT_SUCCESS;
}
