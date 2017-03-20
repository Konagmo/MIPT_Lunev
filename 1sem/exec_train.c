#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>


int main(int argc, char const *argv[]) {
	char *str, *endptr;
	if(argc < 2) {
		printf("Few arguments.\n");
		return 0;
	}
	if(argc >= 2) {
		int i = 0;	
		str = argv[1];
		for(i = 0; i < argc; i++) {
			printf("%s\n", argv[i]);
		}
		execvp(str, argv + 1);
	}
	return 0;
}