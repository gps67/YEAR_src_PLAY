#!/usr/bin/env python
# PY_gen_3.py -- Generate an WML file from code (no input file)

from time import time
t1 = time()

# use common scripts in ../lib_base/
# why wasnt PATH finding the files ?
import sys
from sys import stdout, stderr, argv, path
p0 = path[0]
path.insert(1, p0+"/../lib_base/" ) # used

# put the loader side of the jigsaw pieces together ...
# see also the /etc/sgml paths in base/spin_eh.py
#
from spin_eh import spin_py, SP_EH
#
# spin_py.GDB_INVOKE();

# create a new Event Handler
# This app doesnt use it to handle events though!
# It is setup as a placebo that doesnt receive HTML events, but functions
# TODO: generate OUTPUT HTML from INPUT SGML - wont use EH as destination!
# In particular both input and output need a "dtd", "elem_info_pool", etc

EH = SP_EH( "SPIN_Python_gen" )
# EH.dtd_line = "" # done by setup_HTML for previously blank dtd_line
EH.setup_WML()

# The TOP "WML" element has to be created manually
# WHY ??
# surely that can be donw by EH.setup_TOP
# maybe with later overrides for lang ?

ei_WML = EH.dtd_info.get_elem_info("WML")
el_WML = spin_py.SPIN_elem_obj( ei_WML, [], [] )
EH.tree = el_WML


t2 = time()

# pick a file

WWW_127 = 0 # not local
if len(argv) > 1:
 if argv[1] == "WWW_127":
	WWW_127 = 1 # local
if WWW_127:
	ofile="HTDOCS/index.html"
else:
	ofile="w.wml"

el_CARD = el_WML.add_elem( "CARD" )
el_CARD.add_attr( "id", "card1" ) 
el_CARD.add_attr( "title",    "DEMO TITLE" )

P=el_CARD.add_elem( "P" )
S=P.add_elem("SMALL")

def A_URL_TEXT( P, url, text ):
	if not text:
		text = url
	A=P.add_elem("A")
	A.add_attr( "HREF", url )
	A.add_text( name )
	return A


list = [
	"frio_128_128.jpg",
	"frio_128_160.jpg",
	"uk_128_128.jpg",
	"uk_128_160.jpg",
]
for name in list:
	url = "wap/" + name 
	A_URL_TEXT( S, url, name )
	S.add_elem("BR")
S.add_text( "OK?" )

"""
THIS CAUSED THE FIRST PARAGRAPH TO BE REPEATED
BY THE BROWSER, WHICH IS MUCH THAT SAME VERSION IN MY PHONES
P=el_CARD.add_elem( "P" )
S=P.add_elem("SMALL")
"""

A_URL_TEXT( S, "http://m.google.co.uk", "m.google" )
S.add_elem("BR")
A_URL_TEXT( S,  "http://maps.google.co.uk" , "maps.google" )
S.add_elem("BR")
A_URL_TEXT( S,  "http://www.facebook.co.uk", "facebook" )
S.add_elem("BR")


t3 = time()

# dump/print the EH.tree to ofile
if 1:	
	import sys
	# EG # fd = sys.stdout
	fd = open( ofile, "w");
	# DEAD # fd = spin_py.SPIN_write_buffer( fd, 10240, 102400 )
	lo = 10240
	hi = 102400
	pr = spin_py.SPIN_tree_print( fd, lo, hi )

	# need the DTD to write at the top of the file 
##	pr.write( '<?xml version="1.0"?>' + "\n" )
	pr.write( EH.xml_line + "\n" )
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

