#define _XOPEN_SOURCE 700

#include<string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>


#define TAILLE_PATH 100
#define SIZE 32

char buff_path [TAILLE_PATH];
DIR *pt_Dir;
struct dirent* dirEnt;

int nb_ref = 0;

int traitement(char *file,char *word){
	int fd;
	char buf[SIZE];


    if((fd = open(file,O_RDWR, 0600)) == -1){
         perror("open");
         return errno;
    }
	
	while(read(fd,buf,SIZE) > 0){
		if(strstr(buf,word) != NULL){
			printf("[%s] existe dans [%s]\n",word,file);
			nb_ref++;
		}
		break;
	}

	return 0;
}

int main (int argc, char* argv []) {
	struct stat buf;

	if(argc < 2)
		printf("manque des arguements\n");

		memcpy (buff_path,argv[1],strlen(argv[1]));
	
	if ( ( pt_Dir = opendir (buff_path) ) == NULL) {
		if (errno ==  ENOENT ) {
			/* repertoire n'existe pas - créer le répertoire */
			if (mkdir ( buff_path, S_IRUSR|S_IWUSR|S_IXUSR) == -1) {
				perror ("erreur mkdir\n");
				exit (1);
			}
			else
				return 0;
		}
		else {
			perror ("erreur opendir \n");
			exit (1);
		}
}

	/* lire répertoire */
	while ((dirEnt = readdir (pt_Dir)) !=NULL) {
	
		lstat(dirEnt->d_name,&buf);
		if(!S_ISDIR(buf.st_mode) !=0) 
		traitement(dirEnt->d_name,argv[2]);
	}
	closedir (pt_Dir);
	if(nb_ref == 0) printf("Aucun fichier valide\n");
	return 0;
}
