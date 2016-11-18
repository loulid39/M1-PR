#include "chat_common.h"

struct myshm *msg;
char *id;
char *mem;
char *id_srv;


void init_m(){
int fd,fd1;

if((fd = shm_open(id,O_RDWR | O_CREAT, 0600)) == -1){
	perror("shm_open");
	exit(1);
}

if(ftruncate(fd,10) == -1){
	perror("ftruncate");
	exit(0);
}

if((mem = (char*)mmap(NULL,10,PROT_READ | PROT_WRITE, MAP_SHARED,fd,0)) == MAP_FAILED){
	perror("mmap");
	exit(0);
}

if((fd1 = shm_open(id_srv,O_RDWR , 0600)) == -1){
	perror("shm_open");
	exit(1);
}

if(ftruncate(fd1,sizeof(struct myshm)) == -1){
	perror("ftruncate");
	exit(0);
}

if((msg = mmap(NULL,sizeof(struct myshm),PROT_READ | PROT_WRITE, MAP_SHARED,fd1,0)) == MAP_FAILED){
	perror("mmap");
	exit(0);
}
strcpy(mem , "tmp");
}

void free_m(){
	munmap(mem,10);
	shm_unlink(id);
}

void sig_hand(int sig){
struct message *m = malloc(sizeof(struct message));
	m->type = 3;
	strcpy(m->content ,id);
	sem_wait(&(msg->sem));
	msg->messages[msg->write] = *m;
	msg->write++;
	msg->nb++;
	sem_post(&(msg->sem));
	free_m();
	printf("\nFin Client\n");
exit(0);
}

void *func_em(void *arg){
	while(1){
		struct message *m = malloc(sizeof(struct message));
		printf("\n1 : Connexion\n");
		printf("2 : Diffusion\n");
		printf("Ctrl + c : Deconnexion\n");
		printf("> ");
		scanf("%ld",&m->type);
		if(m->type == 2){
			printf("contenu > ");
			scanf("%s",m->content);
		} else strcpy(m->content,id);
		sem_wait(&(msg->sem));
		msg->messages[msg->write] = *m;
		msg->write++;
		msg->nb++;
		sem_post(&(msg->sem));
	sleep(1);
	}
}

void *func_rec(void *arg){
	char *tmp = "tmp";
	
	while(1){
		if(strcmp(mem, tmp) != 0){
			printf(" message recu : %s\n",mem);
			strcpy(mem, tmp);
		} 
		sleep(2);
		
	}
}

int main(int argc,char **argv){
	if(argc != 3){
		printf("erreur argument\n");
		exit(0);
	}
	id = argv[1];
	id_srv = argv[2];
	pthread_t t_em,t_rec;
	init_m();
	sigset_t sig_proc;
	struct sigaction action;
	sigemptyset(&sig_proc);
	/* changer le traitement */
	action.sa_mask=sig_proc;
	action.sa_flags=0;
	action.sa_handler = sig_hand;
	sigaction(SIGINT, &action,NULL);
	int n = 3;
	if(pthread_create(&t_em,NULL,func_em,&n) != 0){
		perror("pthread_create");
		exit(0);
	} 
		
	if(pthread_create(&t_rec,NULL,func_rec,NULL) != 0){
		perror("pthread_create");
		exit(0);
	}
	
	pthread_join(t_em,NULL);
	pthread_join(t_rec,NULL);

	/*printf("type : %ld, contenu : %s\n",msg->messages[msg->nb].type,msg->messages[msg->nb].content);*/

return EXIT_SUCCESS;
}
