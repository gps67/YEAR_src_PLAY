# python component
# DTD_TDOC
# things specific to TDOC documents

import spin_py
import dtd_info 

# standard python

import traceback
import types
import sys

def setup_DTD_TDOC( eh ):
	print "Initialising DTD_TDOC"
	eh.dtd_info.set_elem_is_pre( "VERB" )
	eh.dtd_info.set_elem_is_pre( "PRE" )
	eh.dtd_info.set_elem_inline( "B" )
	eh.dtd_info.set_elem_inline( "TT" )
	eh.dtd_info.set_elem_inline( "I" )
	eh.dtd_info.set_elem_inline( "U" )

# RUBBISH beyond here

class dtd_tdoc:
	def __init__(self, elem_dict={}):
		pass
		dG
