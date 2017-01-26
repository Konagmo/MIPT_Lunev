#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct MSG {
	long type;
	long num;
};

int main(int argc, char const *argv[]) {

	if(argc < 2) {
		printf("To few arguments\n");
		return 0;
	}

	char *end_ptr;
	int par_id = getpid();
	int msgid;
	int num = strtoll(argv[1], &end_ptr, 10);

	msgid = msgget(ftok(argv[0], getpid()), 0666 | IPC_CREAT);
	int msg_size = sizeof(struct MSG) - sizeof(long);

	int i = 1;
	int rm_flag = 0;
	int pid = 1;

	for(; i <= num; i++) {
		if(getpid() == par_id) {
			if((pid = fork()) < 0) {
				msgctl(msgid, IPC_RMID, NULL);
				break;
			}
			if(pid == 0)
				break;
		}
	}

	if(i == num && pid == 0) {
		struct MSG msg = {1, num + 1};
		if(msgsnd(msgid, &msg, msg_size, MSG_NOERROR) == -1) {
			perror("Error in sending");
			return 2;
		}
	}


	struct MSG msg;
	if((msgrcv(msgid, &msg, msg_size, i, MSG_NOERROR)) == -1) {
		perror("Error in receive");
		return 2;
	}

	if(pid == 0) {
		printf("%d ", i);
		fflush(stdout);
	}

	if(i < msg.num) {
		msg.type++;
		if(msgsnd(msgid, &msg, msg_size, MSG_NOERROR) == -1) {
			perror("Error in sending");
			return 2;
		}
	}
	else if(i == msg.num) {
		msgctl(msgid, IPC_RMID, NULL);
	}
	if(pid != 0)
		printf("\n");
	return 0;
}