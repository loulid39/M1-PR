#define _SOURCE_POSIX 4
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <time.h>


static void interrupt_signal(int signo, siginfo_t *si, void *context){
}


int main(int argc, char **argv){
  if(argc != 2){
    printf("manque des arguments\n");
    exit(0);
  }
  int i,N = atoi(argv[1]);
  pid_t *pid = malloc(N * sizeof(pid_t)),tpid;
  int mysig = 0;
  int ordre;
  union sigval val;
  sigset_t block_mask;
  siginfo_t si;
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
      srand(time(NULL)*i);
        val.sival_int = (int) (10*(float)rand()/ RAND_MAX);
        if(sigqueue(getppid(),SIGRTMIN, val) == -1){
          perror("sigqueue");
          exit(0);  
        }
        exit(0);
      }
      }
  int total = 0;
  for(i=0;i<N;i++){
    if(sigwaitinfo(&block_mask, &si) == -1){
          perror("sigwait");
          exit(0);
        }
        total += si.si_value.sival_int;
  }
  
  for (int i = 0; i < N; ++i)
  {
    wait(NULL);
  }
  printf("somme : %d\n",total);
  printf("Fin\n");
  return EXIT_SUCCESS;
}