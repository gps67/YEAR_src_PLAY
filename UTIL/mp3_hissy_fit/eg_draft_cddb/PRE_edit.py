#!/usr/bin/env python3

import tkinter as tk
from tkinter import ttk

def build_window( main_win ):
    # grid = tk.Grid( main_win )
    root = main_win
    content = ttk.Frame(root)
    frame = ttk.Frame(content, borderwidth=5, relief="ridge", width=200, height=100)
    namelbl = ttk.Label(content, text="Name")
    name = ttk.Entry(content)
    
    onevar = tk.BooleanVar(value=True)
    twovar = tk.BooleanVar(value=False)
    threevar = tk.BooleanVar(value=True)
    
    one = ttk.Checkbutton(content, text="One", variable=onevar, onvalue=True)
    two = ttk.Checkbutton(content, text="Two", variable=twovar, onvalue=True)
    three = ttk.Checkbutton(content, text="Three", variable=threevar, onvalue=True)
    ok = ttk.Button(content, text="Okay")
    cancel = ttk.Button(content, text="Cancel")
    
    content.grid(column=0, row=0)
    frame.grid(column=0, row=0, columnspan=3, rowspan=2)
    namelbl.grid(column=3, row=0, columnspan=2)
    name.grid(column=3, row=1, columnspan=2)
    one.grid(column=0, row=3)
    two.grid(column=1, row=3)
    three.grid(column=2, row=3)
    ok.grid(column=3, row=3)
    cancel.grid(column=4, row=3)

window = tk.Tk()

# X11 is stupid and creates new toplevel windows, buried deep
# window.tkraise()
window.lift() # raise is a reserved word # tkraise #
window.title("EDIT CDDB");

build_window( window )

window.mainloop()

