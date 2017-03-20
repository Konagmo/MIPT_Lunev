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

	if(argc != 2) {
		perror("Invalid number of arguments!");
		return 0;
	}

	struct sembuf semops[5];
	int shmid;
	char buf[SIZE];
	char* shmaddr;

	if((shmid = shmget(ftok("zz", SIZE), SIZE, 0777 | IPC_CREAT)) < 0) {
		perror("Error in shmget!");
		return 0;
	}
	if((shmaddr = shmat(shmid, 0, 0)) == (void*) -1) {
		perror("Error in shmat");
		return 0;
	}

	int s_id = semget(ftok("zz", 3), 3, 0666 | IPC_CREAT); 

	semops[0].sem_num = 1;
	semops[0].sem_op = 0;
	semops[0].sem_flg = IPC_NOWAIT; 

	semops[1].sem_num = 1;
	semops[1].sem_op = 2;
	semops[1].sem_flg = SEM_UNDO;

	if(semop(s_id, semops, 2) != 0) ////
		return 0;

	semctl(s_id, 0, SETVAL, 0);

	semops[0].sem_num = 1;
	semops[0].sem_op = 1;
	semops[0].sem_flg = SEM_UNDO;

	semop(s_id, semops, 1);

	semops[0].sem_num = 2;
	semops[0].sem_op = -1;
	semops[0].sem_flg = 0;

	semops[1].sem_num = 2;
	semops[1].sem_op = 1;
	semops[1].sem_flg = SEM_UNDO;	

	semops[2].sem_num = 0;
	semops[2].sem_op = 1;
	semops[2].sem_flg = 0;

	semops[3].sem_num = 0;
	semops[3].sem_op = -1;
	semops[3].sem_flg = SEM_UNDO;

	semops[4].sem_num = 2;
	semops[4].sem_op = 1;
	semops[4].sem_flg = 0;

	semop(s_id, semops, 5); 

	int fd = open(argv[1], O_RDONLY);
	int rnum = 1;

	while(rnum > 0) {
		rnum = read(fd, buf, SIZE);
		if(rnum <= 0)
			memset(shmaddr, 0, SIZE);
		else {
			*((long*)(shmaddr)) = rnum;
			memcpy(shmaddr + sizeof(long), buf, rnum);
		}

		semops[0].sem_num = 0;
		semops[0].sem_op = 1;
		semops[0].sem_flg = 0;
		semop(s_id, semops, 1); 
		
		semops[0].sem_num = 2;
		semops[0].sem_op = -3;
		semops[0].sem_flg = IPC_NOWAIT;

		semops[1].sem_num = 2;
		semops[1].sem_op = 3;
		semops[1].sem_flg = 0;

		semops[2].sem_num = 0;
		semops[2].sem_op = 0;
		semops[2].sem_flg = 0;

		semop(s_id, semops, 3); 

		if(semctl(s_id, 2, GETVAL) == 1) {
			perror("No reader!");
			return 0;
		}

		if(rnum <= 0)
			break;
	}
	return 0; ////
}

