#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <arpa/inet.h>
#include "Main_Header.h"

#define READ_SIZE 50
#define MAX_EVENTS 5
#define MAX_PLAYERS 10


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
	int running = 1;
	char read_buffer[READ_SIZE];

	size_t bytes_read;
	int event_count;
	int server_sock;
	int new_client_sock;
	int return_value;
	//char cbuf;
	int *th_socket;
	struct sockaddr_in local_addr;
	struct sockaddr_in remote_addr;
	socklen_t	remote_addr_len;
	pthread_t thread_id;;

	struct timeval server_start_time;
	struct timeval server_end_time;
	client_list *all_clients;
	games_list *all_games;
	waiting_players_for_game *waiting_clients;
	backlog *server_log;


	//Basic preparation for server
	gettimeofday(&server_start_time,NULL);

	//Allocation of memory for structures
	create_array_of_games(&all_games);
	create_clients_array(&all_clients);
	create_waiting_for_game(&waiting_clients);


	
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

	struct epoll_event event, events[MAX_EVENTS];
	event.events = EPOLLIN;
	event.data.fd = server_sock;
	int epoll_fd = epoll_create1(0);

	if (epoll_fd == -1)
	{
		fprintf(stderr, "Failed to create epoll file descriptor\n");
		return 1;
	}
	if (epoll_ctl(epoll_fd,EPOLL_CTL_ADD,server_sock,&event))
	{
		printf(stderr, "Failed to add server file descriptor to epoll\n");
		close(epoll_fd);
		return 1;
	}

	while(running){
		printf("Pooling for input...\n");
		event_count = epoll_wait(epoll_fd,&event,MAX_EVENTS,30000);
		printf("%d ready events\n",event_count);
		for (int i = 0; i < event_count; i++){
			printf("Reading file descriptor '%d' -- ",events[i].data.fd);

			if(events[i].data.fd == server_sock){
				printf("new connection to server");
				new_client_sock = accept(server_sock,(struct sockaddr *)&remote_addr,&remote_addr_len);
				if (new_client_sock < 0 ) {
					printf("Trable\n");
					return -1;
				} 
				else {
					printf("New socket accepted with id %d",new_client_sock);
				}
				struct epoll_event new_event;
				new_event.events = EPOLLIN;
				new_event.data.fd = new_client_sock;

				if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &new_event)){			// EPol - add client socket
					fprintf(stderr, "Failed to add file descriptor to epoll\n");
					close(epoll_fd);
					return 1;
				}
			}
			else{

				client *current_client = find_client_by_socket(all_clients, events[i].data.fd);
				//zavoláš funkci pro tvorbu vlákna + file descriptor
				//pthread_create(); //thread detach 

				/*
				bytes_read = recv(events[i].data.fd, &read_buffer, 50, 0); //LOGIN|KUBA|\n														
				char *message_tok = strtok(read_buffer,"|"); 			//LOGIN (NULL) KUBA|\n
				message_tok = strtok(NULL,"|");							//KUBA (NULL) \n
				*/

				//save recieved bytes into read buffer
				bytes_read = recv(events[i].data.fd, &read_buffer, 50, 0);
				//Client want to login as new player or reconnect
				if (strcmp(read_buffer,"login") == 0){
					login(&all_clients, all_games, &server_log, &read_buffer, MAX_PLAYERS, events[i].data.fd, &current_client);
				}
				
				else if(strcmp(read_buffer,"play") == 0){
					if(current_client != NULL){

						if(current_client->state == 0){
							want_to_play(&all_clients, &waiting_clients, &all_games, &server_log,events[i].data.fd, &current_client);
						}
					}
				}

				else if(strcmp(read_buffer,"move") == 0){
					if(current_client != NULL){

						if(current_client->state == 3){
							client_move(&all_clients,&all_games,read_buffer,&server_log);
						}
					}
				}

				else if(strcmp(read_buffer,"is_connected") == 0){

				}
				else if(strcmp(read_buffer,"end_app") == 0){
					
				}

				printf("%zd bytes read.\n", bytes_read);
				read_buffer[bytes_read] = '\0';
				printf("Read '%s'\n", read_buffer);
			
				if(!strncmp(read_buffer, "stop\n", 5))
					running = 0;
				}
			
		}

		if (close(epoll_fd)) {
		fprintf(stderr, "Failed to close epoll file descriptor\n");
		return 1;
		}



		
		

		// EPol - add client socket
		th_socket = malloc(sizeof(int));
		*th_socket = new_client_sock;
		pthread_create(&thread_id, NULL,(void *)&serve_request, (void *)th_socket);
	}

