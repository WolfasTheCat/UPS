from enum import Enum


class Messages(Enum):
    CLIENT_LOGIN = "login"
    CLINET_WANT_TO_PLAY = "play"
    CLIENT_MOVE = "move"
    CLINET_DONT_WANT_TO_PLAY_AGAIN = "no_another_game"
    CLIENT_IS_CONNECTED = "is_connected"
    CLIENT_END_APP = "end_app"
    
    SERVER_ALREADY_IN_QUEUE = "already_in_queue"
    SERVER_IS_CONNECTED = "ping"
    SERVER_LOGIN_OK = "login_ok"
    SERVER_LOGIN_FAILED = "login_failed"
    SERVER_START_GAME = "starting_new_game"
    SERVER_WRONG_MOVE = "wrong_move"
    SERVER_CORRECT_MOVE = "correct_move"
    SERVER_END_MOVE = "end_turn"
    SERVER_UPDATE_GAME_ID = "game_ID_updated"
    SERVER_YOU_ARE_PLAYING = "your_turn"
    SERVER_END_GAME = "game_finished_by_win"
    SERVER_ENG_GAME_LEFT = "game_finished_by_leaving"
    SERVER_END_GAME_TIMEOUT = "game_finished_by_timeout"
    SERVER_OPPONENT_CONNECTION_LOST = "opponent_disconnected" 
    SERVER_CLIENT_CONNECTION_RESTORED = "opponent_is_back"
    