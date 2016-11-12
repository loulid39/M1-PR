#define _POSIX_SOURCE 1

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdint.h>
#include<string.h>
#include <ctype.h>
#include <errno.h>


int upper(char *path)
{
    FILE* fp1, *fp2;
    int c = 1;

    fp1= fopen (path, "r");
    fp2= fopen (path, "r+");
    if ((fp1== NULL) || (fp2== NULL)) {
        perror ("fopen");
	return errno;
    }

    while (c !=EOF) {
        c=fgetc(fp1);
        if (c!=EOF)
        fputc(toupper(c),fp2);
    }

    fclose (fp1);
    fclose (fp2);

    return EXIT_SUCCESS;
}

void* f_upper(void *arg){
	char* path = (char*)arg;
	printf("%u path = %s\n",(unsigned int) pthread_self(),path);
	upper(path);
pthread_exit((void*)0);
}

int main(int argc,char* argv[]){
	if(argc < 2){
		perror("manque des parametres");
		exit(0);
	}
	int i,N = argc -1;
	pthread_t tid[N];
	for(i=0;i<N;i++){
		if(pthread_create(&tid[i],NULL,f_upper,argv[i+1]) !=0){
			perror("pthread_create");
			return errno;
		}
	}
sleep(N+1);
printf("Fin\n");	
return EXIT_SUCCESS;
}



