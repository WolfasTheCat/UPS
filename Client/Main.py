from Connection import ConnectionManager


class Main(): #TODO Main method
     manager = ConnectionManager()
     manager.create_connection("127.0.0.1",10000)
     manager.send_message("Ahoj")
     manager.recieve_message()
     print(manager.DATA)