return 0;
}

void send_message(int client_socket, char *message, backlog **info, int logging) {	
	printf("%d Writed message: %s\n", client_socket, message);
	send(client_socket, message, strlen(message) * sizeof(char), 0);
	if (logging == 1)
	{
		(*info) -> bytes_count += strlen(message) + 1;
		(*info) -> messages_count++;
	}
	return;
}

void server_running(struct timeval start, struct timeval end, backlog **info) {
	(*info) -> server_running_time = (end.tv_sec - start.tv_sec) / 60;
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

int name_exists (client_list *array_clients, char *name) {
	int i;
	for (i = 0; i < array_clients -> clients_count; i++) {
		if (strcmp(array_clients -> clients[i]->nickname, name) == 0) {	
			return 1;
		}
	}
	return 0;
}

void login(client_list **array_clients, games_list *all_games, backlog **info, char *tok, int max_players, int fd, client **client) {
	tok = strtok(NULL, "|");
	char *name = tok;
	if (name_exists((*array_clients), name) == 0) {								
		if (((*array_clients) -> clients_count) < (max_players)) {			
			if ((*client) == NULL) {
				add_client_to_array(array_clients, name, fd);
				(*client) = (*array_clients) -> clients[(*array_clients) -> clients_count - 1];
				pthread_create(&((*client) -> client_thread), NULL, &check_connectivity_by_ping, client); //TODO nutno predelat pro timeout
				printf("Name: %s\n", (*array_clients) -> clients[(*array_clients) -> clients_count -1]->nickname);
				char *message = "login_ok|\n";
				send_message(fd, message, info, 1);					
			}
			else {
				//already logged
			}
		}
		else {
			//too many logged clients
			char *message = "login_failed|2|\n";
			send_message(fd, message, info, 1);
		}		
	}
	else {
		(*client) = get_client_by_name(*array_clients, name);
		if ((*client) -> state == 2) {
			reconnect(array_clients, all_games, info, name, fd, tok, max_players, client);									
		}
		else {
			char *message = "login_failed|3|\n";
			send_message(fd, message, info, 1);
		}								
	} 
	return;
}
/**
 * @brief Make move in the client's game
 * 
 * @param array_of_clients array of all clients
 * @param all_games	array of all games
 * @param message message from client
 * @param log server log
 */
void client_move(client_list **array_of_clients, games_list **all_games, char *message, backlog **log){
	int i = 0;
	char *array[5];

	message = strtok(NULL,"|");

	while(message != NULL){
		array[i++] = message;
		message = strtok(NULL,"|");
	}

	int game_ID = atoi(array[0]);
	int row = atoi(array[1]);
	int column = atoi(array[2]);
	printf("CLIENT MOVE: GAME ID: %d, ROW= %d, COLUMN = %d",game_ID,row,column);

	int status = process_move(all_games, array_of_clients, log, game_ID, row, column);
	if (status == 0){
		printf("Move proccesed sucessfully");
	}
	else{
		printf("Invalid move. Move denied by server");
	}
	return;
}

//Client want to play a game 
void want_to_play(client_list **array_of_clients, waiting_players_for_game **waiting_clients, games_list **all_games, backlog **server_log,int fd, client **current_client){
	client *opponent = NULL;

	add_to_waiting_for_game(waiting_clients,fd);
	(*current_client)->state = 1;

	if((*waiting_clients)->waiting_number >= 2){
		
		int current_client_socket = fd;
        int opponent_client_socket;
        do {
            opponent_client_socket = (*waiting_clients) -> socket_ID_array[rand() % ((*waiting_clients) -> waiting_number)];
        }
        while(opponent_client_socket == current_client_socket);

		remove_from_waiting_for_game(waiting_clients,current_client_socket);
		remove_from_waiting_for_game(waiting_clients,opponent_client_socket);

		opponent = find_client_by_socket(*array_of_clients,opponent_client_socket);

		set_state_status(current_client,3);
		set_state_status(&opponent,4);


	}
	
}
//Klient se pripojil zpet do hry
void reconnect(){

}
//Klient ztratil spojeni
void disconnect(){

}
//Odebere klienta ze serveru
void remove_client_from_server(){

}
//odpoji klienta od serveru
void end_connection_for_client(){

}

int check_if_name_is_valid(client_list **array_clients, char *name){

}

void *check_connectivity_by_ping(){

}

