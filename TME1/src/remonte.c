#define _POSIX_SOURCE 1
/*
= optenir valeur aleatoire
= ecrire dans un fichier avec saute de la ligne ('\r')
= filtrage des entier depuis char*
*/

#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<errno.h>
#include<string.h>

#define SIZE 100

int main(int argc, char* argv[]){
	int n,i,t,fd,cpt = 0;
	n = atoi(argv[1]);
	char buf[SIZE];
	
	pid_t *pid = malloc(n * sizeof(pid_t));
	if((fd = open(argv[2],O_TRUNC | O_CREAT | O_RDWR, 0600)) == -1){
		perror("open");
		return errno;
	}

	for(i=0;i<n;i++){
		if((t = fork()) == 0){
			srand(time(NULL)*i);
			int k = (int) (10*(float)rand()/ RAND_MAX);
			sprintf(buf,"%d",k);
			strcat(buf,"\r");
			write(fd,buf,strlen(buf));
			exit(0);
		} 
		pid[i] = t;
	}

	for(i=0;i<n;i++)
		waitpid(pid[i],NULL,0);
		
	lseek(fd,SEEK_SET,0);
	t = read(fd,buf,SIZE);
	for(i=0;i<t;i++){
		if(buf[i] != '\r') cpt += (buf[i] - 48);
	}
	printf("la somme est : %d\n",cpt);
close(fd);
printf("Fin\n");
return EXIT_SUCCESS;
}
