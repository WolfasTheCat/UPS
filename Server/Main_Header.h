#ifndef SERVER_MAIN_HEADER_H
#define SERVER_MAIN_HEADER_H

typedef enum { 
	IN_LOBBY, 		//0
	WANNA_PLAY,		//1
	DISCONNECT,		//2
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
    // pthread_t client_thread; //chybí knihovna
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

#endif