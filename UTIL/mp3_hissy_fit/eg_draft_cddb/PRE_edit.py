#!/usr/bin/env python3

import tkinter as tk

window = tk.Tk()

# X11 is stupid and creates new toplevel windows, buried deep
# window.tkraise()
window.lift() # raise is a reserved word # tkraise #

window.title("EDIT CDDB");

window.mainloop()

