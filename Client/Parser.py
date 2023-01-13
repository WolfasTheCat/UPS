from sys import argv


class Parser(): #TODO Parser of responses from server
    MESSAGE = None
    
    def __init__(self) -> None:
        pass
    
    def parse_message(self, message):
        self.MESSAGE = message.split("|")
        
        if self.MESSAGE[0] is "OK":
            pass