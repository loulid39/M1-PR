#define _POSIX_SOURCE 1

#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include <errno.h>


#define SIZE 256

int main (int argc, char* argv[])
{

	if(argc < 5){
		printf("manque des arguments\n");
		exit(0);
	}

	char buf[SIZE];
	int fd2,i;
	char* file_name = argv[1];
	char* word = argv[2];
    if((fd2 = open(file_name,O_RDWR | O_TRUNC | O_CREAT, 0600)) == -1){
         perror(argv[1]);
         return errno;
    }
    
    for(i=3;i<argc;i++){
    	if(write(fd2,argv[i], strlen(argv[i])) == -1){
				perror("write");
				return errno;
    	}
    	if(write(fd2," ",strlen(" ")) == -1){
				perror("write");
				return errno;
    	}
    }    
    
    if(lseek(fd2,0,SEEK_SET) == -1){
    	perror("lseek");
    	return errno;
    }
    
    while(read(fd2,buf,SIZE) > 0){
			for(i=0;i<SIZE;i++){	
				if(buf[i] == ' '){
					if(lseek(fd2,0,SEEK_SET) == -1){
						perror("lseek");
						return errno;
					}
					if(lseek(fd2,(i+1),SEEK_CUR) == -1){
						perror("lseek");
						return errno;
					}
					if(write(fd2,word,strlen(word)) == -1){
						perror("write");
						return errno;
					}	
					break;							
				}
			}
			break;
    }
    
    printf("Fin\n");
    return 0;
}
