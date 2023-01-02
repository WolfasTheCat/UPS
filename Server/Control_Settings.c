#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "Main_Header.h"

void process_move(game Game){
    
}


void insert_into_array(char *array[3][3], int row, int column, char symbol){
    if ((row < 0 || row > 2) || (column < 0 || column > 2)){
        printf("Wrong move, invalid position");
        return -1;
    }
    
    if(*array[row][column] != " "){
        printf("Wrong move, piece is already there");
        return -2;
    }
    *array[row][column] = symbol;
}

/**
 * @brief Check if somebody won in this match
 * 
 * @param array array with inserted values from players 
 * @return char N - nobody won, X - player with X, O - player with O
 */
char check_state(char array[3][3]){
    /*
    Horizontal check
    */
    if ((array[1][1]==array[1][2]) && (array[1][2] == array[1][3])){ //
        return array[1][1];
    }
    if ((array[2][1]==array[2][2]) && (array[2][2] == array[2][3])){
        return array[2][1];
    }
    if ((array[3][1]==array[3][2]) && (array[3][2] == array[3][3])){
        return array[3][1];
    }
    /*
    Vertical check
    */
    if ((array[1][1]==array[2][1]) && (array[2][1] == array[3][1])){
        return array[1][1];
    }
    if ((array[1][2]==array[2][2]) && (array[2][2] == array[3][2])){
        return array[1][2];
    }
    if ((array[1][3]==array[2][3]) && (array[2][3] == array[3][3])){
        return array[1][3];
    }
    /*
    Diagonal check
    */
    if ((array[1][1]==array[2][2]) && (array[2][2] == array[3][3])){
        return array[1][1];
    }
    if ((array[1][3]==array[2][2]) && (array[2][2] == array[3][1]))
    {
        return array[1][3];
    }
    
    /*
    No match found
    */
    return "N";
}