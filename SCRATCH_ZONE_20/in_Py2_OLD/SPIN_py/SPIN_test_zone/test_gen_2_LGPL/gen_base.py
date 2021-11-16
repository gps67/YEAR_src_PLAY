
# gen_base

from spin_eh import spin_py, SP_EH

class gen_base( SP_EH ):

	"""
		gen_base is for apps that GENERATE HTML files but dont parse anything
	"""

	def __init__( self, name="Generator" ):
		SP_EH.__init__( self, name )
		self.generator = name
		# first set the DTD related settings
		self.setup_HTML()
		self.dtd_line = '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">'
		# The TREE TOP element has to be created manually
		HTML_info = self.dtd_info.get_elem_info("HTML")
		HTML = spin_py.SPIN_elem_obj( HTML_info, [], [] )
		self.tree = HTML

	def default_HTML_top( self, title, css = "./style1.css" ):
		# tree TOP is the HTML element
		HTML = self.tree
		#
		HEAD = HTML.add_elem( "HEAD" )
			
		TITLE = HEAD.add_elem( "TITLE" )
		TITLE.add_text( title )
			
		LINK = HEAD.add_elem( "LINK" )
		LINK.add_attr( "rel", "stylesheet" ) 
		LINK.add_attr( "href", css ) 
		LINK.add_attr( "type", "text/css" ) 
		 
		META = HEAD.add_elem( "META" )
		META.add_attr( "name", "GENERATOR" ) 
		META.add_attr( "CONTENT", self.generator ) 
		 
		BODY = HTML.add_elem( "BODY" )

		return BODY

		# BODY.add_attr( "background", "#DDDD77" ) 
		if 0:
			NK = BODY.add_elem( "NotKnown" ) 
			NK.add_text("This element as not pre-initialised, by the KNOWN dtd_info")

	def print_tree_to_fd( self, fd ):
		lo = 10240
		hi = 102400
		pr = spin_py.SPIN_tree_print( fd, lo, hi )
			# need the DTD to write at the top of the file 
		pr.write( self.dtd_line + "\n" )
		pr.print_tree( self.tree )
		# pr = "NOW DELETED"

	def print_tree_to_filename( self, filename ):
		print "Writing", filename, "..."
		fd = open( filename, "w");
		self.print_tree_to_fd( fd )

	
