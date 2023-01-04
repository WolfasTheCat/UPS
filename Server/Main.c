#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <Header.h>
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <arpa/inet.h>

// telo vlakna co obsluhuje prichozi spojeni
void *serve_request(void *arg){
	int client_sock;
	char cbuf[4];

	//pretypujem parametr z netypoveho ukazate na ukazatel na int
	client_sock = *(int *) arg;

        printf("(Vlakno:) Huraaa nove spojeni\n");
        recv(client_sock, &cbuf, sizeof(char)*4, 0);
        printf("(Vlakno:) Dostal jsem %c\n",cbuf);
        //read(client_sock, &cbuf, sizeof(char)*4);
        printf("(Vlakno:) Dostal jsem %c\n",cbuf);
        send(client_sock,"Hi",sizeof(char)*2,0);
        close(client_sock);

	// uvolnujeme pamet
	free(arg);
	return 0;
}

int main (void){
	int server_sock;
	int client_sock;
	int return_value;
	char cbuf;
	int *th_socket;
	struct sockaddr_in local_addr;
	struct sockaddr_in remote_addr;
	socklen_t	remote_addr_len;
	pthread_t thread_id;;
	
	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (server_sock<=0) return -1;
	
	memset(&local_addr, 0, sizeof(struct sockaddr_in));

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(10000);
	local_addr.sin_addr.s_addr = INADDR_ANY;

	return_value = bind(server_sock, (struct sockaddr *)&local_addr,\
                sizeof(struct sockaddr_in));

	if (return_value == 0)
		printf("Bind OK\n");
	else{
		printf("Bind ER\n");
		return -1;
	}

	return_value = listen(server_sock, 5);
	if (return_value == 0)
		printf("Listen OK\n");
	else{
		printf("Listen ER\n");
	}

	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = server_sock;
	int epoll_fd = epoll_create1(0);
	epoll_ctl(epoll_fd,EPOLL_CTL_ADD,server_sock,&event);
    //EPOl - add server socket
    //WAIT EPOL - until some action
	epoll_wait();

	while(1){
		client_sock = accept(server_sock,\
			(struct sockaddr *)&remote_addr,\
			&remote_addr_len);
		if (client_sock > 0 ) {
            // EPol - add client socket
			th_socket=malloc(sizeof(int));
			*th_socket=client_sock;
			pthread_create(&thread_id, NULL,\
                                  (void *)&serve_request, (void *)th_socket);
		} else {
			printf("Trable\n");
			return -1;
		}
	}

return 0;
}

void send_message(int client_socket, char *message, log_info **info, int logging) {	
	printf("%d Writed message: %s\n", client_socket, message);
	send(client_socket, message, strlen(message) * sizeof(char), 0);
	if (logging == 1)
	{
		(*info) -> count_bytes += strlen(message) + 1;
		(*info) -> count_messages++;
	}
	return;
}

void server_running(struct timeval start, struct timeval end, log_info **info) {
	(*info) -> server_running_minutes = (end.tv_sec - start.tv_sec) / 60;
	return;
}

int check_if_contains_delimiters(char *cbuf) {
	char *e;
	int index;
	e = strchr(cbuf, '|');
	if (e == NULL) return 0;
	index = (int)(e - cbuf);
	if (index > 20 || index <= 0) return 0;
	else return 1;
}

int validate_input(char *cbuf) {
	int i;
	for (i = 0; i < strlen(cbuf); i++) {
		if (!isascii(cbuf[i])) return 0; 
		else {
			if (i < (strlen(cbuf) - 1)) {
				if ( (cbuf[i] == '|') && (cbuf[i+1] == 10) ) break;
			}
		}
	}
	return 1;
}

