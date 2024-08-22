#!/usr/bin/env python2
#! /tools/x86_64/py/bin/python3.12
#!/usr/bin/env python

# visual clue that 7 circles EXACTLY touch

try:
	from tkinter import *
	print("using python3 tkinter");
except:
	print("failed to import tkinter (maybe because py2 not py3")
	try:
		from Tkinter import *
		print("using python2 Tkinter");
	except:
		pass

from XY import *
from MAT4 import *

unit = MAT4_unit()
angle = 90
angle = 0
pi = math.pi
turn = MAT4_rotation( 0 )

T=10
A = 5*T
B = 3*T
gap = 2*T
radius = 2*T
full_width = gap + radius * 6 + gap
half_width = full_width / 2
zero = XY( half_width, half_width )
one = XY( radius*2, 0 )
one.add( zero )
global p_prev
global p_curr
p_prev = one
p_curr = one

def act1( xy ):
	p = xy.copy()
	p.sub( zero )
	p = turn.mult_XY( p )
	p.add( zero )
	return p

def oval( w1, pa, pb, colr_fill, colr_outline ):
	if 0:
		t = w1.create_polygon(
			pa.xy(), pa.x, pb.y,
			pb.xy(), pb.x, pa.y,
			{
				"disabledfill": "",
				"outline": "black"
			}
		)
	w1.create_oval( pa.xy(), pb.xy(), {"fill": colr_fill, "outline": colr_outline } )

def circle( w1, p_centre, radius, colr_fill, colr_outline ):
	global p_prev
	global p_curr
	p_prev = p_curr
	p_curr = p_centre
	tl= XY( -radius, -radius )
	br= XY( +radius, +radius )
	
	tl.add( p_centre )
	br.add( p_centre )
	oval( w1, tl, br, colr_fill, colr_outline )

	dot = 2
	dot = 1
	tl= XY( -dot, -dot )
	br= XY( +dot, +dot )
	tl.add( p_centre )
	br.add( p_centre )
	oval( w1, tl, br, colr_fill, colr_outline )

w0 = Tk()
w1 = Canvas( w0 )
w1.configure( {"bg": "yellow"} )
w1.pack( side=LEFT, expand=1, fill=BOTH )                                
w1.configure({
	"width": full_width,
	"height": full_width
})

c1 = zero.copy()
c2 = zero.copy()

circle( w1, zero, radius, "white", "black" )
circle( w1, zero, radius, "green", "black" )

angle7 = 360.0 / 6.0
pos = one.copy()
for i in range(7):
	degs = i * angle7
	rads = pi/180.0*degs
	turn = MAT4_rotation( rads )
	pa = act1( pos )
	# pa.prints("pa") ; print
	circle( w1, pa, radius, "green", "black" )

w1.create_line( zero.xy(), p_prev.xy(), p_curr.xy(), zero.xy() )

w0.mainloop()
