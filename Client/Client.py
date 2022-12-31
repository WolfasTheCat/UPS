class Client():
    NAME = ""
    STATE = -1
    ISCONNECTED = False
    CONNECTED = True
    
    def __init__(self,name) -> None: #TODO poslat jmeno serveru a predelat jmena na serveru - ZEPTAT
        if (("|" not in name) and ((len(name)>0) and (len(name)<8)) ): 
            self.NAME = name
            self.CONNECTED = False
        else:
            print("This name isn't valid...")
            print("Valid names: 1-8 symbols where | is forbidden")
            return
