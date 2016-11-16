#define _POSIX_SOURCE 1

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include<fcntl.h>

#define S_BUF 100
int n; 
char buffer[S_BUF];

int main(int argc, char** argv){
	int fd_write,c = 0,i;
	
	if (( fd_write = open (argv[1],	O_WRONLY)) == -1) {
		perror ("open");
		exit (2);
	}
	
	i = 0;
	while(c != EOF){
	while((c = getchar()) != '\n'){ 
	buffer[i] = c;
	i++;
	}
	
	buffer[i] = '\0';
	
	if(write(fd_write,buffer,i) == -1){
		perror("write");
		exit(4);
	}
		
}

close(fd_write);

return EXIT_SUCCESS;
}
