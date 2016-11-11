#define _POSIX_SOURCE 1
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>

#define SIZE 100

int main(int argc, char* argv[]){
	int fd1,fd2;
	char buffer[SIZE];
	struct stat fileStat;
	if(stat(argv[1],&fileStat) < 0){
		perror(argv[1]);
		return errno;
	}

	if(!(S_ISREG(fileStat.st_mode) && (S_IRUSR & fileStat.st_mode))){
		perror(argv[1]);
		return errno;
	}
	
	if((fd1 = open(argv[1], O_RDONLY , 0600)) == -1){
		perror("open");
		return errno;
	}
	
	if((fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0600)) == -1){
		perror("open");
		return errno;
	}
	int nb;
	while((nb = read(fd1,buffer,SIZE)) > 0){
		write(fd2,buffer,nb);
	}
 
	close(fd1);
	close(fd2);
  printf("FIN\n");  
return EXIT_SUCCESS;
}
