#define _POSIX_SOURCE 1

#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <errno.h>


#define SIZE 1

int main (int argc, char* argv[])
{

	char buf[SIZE];
	int fd1,fd2;
	char* file_name = "fichier_inverser.txt";
	char* file = argv[1];

	if(argc < 1){
		printf("manque des arguments\n");
		exit(0);
	}
    if((fd1 = open(file,O_RDWR , 0600)) == -1){
         perror("open");
         return errno;
    }
    if((fd2 = open(file_name,O_RDWR | O_TRUNC | O_CREAT, 0600)) == -1){
         perror(argv[1]);
         return errno;
    }
    
    
   if(lseek(fd1,0,SEEK_END) == -1){
    	perror("lseek");
    	return errno;
    }
    
   	if(lseek(fd1,(-(SIZE+1)),SEEK_CUR) == -1){
				perror("lseek");
				return errno;
			}
    
    while(read(fd1,buf,SIZE) > 0){
    	printf("[%s]\n",buf);
    	write(fd2,buf,SIZE);
			if(lseek(fd1,(-(SIZE+1)),SEEK_CUR) == -1){
				perror("lseek");
				return errno;
			}
    }
    
    printf("Fin\n");
    return 0;
}
