#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>

#define count 100
#define timelen 20

int main(int argc, char const *argv[]) {
	int fd[2];
	pipe(fd);
	int flags = fcntl(fd[1], F_GETFL);
	flags |= O_NONBLOCK;
	flags = fcntl(fd[1], F_SETFL, flags);
	char c = 'a';
	int bufsize = 0;
	while(1) {
		bufsize++;
		if((write(fd[1], &c, 1)) <= 0)
			break;
	}
	//printf("%d\n", bufsize);
	char* buf = calloc(bufsize, sizeof(char));
	if((read(fd[0], buf, bufsize)) <= 0) {
		perror("Error in read!");
		exit(0);
	}
	flags &= ~O_NONBLOCK;
	flags = fcntl(fd[1], F_SETFL, flags);

	struct timeval  tv;
	struct timezone tz;

	int child = fork();

	if(child != 0) { 
		if(gettimeofday(&tv, &tz) != 0) {
			perror("You have no more time =(");
			exit(0);
		}
		//printf("sec-%d\nmicrosec-%d\n", tv.tv_sec, tv.tv_usec);
		int i;
		for(i = 0; i < count; i++) {
			if(write(fd[1], buf, bufsize) <= 0) {
				perror("Error in write to child");
				exit(0);
			}
		}
		char sec[timelen];
		char usec[timelen];
		sprintf(sec, "%d", tv.tv_sec);
		sprintf(usec, "%d", tv.tv_usec);
		if(write(fd[1], sec, timelen) <= 0) {
			perror("Error in write to child");
			exit(0);
		}
		if(write(fd[1], usec, timelen) <= 0) {
			perror("Error in write to child");
			exit(0);
		}
		wait(NULL);
	}

	else {
		int i;
		for(i = 0; i < count; i++) {
			if(read(fd[0], buf, bufsize) <= 0) {
				perror("Error in read buf in child");
				exit(0);
			}
		}
		if(gettimeofday(&tv, &tz) != 0) {
			perror("You have no more time with children =(");
			exit(0);
		}
		char sec[timelen];
		char usec[timelen];
		if(read(fd[0], sec, timelen) <= 0) {
			perror("Error in read time in child");
			exit(0);
		}
		if(read(fd[0], usec, timelen) <= 0) {
			perror("Error in read time in child");
			exit(0);
		}
		int timesec = tv.tv_sec - strtoll(sec, NULL, 10);
		int timeusec = tv.tv_usec - strtoll(usec, NULL, 10);
		double abs_time = timesec + (double)timeusec/1000;
		printf("%.1lf\n", bufsize*count/abs_time);
		//printf("%d\n%d\n", strtoll(sec, NULL, 10), strtoll(usec, NULL, 10));
		//printf("%s\n%s", sec, usec);
	}


	return 0;
}
