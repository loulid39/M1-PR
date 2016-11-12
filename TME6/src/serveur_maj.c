#define _OPEN_SOURCE 1

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<signal.h>

#define S_BUF 100

void handler(int i){
}

int main(int argc, char** argv){
	int i,fd_read,n,c;
	
	struct sigaction action;
	sigset_t sig_p;
	
	sigemptyset(&sig_p);
	
	action.sa_mask = sig_p;
	action.sa_flags=0;
	action.sa_handler = handler;
	sigaction(SIGINT, &action, NULL);
	
	sigfillset(&sig_p);
	sigdelset(&sig_p,SIGINT);
	
	char buffer[S_BUF];
	if ( mkfifo(argv[1],S_IRUSR|S_IWUSR) == -1) {
		perror("mkfifo");
	exit (1);
	}
	
	if((fd_read = open(argv[1],O_RDONLY)) == -1){
		perror("open");
		exit(1);
	}
	while(1)
	if((n = read(fd_read,buffer,S_BUF)) == -1){
		perror("read");
		exit(2);
	}else{
		buffer[n] = '\n';
		for(i=0;i<n;i++)
			buffer[i] = toupper(buffer[i]);
		printf("%s\n",buffer);
	}

	sigsuspend(&sig_p);

	unlink(argv[1]);
	close (fd_read);
	
printf("Fin\n");
return EXIT_SUCCESS;
}
