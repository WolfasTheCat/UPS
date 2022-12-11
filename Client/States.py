from enum import Enum


class States(Enum):
    LOGGING = -1
    IN_LOBBY = 0
    WANNA_PLAY = 1
    DISCONNECT = 2
    YOU_PLAYING = 3
    OPPONENT_PLAYING = 4