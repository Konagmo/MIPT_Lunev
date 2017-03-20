#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h> 


int main(int argc, char const *argv[]) {

	int fd, bytes;
	char buf[1024] = "";
	char *readerFifo = "readerFifo";
	char *myFifo 	 = "myFifo";
	char *writerFifo = "writerFifo"

	errno = 0;

	if(mkfifo(readerFifo, 0666)) {
		if(errno == EEXIST){
			perror("FIFO is busy at the moment!\n");
      		return 1;
		}
		else {
			perror("FIFO can not be created!\n");
      		return 1;
		}
	}

	errno = 0;

	if(mkfifo(myFifo, 0666 | O_NDELAY) == -1 
		&& errno != EEXIST) {
		perror("Failed to open the fifo.\n");
		return 1;
	}

	if ((fd = open(readerFifo, O_RDONLY)) <= 0) {
		perror("Failed to open the fifo.\n");
		return 1;
	}

	if((bytes = read(fd, buf, 1024)) > 0) {
   		write(STDOUT_FILENO, buf, bytes);
   		myFifo = calloc(1024, sizeof(char));
   		strcpy(myFifo, buf);
   		close(fd);
   		memset(buf, 0, sizeof(buf));
   	}

   	if(myFifo != NULL) {
	   	if ((fd = open(myFifo, O_RDONLY)) <= 0) {
			perror("Failed to open the fifo.\n");
			return 1;
		}

		while((bytes = read(fd, buf, 1024)) > 0) {
	   		write(STDOUT_FILENO, buf, bytes);
	   		memset(buf, 0, 1024);
	   	}

	   	free(myFifo);
   	}

   	
	return 0;
}