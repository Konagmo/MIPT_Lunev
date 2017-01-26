#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


int return_symb = 0, cnt = 128;
pid_t pid;

void childExit(int signo) {
    exit(EXIT_SUCCESS);
}

void parentExit(int signo) { 
    exit(EXIT_SUCCESS);
}

void empty(int signo) {}

void oneTask(int signo) {
    return_symb += cnt;
    cnt /= 2;	
    kill(pid, SIGUSR1);
}

void nullTask(int signo) { 
    cnt /= 2;	
    kill(pid, SIGUSR1);
}


int main(int argc, char ** argv) {
    if (argc != 2) {
        fprintf(stderr, "Invalid number of arguments!");
        exit(EXIT_FAILURE);
    }
    pid_t parpid = getpid(); 

    struct sigaction exit_task;
    struct sigaction one_task;
    struct sigaction null_task;

    memset(&exit_task, 0, sizeof(exit_task));
    memset(&one_task,  0, sizeof(one_task));
    memset(&null_task, 0, sizeof(null_task));

    exit_task.sa_handler = childExit;
    one_task.sa_handler  = oneTask; 
    null_task.sa_handler = nullTask;

    sigfillset(&exit_task.sa_mask); 
    sigfillset(&one_task.sa_mask);
    sigfillset(&null_task.sa_mask);    

    sigaction(SIGCHLD, &exit_task, NULL); 
    sigaction(SIGUSR1, &one_task,  NULL);
    sigaction(SIGUSR2, &null_task, NULL);

    sigset_t set;
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigaddset(&set, SIGCHLD);

    sigprocmask(SIG_BLOCK, &set, NULL); /////////// 
    sigemptyset(&set);

    pid = fork();     /////

    if (pid == 0) {
        int fd = 0;
        char symb = 0;
        sigemptyset(&set); 

        struct sigaction empty_task;                    
        struct sigaction alarm_task;

        memset(&empty_task, 0, sizeof(empty_task));
        memset(&alarm_task, 0, sizeof(alarm_task));

        empty_task.sa_handler = empty;
        alarm_task.sa_handler = parentExit;

        sigfillset(&empty_task.sa_mask);    
        sigfillset(&alarm_task.sa_mask);

        sigaction(SIGUSR1, &empty_task, NULL);
        sigaction(SIGALRM, &alarm_task, NULL);

        if ((fd = open(argv[1], O_RDONLY)) < 0 ) {
            perror("Can't open file");
            exit(EXIT_FAILURE);
        }

        int i;

        while (read(fd, &symb, 1) > 0) {	
            alarm(1);           /////
            
            for (i = 128; i >= 1; i /= 2) {
                if (i & symb)               
                    kill(parpid, SIGUSR1);  ////
                else 
                    kill(parpid, SIGUSR2);  ////
            sigsuspend(&set);  ///////////      ////
            }
        }
        exit(EXIT_SUCCESS);
    }

    errno = 0;
  
    while (1) {	
        if(cnt == 0) {

            write(STDOUT_FILENO, &return_symb, 1);       
            fflush(stdout);
            cnt = 128;
            return_symb = 0;
        }
        sigsuspend(&set);  //////////
    }

    exit(EXIT_SUCCESS);
}