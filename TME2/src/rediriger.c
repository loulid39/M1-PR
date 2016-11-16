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

int Rediriger_stdout(const char *file){
	FILE *fp;
	int sv_fd = dup(1);
	fp = freopen(file,"w+",stdout);
	if(fp){}

return sv_fd;
}

void Restaurer_stdout(int fd){
	dup2(fd, STDOUT_FILENO);
}

int main(int argc, char* argv[]){

	const char *file = argv[1];
	int fd;
	if(argc < 2){
		printf("manque des arguments\n");
		exit(0);
	}
	printf("Avant appel de Rediriger_stdout\n");
	fflush(stdout);
	fd = Rediriger_stdout(file);
	printf("Apres appel de Rediriger_stdout\n");
	fflush(stdout);
	printf("Avant appel de Restaurer_stdout\n");
	fflush(stdout);
	Restaurer_stdout(fd);
	printf("Apres appel de Restaurer_stdout\n");
	fflush(stdout);
	printf("Fin\n");
return EXIT_SUCCESS;
}
