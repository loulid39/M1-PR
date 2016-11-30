#define _OPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
#include<netdb.h>
#include<sys/un.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>


int main(int argc, char	**argv){
	if(argc < 2){
		printf("manque des arguments\n");
		exit(0);
	}
	fd_set ens,old_ens;
	struct sockaddr_in *sin;
	struct sockaddr_in exp;
	int i,fd,y,T = (argc -1),fromlen,scom;
	int *sc = malloc(T * sizeof(int));
	
	sin = malloc(T * sizeof(struct sockaddr_in));
	FD_ZERO(&ens);
	FD_ZERO(&old_ens);
	for(i=0;i<(T);i++){
		if((sc[i] = socket(AF_INET,SOCK_STREAM,0)) < 0){
			perror("socket");
			exit(0);
		}
		memset((char*)&sin[i],0,sizeof(sin[i]));
		sin[i].sin_addr.s_addr = htonl(INADDR_ANY);
		sin[i].sin_port = htons(atoi(argv[i+1]));
		sin[i].sin_family = AF_INET;
		if(bind(sc[i],(struct sockaddr*)&sin[i],sizeof(sin[i])) < 0){
			perror("bind");
			exit(0);
		}
		listen(sc[i],1);
		FD_SET(sc[i],&ens);
	}
	old_ens = ens;
	char buf[20];
	if((fd = open("cx.log",O_CREAT | O_RDWR, 0600)) == -1){
		perror("open");
		exit(0);
	} 
	printf("***************** Server ***************\n");
	for(;;){
	if(select(sc[i-1]+1,&old_ens,NULL,NULL,NULL) == -1){
		perror("select");
		exit(0);
	}

		for(y=0;y<(argc - 1);y++)
	if(FD_ISSET(sc[y],&old_ens)) {
	old_ens = ens;
		if((scom = accept(sc[y],(struct sockaddr*)&exp,&fromlen))== -1){
			perror("accept");
			exit(0);
		}	
int por = ntohs(exp.sin_port);
		inet_ntop(AF_INET, &(exp.sin_addr),buf,20);
		printf("connection d'un client .....[%s:%d] \n",buf, por);
		char p[4];
		sprintf(p,"%d",por);
		strcat(buf,":");
		strcat(buf,p);
	strcat(buf,"\n");
		if(write(fd,buf,strlen(buf)) == -1){
			perror("write");
			exit(0);
		}
		close(scom);
		} 
	}
return EXIT_SUCCESS;
}
