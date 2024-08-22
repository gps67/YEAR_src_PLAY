#!/usr/bin/env python3
#!/usr/bin/env python2.7
#!/usr/bin/env python
#!/usr/bin/env python2.2
# visual clue to let you suss Pythagorus's theorum, see pythag.html

from tkinter import *
# from Tkinter import * # ok in python2.7
from XY import *
from MAT4 import *

# sin()/cos() use radians not degrees
# 2 pi is a full circle
# why is pi the MIDDLE half of spin ?
pi = math.pi
pi_180 = math.pi/180.0

class rotated_sub_surface:

	def __init__( self, canvas ):
		self.canvas = canvas
		self.set_zero( XY(0,0) )
		self.rotated_degs( 0 )

	def rotated_degs( self, degs ):
		global p_180
		rads = pi_180 * degs
		self.turn = MAT4_rotation( rads )
		self.degs_kept = degs

	def set_zero( self, xy ):
		self.zero = xy.copy()

	def transform_point( self, xy ):
		p = xy.copy()
		p.sub( self.zero )
		p = self.turn.mult_XY( p )
		p.add( self.zero )
		return p

	def draw_mapped_text( self, pa, text, opts ):
		#
		p1 = self.transform_point( pa )
		self.canvas.create_text( p1.xy(), "-text", text, opts )

	def draw_mapped_line( self, pa, pb, opts ):
		#
		p1 = self.transform_point( pa )
		p2 = self.transform_point( p2 )
		canvas.create_polygon( p1.xy(), p2.xy(), opts )

	def draw_mapped_triangle( self, pa, pb, pc, opts ):
		#
		p1 = self.transform_point( pa )
		p2 = self.transform_point( pb )
		p3 = self.transform_point( pc )
		t = self.canvas.create_polygon(
			p1.xy(),
			p2.xy(),
			p3.xy(),
			opts
		)

class Shape_tri:

	def __init__( self, p1, p2, p3, opts_1, opts_2, with_ABC=1 ):
		self.p1 = p1
		self.p2 = p2
		self.p3 = p3
		self.opts_1 = opts_1
		self.opts_2 = opts_2
		self.p_mid = XY_mid3( p1, p2, p3 )
		self.adj_mid = XY( 10, 10 )
		self.with_ABC = with_ABC

	def draw_mapped( self, shape_one ):
		shape_one.draw_mapped_triangle(
			self.p1,
			self.p2,
			self.p3,
			self.opts_1
		)
		if self.with_ABC:
		 self.add_midline_labels( shape_one )

	def add_midline_labels( self, shape_one ):
		 self.add_midline_label( shape_one, self.p1, self.p2, "C" )
		 self.add_midline_label( shape_one, self.p2, self.p3, "A" )
		 self.add_midline_label( shape_one, self.p3, self.p1, "B" )

	def add_midline_label( self, shape_one, p1, p2, str ):
		step = 15 # 10 pixels
		p12 = XY_mid( p1, p2 )
		vec =  XY_unit_vector( p12, self.p_mid, step )
		p12.add( vec )
		shape_one.draw_mapped_text( p12, str, self.opts_2 )


class Shape_One( rotated_sub_surface ):

	def __init__( self, canvas, scale, A1, B1 ):
		rotated_sub_surface.__init__( self, canvas )
		self.tag = "a1"
		self.opts_white = {
			"tag": self.tag,
			"fill": "white",
			"outline": "black"
		}
		self.opts_green = {
			"tag": self.tag,
			"fill": "green",
			"outline": "black"
		}
		self.opts_line = {
			"tag": self.tag,
			"fill": "blue",
			"outline": "blue"
		}
		self.opts_labels = {
			"tag": self.tag,
			"fill": "red"
		}
		self.calc_canvas_layout( scale, A1, B1 )
		self.change_shape_one( scale, A1, B1 )

	def calc_shape_layout( self, scale, A1, B1 ):

		self.A = A1 * scale
		self.B = B1 * scale
		self.C = math.sqrt( A1*A1 + B1*B1 ) # post boot-strap proof ;-)
		self.GAP = ( A1 + B1 ) / 5.0
		self.G = self.GAP * scale 

	def calc_canvas_layout( self, scale, A1, B1 ):
		""" set zero width height to suit initial layout """
		global set_once
		try:
			junk = set_once
			return
		except:
			set_once = 1
			pass

		self.calc_shape_layout( scale, A1, B1 )

		# the size of the canvas is effected by
		# the size of the shape
		#
		full_width = self.G + self.A + self.B + self.G
		full_height = full_width
		half_width = full_width / 2
		half_height = half_width
		#
		self.canvas.configure({
			"width": full_width,
			"height": full_height 
		})
		# rotation is around 'zero', middle of window
		self.zero = XY( half_width, half_height )

	def change_shape_one( self, scale, A1, B1 ):
		#
		# the shape is a set of points, untransformed
		# zero is calculated as the middle, with a gap
		# set canvas size
		#
		self.calc_shape_layout( scale, A1, B1 )
		A = self.A
		B = self.B

		# (0,0) is at top left
		# Start at top point
		X = self.G + B
		Y = self.G

		POS = XY( X, Y )

		self.p1 = POS.copy()
		POS.down( B )
		self.p5 = POS.copy()
		POS.right( A )
		self.p2 = POS.copy()
		POS.left( B )
		self.p6 = POS.copy()
		POS.down( A )
		self.p3 = POS.copy()
		POS.up( B )
		self.p7 = POS.copy()
		POS.left( A )
		self.p4 = POS.copy()
		POS.right( B )
		self.p8 = POS.copy()

		if 0:
		 POS.up( A )
		 self.p1_TEST = POS.copy()
		 # now check that p1_test == p1

		self.p23 = self.p3.copy()
		self.p23.right( B )

		self.T1 = Shape_tri( self.p1, self.p2, self.p5,  self.opts_green, self.opts_labels, 0 )
		self.T2 = Shape_tri( self.p2, self.p3, self.p6,  self.opts_green, self.opts_labels, 1 )
		self.T3 = Shape_tri( self.p3, self.p2, self.p23, self.opts_white, self.opts_labels, 1 )
		self.T4 = Shape_tri( self.p3, self.p4, self.p7,  self.opts_green, self.opts_labels, 0 )
		self.T5 = Shape_tri( self.p4, self.p1, self.p8,  self.opts_green, self.opts_labels, 1 )

	def show_degs( self ):
		global speed
		degs_str = "%5.1f degs" % self.degs_kept
		degs_str = "%5.1f degs %d ms " % ( self.degs_kept, speed )
		degs_str = "degs %5.1f ms %2d A %3.1f B %3.1f" % ( self.degs_kept, speed, self.A, self.B )
		canvas.create_text( 120, 20, "-text", degs_str, {
				"tag": self.tag
			}
		)

	def show_p_label( self, point, name ):
		p1 = self.transform_point( point )
		dx = 10
		dy = 10
		if( point.x < 0 ): dx = -dx
		if( point.y < 0 ): dy = -dy
		adj = XY( dx, dy )
		p2 = p1.copy()
		p2.add( adj )
		p3 = p2.copy()
		p3.add( adj )
		self.canvas.create_text( p3.xy(), "-text", name, {
				"tag": self.tag
                        }
                )
		self.canvas.create_polygon(
			p1.xy(),
			p2.xy(),
			self.opts_line
		)

	def show_p_labels( self ):
		list = [ "p23", "p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8" ]
		for pname in list:
			point = self.__dict__[ pname ]
			self.show_p_label( point, pname )

		# PROBLEM: want letter WITHIN triangle
		# CONFUSE A/B along same side

	def re_draw( self ):
		self.canvas.delete( self.tag )

		self.T1.draw_mapped( self )
		self.T2.draw_mapped( self )
		self.T3.draw_mapped( self )
		self.T4.draw_mapped( self )
		self.T5.draw_mapped( self )
		self.show_degs()
		# self.show_p_labels()


