# MAT4 is a 2x2 matrix
# my math is a bit rusty, so it might be upside down
# but it seems to do the job

import math
from XY import *

class MAT4:
 def __init__( self, a, b, c, d ):
 	""" initialise a 2x2 matrix """
 	self.a = a
 	self.b = b
 	self.c = c
 	self.d = d

 def mult_MAT4( A, B ):
 	""" multiply two matrices (self, B), returning the third """
 	C_a = A.a * B.a  + A.b * B.c
 	C_b = A.a * B.b  + A.b * B.d
 	C_c = A.c * B.a  + A.d * B.c
 	C_d = A.c * B.b  + A.d * B.d
 	return MAT4( C_a, C_b, C_c, C_d )

 def mult_XY( A, B ):
 	"""multiply point B by this A, return new point """
 	x = A.a * B.x  + A.b * B.y
 	y = A.c * B.x  + A.d * B.y
 	return XY( x, y )

 def prints( A, name ):
 	""" debug prints """
 	print( "%s ( %8.4f %8.4f )" %(name, A.a, A.b ) )
 	print( "%s ( %8.4f %8.4f )" %(name, A.c, A.d ) )




def MAT4_unit():
	""" create a unit/identity matrix """
	return MAT4( 1, 0, 0, 1 )

def MAT4_rotation( angle_radians ):
	""" create a rotation matrix """
	# as the number of degrees increases
	# the shape should turn LEFT
	s = math.sin( angle_radians )
	c = math.cos( angle_radians )
	return MAT4( c, s, -s, c )
