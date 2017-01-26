#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char const *argv[]) {
	int fd[2];
	pipe(fd);
	int c = fork();
	if(c != 0) {
		wait(NULL);
	}
	else {
		close(fd[0]);
		fcntl(fd[1], F_SETFL, O_NONBLOCK);
		int i = 0;
		char a = 'a';
		while(1) {
			i++;
			if((write(fd[1], &i, 1)) <= 0)
				break;
		}
		printf("%d\n", i);
	}
	return 0;
}