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
		char* fich1 = argv[1];
		char* fich2 = argv[2];
		
		struct stat buf1;
		struct stat buf2;

	if(argc < 3){
		printf("manque des parametres\n");
		exit(0);
		}
		
		
		stat(fich1,&buf1);
		stat(fich2,&buf2);
		
		if(buf1.st_ino == buf2.st_ino)
			printf("les fichier sont identique\n");
		else
			printf("les fichiers sont differents\n");


return EXIT_SUCCESS;
}

