#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

struct msgbuf {
	long mtype;
	char mtext[80];
};

static void get_msg(int qid, int msgtype) {
	struct msgbuf msg;

	if (msgrcv(qid, (void *) &msg, sizeof(msg.mtext),
	    msgtype, MSG_NOERROR | IPC_NOWAIT) == -1) {
    	if (errno != ENOMSG) {
        	perror("msgrcv");
        	exit(EXIT_FAILURE);
    	}
    	printf("No message available for msgrcv()\n");
	} else
    	printf("pid %d was %s\n", getpid(), msg.mtext);

}

static void send_msg(int qid, int msgtype, int queue_id) {
	struct msgbuf msg;
	msg.mtype = msgtype;

	snprintf(msg.mtext, sizeof(msg.mtext), "%d",
             queue_id);

	if(msgsnd(qid, (void *) &msg, sizeof(msg.mtext),
		IPC_NOWAIT) == -1) {
		perror("msgsnd error");
		exit(EXIT_FAILURE);
	}
	//printf("snt %d by %s\n", getpid(), msg.mtext);
}

int main(int argc, char const *argv[]) {

	char *str, *endptr;
	int i;
	int parid = getpid();
	int num = 0;
	int wait_id = 0;
	int msgtype = 1;
	int qid;

	//printf("IPC_PRIVATE - %d\n", IPC_PRIVATE);
	//printf("IPC_CREAT - %d\n", IPC_CREAT);

	if(argc < 2) {
		printf("To few arguments\n");
		return 0;
	}

	str = argv[1];
	num = strtoll(str, &endptr, 10);
	
	for(i = 1; i <= num ; i++) {
		if(getpid() == parid) {
			fork();
			if(getpid() != parid) {	
				qid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
				send_msg(qid, msgtype, i);
			}
		}
	}
	//sleep(1);
	if(getpid() != parid)
		get_msg(qid, msgtype);
	return 0;
}