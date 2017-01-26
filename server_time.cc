#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <poll.h>
int main() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in laddr;
	laddr.sin_family = AF_INET;
	laddr.sin_addr.s_addr = htonl(INADDR_ANY);
	laddr.sin_port = htons(3500);
	if(-1 == bind(sock,(struct sockaddr*)&laddr,sizeof(laddr))) {
		perror("");
		return 1;
	}
	listen(sock,10);
	while (1) {
		struct sockaddr_in raddr;
		socklen_t raddr_size=sizeof(raddr);
		int *clients = NULL;
		unsigned int num_clients = 0;
		int client_sock;
		while(1) {
			struct pollfd fds = {sock, POLLIN, 0};
			int res = poll(&fds, 1, 1000);
			if(res < 0) {
				perror("poll error");
				return 1;
			}
			if(res == 1) {
				client_sock=accept(sock,(struct sockaddr*)&raddr, &raddr_size);
				if(-1 == client_sock){
					perror("sock err");
					continue;
				}
				num_clients++;
				clients = (int*)realloc(clients, sizeof(int)*num_clients);
				clients[num_clients-1] = client_sock;
				char str_addr[INET_ADDRSTRLEN];
				inet_ntop(AF_INET,&raddr.sin_addr,str_addr,sizeof(str_addr));
				printf ("Adress:%s\n", str_addr);
				printf("PORT:%d\n",ntohs(raddr.sin_port));
			}
			else {
				for(unsigned int i = 0; i < num_clients; i++){
					char buf[1024];
					time_t t = time(NULL);
					ctime_r(&t, buf);
					if(-1==send(clients[i],buf,strlen(buf),MSG_NOSIGNAL)){	
						close(clients[i]);
						for(unsigned int j = i; j < num_clients - 1; j++){
							clients[j] = clients[j+1];
						}
						num_clients--;
						clients = (int*)realloc(clients, sizeof(int)*num_clients);
					}	
				}
			}
		}/*
		//char i[1000];
		char str_addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET,&raddr.sin_addr,str_addr,sizeof(str_addr));
			printf ("Adress:%s\n", str_addr);
			printf("PORT:%d\n",ntohs(raddr.sin_port));
		const char header[]="HTTP/1.1 200 OK\r\nConnection:close\r\n\r\n";
		if(-1==send(clients[0], header,strlen(header),MSG_NOSIGNAL)){
			printf("error2");
		char str_addr[INET_ADDRSTRLEN];
			return 1;
		}
		char buf[1024];
		for(int j = 0; j < 10; j++){
			time_t t = time(NULL);
			ctime_r(&t, buf);
			printf("error2");
			if(-1==send(clients[0],buf,strlen(buf),MSG_NOSIGNAL)){	
				close(clients[0]);
			}
			sleep(1);
		}
		shutdown(clients[0], SHUT_RDWR);
		for(int i = 0; i < num_clients; i++){
			close(clients[0]);	
		}*/
	}
	return 0;
}
