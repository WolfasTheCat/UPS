from Connection import ConnectionManager
from Messages import Messages
from Parser import *
from GUI import *
from States import *


class Main(): #TODO Main method
     manager = ConnectionManager()
     manager.create_connection("127.0.0.1",10000)
     manager.send_message("Ahoj")
     manager.recieve_message()
     print(manager.DATA)
     
     
     def login():
          pass
          #Messages.CLIENT_END_APP
