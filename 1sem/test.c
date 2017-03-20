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
		long long int n = 0;
		char number = 1;
		if(argv[1][0] == '-') {
			printf("Number can't be negative.\n");
			return 0;
		}

		str = argv[1];
		errno = 0;
		n = strtoll(str, &endptr, 10);

		if ((errno == ERANGE && (n == LONG_MAX || n == LONG_MIN))
                   || (errno != 0 && n == 0)) {
     		perror("strtol");
     		return 0;
  		}

		if (*endptr) {
     		printf("Wrong Input.\n");
    		return 0;
    	}

		printf("%lld\n", n);

	}
	return 0;
}