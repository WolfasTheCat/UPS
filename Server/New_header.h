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
    /* data */
};
