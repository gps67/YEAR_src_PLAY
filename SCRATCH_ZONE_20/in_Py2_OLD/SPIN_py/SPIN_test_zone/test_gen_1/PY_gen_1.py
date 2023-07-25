#!/usr/bin/env python

# PY_gen_1.py -- Generate an HTML file from code (no input file)


# why wasnt PATH finding the files ?

import sys
from sys import stdout, stderr, argv, path
p0 = path[0]
# OK # path.insert(1, p0+"/../lib_base/" ) # used


# put the loader side of the jigsaw pieces together ...
#
from spin_eh import spin_py, SP_EH
# see also the /etc/sgml paths in base/spin_eh.py

from time import time
t1 = time()

# pick a file

file="test1.sg"
ofile="test1.sgm"
if len(argv) == 2:
	file = argv[1] 
else:
	if len(argv) == 3:
		file = argv[1] 
		ofile = argv[2] 


# SP_EH is a generic base class of SPIN_LDOC
# it is the SPIN event handler
# it activates spin_py, and gets called back
# it contains the /etc/sgml/catalog parameters
# so if you don't have /usr/src/sp/pubtext/html.soc - edit that back in

EH = SP_EH( "gen1" )

# run SP over it
# the result is placed by DocumentEnd
# into EH.tree
if 0:
	files = [file]
	t2 = time()
	ret = EH.Run( files )	## the MAIN part of the program ##
	t3 = time()
	print "ret val ", ret
	print "--"
# SP has now disappeared, and the document is loaded as EH.tree

# - or instead - build the tree manually

NotaBene = """
	To support scripting, I'm adding functions to elem_obj.
	In particular, an ELEM_OBJ should know (from the DTD which we
	dont really have), which children are allowed. OK Thats an ELEM_INFO
	function, but working from the OBJ, where the created OBJ will be
	appended.

	The TOP element "<HTML>" has to be manually created, then its
	children auto come from the same dtd_pool

	NB an unlimited number of variations are not supported.
	eg append new instance is inst1.data.append( inst2 )
"""
t2 = time()

# first set the DTD related settings
EH.dtd_line = ""
EH.setup_HTML()

# The TOP element has to be created manually
# WAS # ei_HTML = EH.dtd_info.get_elem_info("HTML")
# WAS # HTML = spin_py.SPIN_elem_obj( ei_HTML, [], [] )
HTML = EH.dtd_info.new_elem_inst("HTML")
EH.tree = HTML

# It can then create children in quicker steps
# This uses an ELEM_OBJ to find the ELEM_INFO_POOL (dtd)
# then lookup "HEAD" to get its elem_info (auto add as ... default type)
# then create an ELEM_OBJ (with atts and body defaulting to empty lists)
# then APPEND that to the first element
# and return to paste children or set attributes
#
HEAD = HTML.add_elem( "HEAD" )

TITLE = HEAD.add_elem( "TITLE" )
TITLE.add_text( "This is the documents title" )

LINK = HEAD.add_elem( "LINK" )
LINK.add_attr( "rel", "stylesheet" ) 
LINK.add_attr( "href", "./style1.css" ) 
LINK.add_attr( "type", "text/css" ) 
 
META = HEAD.add_elem( "META" )
META.add_attr( "name", "GENERATOR" ) 
META.add_attr( "CONTENT", "spin_py" ) 
 
BODY = HTML.add_elem( "BODY" )
BODY.add_attr( "background", "#DDDD77" ) 

# what should happen to unknown elements
# added via the recent elem_inst.add_elem()
# TODO
if 0:
	NK = BODY.add_elem( "NotKnown" ) 
	NK.add_text("This element as not pre-initialised, by the KNOWN dtd_info")

t3 = time()

# dump/print the EH.tree to stdout
if 1:	
	import sys
	# EG # fd = sys.stdout
	fd = open( ofile, "w");
	# DEAD # fd = spin_py.SPIN_write_buffer( fd, 10240, 102400 )
	lo = 10240
	hi = 102400
	pr = spin_py.SPIN_tree_print( fd, lo, hi )

	# need the DTD to write at the top of the file 
	pr.write( EH.dtd_line + "\n" )
	pr.print_tree( EH.tree )
	# no need to call flush2() manually
	# pr.flush2()
	# fd.flush2()
t4 = time()

# all done calculate the times

def fmt( str, t1, t2 ):
	dt = t2 - t1
	s = " %s - %4.2f \n" % (str, dt)
	stderr.write( s )

print "# How long did it take?"
fmt( "setup", t1, t2 ) # usually 0.00
fmt( "load", t2, t3 )
fmt( "dump", t3, t4 )

