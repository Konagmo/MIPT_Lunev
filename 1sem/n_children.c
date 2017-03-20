#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[]) {

	char *str, *endptr;
	int i;
	int parid = getpid();
	int num = 0;
	int wait_id = 0;

	if(argc < 2) {
		printf("To few arguments\n");
		return 0;
	}

	str = argv[1];
	num = strtoll(str, &endptr, 10);
	printf("1 %d\n", getpid());	
	for(i = 1; i < num ; i++) {
		if(getpid() == parid) {
			fork();
			wait(&wait_id);
			if(getpid() != parid) {
				printf("%d %d\n", i + 1, getpid());	
			}
		}
	}
	/*for(i = 0; i < num; i++){
		
	}*/
	return 0;
}