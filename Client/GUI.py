import tkinter as tk
from tkinter import * 

#Dynamic - add self as parameter
#Static - Name of class

#Creates game enviroment
class GameVisualisation(tk.Frame): #TODO game visualisation
    turn = True
    count = 1

    def insert_symbol(self,button,symbol):
        button["text"] = symbol

    
    def set_state(self,button):
        if button["text"] == " "  and self.turn == True:
            if(self.count % 2 == 0):
                self.insert_symbol(button, "X")
            else:
                self.insert_symbol(button, "O")
            self.count += 1 #TODO predelat
            self.turn = True
            
    
    def __init__(self):
        self.game = Tk()
        self.game.title('Tic-Tac-Toe')

        self.turn = True
        self.count = 1

        #Create buttons for game board
        b1 = Button(self.game,name= "b1" ,text= " ", font=("Helvetica",20), height=3,width=6, bg=("white"), command=lambda: self.set_state(b1))
        b2 = Button(self.game,name= "b2", text= " ", font=("Helvetica",20), height=3,width=6, bg=("white"), command=lambda: self.set_state(b2))
        b3 = Button(self.game,name= "b3", text= " ", font=("Helvetica",20), height=3,width=6, bg=("white"), command=lambda: self.set_state(b3))

        b4 = Button(self.game,name= "b4", text= " ", font=("Helvetica",20), height=3,width=6, bg=("white"), command=lambda: self.set_state(b4))
        b5 = Button(self.game,name= "b5", text= " ", font=("Helvetica",20), height=3,width=6, bg=("white"), command=lambda: self.set_state(b5))
        b6 = Button(self.game,name= "b6", text= " ", font=("Helvetica",20), height=3,width=6, bg=("white"), command=lambda: self.set_state(b6))

        b7 = Button(self.game,name= "b7", text= " ", font=("Helvetica",20), height=3,width=6, bg=("white"), command=lambda: self.set_state(b7))
        b8 = Button(self.game,name= "b8", text= " ", font=("Helvetica",20), height=3,width=6, bg=("white"), command=lambda: self.set_state(b8))
        b9 = Button(self.game,name= "b9", text= " ", font=("Helvetica",20), height=3,width=6, bg=("white"), command=lambda: self.set_state(b9))

        #Grid our buttons to the screen
        b1.grid(row=0,column=0)
        b2.grid(row=0,column=1)
        b3.grid(row=0,column=2)

        b4.grid(row=1,column=0)
        b5.grid(row=1,column=1)
        b6.grid(row=1,column=2)

        b7.grid(row=2,column=0)
        b8.grid(row=2,column=1)
        b9.grid(row=2,column=2)

        self.game.mainloop()

#Creates login window 
class LoginView(tk.Frame): #TODO Add opening of game visualisation

    """
    Send to server his name -> accepted -> Wait/Open game
                            -> failure  -> display problem message
    """
    
    def b_find_clicked(self,b):
            self.find_game(b)
    
    def find_game(self,name):
            game = GameVisualisation()
            
        
    
    def __init__(self, root):

        tk.Frame.__init__(self, root)

        username_label = Label(root,text="User name", font=("Helvetica",10)).grid(row=0, column=0)
        username = ""
        username_entry = Entry(root, textvariable=username).grid(row=0, column=1,columnspan = 2, sticky = tk.W+tk.E)  
        b_find = Button(root, text= "Find", font=("Helvetica",20), height=3,width=6, bg=("white"), command=lambda: self.b_find_clicked(b_find)).grid(row=3, column=0, columnspan = 3, sticky = tk.W+tk.E)        

#Creates menu view
class MenuView(tk.Frame):
    
    def b_exit_clicked(self,b):
            exit(0)

    def b_multi_clicked(self,root,b):
        login_screen = tk.Tk()
        login_screen.geometry("220x130")
        login = LoginView(login_screen)
        root.destroy()
        login_screen.mainloop()

    def __init__(self, root):

        tk.Frame.__init__(self, root)
        b_multi  =   Button(root, text= "Multiplayer", font=("Helvetica",20), height=3,width=6, bg=("white"), command=lambda: self.b_multi_clicked(root,b_multi))
        b_multi.pack(side="top", fill="both", expand=True)
        b_leave =   Button(root, text= "Leave", font=("Helvetica",20), height=3,width=6, bg=("white"), command=lambda: self.b_exit_clicked(b_leave))
        b_leave.pack(side="top", fill="both", expand=True)

class GUI():
    main_menu = tk.Tk()
    main_menu.geometry("750x450")
    view = MenuView(main_menu)
    view.pack(side="top", fill="both", expand=True)
    main_menu.mainloop()


if __name__=='__main__':
    beggining = GUI()
