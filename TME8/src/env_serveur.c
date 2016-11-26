#define _XOPEN_SOURCE 700
/*UDP - Server
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <time.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char **argv){
struct msg{
	char type;
	char id[30];
	char val[30];
};
	struct sockaddr_in sin;
	struct sockaddr_in exp;
	char host[64];
	int sc;
	int fromlen = sizeof(exp);
	char message[80],op,id[10],val[10];
	int cpt = 0;
	/*creation de la socket*/
	if((sc = socket(AF_INET,SOCK_DGRAM,0)) < 0){
		perror("socket");
		exit(0);
	}
	/*remplir le nom*/
	memset((char*)&sin,0,sizeof(sin));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(atoi(argv[1]));
	sin.sin_family = AF_INET;
	/*nommage*/
	if(bind(sc,(struct sockaddr*)&sin,sizeof(sin)) < 0){
		perror("bind");
		exit(0);
	}
	
	struct msg m;
	printf("-------------- Server ------------\n");
	/*receprion du message*/	
while(1){
		if(recvfrom(sc,&m,sizeof(m),0,(struct sockaddr*)&exp,&fromlen) == -1){
		perror("recform");
		exit(0);
	}
	/*traitement*/
	printf("--------------  ------------\n");
	if(m.type == 'S'){
		printf("requete Set\n");
		setenv(m.id,m.val,0);

	} 
	if(m.type == 'G') {
	printf("requete Get \n");
		strcpy(m.val , getenv(m.id));
		printf("%s\n",getenv(m.id));
		if(sendto(sc,&m,sizeof(m)+1,0,(struct 	sockaddr*)&exp,sizeof(exp)) == -1){
		perror("sendto");
		exit(0);
	}
	}
	}

	close(sc);
	
return EXIT_SUCCESS;
}
