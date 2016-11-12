#define _XOPEN_SOURCE 700

#include<sys/mman.h>
#include<mqueue.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<time.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>

#define MSGSIZE 10

#define DESC_SHM(id, arg)\
struct mq_attr attr;\
	attr.mq_flags = 0;attr.mq_maxmsg = MSGSIZE;	attr.mq_msgsize = MSGSIZE;attr.mq_curmsgs = MSGSIZE;\
  	id = mq_open(arg,O_RDWR | O_CREAT,0600,&attr);\
  	
#define DESC_SHMF(id, arg)\
	id = shm_open(arg,O_RDWR | O_CREAT,0600) 


#define CTRL_DESC(id,n)\
	id == -1 ? 1 : 0
	
#define ALLOC_SHM(ptr, id, n)\
	ftruncate(id,n * sizeof(int));\
	ptr = mmap(NULL,n * sizeof(int*),PROT_READ | PROT_WRITE, MAP_SHARED,id,0)
	
#define CTRL_SHM(ptr)\
	ptr == NULL ? 1 : 0
char m[3] = " ";
#define SET_SHM(ptr, id, i,val)\
	sprintf(m,"%d",val);\
	mq_send(id,m,strlen(m),0)

#define SET_SHMF(ptr, id, i,val)\
	ptr[i] = val\
	

#define GET_SHM(res, ptr,i, id)\
	char buf[MSGSIZE];\
	int yo = mq_receive(id,buf,MSGSIZE,NULL);\
	buf[yo] = '\0';\
	res = atoi(buf)

#define GET_SHMF(res, ptr,i, id)\
	res = ptr[i]

#define FREE_SHM(id, ptr, n, arg)\
	mq_close(id);\
	mq_unlink(arg);
 	
void handler(int sig) {

}

int main(int argc, char *argv[]){

  int n, i, total, res;
  int **ptr = NULL;
  int *pids;
  int *id;
  mqd_t idm;
  sigset_t sig;
  struct sigaction action;
  
  n = (argc < 2) ? 0 : strtol(argv[1], NULL, 10);
  if (n <= 0) {
    fprintf(stderr, "Usage: %s nombre\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  sigemptyset(&sig);
  action.sa_mask=sig;
  action.sa_flags=0;
  action.sa_handler = handler;
  sigaction(SIGUSR1,&action,NULL);
  sigaddset(&sig,SIGUSR1);
  sigprocmask(SIG_SETMASK,&sig,NULL);
  sigfillset(&sig);
  sigdelset(&sig,SIGUSR1);
  
	id = malloc(n * sizeof(int));
	ptr = malloc(n * sizeof(int));

  /* Demonde un descripteur de ressource partagee, eventuellement deja creee. */
  DESC_SHM(idm, argv[2]);
  for(i=0;i<n;i++){
		char nom[4];
		nom[0] = '/';
		nom[1] = i + '0';
		DESC_SHMF(id[i], nom);
  }


  /* Controle du retour du descripteur */

  if (CTRL_DESC(idm, n)) {
    perror("Echec de l'allocation du descripteur\n");
    exit(errno);
  }



  pids = malloc(n * sizeof(int));

  for(i=0; i<n; i++){
    int pid = fork();
    if (pid == -1){
      perror("fork");
      return -1;
    } else if (pid) {
      pids[i] = pid;
    } else {
    	int pos = i;
      srand(time(NULL)*i);
      int max_msg = (int) (n*(float)rand()/ RAND_MAX);
      if(max_msg == 0) max_msg++;
        //ptr[pos] = malloc(max_msg * sizeof(int));

           
        /* Allocation */
  ALLOC_SHM(ptr[pos], id[pos], max_msg);
   if (CTRL_SHM(ptr[pos])) {
    fprintf(stderr, "Echec de l'allocation de la ressource partagee\n");
    exit(errno);
  } 
          printf(":*%d: dmd %d\n",pos,max_msg);
      SET_SHM(ptr[pos], idm, i, max_msg);
  /* Controle du retour de l'allocation */
    printf("ppppppppp1\n");
       int total = 0,res=0;
	sigsuspend(&sig);
  for(i=0; i<max_msg; i++){
    /* Lecture */
    GET_SHMF(res, ptr[pos],i, id[pos]);
    total += res;
    printf("%d , ",res);

  }
      printf("[%d] ****** total %d\n",pos,total);
      exit(EXIT_SUCCESS);
    }
  }


sleep(3);
  for(i=0; i<n; i++){
    /* Lecture dans la ressource partagee */
    GET_SHM(res, ptr,i, idm);
    int y;
    srand(time(NULL)*res*i);
       printf("sig env %d\n",res);
    for(y=0;y<res;y++){
      int msg = (int) (100*(float)rand()/ RAND_MAX); 
      SET_SHMF(ptr[i], id[i], i, msg);
             
    }
    
    kill(pids[i],SIGUSR1);

    }

  for(i=0; i<n; i++){
    int status;
    waitpid(pids[i], &status, 0);
  }
  
  free(pids);


	FREE_SHM(idm, ptr, n, argv[2]);
	 for(i=0;i<n;i++){
  char nom[4];
	nom[0] = '/';
	nom[1] = i + '0';
  FREE_SHM(id[i], ptr, n, nom);
  
  }
  printf("\n\nFin\n");
  return EXIT_SUCCESS;
}
