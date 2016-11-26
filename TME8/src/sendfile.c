#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <time.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>



int main(int argc,char **argv){
	struct sockaddr_in dest;
	int sock;
	int fromlen = sizeof(dest);	
	struct addrinfo *result;
	int fd;
	
	if((sock= socket(AF_INET,SOCK_STREAM,0)) == -1){
		perror("sock");
		exit(0);
	}
	
	/*remplir la structure dest*/
if(getaddrinfo(argv[1],NULL,NULL,&result) != 0){
		perror("addrinfo");
		exit(0);
	}
	
	memset((char*)&dest,0,sizeof(dest));
	memcpy((void*)&dest.sin_addr,(void*)&((struct 				sockaddr_in*)result->ai_addr)->sin_addr,sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(atoi(argv[2]));
	
	/*connecter au srv*/
	if(connect(sock, (struct sockaddr*)&dest, sizeof(dest)) == -1){
		perror("connect");
		exit(0);
	}
	/*envoyer les msg*/
	/*envoie le nom du fichier*/
	char m[1024] ;
	strcpy(m, argv[3]);
	printf("%s\n",m);
	if(write(sock,m,sizeof(m)) == -1){
		perror("write");
		exit(0);	
	}
	fd = open(argv[3],O_RDWR,0600);
	/*envoie le contenu du fichier*/
	while(read(fd,m,sizeof(m)) > 0){

	if(write(sock,m,strlen(m)) == -1){
		perror("write");
		exit(0);	
	}
}
	close(fd);
	shutdown(sock,2);
	close(sock);
	
return EXIT_SUCCESS;
}
