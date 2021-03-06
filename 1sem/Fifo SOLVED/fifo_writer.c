#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>


#define bufSize 8

int main(int argc, char const *argv[]) {
	if(argc != 2) {
		perror("Invalid number of arguments!");
		return 1;
	}

	int file_fd, fifo_fd, bytes;
	char *translator = "translator";
	char fifoName[bufSize];
	char buf[PIPE_BUF];
	sprintf(fifoName, "%d", getpid());
	//printf("Fifo Name - %s\n", fifoName);

	errno = 0;
	if(mkfifo(translator, 0777) && (errno != EEXIST)) {
		perror("Can't create fifo translator");
		return 1;
	}

	errno = 0;

	if(mkfifo(fifoName, 0777) && (errno != EEXIST)) {
		perror("Cant't create fifo");
		return 1;
	}

	int translator_fd;

	if((translator_fd = open(translator, O_WRONLY)) <= 0) {
		perror("Can't open the fifo translator");
		return 1;
	}

	write(translator_fd, fifoName, bufSize);

	if((file_fd = open(argv[1], O_RDONLY)) <= 0) {
		perror("Can't open the file.");
		return 1;
	}

	sleep(1);
	if((fifo_fd = open(fifoName, O_WRONLY | O_NONBLOCK)) < 0) {
		perror("Can't open the fifo.");
		return 1;
	}

	int readNum = 1;

	while(readNum > 0) {
		readNum = read(file_fd, buf, PIPE_BUF);
		write(fifo_fd, buf, readNum);
	}

	close(file_fd);
	close(fifo_fd);
	close(translator_fd);
	return 0;
}