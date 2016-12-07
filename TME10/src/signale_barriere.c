#define _SOURCE_POSIX 4
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

int N;
siginfo_t si;
sigset_t block_mask;
union sigval val;

static void interrupt_signal(int signo, siginfo_t *si, void *context){
}

void wait_barrier(int ordre){

  if(ordre == (N -1)){
    val.sival_int = (int) getppid();
    kill(getppid(),SIGRTMIN);    
  } else {
        if(sigwaitinfo(&block_mask, &si) == -1){
          perror("sigwait");
          exit(0);
        }
  }
}

void process(int N){
  printf("avant barriere\n");
  wait_barrier(N);
  printf("apres barriere\n");
  exit(0);
}

int main(int argc, char **argv){
  if(argc != 2){
    printf("manque des arguments\n");
    exit(0);
  }
  int i;
  N = atoi(argv[1]);
  pid_t *pid = malloc(N * sizeof(pid_t)),tpid;
  int mysig = 0;  
  
  struct sigaction action;
  memset (&action, '\0', sizeof(action));

  action.sa_sigaction = &interrupt_signal;
  action.sa_flags = SA_SIGINFO;
  sigfillset(&block_mask);
  action.sa_mask = block_mask;
  sigaction(SIGRTMIN, &action, 0);
  sigprocmask(SIG_SETMASK, &block_mask, 0);
  sigemptyset(&block_mask);
  sigaddset(&block_mask, SIGRTMIN);

  for(i=0;i<N;i++){
    if((tpid = fork()) == 0){
        process(i);
        exit(0);
      } else pid[i] = tpid;
      } 
  
  if(sigwaitinfo(&block_mask, &si) == -1){
    perror("sigwait");
    exit(0);
   }

  for(i=0;i<(N-1);i++){    
     /*if(sigqueue(pid[i],SIGRTMIN, val) == -1){
          perror("sigqueue");
          exit(0);  
        } */  
    kill(pid[i],SIGRTMIN);
  }
  
  for (int i = 0; i < N; ++i)
  {
    wait(NULL);
  }
  
  return EXIT_SUCCESS;
}