#define _XOPEN_SOURCE 700

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
	struct stat bufs;
	off_t len;
	int i;
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
    
    
    stat(file,&bufs);
    len = bufs.st_size;
    
    i = 2;
    while(pread(fd1,buf,SIZE,(len - i)) > 0){
    	printf("[%s]\n",buf);
    	i+=1;
    	write(fd2,buf,SIZE);

    }
    
    printf("Fin\n");
    return 0;
}
