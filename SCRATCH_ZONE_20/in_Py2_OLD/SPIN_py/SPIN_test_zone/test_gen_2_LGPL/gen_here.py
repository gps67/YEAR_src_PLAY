
# gen_here.py
#
# customised to this site, this stylesheet, etc


# from gen_base import spin_py, gen_base
from gen_base import gen_base

class gen_here( gen_base ):
	"""
	"""

	def __init__( self, filename, title, css="./style1.css", bg="#DDDD77" ):
		gen_base.__init__( self, "spin_py" )
		self.filename = filename
		el_HTML = self.tree
		el_BODY = self.default_HTML_top( title, css )
		el_BODY.add_attr( "background", bg ) 
		self.el_BODY = el_BODY

	def print_tree_to_file( self ):
		self.print_tree_to_filename( self.filename )

