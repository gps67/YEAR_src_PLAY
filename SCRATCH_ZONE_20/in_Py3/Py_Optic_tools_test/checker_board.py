#!/usr/bin/env python3.12 
#!/usr/bin/env python3

# optical illusion
# needs better fgbg choice
# needs selection of lens - per dot !
# needs less contrast on dots - semi 3D shine effect
# ... 

from tkinter import *                                                           



w_square = 40
w_circle = 5
w_circle = 10
w_gap = 2	# from edge of square to circle
w_gap_board = 2	# from edge of square to edge of window

N_squares_15 = 15
N_mid_7 = 7

full_width = w_gap_board + N_squares_15 * w_square + w_gap_board
# print( "full_width", full_width );

colr_dot_dark = "black"
colr_dot_light = "white"
colr_dot_dark = "#b60"
colr_dot_light = "#fe0"
colr_square_light = "yellow"
colr_square_dark = "#da0"

dot_dark = {"fill": colr_dot_dark, "outline": colr_dot_dark } 
dot_light = {"fill": colr_dot_light, "outline": colr_dot_light } 
square_dark = {"fill": colr_square_dark, "outline": colr_square_dark } 

dots_bc = "bc" # down
dots_ad = "ad" # up
dots_ab = "ab" # top 2
dots_cd = "cd" # bottom 2
dots_ac = "ac" # left 2
dots_bd = "bd" # right 2
dots_nn = "  " # none

# dots_b for (mid-1, 0)
# and others like it
# also no so much contrast

def is_dot( dots, ch ):
   return -1 != dots.find(ch)

def is_lens( i, j ):
	return 1
	mid = N_mid_7
	if i == mid: return 1
	if j == mid: return 1
	dy = j - mid
	dx = i - mid
	r2 = dy * dy + dx * dx
	return r2 < (mid * mid )

def pick_dots( i, j ):

   mid = N_mid_7

   if not is_lens( i, j ):
    return ""

###if
   if i < mid: # Y top half
    if j < mid: # X left half
     return dots_bc
    if j == mid: # mid line
     return dots_cd
    else:
     return dots_ad

###if
   if i == mid: # horiz mid line
    if j < mid:
     return dots_bd
    if j == mid:
     return dots_nn # centre point
    else:
     return dots_ac

    
###if
   ## i > mid: # Y bottom half
   if j < mid: # X left half
     return dots_ad
   if j == mid: # mid line
     return dots_ab
   else:
     return dots_bc

###if
   return "abcd"

def square( w1, x1, y1, x2, y2, i, j ):
   is_light = (i^j) & 1 
   if( is_light ): 
    pass
   else:
    w1.create_rectangle( x1, y1, x2, y2, square_dark )
    pass

   if is_light:
    dot_colr = dot_dark
   else:
    dot_colr = dot_light

   dots = pick_dots( i, j )
   # print("dots", dots, i, j )

   dot_a_top = y1 + w_gap
   dot_a_btm = dot_a_top + w_circle
   dot_a_left = x1 + w_gap
   dot_a_right = dot_a_left + w_circle

   dot_b_top = dot_a_top
   dot_b_btm = dot_a_btm
   dot_b_right = x2 - w_gap
   dot_b_left = dot_b_right - w_circle

   dot_c_btm = y2 - w_gap
   dot_c_top = dot_c_btm - w_circle
   dot_c_left = dot_a_left
   dot_c_right = dot_a_right

   dot_d_btm = dot_c_btm
   dot_d_top = dot_c_top
   dot_d_left = dot_b_left
   dot_d_right = dot_b_right

   colr_fill = colr_dot_dark
   colr_outline = colr_fill
   if is_dot(dots,'a'):
    w1.create_oval( ( dot_a_left, dot_a_top ), ( dot_a_right, dot_a_btm  ),  dot_colr )
   if is_dot(dots,'b'):
    w1.create_oval( ( dot_b_left, dot_b_top ), ( dot_b_right, dot_b_btm  ),  dot_colr )
   if is_dot(dots,'c'):
    w1.create_oval( ( dot_c_left, dot_c_top ), ( dot_c_right, dot_c_btm  ),  dot_colr )
   if is_dot(dots,'d'):
    w1.create_oval( ( dot_d_left, dot_d_top ), ( dot_d_right, dot_d_btm  ),  dot_colr )

def board( w1 ):
 for i in range(N_squares_15):
  y1 = w_gap_board + i * w_square
  y2 = y1 + w_square
  for j in range(N_squares_15):
   x1 = w_gap_board + j * w_square
   x2 = x1 + w_square
   square( w1, x1, y1, x2, y2, i, j )


w0 = Tk()
w1 = Canvas( w0 )
w1.configure( {"bg": colr_square_light } )
w1.pack( side=LEFT, expand=1, fill=BOTH )                                
w1.configure({
	"width": full_width,
	"height": full_width
})

import sys
print ( "Python sys.version %s" % sys.version)

# import tkinter
# tcl_intrpr = tkinter.Tcl()
tcl_intrpr = Tcl()
print ( "tcl_version %s" %
	tcl_intrpr.call("set", "tcl_version")
)
# /tools/x86_64/py/bin/python3.12
# /tools/x86_64/tcl/bin/tclsh9.0
# -BUT-
# tk8.6 is winning somehow
# TODO configure python so that it finds tk9.0 # TODO #
# that is for a module that works with tkinter and is 9.0+
# 

# exit(1)

board( w1 )
w1.update()
f="eg_output"
f_ps = f + ".ps"
f_png = f + ".png"
w1.postscript(file=f_ps, colormode='color')
import os
os.system("ps2png " + f_ps + " " + f_png )
os.system("m " + f_png )


w0.mainloop()
