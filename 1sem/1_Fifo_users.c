#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


int main(int argc, char const *argv[]) {
	if(argc < 2) {
		printf("To few arguments!\n");
		return 0;
	}
	const char *str = argv[1];
	char buf[1024] = "";
	int input;
	if((input = open(str, O_RDONLY)) == -1) {
		printf("Error in opening file\n");
	}

    mkfifo(nf, 0666);

    int cpid = fork();
    if(cpid == 0) {
    	close(fd[0]);
    	while(read(input, buf, 1024) > 0) {
    		write(fd[1], buf, strlen(buf));
		}
		write(fd[1], "\0", 1);
		close(fd[1]);
    }
    else {
    	close(fd[1]);
    	while(read(fd[0], buf, 1024) > 0) {
	    	write(STDOUT_FILENO, buf, strlen(buf));
		}
		close(fd[0]);
    }
    return 0;
}


