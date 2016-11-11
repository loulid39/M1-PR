#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>


int main(int argc, char* argv[]){
	if(argc < 4){
		printf("manque des parametres\n");
		exit(0);
		}
		
		char* nom_op = argv[1];
		char* nom_fich = argv[2];
		char* nom_n_fich = argv[3];
		
		struct stat buf;
		stat(nom_fich,&buf);
		
		if(S_ISDIR(buf.st_mode) == 0){
			printf("le deuxieme arguement ne doit pas etre un repertoire\n");
			exit(0);
		}
		
		if(strcmp(nom_op,"E")== 0){
			if(unlink(nom_fich) != 0){
				perror("unlink");
				return errno;
			}
		}
		
		if(strcmp(nom_op,"R")== 0){
		printf("je suis la\n");
			if(rename(nom_fich, nom_n_fich)){
				perror("rename");
				return errno;
			}
		}
		
		if(strcmp(nom_op,"C") == 0){
		mode_t mode;
			if(strcmp(nom_n_fich,"R") == 0){
					if(strcmp(nom_n_fich,"R")== 0){
						mode = S_IROTH | S_IRGRP | S_IRUSR | S_IWUSR;
					}
					if(strcmp(nom_n_fich,"W")== 0){
						mode = S_IRUSR | S_IWUSR;
					}
					if(chmod(nom_fich,mode)!=0){
						perror("chmod");
						return errno;
					}		
		}
	}	

return EXIT_SUCCESS;
}
