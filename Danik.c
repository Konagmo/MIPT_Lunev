#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/dir.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <errno.h>

#define CONS_AV 0
#define PROD_AV 1
#define ITER_END 2

long int produser_sum = 0;
long int consumer_sum = 0;
int max_queue_len = 0;
int tmp_queue_len = 0;

int SEM_OP(int sem, int num, int opt) {
    struct sembuf sops;
    sops.sem_num = (unsigned short) num;
    sops.sem_op = (short) opt;
    sops.sem_flg = 0;
    int ret = semop(sem, &sops, 1);
    return ret;
}

struct msgbuf {
    long mtype;
    char mtext[7];
};

static void send_msg(int qid, int msgtype, int num) {
    struct msgbuf msg;

    msg.mtype = msgtype;

    sprintf(msg.mtext, "%d", num);

    if (msgsnd(qid, (void *) &msg, sizeof(msg.mtext), IPC_NOWAIT) == -1) {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }

    printf("message sent: %s\n", msg.mtext);
    produser_sum += num;
    tmp_queue_len++;
    if(tmp_queue_len > max_queue_len)
        max_queue_len = tmp_queue_len;
}

static void get_msg(int qid, int msgtype) {
    struct msgbuf msg;

    if (msgrcv(qid, (void *) &msg, sizeof(msg.mtext), msgtype, MSG_NOERROR | IPC_NOWAIT) == -1) {
        if (errno != ENOMSG) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        printf("No message available for msgrcv()\n");
    } else
        printf("message received: %s\n", msg.mtext);
    consumer_sum += atoi(msg.mtext);
    tmp_queue_len--;
}

void produse_item(int msgid, int semid) {
    SEM_OP(semid, CONS_AV, +1);
    SEM_OP(semid, PROD_AV, -1);
    SEM_OP(semid, ITER_END, -1);
    send_msg(msgid, 1, rand());
}

void consume_item(int msgid, int semid) {
    SEM_OP(semid, CONS_AV, -1);
    SEM_OP(semid, PROD_AV, +1);
    get_msg(msgid, 1);
}



int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a > b ? b : a;
}

int main(int argc, char* argv[]) {
    if(argc < 5) {
        perror("Not enough arguments!\n");
        goto out;
    }
    int i = 0;

    int semid = semget(IPC_PRIVATE, 3, IPC_CREAT|IPC_EXCL|0666);
    int msgid = msgget(IPC_PRIVATE, IPC_CREAT|IPC_EXCL);

    int n_produser = atoi(argv[1]);
    int n_consumer = atoi(argv[2]);
    int len_queue = atoi(argv[3]);
    int n_iter = atoi(argv[4]);

    SEM_OP(semid, CONS_AV, +1);
    SEM_OP(semid, PROD_AV, +len_queue);
    SEM_OP(semid, ITER_END, +n_iter);

    printf("okay in 114!\n");
    pid_t child = fork();
    for(i = 0; i < n_consumer; i++){
        
        if(!child){
            while(1){
                consume_item(msgid, semid);
            }
        }
        //wait(NULL);
    }
    printf("okay in 123!\n");
    for(i = 0; i < n_produser; i++){
        if(!child){
            while(1){
                produse_item(msgid, semid);
            }
            exit(0);
        }
        wait(NULL);
    }

    printf("okay in 132!\n");

    msgctl(msgid, 0, IPC_RMID);
    semctl(semid, 0, IPC_RMID);
    printf("Producer_sum = %ld\n", produser_sum);
    printf("Consumer_sum = %ld\n", consumer_sum);
    printf("Max queue sum = %d\n", max_queue_len);
out:
    return 0;
}
