#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "Main_Header.h"
/**
 * @brief Create a array of games object
 * 
 * @param games array of all games on server
 */
create_array_of_games(games_list **games){
    int max_games = 10;
    (*games) = calloc(1,sizeof(games_list));
    (*games)->games_count = 0;
    (*games)->games = calloc(1, max_games * sizeof(game));
}

/**
 * @brief Create a single game object
 * 
 * @param gm game object
 * @param player_1 first player
 * @param player_2 second player
 * @param now_playing who will be playing
 */
create_game(game **gm, char *player_1, char *player_2, char *now_playing){
    (*gm) = malloc(sizeof(game));
    (*gm)->first_player = player_1;
    (*gm)->second_player = player_2;
    (*gm)->now_playing = now_playing;
    inicialize_game_field_array(gm);
}
/**
 * @brief Construct a new inicialize game field array object
 * 
 * @param gm game object
 */
inicialize_game_field_array(game **gm){
    (*gm)->game_array = malloc(9*sizeof(char));
    int lenght = 9;

    for (int i = 0; i < lenght; i++)
    {
        (*gm)->game_array[i] = ' ';
    }
    
}

/**
 * @brief Construct and add new game to array object
 * 
 * @param games array of all games
 * @param player_1 first player
 * @param player_2  second player
 * @param now_playing who will be playing
 */
add_to_game_array(games_list **games, char *player_1, char *player_2, char *now_playing){
    (*games)->games_count++;
    printf("Games count after adding one: %d\n",(*games)->games_count);
    (*games)->games = realloc((*games)->games,(*games)->games_count * sizeof(game));
    game *game = NULL;
    create_game(&game, player_1, player_2, now_playing);
    (*games)->games[((*games)->games_count) - 1] = game;
    (*games)->games[((*games)->games_count) - 1]->game_id = ((*games)->games_count-1);

}

/**
 * @brief Remove game from array object
 * 
 * @param clients array of clients
 * @param games  array of games
 * @param info  server log
 * @param game_ID ID of game
 */
remove_game_from_array(client_list **clients, games_list **games, backlog **info, int game_ID){
    int count = (*games)->games_count;
    int index;
    int update_game_ID = 0;
    for (int i = 0; i < count; i++) {
		if (i == game_ID) {					
			if (i < (count - 1)) {
				free((*games) -> games[i]);
				(*games) -> games[i] = (*games) -> games[((*games) -> games_count) - 1];		
				update_game_ID = 1;
			}
											
			(*games) -> games_count--;	
			if (((*games) -> games_count) > 0) {
				//(*all_games) -> games[((*all_games) -> games_count) - 1] = NULL;			
				(*games) -> games = realloc((*games) -> games, (*games) -> games_count * sizeof(game));

				index = i;			
				(*games) -> games[i]->game_id = index;
			}
			break;
		}
	}
    
    if (update_game_ID == 1) {
		char message_1[30];
		sprintf(message_1, "update_game_ID;%d;\n", index);
		char message_2[30];
		sprintf(message_2, "update_game_ID;%d;\n", index);
		client *cl_1 = get_client_by_name(*clients, (*games) -> games[index] -> first_player);
		send_message(cl_1 -> socket_ID, message_1, info);
		client *cl_2 = get_client_by_name(*clients, (*games) -> games[index] -> second_player);
		send_message(cl_2 -> socket_ID, message_2, info);
	}
}

/**
 * @brief Create a waiting for game array
 * 
 * @param want_play  waiting for game array
 */
create_waiting_for_game(waiting_players_for_game **want_play){
    (*want_play) = calloc(1, sizeof(waiting_players_for_game));
    (*want_play)->waiting_number = 0;
    (*want_play)->socket_ID_array = calloc(1, sizeof(int));
}

/**
 * @brief Construct and add player to waiting for game array
 * 
 * @param want_play waiting for game array
 * @param socket_ID player who wants to play
 */
add_to_waiting_for_game(waiting_players_for_game **want_play, int socket_ID){
    (*want_play)->waiting_number++;
    printf("Socked with ID %d wants to play a game\n",socket_ID);
    (*want_play)->socket_ID_array = realloc((*want_play)->socket_ID_array, (*want_play)->waiting_number * sizeof(int));
    (*want_play)->socket_ID_array[((*want_play)->waiting_number) - 1] = socket_ID;
    printf("%d clients are waiting for a game", (*want_play)->waiting_number);
}

/**
 * @brief Remove player from waiting for game array
 * 
 * @param want_play 
 * @param socket_ID 
 */
remove_from_waiting_for_game(waiting_players_for_game **want_play, int socket_ID){
	int socket;
	int count = (*want_play) -> waiting_number;
	for(int i = 0; i < count; i++) {
		socket = (*want_play) -> socket_ID_array[i];
		if (socket == socket_ID) {
			(*want_play) -> waiting_number--;			
			if (i < (count - 1)) {
				(*want_play) -> socket_ID_array[i] = (*want_play) -> socket_ID_array[((*want_play) -> waiting_number)];								
			}	
			(*want_play) -> socket_ID_array = realloc((*want_play) -> socket_ID_array, (*want_play) -> waiting_number * sizeof(waiting_players_for_game));
			printf("socket ID %d removed from queue\n", socket_ID);
			printf("%d client/s wants to play a game:\n", (*want_play) -> waiting_number);
			return;
		}
	}
}

/**
 * @brief Find game by given game name
 * 
 * @param all_games array of all games
 * @param name game name
 * @return game* 
 */
game *find_game_by_name(games_list *all_games, char *name) {
	int i;
	for (i = 0; i < all_games -> games_count; i++) {
		if (strcmp(name, all_games -> games[i] -> first_player) == 0 || strcmp(name, all_games -> games[i] -> second_player) == 0) {
			return all_games -> games[i];
		}  
	}
	return NULL;
}

/**
 * @brief Check if player is in array of players who wants to play a game
 * 
 * @param want_play - array of clients who wants to play a game
 * @param cl - logging client
 * @return int 
 */
int player_wanna_play(waiting_players_for_game *want_play, client *cl) {
	if (cl == NULL) return 0;

	int i;
	for (i = 0; i < want_play -> waiting_number; i++) {
		if (want_play -> socket_ID_array[i] == cl -> socket_ID) {
			return 1;
		}
	}
	return 0;
}