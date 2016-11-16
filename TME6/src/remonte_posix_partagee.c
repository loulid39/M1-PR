#define _XOPEN_SOURCE 700

#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>

#define DESC_SHM(id, arg)\
	id = shm_open(arg,O_RDWR | O_CREAT,0600) 
  
#define CTRL_DESC(id,n)\
	id == -1 ? 1 : 0
	
#define ALLOC_SHM(ptr, id, n)\
	ftruncate(id,n * sizeof(int));\
	ptr[0] = mmap(NULL,n * sizeof(int*),PROT_READ | PROT_WRITE, MAP_SHARED,id,0)
	
#define CTRL_SHM(ptr)\
	ptr == NULL ? 1 : 0

#define SET_SHM(ptr, id, i,val)\
	ptr[i] = val\
	

#define GET_SHM(res, ptr,i, id)\
	res = ptr[i]

#define FREE_SHM(id, ptr, n, arg)\
	munmap(ptr,n * sizeof(int));\
	shm_unlink(arg)
 	

int main(int argc, char *argv[]){

  int id, n, i, total, res;
  int **ptr = NULL;
  int *pids;
  
  n = (argc < 2) ? 0 : strtol(argv[1], NULL, 10);
  if (n <= 0) {
    fprintf(stderr, "Usage: %s nombre\n", argv[0]);
    exit(EXIT_FAILURE);
  }
*ptr = malloc(3 * sizeof(int));
  /* Demonde un descripteur de ressource partagee, eventuellement deja creee. */
  DESC_SHM(id, argv[2]);

  /* Controle du retour du descripteur */
  if (CTRL_DESC(id, n)) {
    perror("Echec de l'allocation du descripteur\n");
    exit(errno);
  }

  /* Allocation */
  ALLOC_SHM(ptr, id, n);
    printf("ppppppppp1\n");
  /* Controle du retour de l'allocation */
  if (CTRL_SHM(ptr[0])) {
    fprintf(stderr, "Echec de l'allocation de la ressource partagee\n");
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
      srand(time(NULL)*i);
      /* Ecriture dans la ressource partagee */
      SET_SHM(ptr[0], id, i, (int) (10*(float)rand()/ RAND_MAX));
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
    GET_SHM(res, ptr[0],i, id);
    printf("pid %d envoie %d\n", pids[i], res);
    total += res;
  }

  free(pids);

  printf("total: %d\n", total);
	FREE_SHM(id, ptr[0], n, argv[2]);
  /* Liberation de la ressource */
  /*if (FREE_SHM(id, ptr, n, argv[2])){
    fprintf(stderr, "Ressource partagee mal rendue\n");
    exit(EXIT_FAILURE);
  }*/
  return EXIT_SUCCESS;
}
