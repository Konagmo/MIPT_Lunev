#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_EXT 100
#define PROG_LEN 100
#define MAX_PROC 100
#define MAX_LEN 100

int n_ext;
int n_proc;
char extensions[MAX_EXT][PROG_LEN];
char programms[MAX_EXT][PROG_LEN];

char pids[MAX_PROC][10];
char names[MAX_PROC][MAX_LEN];

void parse_cfg() {
	FILE* f = fopen("autoopen.cfg", "r");
	
	char* line = NULL;
	size_t size = 0;
	int len, i;
	n_ext = 0;
	while(1) {
		len = getline(&line, &size, f);
		if(len < 0)
			break;
		line[len] = '\0';
		if(line[0] == '#') 
			continue;

		char *ext = strtok(line, "=");
		char *prog = strtok(NULL, "\n");
		strcpy(extensions[n_ext], ext);
		strcpy(programms[n_ext], prog);
		n_ext++;
	}
	
	/*for(i = 0; i < n_ext; i++) {
		printf("%s-%s\n", extensions[i], programms[i]);
	}*/
}


void open_task(char* arg) {

	char **args = malloc(2*sizeof(char*));
	args[1] = malloc((strlen(arg))*sizeof(char));
	
	strncpy(args[1], arg, strlen(arg));
	char *name = strtok(arg, ".");
	char *ext = strtok(NULL, "\n");
	char *prog = NULL;
	int found = 0;
	int i = 0;

	//printf("name -%s\next - %s\n", name, ext);
	for(; i < n_ext; i++) {
		if(strcmp(ext, extensions[i]) == 0) {
			found = 1;
			prog = malloc(strlen(programms[i])*sizeof(char));
			strcpy(prog, programms[i]);
			break;
		}
	}
	if(found == 0) {
		printf("No prog for that extension\n");
		printf("You can modify autoopen.cfg file..\n");

		char **argum = malloc(2*sizeof(char*));
		argum[0] = malloc((strlen("gedit")*sizeof(char)));
		argum[1] = malloc((strlen("autoopen.cfg")*sizeof(char)));
		strcpy(argum[1], "autoopen.cfg");
		strcpy(argum[0], "gedit");
		int pid = fork();
		if (pid == -1) {
			perror("Can't fork");
			return ;	 
		}
		if(pid == 0) { 
			sleep(1);
			int exec_stat = execvp(argum[0], argum);

			if(exec_stat == -1) {
				perror("Can't execute gedit");
				exit(EXIT_FAILURE);
			}
			exit(0);
		}
		else {
			int wait_stat;
			int ret_stat;

			wait_stat = waitpid(pid, &ret_stat, 0);
			if (wait_stat == -1) {
				perror("Error in waitpid");
				return ;
			}
			printf("Now trying again\n");
			parse_cfg();
			open_task(args[1]);
		}
		free(argum[0]);
		free(argum[1]);
		free(argum);
		return;
	}

	int pid = fork();
	if (pid == -1) {
		perror("Can't fork");
		return ;	 
	}
	if(pid == 0) {
		args[0] = prog;
		
		int exec_stat = execvp(prog, args);
		if(exec_stat == -1) {
			perror("Can't execute command");
			exit(EXIT_FAILURE);
		}
		exit(0);
	}
	else {
		int wait_stat;
		int ret_stat;
		char cpid[7];
		sprintf(cpid, "%d", pid);
		strcpy(names[n_proc], args[1]);
		strcpy(pids[n_proc], cpid);
		n_proc++; 

		wait_stat = waitpid(pid, &ret_stat, WNOHANG);
		if (wait_stat == -1) {
			perror("Error in waitpid");
			return ;
		}
		
		/*int j = 0, flag = 1;
		for(; j < MAX_PROC; j++) {
			//printf("%d - %d\n", j, atoi(pids[j]));
			if(strcmp(pids[j], cpid) == 0) {
				strcpy(pids[MAX_PROC-1], "");
				strcpy(names[MAX_PROC-1], "");
				flag = 0;
				n_proc--;
				printf("nproc - /%d/\n", n_proc);
			}
			if(!flag && j != MAX_PROC - 1) {
				strcpy(pids[j], pids[j+1]);
				strcpy(names[j], names[j+1]);
			}
		}*/
	}

	//printf("name - %s\next - %s\n", name, ext);
	//printf("prog - %s/arg - %s/\n", prog, arg);
	free(args[0]);
	free(args[1]);
	free(args);
	free(prog);
}


void show_task() {
	int i = 0;
	//printf("n_proc - %d\n", n_proc);
	for(;i < n_proc; i++) {
		printf("%s opens %s\n", pids[i], names[i]);
	}
	return;
}

void quit_task() {
	printf("Bye-bye\n");
	exit(0);
}

void run() {

	printf(">");

	char* line = NULL;
	size_t size = 0;
	int len;
	len = getline(&line, &size, stdin);
	if (len == -1){
		fprintf(stderr, "Error in reading commands\n");
		return ;	
	}
	
	char *cmd = strtok(line, " ");
	char *arg = strtok(NULL, "\n");

	if(strcmp(cmd, "open") == 0)
		open_task(arg);	
	if(strcmp(cmd, "show\n") == 0)
		show_task();	
	if(strcmp(cmd, "quit\n") == 0) 
		quit_task();
	return ;
}


int main(int argc, char const *argv[]) {
	parse_cfg();
	printf("Enter some commands:\n");
	while(1) {
		run();
	}
}