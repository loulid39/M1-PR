#define _XOPEN_SOURCE 700

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
#include <sys/socket.h>

int main(int argc, char** argv){
if(argc != 3){
	printf("erreur arguments\n");
	exit(0);
}
	int i,total,n;
	n = atoi(argv[2]);
	char *sname = argv[1];
	char msg[50];
	pid_t *pids;
	pids = malloc(n * sizeof(pid_t));
	int s,s_c;
	

	if((s = socket(AF_UNIX | AF_LOCAL, SOCK_DGRAM, 0)) == -1){
		perror("socket");
		exit(0);	
	} 

	struct sockaddr_un addr;
	memset(&addr,'\0', sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, sname);

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) == -1){
		perror("Erreur au nommage");
		exit(2);
		} 
		

 for(i=0; i<n; i++){
   int pid = fork();
   if (pid == -1){
     perror("fork");
     return -1;
   } else if (pid) {
     pids[i] = pid;
   } else {
     srand(time(NULL)*i);
     int nb =  (int) (10*(float)rand()/ RAND_MAX);
     sprintf(msg, "%d", nb);
     if(sendto(s,msg,strlen(msg)+1,0,(struct sockaddr *)&addr, sizeof(addr)) < 0){
     	perror("send");
     	exit(0);
     }
     printf("[%d] : %d\n",getpid(),nb);
     exit(EXIT_SUCCESS);
   }
 }
 for(i=0; i<n; i++){
   waitpid(pids[i], NULL, 0);
 }

 total = 0;

 for(i=0; i<n; i++){
   /* reception des messages */
	if((recvfrom(s,msg,sizeof(msg),0,NULL, NULL)) == -1){
	perror("recvfrom");
	exit(0);
	}
	total += atoi(msg);
 }


printf("total: %d\n", total);
close(s);
unlink(sname);

return EXIT_SUCCESS;
}




