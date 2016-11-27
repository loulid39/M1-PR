#define _XOPEN_SOURCE 700

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<netdb.h>
#include<time.h>
#include<sys/un.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>

#define MSIZE 128

int main(int argc, char **argv){
	if(argc != 3){
		perror("manque des arguments");
		exit(0);
	}
	char msg[MSIZE];
	int sock;
	char* HOST = argv[1];
	int PORT = atoi(argv[2]);
	struct sockaddr_in dest;
	struct addrinfo *result;	

	/*creation de la socket*/
	if((sock= socket(AF_INET,SOCK_STREAM,0)) == -1){
		perror("sock");
		exit(0);
	}
	/*init struct dest*/
	if(getaddrinfo(HOST,NULL,NULL,&result) != 0){
		perror("addrinfo");
		exit(0);
	}
	
	memset((char*)&dest,0,sizeof(dest));
	memcpy((void*)&dest.sin_addr,(void*)&((struct 				sockaddr_in*)result->ai_addr)->sin_addr,sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(PORT);
	
	/*connection au srv*/
	if(connect(sock,(struct sockaddr*)&dest,sizeof(dest)) == -1){
		perror("connect");
		exit(0);
	}
	
	int fd;
	/*preparer les requetes*/
	printf("Enter \n");
	printf("1: list des fichiers\n");
	printf("2: telecharger un fichier\n");
	printf("3: envoyer un fichier\n");
	printf("> ");
	char c;
	fflush(stdin);
	scanf("%c",&c);
	if(c == '1'){
	strcpy(msg,"LIST");
	
	/*evoie de requete*/
	if(write(sock,msg,sizeof(msg)) == -1){
		perror("write");
		exit(0);	
	}
	
	/*reception reponse du serveur*/
	if(read(sock, &msg, MSIZE) < 0){
			perror("read");
			exit(0);
		}
		printf("nombre des fichiers : %s\n",msg);
	int i,nf = atoi(msg);
	for(i=0;i<nf;i++){
		if(read(sock, &msg, MSIZE) < 0){
			perror("read");
			exit(0);
		}
		printf("- %s\n",msg);
	}
	}

	printf("> ");
	fflush(stdin);
	scanf(" %c",&c);	
	if(c == '2'){
		strcpy(msg,"DOWNLOAD");
		if(write(sock,msg,sizeof(msg)) == -1){
			perror("write");
			exit(0);	
		}
		printf("nom fichier : ");
		scanf(" %s",msg);
		if(write(sock,msg,sizeof(msg)) == -1){
			perror("write");
			exit(0);	
		}
		fd = (open(msg,O_RDWR | O_TRUNC | O_CREAT, 0600));
		while(read(sock, &msg, MSIZE) > 0){
			write(fd,msg, strlen(msg));
		}		
		
	} else if(c == '3'){
		strcpy(msg,"UPLOAD");
		if(write(sock,msg,sizeof(msg)) == -1){
			perror("write");
			exit(0);	
		}
		printf("nom fichier : ");
		scanf(" %s",msg);
		if(write(sock,msg,sizeof(msg)) == -1){
			perror("write");
			exit(0);	
		}
		fd = open(msg, O_RDWR, 0600);
		while(read(fd,msg,sizeof(msg)) > 0){
			if(write(sock, msg, strlen(msg)) == -1){
				perror("write");
				exit(0);
			 }
		}	
		
	}
	close(fd);
	shutdown(sock, 2);
	close(sock);
return EXIT_SUCCESS;
}
