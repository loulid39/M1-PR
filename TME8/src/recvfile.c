#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include	<fcntl.h>
#include <string.h>
#include <netdb.h>
#include <time.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>



int main(int argc,char **argv){

	struct sockaddr_in sin;
	struct sockaddr_in exp;
	int sc;
	int scom;
	struct hostent *hp;
	int fromlen = sizeof(exp);
	int cpt,fd;
	
	/*creation de la socket*/
	if((sc = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket");
		exit(0);
	}
	memset((char*)&sin,0,sizeof(sin));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(atoi(argv[1]));
	sin.sin_family = AF_INET;
	
	/* nommage */
	if(bind(sc,(struct sockaddr*)&sin,sizeof(sin)) < 0){
		perror("bind");
		exit(0);
	}
	listen(sc,5);
	
	for(;;){
	
	if((scom = accept(sc,(struct sockaddr*)&exp,&fromlen)) == -1){
		perror("accept");
		exit(0);
	}
	/*processus par requete*/
	if(fork() == 0){
		/*reception du msg*/
		char msg[1024];
		if(read(scom, &msg, 1024) < 0){
			perror("read");
			exit(0);
		}
		/*traitement*/
		/*creation du fichier*/
				printf("%s\n",msg);
		if((fd = open(msg,O_RDWR | O_TRUNC | O_CREAT, 0600)) == -1){
       perror(argv[1]);
       exit(0);
    }
    
    while(read(scom, &msg, 1024) > 0){
			write(fd,msg,strlen(msg));	
	}
}
}
close(fd);
shutdown(scom, 2);
close(scom);	
close(sc);
return EXIT_SUCCESS;
}
