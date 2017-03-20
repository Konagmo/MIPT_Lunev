#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define bufSize 10


int main(int argc, char const *argv[]) {

	char *translator = "translator";
	int fd, bytes;

	if(argc != 1) {
		perror("Invalid number of arguments!");
		return 1;
	}
 
 	errno = 0;
	if(mkfifo(translator, 0777) & (errno != EEXIST)) {
		perror("Can't create fifo translator.");
		return 1;
	}

	if((fd = open(translator, O_RDONLY)) <= 0) {
		perror("Can't open the fifo translator.");
		return 1;
	}

	char translatorName[bufSize];
	if((bytes = read(fd, translatorName, bufSize)) <= 0) {
		perror("Can't read more data!");
		return 1;
	}

	//printf("Translator Name - %s\n", translatorName);

	errno = 0;
	if(mkfifo(translatorName, 0777) && (errno != EEXIST)) {
		perror("Can't organise fifo translation.");
		return 1;
	}

	int fifo_fd;
	if((fifo_fd = open(translatorName, O_RDONLY 
		| O_NONBLOCK)) <= 0) {
		perror("Can't open the fifo translator.");
		return 1;
	}	
	
	sleep(2);
	char buf[bufSize];

	int read_num = 1;
	while(read_num > 0) {
		read_num = read(fifo_fd, buf, bufSize);
		write(STDOUT_FILENO, buf, read_num);
	}

	close(fifo_fd);
	close(fd);
	return 0;
}