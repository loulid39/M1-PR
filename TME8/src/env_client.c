#define _XOPEN_SOURCE 700
/* UDP - CLient
prob : marche 1 seul fois :(, faut redemarrer 
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
	int rep;
	struct sockaddr_in dest;
	int sock;
	int fromlen = sizeof(dest);
	struct addrinfo *result;
	
	if((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1){
		perror("socket");
		exit(0);
	}
	
	/*init struct dest, init addresse*/
	if(getaddrinfo(argv[1],NULL,NULL,&result) != 0){
		perror("addrinfo");
		exit(0);
	}
	
	memset((char*)&dest,0,sizeof(dest));
	memcpy((void*)&dest.sin_addr,(void*)&((struct sockaddr_in*)result->ai_addr)->sin_addr,sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(atoi(argv[2]));
	struct msg m;

	printf("----------------- Client -------------\n");

	printf("Type : ");
	m.type = getchar();
	if(m.type == 'Q'){
		close(sock);
		exit(0);
	}
	
	printf("Id : ");
	scanf("%s",m.id);
	if(m.type == 'S'){
		printf("Val : ");
		scanf("%s",m.val);
	}

		if(sendto(sock,&m,sizeof(m)+1,0,(struct sockaddr*)&dest,sizeof(dest)) == -1){
		perror("sendto");
		exit(0);
	}
	if(m.type == 'G'){
	int ns;
	if(recvfrom(sock,&m,sizeof(m),0,(struct sockaddr*)&dest,&ns) == -1){
		perror("recform");
		exit(0);
	}
	printf("valeur : %s\n",m.val);
	}


return EXIT_SUCCESS;
}
