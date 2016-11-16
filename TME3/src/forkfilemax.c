#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include<signal.h>

void sig_hand_f(int sig){

}

int main (void) {
    int fd1, fd2, fd3, pid;
    sigset_t sig;
    struct sigaction action;
    
  sigemptyset(&sig);
	action.sa_mask=sig;
	action.sa_flags=0;
	action.sa_handler = sig_hand_f;
	sigaction(SIGUSR1, &action,NULL);
	sigaddset (&sig, SIGUSR1);
	sigprocmask (SIG_SETMASK,&sig, NULL);
	sigfillset (&sig);
	sigdelset (&sig, SIGUSR1);
	
    if ((fd1 = open ("./fich1", O_RDWR| O_CREAT | O_TRUNC, 0600)) == -1) {
      perror("./fich1");
      return errno;
    }

    if (write (fd1,"abcde", strlen ("abcde")) == -1) {
      perror("./fich1");
      return errno;
    }
    if ((pid = fork ()) == 0) {
        if ((fd2 = open ("./fich1", O_RDWR)) == -1) {
	  perror("./fich1");
	  return errno;
	}
	kill(getppid(),SIGUSR1);
	
        if (write (fd1,"123", strlen ("123")) == -1) {
	  perror("./fich1");
	  return errno;
	}
	sigsuspend(&sig);
        if (write (fd2,"45", strlen ("45")) == -1) {
	  perror("./fich1");
	  return errno;
	}
        close(fd2); 
    } else {
    sigsuspend(&sig);
        fd3 = dup(fd1);
        if (lseek (fd3,0,SEEK_SET) == -1) {
	  perror("./fich1");
	  return errno;
	}
	kill(pid,SIGUSR1);
        if (write (fd3,"fg", strlen ("fg")) == -1) {
	  perror("./fich1");
	  return errno;
	}
	
	if (write (fd1,"hi", strlen ("hi")) == -1)  {
	  perror("./fich1");
	  return errno;
	}
	
        wait (NULL);
        close (fd1);
        close(fd3);
        return EXIT_SUCCESS;
    }
}
