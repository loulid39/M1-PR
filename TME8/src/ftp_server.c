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
#include<sys/stat.h>

#include <dirent.h>

#define MSIZE 128

void list_file(char *dir, int scom){
	char msg[MSIZE];
	DIR *pt_Dir;
	struct dirent* dirEnt;
	struct stat statbuf;
	if ((pt_Dir = opendir(dir)) == NULL) {
		perror("opendir");
		exit(0);
	}
	
	while((dirEnt = readdir(pt_Dir)) !=NULL){
		strcpy(msg,dirEnt->d_name);
		if(write(scom,msg,sizeof(msg)) < 0){
			perror("write");
			exit(0);
		}
	}
	closedir(pt_Dir);

}

int nb_file(char *dir){
	int n = 0,i;
	DIR *pt_Dir;
	struct dirent* dirEnt;
	struct stat statbuf;
	if ((pt_Dir = opendir(dir)) == NULL) {
		perror("opendir");
		exit(0);
	}

	while((dirEnt = readdir(pt_Dir)) !=NULL){
		n++;
	}
	closedir(pt_Dir);
	return n;
}

int main(int argc,char **argv){
	if(argc != 3){
		perror("manque des arguements");
		exit(0);
	}
	char msg[MSIZE];
	struct sockaddr_in sin;
	struct sockaddr_in exp;
	int sc,scom;
	int PORT = atoi(argv[1]),fromlen;
	struct hostent *hp;
	/* reation de la socket */
	if((sc = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket");
		exit(0);
	}
	memset((char*)&sin,0,sizeof(sin));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PORT);
	sin.sin_family = AF_INET;
	/*nommage*/
	if(bind(sc,(struct sockaddr*)&sin,sizeof(sin)) < 0){
		perror("bind");
		exit(0);
	}
	/*creation de la fille d'attente, 1 client au max dans cette exempple*/
	listen(sc,1);
	
	/*Serveur va boucler infiniment */
	for(;;){
		/*attente la connexion d'un client => bloquant*/
		if((scom = accept(sc,(struct sockaddr*)&exp,&fromlen)) == -1){
			perror("accept");
			exit(0);
		}	
		
		/*attendre premiere requete*/
		if(read(scom, &msg, MSIZE) < 0){
			perror("read");
			exit(0);
		}
		printf("requete : %s\n",msg);
		if(strcmp(msg,"LIST") == 0){
		int nb_f = nb_file(argv[2]);
		sprintf(msg,"%d",nb_f);
		if(write(scom,msg,sizeof(msg)) < 0){
			perror("write");
			exit(0);
		}
		list_file(argv[2],scom);
		}
		int fd;
		char * di;
		/*attente upload ou download*/
		if(read(scom, &msg, MSIZE) < 0){
			perror("read");
			exit(0);
		}
		if(strcmp(msg,"UPLOAD") == 0){
			/*attendre le nom de fichier*/
			if(read(scom, &msg, MSIZE) < 0){
				perror("read");
				exit(0);
			}
			printf("UPLOAD : %s\n",msg);
			di = argv[2];
			strcat(di,"/");
			strcat(di,msg);
			printf("%s\n",di);
			fd = open(di,O_RDWR | O_TRUNC | O_CREAT, 0600);
			while(read(scom, &msg, MSIZE) > 0){
				write(fd,msg, strlen(msg));
			}
		}
		if(strcmp(msg,"DOWNLOAD") == 0){
			/*attendre le nom de fichier*/
			if(read(scom, &msg, MSIZE) < 0){
				perror("read");
				exit(0);
			}		
			di = argv[2];
			strcat(di,"/");
			strcat(di,msg);	
			printf("DOWNLOAD : %s\n",msg);
			fd = open(di, O_RDWR, 0600);
			while(read(fd,msg,sizeof(msg)) > 0){
				if(write(scom, msg, strlen(msg)) == -1){
					perror("write");
					exit(0);
				}
			}
		}
		close(fd);
		shutdown(scom, 2);
		close(scom);
		
	}
	


return EXIT_SUCCESS;
}
