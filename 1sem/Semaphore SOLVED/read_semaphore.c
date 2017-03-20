#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define SIZE 10000

int main(int argc, char const *argv[]) {
	if(argc != 1) {
		perror("Invalid number of arguments!");
		return -1;
	}
	char buf[SIZE];
	struct sembuf semops[6];

	int shmid;
	char* shmaddr;

	if((shmid = shmget(ftok("zz", SIZE), SIZE, 0777 | IPC_CREAT)) < 0) {
		perror("Error in shmget!");
		return -1;
	}

	if((shmaddr = shmat(shmid, 0, 0)) == (void*) -1) {
		perror("Error in shmat!");
		return -1;
	}

	int sem_id = semget(ftok("zz", 3), 3, 0666 | IPC_CREAT);

	semops[0].sem_num = 2;
	semops[0].sem_op = 0;
	semops[0].sem_flg = IPC_NOWAIT;

	semops[1].sem_num = 2;
	semops[1].sem_op = 2;
	semops[1].sem_flg = SEM_UNDO;

	if(semop(sem_id, semops, 2) != 0) /////1/////
		return -1;

	semops[0].sem_num = 1;
	semops[0].sem_op = -3;
	semops[0].sem_flg = 0;

	semops[1].sem_num = 0;
	semops[1].sem_op = -1;
	semops[1].sem_flg = 0;
	
	semops[2].sem_num = 0;
	semops[2].sem_op = 1;
	semops[2].sem_flg = SEM_UNDO;
	
	semops[3].sem_num = 1;
	semops[3].sem_op = 1;
	semops[3].sem_flg = SEM_UNDO;
	
	semops[4].sem_num = 1;
	semops[4].sem_op = 3;
	semops[4].sem_flg = 0;
	semop(sem_id, semops, 5);

	int rnum = 1;
	
	while(rnum > 0) { // 1 - 4 0 - 1
		
		semops[0].sem_num = 0;
		semops[0].sem_op = -1;
		semops[0].sem_flg = 0;
		
		semops[1].sem_num = 0;
		semops[1].sem_op = 1;
		semops[1].sem_flg = 0;
		semop(sem_id, semops, 2); ////2////

		if(semctl(sem_id, 1, GETVAL) == 1) {
			perror("No writer!");
			return -1;
		}

		rnum = *((long *)shmaddr);
		
		if(rnum > 0) {
			memcpy(buf, shmaddr + sizeof(long), rnum);
			write(1, buf, rnum);
		}

		semops[0].sem_num = 0;
		semops[0].sem_op = -1;
		semops[0].sem_flg = 0;
		semop(sem_id, semops, 1); ////2////

		if(rnum <= 0)
			break;
	
	}
	
	return 0; ////1////
}


