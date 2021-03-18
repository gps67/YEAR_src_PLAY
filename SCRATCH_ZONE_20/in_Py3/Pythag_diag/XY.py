import math

class XY:
	""" an (x,y) point """
	def __init__( self, x, y ):
		self.x = x
		self.y = y

	def copy( self ):
		return XY( self.x, self.y )

	def add( self, vect ):
		self.x += vect.x
		self.y += vect.y

	def sub( self, vect ):
		self.x -= vect.x
		self.y -= vect.y

	def xy( self ):
		""" convert XY to tuple that Tk likes """
		return (self.x, self.y )

	def prints( self, name ):
	#	print( "hello" )
		print( "%s = (%7.2f, %7.2f )"%( name, self.x, self.y ) )
# OK 2.7	print "%s = (%7.2f, %7.2f )"%( name, self.x, self.y )

	def left( self, dx ):
		self.x -= dx

	def right( self, dx ):
		self.x += dx

	def up( self, dy ):
		self.y -= dy

	def down( self, dy ):
		self.y += dy

def XY_mid( p1, p2 ):
	return XY( (p1.x + p2.x)/2, (p1.y + p2.y)/2 )

def XY_mid3( p1, p2, p3 ):
	# Im sure this is not what I want, but it is vagely like it
	return XY( (p1.x + p2.x + p3.x)/3, (p1.y + p2.y + p3.y)/3 )

def XY_unit_vector( p1, p2, unit=1.0 ):
	# the unit vector along the direction p1 to p2
	p12_x = (p1.x + p2.x) / 2
	p12_y = (p1.y + p2.y) / 2
	v12_x = p2.x - p1.x
	v12_y = p2.y - p1.y
	v12_len = math.sqrt( v12_x * v12_x + v12_y * v12_y ) / unit
	if v12_len < 0.001: v12_len = 0.001
	return XY( v12_x / v12_len, v12_y / v12_len )

def XY_step_towards( p1, step, p2 ):
	p3 = XY_unit_vector( p1, p2, step )
	p3.add( p1 )
	return p3
		

	

