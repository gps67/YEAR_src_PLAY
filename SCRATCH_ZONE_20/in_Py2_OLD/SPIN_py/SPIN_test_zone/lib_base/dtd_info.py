# python component

from dtd_info_base import Dtd_info_base

class Dtd_info(Dtd_info_base):
	"""
		lists of known tags for known DTD's

		To attach code to tags, put that in EH,
		this is mostly for tree_print

		TODO: elem_info needs block !=== breaks_word
	"""

	def __init__( self, pool ):
		Dtd_info_base.__init__(self, pool )

	def setup_HTML( self ):
		"""
			In your EH, each element might have handler code lines,
			those would be set here.
			
			It really helps the tree_printer to have most tags
			known. Those are set in dtd_info !

			TODO Split element_breaks_word -vs- is_block

			TODO more style attributes, such as omit-end-tag
			not to mention attr names ... iie DTD + CSS + INFO

			TODO: FORM, INPUT are flowing blocks ?
		"""
		for tag in [ 
			"HTML", "HEAD",
			"BODY", 
			"H1", "H2", "H3", "H4",
			"DIV", "CENTER", "P",
			"TABLE", "TH", "TR", "TD",
			"OL", "UL", "LI", 

			"FORM", "INPUT",
		]:
			self.set_elem_block( tag )

#		for tag in [ 
#			"H1", "H2", "H3", "H4",
#		]:
#			self.set_gap_before( tag )

		for tag in [ 
			"TITLE",	# spaces/NL appear as splot
			"SGML_ERROR",	# special - already set?
		]:
			self.set_elem_phrase( tag )
		#
		#
		for tag in [ 
			"B", "I", "U", 
			"A", 
			"CODE",
			"FONT",
		]:
			self.set_elem_inline( tag )
		#
		#
		for tag in [ 
			"IMG",
		]:
			self.set_elem_inline( tag )
		#
		#
		for tag in [ 
			"STYLE",
			"SCRIPT",
			"PRE"
		]:
			self.set_elem_is_pre( tag )
		#
		#
		for tag in [ 
			"LINK", "META",
			"BR", "HR"
		]:
			self.set_elem_empty( tag )

	def setup_WML( self ):
		"""
			copied from setup_HTML 
			added DOCTYPE here and xml header line
		"""
		for tag in [ 
		#	"HTML", "HEAD",
		#	"BODY", 
			"WML", "CARD",

			"H1", "H2", "H3", "H4",
			"DIV", "CENTER", "P",
			"TABLE", "TH", "TR", "TD",
			"OL", "UL", "LI", 

			"FORM", "INPUT",
		]:
			self.set_elem_block( tag )

#		for tag in [ 
#			"H1", "H2", "H3", "H4",
#		]:
#			self.set_gap_before( tag )

		for tag in [ 
			"TITLE",	# spaces/NL appear as splot
			"SGML_ERROR",	# special - already set?
		]:
			self.set_elem_phrase( tag )
		#
		#
		for tag in [ 
			"B", "I", "U", 
			"A", 
			"HREF", 
			"CODE",
			"FONT",
			"SMALL",
		]:
			self.set_elem_inline( tag )
		#
		#
		for tag in [ 
			"IMG",
		]:
			self.set_elem_inline( tag )
		#
		#
		for tag in [ 
			"STYLE",
			"SCRIPT",
			"PRE"
		]:
			self.set_elem_is_pre( tag )
		#
		#
		for tag in [ 
			"LINK", "META",
			"BR", "HR"
		]:
			self.set_elem_empty( tag )
