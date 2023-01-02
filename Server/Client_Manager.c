#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "Main_Header.h"

void create_clients_array(client_list **array_clients){
    (*array_clients) = calloc(1,sizeof(client_list));
    (*array_clients)->clients_count = 0;
    (*array_clients)->clients = calloc(1,sizeof(client));
}

void create_client(client **client, char *name, int socket){
    (*client) = calloc(1, sizeof(client));
    (*client)->nickname = calloc(1,strlen(name) * sizeof(char));
    strcpy((*client) -> nickname, name);
    (*client)->socket_ID = socket;
    (*client)->connected = 1;
    (*client)->state = 0;
    (*client)->disconnected_time = 0;
}

void add_client_to_array(client_list **array_clients, char *name, int socket_ID) {
	(*array_clients) -> clients_count++;

	printf("Clients count: %d\n", (*array_clients) -> clients_count);
	(*array_clients) -> clients = realloc((*array_clients) -> clients, (*array_clients)->clients_count * sizeof(client));
	client *client = NULL;
	create_client(&client, name, socket_ID);
	(*array_clients) -> clients[((*array_clients) -> clients_count) - 1] = client;

}
//TODO Dodělat
void remove_client_from_array(client_list **array_clients, waiting_for_game **waiting_players, int socket_ID){
    int count = (*array_clients)->clients_count;
    int socket;
    for (int i = 0; i < count; i++)
    {
        socket = (*array_clients)->clients[i]->socket_ID;
        if (socket == socket_ID)
        {
            /* code */
        }
        
    }
    
}

client *find_client_name(client_list *array_of_clients, char *name){
    int count = array_of_clients->clients_count;
    char *temp_name;
    for (int i = 0; i < count; i++)
    {
        temp_name = array_of_clients->clients[i]->nickname;
        if(strcmp(temp_name,name)==0){
            return array_of_clients->clients[i];
        }
    }
    return NULL;
}

client *find_client_by_socket(client_list *array_of_clients, int demanded_socket){
    int count = array_of_clients->clients_count;
    int temp_socket;
    for (int i = 0; i < count; i++)
    {
        temp_socket = array_of_clients->clients[i]->socket_ID;
        if (temp_socket == demanded_socket)
        {
            return array_of_clients->clients[i];
        }
        
    }
    return NULL;
}

void set_connected_status(client **client, int connected){
    (*client)->connected = connected;
}

void set_state_status(client **client, int state){
    (*client)->state = state;
}

void set_socket_ID(client **client, int socket){
    (*client)->socket_ID = socket;
}

void set_disconnected_time_status(client **client, int disconnected_time){
(*client)->disconnected_time= disconnected_time;
}