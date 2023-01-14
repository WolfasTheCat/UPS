#ifndef SERVER_MAIN_HEADER_H
#define SERVER_MAIN_HEADER_H

typedef enum { 
	LOGGED, 		    //0
	WAITING_FOR_GAME,	//1
	DISCONNECT,		    //2
	YOU_PLAYING,		//3
	OPPONENT_PLAYING	//4
} STATES;

typedef struct Game{
    int game_id;
    char *first_player;
    char *second_player;
    char *now_playing;
    char *game_array;
    int turn; //max 9 kol
        //0 pro prvního hráče, 1 pro druhého hráče
}game;

typedef struct Games_list{
    int games_count;
    game **games; 
}games_list;

typedef struct Client{
    char *nickname;
    int socket_ID;
    STATES state;
    pthread_t client_thread; 
    char symbol; //X or O
    int connected;
    int disconnected_time;
}client;

typedef struct Client_list{
    int clients_count;
    client **clients;
}client_list;

typedef struct Waiting_players
{
    int waiting_number;
    int *socket_ID_array;
}waiting_players_for_game;

typedef struct Backlog
{
    int32_t bytes_count;
    int32_t messages_count;
    int connections_count;
    int bad_connections_count;
    int server_running_time; //running minutes count
}backlog;


//Game.c

create_array_of_games(games_list **games);
add_to_game_array(games_list **games, char *player_1, char *player_2, char *now_playing);
remove_game_from_array(client_list **clients, games_list **games, backlog **info, int game_ID);

create_game(game **gm, char *player_1, char *player_2, char *now_playing);
inicialize_game_field_array(game **gm);

create_waiting_for_game(waiting_players_for_game **want_play);
add_to_waiting_for_game(waiting_players_for_game **want_play, int socket_ID);
remove_from_waiting_for_game(waiting_players_for_game **want_play, int socket_ID);

game *find_game_by_name(games_list *all_games, char *name);
int player_wanna_play(waiting_players_for_game *want_play, client *cl);

//Client_manager.c
void create_clients_array(client_list **array_clients);
void create_client(client **client, char *name, int socket);
void add_client_to_array(client_list **array_clients, char *name, int socket_ID);
void remove_client_from_array(client_list **array_clients, waiting_players_for_game **waiting_players, int socket_ID);
client *find_client_by_name(client_list *array_of_clients, char *name);
client *find_client_by_socket(client_list *array_of_clients, int demanded_socket);
void set_connected_status(client **client, int connected);
void set_state_status(client **client, int state);
void set_socket_ID(client **client, int socket);
void set_disconnected_time_status(client **client, int disconnected_time);
void set_symbol_status(client **client, char letter);

//Server.c
void send_message(int client_socket, char *message, backlog **info, int logging);

//Control_Settings.c
int process_move(games_list **all_games, client_list *clients, backlog **info, int game_ID, int row, int column);

#endif