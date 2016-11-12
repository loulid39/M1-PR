#define _OPEN_SOURCE 1

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define S_BUF 100


int main(int argc, char** argv){
	int tubeDesc[2];
	char buffer[S_BUF];
	int n;
	int c,i; 
	pid_t pid_fils;
	
	if(pipe(tubeDesc) == -1){
		perror("pipe");
		exit(1);
	}
	
	if((pid_fils = fork()) == -1){
		perror("fork");
		exit(2);
	}
	
	if(pid_fils == 0){
		if((n = read(tubeDesc[0],buffer,S_BUF)) == -1){
			perror("read");
			exit(3);
		} else {
				buffer[n] = '\0';
				for(i=0;i<n;i++)
					buffer[i] = toupper(buffer[i]);
				printf("%s\n",buffer);
	}
	exit(0);
	} else {
	while((c = getchar()) != EOF){ 
	if(write(tubeDesc[1],&c,1) == -1){
		perror("write");
		exit(4);
	}
	}

	wait(NULL);
	}
	
		
printf("Fin\n");
return EXIT_SUCCESS;
}
