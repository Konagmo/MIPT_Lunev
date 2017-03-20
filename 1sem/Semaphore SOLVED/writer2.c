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
#define N_SEM 3

#define SEM_OP(sembuf_num, num, op, flg)\
			semops[sembuf_num].sem_num = num;\
			semops[sembuf_num].sem_op = op;\
			semops[sembuf_num].sem_flg = flg;\


int main(int argc, char const *argv[]) {

	if(argc != 2) {
		perror("Invalid number of arguments!");
		return 0;
	}

	struct sembuf semops[5];

	int sh_id;
	char* shmptr;
	char buf[SIZE];

	if((sh_id = shmget(ftok("zz", SIZE), SIZE, 0777 | IPC_CREAT)) < 0)
	{
		perror("shmget error");
		return -1;
	}

	if((shmptr = shmat(sh_id, 0, 0)) == (void*) -1)
	{
		perror("shmat error");
		return -1;
	}

	int key = ftok("zz", N_SEM);
	printf("%d\n", key);
	int sem_id = semget(key, N_SEM, 0666 | IPC_CREAT);


	SEM_OP(0, 1, 0, IPC_NOWAIT);
	SEM_OP(1, 1, 2, SEM_UNDO);

	
	if(semop(sem_id, semops, 2) != 0)
	{
		return -1;
	}

	semctl(sem_id, 0, SETVAL, 0);

	SEM_OP(0, 1, 1, SEM_UNDO);
	semop(sem_id, semops, 1);


	SEM_OP(0, 2, -1, 0);
	SEM_OP(1, 2, 1, SEM_UNDO);
	SEM_OP(2, 0, 1 , 0);
	SEM_OP(3, 0, -1, SEM_UNDO);
	SEM_OP(4, 2, 1, 0);
	semop(sem_id, semops, 5);

	int fd = open(argv[1], O_RDONLY);
	int rnum = 1;
	while(rnum > 0)
	{
		int rnum = read(fd, buf, SIZE);	
		if(rnum <= 0)
			memset(shmptr, 0, SIZE);
		else
		{
			*((long *)(shmptr)) = rnum;
			memcpy(shmptr + sizeof(long), buf, rnum);
		}

		SEM_OP(0, 0, 1, 0);
		semop(sem_id, semops, 1);

		SEM_OP(0, 2, -3, IPC_NOWAIT);
		SEM_OP(1, 2, 3, 0);
		SEM_OP(2, 0, 0, 0);

		semop(sem_id, semops, 3);

		if(semctl(sem_id, 2, GETVAL) == 1)
		{
			perror("reader is dead ");
			return -1;
		}
		

		if(rnum <= 0)
			break;

	}

	return 0;
}