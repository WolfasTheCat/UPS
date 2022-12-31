typedef enum { 
	IN_LOBBY, 		//0
	WANNA_PLAY,		//1
	DISCONNECT,		//2
	YOU_PLAYING,		//3
	OPPONENT_PLAYING	//4
} STATES;

struct game
{
    int game_id;
    char first_player[15];
    char second_player[15];
    char game_array[3][3];
    int turn; //max 9 kol
    int now_playing;//0 pro prvního hráče, 1 pro druhého hráče
};

struct games_list
{
    int games_count;
    game **games; 
};

struct client
{
    int socket_ID;
    STATES state;
    pthread_t client_thread; //chybí knihovna
    int connected;
    int disconnected_time;
};