def pauses( ms ):
	canvas.update_idletasks()
	if ms : canvas.after( ms )

def canvas_print( canvas, filename ):
	# .canvas postscript output filename
	canvas.update_idletasks()
	opts = {
		'file': filename
	}
	canvas.postscript( opts )
	#
	# VALID OPTIONS: colormap, colormode, file, fontmap,
        # height, pageanchor, pageheight, pagewidth, pagex, pagey,
        # rotate, witdh, x, y.
	# lib/Tkinter.py

w0 = Tk()
canvas = Canvas( w0 )
canvas.configure( {"bg": "yellow"} )
canvas.pack( side=LEFT, expand=1, fill=BOTH )                                

scale=50 # (2 + 3 + 5 + 2 ) * 50 = 600 pixels window (reduce to suit)
A1 = 5
B1 = 3
# C = almost 6 (not 3/4/5 but it looks good)
# C = SQRT( A*A + B*B ) # but we dont know that yet ;-)

shape_one = Shape_One( canvas, scale, A1, B1 )

degs_confuse = 45	# can you be sure its aligned square?
degs_confuse = 0	# show how horiz/vert/90' help
N_steps_60 = 60
MS10=10

def anim_spin( angle_1, angle_2, n_one, nsteps, delay ):
	""" prepare a number of animation frames, display at fake speed """
	nsteps = nsteps * 1.0
	degs_step = (angle_2 - angle_1) / nsteps
	for i in range( n_one, int( nsteps) + 1 ):
		degs = angle_1 + i * degs_step

		shape_one.rotated_degs( degs )
		shape_one.re_draw()
		pauses( delay )

def anim_iris( A1, B1, n_one, nsteps, delay ):
	""" prepare a number of animation frames, display at fake speed """
	# now show what happens when the triangle changes shape
	# A and B were set above, (A<B) to look OK
	# here A2 varies from A1 to B1, which looks good
	# C stays the same, and we must know A^2 + B^2 == C^2
	# GAP and zero stay the same
	# rotation as as left by above

	C1 = math.sqrt( A1*A1 + B1*B1 )

	nsteps = int( nsteps ) # must be int
	step_size = (A1 - B1 ) / float( nsteps )

	for step in range(n_one, nsteps + 1):
		A2 = A1 - (step * step_size )
		B2 = math.sqrt( C1*C1 - A2*A2 )
		shape_one.change_shape_one( scale, A2, B2 )
		shape_one.re_draw()
		pauses( delay )

degs_1 = degs_confuse
degs_2 = degs_1 + 90
degs_3 = degs_2 + 90
delay = 0
speed = delay
anim_spin( degs_1, degs_1, 1, 1, 0 )
if 1: canvas_print( canvas, "pythag.ps" )

if 0:
 for repeat in range(6):
 	speed = delay

 	anim_iris( A1, B1,         1, N_steps_60, MS10 )
 	anim_spin( degs_1, degs_2, 1, N_steps_60, MS10 )
 	anim_iris( B1, A1,         1, N_steps_60, MS10 )
 	anim_spin( degs_2, degs_3, 1, N_steps_60, MS10 )
 	anim_iris( A1, B1,         1, N_steps_60, MS10 )
 	anim_spin( degs_3, degs_2, 1, N_steps_60, MS10 )
 	anim_iris( B1, A1,         1, N_steps_60, MS10 )
 	anim_spin( degs_2, degs_1, 1, N_steps_60, MS10 )

 	if 0:
 		degs_1 += 15
 		degs_2 += 15
 		delay += 4

w0.mainloop()


