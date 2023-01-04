#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "Main_Header.h"

void process_move(game *game, client_list *clients){
    int turn = game->turn;
    
}


void insert_into_array(char **array, int row, int column, char symbol){
    if ((row < 0 || row > 2) || (column < 0 || column > 2)){
        printf("Wrong move, invalid position");
        return -1;
    }
    
    if(*array[row * 3 + column] != ' '){
        printf("Wrong move, piece is already there");
        return -2;
    }
    *array[row * 3 + column] = symbol;
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