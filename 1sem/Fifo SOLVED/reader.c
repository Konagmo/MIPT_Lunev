#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define bufSize 1000


int main(int argc, char const *argv[]) {
	char *chat = "/tmp/chat";
	int fd, bytes;
	if(argc != 1) {
		perror("Invalid number of arguments!");
		return 1;
	}
 
 	errno = 0;

	if(mkfifo(chat, 0777) & (errno != EEXIST)) {
		perror("Can't create fifo chat");
		return 1;
	}

	if((fd = open(chat, O_RDONLY)) <= 0) {
		perror("Can't open the fifo chat.");
		unlink(chat);
		return 1;
	}

	char chatName[bufSize];

	if((bytes = read(fd, chatName, bufSize)) <= 0) {
		perror("Can't read more data!");
		unlink(chat);
		return 1;
	}

	errno = 0;
	if(mkfifo(chatName, 0777) && (errno != EEXIST)) {
		perror("Can't organise fifo chatting");
		return 1;
	}

	int fifo_fd;
	close(fd);

	if((fifo_fd = open(chatName, O_RDONLY | O_NONBLOCK)) <= 0) {
		perror("Can't open the fifo chat");
		return 1;
	}

	char buf[bufSize];

	int fl = fcntl(fifo_fd, F_GETFL, 0);

	fl = fl & ~O_NONBLOCK;
	fcntl(fifo_fd, F_SETFL, fl);

	int nRead = 1;

	while(nRead > 0) {
		nRead = read(fifo_fd, buf, bufSize);
		write(STDOUT_FILENO, buf, nRead);
	}

	close(fifo_fd);
	return 0;
}