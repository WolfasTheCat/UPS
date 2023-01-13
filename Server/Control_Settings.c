#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "Main_Header.h"

int process_move(games_list **all_games, client_list *clients, backlog **info, int game_ID, int row, int column){
    game *current_game = (*all_games)->games[game_ID];
    client *client_1 = find_client_by_name(clients, current_game->now_playing);
    client *client_2;
    

    if(strcmp(current_game->now_playing,current_game->first_player) == 0){
        client_2 = find_client_by_name(clients, current_game->second_player);
    }
    else{
        client_2 = find_client_by_name(clients, current_game->first_player);
    }

    int move_status = insert_into_array(&(current_game->game_array), row, column, client_1->symbol); //! Možná je blbě odkaz na herní pole (current_game->game_array) by mohlo stačit

    if(move_status == -1){
        send_message(client_1->socket_ID,"wrong_move|-1|\n",info,1);
        return -1;
    }
    else if (move_status == -2)
    {
        send_message(client_1->socket_ID,"wrong_move|-2|\n",info,1);
        return -2;
    }
    else{

        char message[1024];
        sprintf(message, "opponent_move|%d|%d|\n", row, column);
        send_message(client_1->socket_ID,"correct_move|\n",info,1);
        send_message(client_2->socket_ID,message,info,1);
    }
    
    current_game->turn == current_game->turn++;

    char who_won = check_state(current_game->game_array);
    if ((who_won == 'X') && (client_1->symbol == 'X')){  //! Nejspíš jsou blbě klienti, protože nahoře se přehazují - SNAD VYŘEŠENO
        game_end(all_games, game_ID, 1, clients, client_1->socket_ID, client_2->socket_ID, info);
    }
    if ((who_won == 'O') && (client_1->symbol == 'O'))
    {
        game_end(all_games, game_ID, 2, clients, client_2->socket_ID, client_1->socket_ID, info);
    }
    
    if ((current_game->turn > 9) && (who_won == 'N'))
    {
        game_end(all_games, game_ID, 0, clients, client_1->socket_ID, client_2->socket_ID, info);
    }
    
    //Možná nadbytečné
    char correct_message[100];
    sprintf(correct_message, "correct_move|%d|%d|\n", row, column);
    send_message(client_2,correct_message,info,1); //Posílá message oponentu, že hráč provedl pohyb
    //

    set_state_status(&client_1,4);
    set_state_status(&client_2,3);

    send_message(client_1->socket_ID, "end_move|\n", info,1);
    send_message(client_2->socket_ID, "play_next_player|\n", info,1);

    current_game->now_playing = client_2->nickname;
    
    return 0;
}

int insert_into_array(char **array, int row, int column, char symbol){
    if ((row < 0 || row > 2) || (column < 0 || column > 2)){
        printf("Wrong move, invalid position");
        return -1;
    }
    
    if(*array[row * 3 + column] != ' '){
        printf("Wrong move, piece is already there");
        return -2;
    }
    *array[row * 3 + column] = symbol;
    return 0;
}

/**
 * @brief Check if somebody won in this match
 * 
 * @param array array with inserted values from players 
 * @return char N - nobody won, X - player with X, O - player with O
 */
char check_state(char *array){
    /*
    Horizontal check
    */
    if ((array[0]==array[1]) && (array[1] == array[2]) && (array[0] != ' ')){
        return array[0];
    }
    if ((array[3]==array[4]) && (array[4] == array[5]) && (array[3] != ' ')){
        return array[3];
    }
    if ((array[6]==array[7]) && (array[7] == array[8]) && (array[6] != ' ')){
        return array[6];
    }
    /*
    Vertical check
    */
    if ((array[0]==array[3]) && (array[3] == array[6]) && (array[0] != ' ')){
        return array[0];
    }
    if ((array[1]==array[4]) && (array[4] == array[7]) && (array[1] != ' ')){
        return array[1];
    }
    if ((array[2]==array[5]) && (array[5] == array[8]) && (array[2] != ' ')){
        return array[2];
    }
    /*
    Diagonal check
    */
    if ((array[0]==array[4]) && (array[4] == array[8]) && (array[0] != ' ')){
        return array[0];
    }
    if ((array[2]==array[4]) && (array[4] == array[6]) && (array[2] != ' ')){
        return array[2];
    }

    return "N";
}

void game_end(games_list **games, int game_ID, int status, client_list **clients, int first_player_socket, int second_player_socket, backlog **info){
    switch (status){
        case 0:
            send_message(first_player_socket, "end_game|draw|\n", info,1);
            send_message(second_player_socket, "end_game|draw|\n", info,1);
            break;
        case 1:
            send_message(first_player_socket, "end_game|win;|n", info,1);
            send_message(second_player_socket, "end_game|lost|\n", info,1);
            break;
        case 2:
            send_message(first_player_socket, "end_game|lost|\n", info,1);
            send_message(second_player_socket, "end_game|win|\n", info,1);
            break;
    }

    client *cl1 = find_client_by_socket(*clients,first_player_socket);
    client *cl2 = find_client_by_socket(*clients,second_player_socket);

    set_state_status(&cl1,0);
    set_state_status(&cl2,0);
    remove_game_from_array(clients, games, info, game_ID);
	return;
}