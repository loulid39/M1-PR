#include "chat_common.h"

char *id;
struct myshm *msg;
char *clients[MAX_USERS];
char *clt_seg[MAX_USERS];

int indx_c = 0;

void init_m(){
int fd;
if((fd = shm_open(id,O_RDWR | O_CREAT, 0600)) == -1){
	perror("shm_open");
	exit(1);
}

if(ftruncate(fd,sizeof(struct myshm)) == -1){
	perror("ftruncate");
	exit(0);
}

if((msg = mmap(NULL,sizeof(struct myshm),PROT_READ | PROT_WRITE, MAP_SHARED,fd,0)) == MAP_FAILED){
	perror("mmap");
	exit(0);
}
msg->nb = 0;
msg->read = 0;
msg->write = 0;
sem_init(&(msg->sem),1,1);//un proc le possede a un instant !!
}

void free_m(){
	munmap(msg,sizeof(struct myshm));
	shm_unlink(id);
}

void connexion(char* id_c){
printf("> connexion : %s\n",id_c);
	int fd;
if((fd = shm_open(id_c,O_RDWR , 0600)) == -1){
	perror("shm_open");
	exit(1);
}

if(ftruncate(fd,10) == -1){
	perror("ftruncate");
	exit(0);
}

if((clt_seg[indx_c] = (char*)mmap(NULL,10,PROT_READ | PROT_WRITE, MAP_SHARED,fd,0)) == MAP_FAILED){
	perror("mmap");
	exit(0);
}
if(indx_c < MAX_USERS){
	clients[indx_c] = id_c;
	indx_c++;
}
}

void deconnexion(char* id_c){
printf("> deconnexion : %s\n",id_c);
int i,y,n = indx_c;
	for(i=0;i<n;i++){
		if(strcmp(clients[i],id_c) == 0){
			for(y=i;y<n;y++){
				clients[y] = clients[y+1];
				clt_seg[y] = clt_seg[y+1];
				indx_c--;
			}
				break;
		}
	}
}

void diffusion(char* mesg){
printf("> diffusion : %s\n",mesg);
	int i;

	for(i=0;i<indx_c;i++){
		strcpy(clt_seg[i] , mesg);
	}
}

void traiter_msg(struct message m){
	switch(m.type){
		case 1 :connexion(m.content);break;
		case 2 :diffusion(m.content);break;
		case 3 :deconnexion(m.content);break;
		};
}

void sig_hand(int sig){
	free_m();
	printf("Fin Server\n");
exit(0);
}

int main(int argc, char** argv){
	if(argc != 2){
		printf("erreur argument\n");
		exit(0);
	}
	int i,n,y;
	sigset_t sig_proc;
	struct sigaction action;
	id = argv[1];
	init_m();
	printf("------------Server--------------\n");
	sigemptyset(&sig_proc);
	/* changer le traitement */
	action.sa_mask=sig_proc;
	action.sa_flags=0;
	action.sa_handler = sig_hand;
	sigaction(SIGINT, &action,NULL);
	
	while(1){
		sem_wait(&(msg->sem));
		//printf("sem_wait %d ? %d\n",msg->nb,msg->read);
		if(msg->nb > msg->read){
			n = msg->nb - msg->read;
			y = msg->read;
			//printf("sem_post if %d ? %d [%d, %d]\n",msg->nb,msg->read,y,n);
			sem_post(&(msg->sem));
			struct message m = msg->messages[y];				
			traiter_msg(m);
			msg->read++;
			
		} else{
		sem_post(&(msg->sem));
}
		sleep(3);
	}
	
return EXIT_SUCCESS;
}
