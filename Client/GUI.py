import tkinter as tk
from tkinter import font as tkfont
from Action_manager import *

class ResultView(tk.Tk):
    
    def __init__(self, parent, controller, action_manager):
        pass

class GameView(tk.Tk):
    
    
    def __init__(self, parent, controller, action_manager):
        self.inicialize_array()
        
    
    
    
    def inicialize_array(self):
        self.game_array = [9]
        for index in range(9):
            self.game_array[index] = " "
            
class LoginView(tk.Tk):
    def __init__(self, parent, controller, action_manager):
        pass

class MenuView(tk.Tk):
    def __init__(self, parent, controller, action_manager):
        tk.Frame.__init__(self,parent)
        self.controller = controller
        b_multi  =   tk.Button(self, text= "Multiplayer", font=controller.title_font , height=3,width=6, bg=("white"), command=lambda: self.b_multi_clicked(root,b_multi))
        b_multi.pack(side="top", fill="both", expand=True)
        b_leave =   tk.Button(self, text= "Leave", font=("Helvetica",20), height=3,width=6, bg=("white"), command=lambda: self.b_exit_clicked(b_leave))
        b_leave.pack(side="top", fill="both", expand=True)
        
    
    def b_exit_clicked(self,b):
            exit(0)

    def b_multi_clicked(self,root,b):
        pass

class App(tk.Tk):
    def __init__(self, action_controller):
            tk.Tk.__init__(self, action_controller)

            # the container is where we'll stack a bunch of frames
            # on top of each other, then the one we want visible
            # will be raised above the others
            self.title_font = tkfont.Font(family='Helvetica', size=18, weight="bold", slant="italic")
            
            container = tk.Frame(self) 
            container.pack(side="top", fill="both", expand=True)
            container.grid_rowconfigure(0, weight=1)
            container.grid_columnconfigure(0, weight=1)

            self.frames = {}
            for F in (MenuView, LoginView, GameView, ResultView):
                stage_name = F.__name__
                frame = F(parent = container,controller = self, action_manager = action_controller) 
                self.frames[stage_name] = frame 

                # put all of the pages in the same location;
                # the one on the top of the stacking order
                # will be the one that is visible.
                frame.grid(row=0, column=0, sticky="nsew")

            self.show_frame("MenuView")

    def show_frame(self, stage_name):
        '''Show a frame for the given page name'''
        frame = self.frames[stage_name]
        frame.tkraise() 

    

if __name__=='__main__':
        action_controller = Action_manager()
        beggining = App(action_controller)
        beggining.mainloop()