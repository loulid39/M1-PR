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

#define MSGSIZE 10

#define DESC_SHM(id, arg, attr)\
	id = mq_open(arg,O_RDWR | O_CREAT,0600,attr);\
  
#define CTRL_DESC(id,n)\
	id == -1 ? 1 : 0
	
#define ALLOC_SHM(ptr, id, n)\

	
#define CTRL_SHM(ptr)\
	if(ptr == NULL) ? 1 : 0

#define SET_SHM(ptr, id, i,val)\
	char m[3];\
	sprintf(m,"%d",val);\
	mq_send(id,m,strlen(m),0)
	

#define GET_SHM(res, ptr,i, id)\
	char buf[MSGSIZE];\
	mq_receive(id,buf,MSGSIZE,NULL);\
	res = atoi(buf)

#define FREE_SHM(id, ptr, n, arg)\
	mq_close(id);\
	mq_unlink(arg);
 	

int main(int argc, char *argv[]){

  int n, i, total, res;
  int *ptr = NULL;
  int *pids;
  mqd_t id;
  n = (argc < 2) ? 0 : strtol(argv[1], NULL, 10);
  if (n <= 0) {
    fprintf(stderr, "Usage: %s nombre\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  struct mq_attr attr;
	attr.mq_flags = 0;  
	attr.mq_maxmsg = n;  
	attr.mq_msgsize = MSGSIZE;  
	attr.mq_curmsgs = n;

  /* Demonde un descripteur de ressource partagee, eventuellement deja creee. */
  DESC_SHM(id, argv[2],&attr);

  /* Controle du retour du descripteur */
  if (CTRL_DESC(id, n)) {
    perror("Echec de l'allocation du descripteur\n");
    exit(errno);
  }

  /* Allocation */
  ALLOC_SHM(ptr, id, n);

  /* Controle du retour de l'allocation */
  /*if (CTRL_SHM(ptr)) {
    fprintf(stderr, "Echec de l'allocation de la ressource partagee\n");
    exit(errno);
  }*/

  pids = malloc(n * sizeof(int));

  for(i=0; i<n; i++){
    int pid = fork();
    if (pid == -1){
      perror("fork");
      return -1;
    } else if (pid) {
      pids[i] = pid;
    } else {
      srand(time(NULL)*i);
      /* Ecriture dans la ressource partagee */
      SET_SHM(ptr, id, i, (int) (10*(float)rand()/ RAND_MAX));
      exit(EXIT_SUCCESS);
    }
  }
  for(i=0; i<n; i++){
    int status;
    waitpid(pids[i], &status, 0);
  }

  total = 0;

  for(i=0; i<n; i++){
    /* Lecture dans la ressource partagee */
    GET_SHM(res, ptr,i, id);
    printf("pid %d envoie %d\n", pids[i], res);
    total += res;
  }

  free(pids);

  printf("total: %d\n", total);
	FREE_SHM(id, ptr, n, argv[2]);
  /* Liberation de la ressource */
  /*if (FREE_SHM(id, ptr, n, argv[2])){
    fprintf(stderr, "Ressource partagee mal rendue\n");
    exit(EXIT_FAILURE);
  }*/
  return EXIT_SUCCESS;
}
