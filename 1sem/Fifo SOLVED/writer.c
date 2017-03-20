#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>


#define bufSize 1000

int main(int argc, char const *argv[]) {
	if(argc != 2) {
		perror("Invalid number of arguments!");
		return 1;
	}

	int file_fd, fifo_fd, bytes;
	char *chat = "/tmp/chat";
	char nFifo[bufSize];
	char buf[PIPE_BUF];

	sprintf(nFifo, "%d", getpid());
	printf("%s\n", nFifo);

	errno = 0;
	if(mkfifo(chat, 0777) && (errno != EEXIST)) {
		perror("Can't create fifo chat");
		return 1;
	}

	errno = 0;

	if(mkfifo(nFifo, 0777) && (errno != EEXIST)) {
		perror("Cant't create fifo");
		return 1;
	}

	int chat_fd;

	if((chat_fd = open(chat, O_WRONLY)) <= 0) {
		perror("Can't open the fifo chat");
		return 1;
	}


	if((file_fd = open(argv[1], O_RDONLY)) <= 0) {
		perror("Can't open the file.");
		return 1;
	}


	if((fifo_fd = open(nFifo,  O_RDONLY | O_NONBLOCK)) < 0) {
		perror("can not open fifo");
		return 1;
	}

	int fifo_write;
	if((fifo_write = open(nFifo, O_WRONLY | O_NONBLOCK)) < 0) {
		perror("Can't open the fifo.");
		return 1;
	}

	int nRead = 1;
	int bufLength = PIPE_BUF;

	nRead = read(file_fd, buf, bufLength);
	printf("%d\n", nRead);
	write(fifo_write, buf, nRead);

	write(chat_fd, nFifo, bufSize);
	close(chat_fd);

	sleep(1);

	int fl = fcntl(fifo_write, F_GETFL, 0);
	fl = fl & ~O_NONBLOCK;
	fcntl(fifo_write, F_SETFL, fl);
	close(fifo_fd);

	while(nRead > 0) {
		nRead = read(file_fd, buf, bufLength);
		printf("%d\n", nRead);
		write(fifo_write, buf, nRead);
	}

	close(chat_fd);
	close(file_fd);
	return 0;
}