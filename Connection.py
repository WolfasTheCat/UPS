from argparse import OPTIONAL
from mailbox import Message
import socket
import socketserver
import sys
import tkinter


class ConnectionManager():
    SOCKET = None
    ADDRESS = "127.0.0.1"
    LOCALPORT = 10000
    INET_ADDRESS = None
    CONNECTED = False
    READER = None
    D_HOST = None
    D_PORT = None
    
    
    
    def __init__(self) -> None: #Co to jako je?
        
        
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            option = len(sys.argv)
            match option:
                case 4:
                    pass
                    #Prenastavit local port - zatim neni potreba
                case 2:
                    if(sys.argv[2] == "a"):
                        pass
            

    
    def create_connection(self, D_HOST,D_PORT):
        try:
            self.D_HOST = D_HOST
            self.D_PORT = D_PORT
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.SOCKET = s
            s.connect((D_HOST,D_PORT))
            
            
        finally:
            self.CONNECTED = False
            print("Couldn't connect")
            tkinter.messagebox.showerror(title=None, message=None, **OPTIONAL)
        
        self.CONNECTED = True
        self.INET_ADDRESS = self.SOCKET.getpeername()
    
    def send_message(self, message):
        if ((self.CONNECTED) != True):
            print("Can't send message. Connection isn't established")
        else:
            print("Sending message: " + message)
            self.SOCKET.sendall(message)