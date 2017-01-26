#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char const *argv[]) {

	int  fd, input;
  char buf[100] = "";
  char *readerFifo = "readerFifo";
  char *writerFifo = "writerFifo";
  char *myFifo  = "myFifo";

	if(argc != 2) {
    perror("To few arguments!\n");
    return 0;
  }

  errno = 0;

	if(mkfifo(writerFifo, 0666) == -1) {
    if(errno == EEXIST) {
      perror("FIFO is busy at the moment!\n");
      return 1;
    }
    else {
      perror("FIFO can not be created!\n");
      return 1;
    }
  }

  errno = 0;
	if(mkfifo(myFifo, 0666) == -1 && (errno != EEXIST)) {
    perror("FIFO can not be created!\n");
    return 1;
  }


	write(fd, myFifo, strlen(myFifo));
  memset(buf, 0, sizeof(buf));
 	close(fd);

	if((input = open(argv[1], O_RDONLY)) <= 0) {
  	perror("Failed to open the file.\n");
    unlink(myFifo);
    unlink(writerFifo);
		return 1;
  }

  if((fd = open(myFifo, O_WRONLY)) <= 0) {
    perror("Failed to open the fifo.\n");
    unlink(myFifo);
    unlink(writerFifo);
    return 1;
  }

  int bytes;

  while((bytes = read(input, buf, 10)) > 0) {
  	write(fd, buf, bytes);
  	memset(buf, 0, 10);
  }

  unlink(myFifo);
  unlink(writerFifo);
 	close(fd);
 	close(input);
	return 0;
}