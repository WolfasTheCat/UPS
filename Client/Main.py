from Connection import ConnectionManager
from Messages import Messages
from Parser import *
from States import *
from GUI import *


class Main(): #TODO Main method
     print("jsem tu")
     manager = ConnectionManager()
     #manager.create_connection("127.0.0.1",10000)
     
     GUI = tk.Tk()
     GUI.geometry("750x450")
     view = MenuView(GUI)
     view.pack(side="top", fill="both", expand=True)
     GUI.mainloop()
     
     
     #manager.send_message("Ahoj")
     #manager.recieve_message()
     #print(manager.DATA)
     
     
