#!/usr/bin/env python
# PY_gen_3.py -- Generate an HTML file from code (no input file)

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
EH.setup_HTML()

# The TOP "HTML" element has to be created manually
#
# EH.dtd_info is a pool of ei-elem-info's
# build a (unique) el-element from that (reused) name
# set that as the output TOP (EH does everything, and a few extra things)
#
ei_HTML = EH.dtd_info.get_elem_info("HTML")
el_HTML = spin_py.SPIN_elem_obj( ei_HTML, [], [] )
EH.tree = el_HTML


t2 = time()

# pick a file

ofile="index.htm"

# adding child elements:
#
# you can create sub items as above, but there is a (new) shortcut
# instead of DTD, it uses the C++ links: ELEM -> elem_info -> elem_info_pool
# currently it is dumb, but it could CHECK that child is valid
# that might also speed up lookups
#
# use local vars to hold the returned value (also grafted in parent)
# and maybe think out consistent names for ei_XXX el_XXX XXX
#
# adding text and attributes is similar

el_HEAD = el_HTML.add_elem( "HEAD" )

el_TITLE = el_HEAD.add_elem( "TITLE" )
el_TITLE.add_text( "gps.uklinux.net and Information Cascade Ltd" )

if 0:
	el_LINK = el_HEAD.add_elem( "LINK" )
	el_LINK.add_attr( "rel", "stylesheet" ) 
	el_LINK.add_attr( "href", "./style1.css" ) 
	el_LINK.add_attr( "type", "text/css" ) 

el_META = el_HEAD.add_elem( "META" )
el_META.add_attr( "name", "GENERATOR" ) 
el_META.add_attr( "CONTENT", "spin_py" ) 

el_BODY = el_HTML.add_elem( "BODY" )
el_BODY.add_attr( "background", "#DDDD77" ) 
el_BODY.add_attr( "bgcolor",    "#FFFFFF" )
el_BODY.add_attr( "text",       "#000000" )
el_BODY.add_attr( "link",       "#0000FF" )
el_BODY.add_attr( "alink",      "#0000FF" )
el_BODY.add_attr( "vlink",      "#FF0000" )

el_BODY.add_elem( "BR" )
el_BODY.add_elem( "BR" )
CENTER=el_BODY.add_elem( "CENTER" )

T1=CENTER.add_elem( "TABLE" )
T1.add_attr( "border", "1" )

# ROW heading
if 1:
	TR=T1.add_elem( "TR" )
	TD=TR.add_elem( "TD" )
	TD.add_attr( "colspan", "2" )
	TD.add_attr( "align", "center" )
	
	TD.add_elem( "BR" )
	TD.add_sdata( "nbsp" )	# pad the heading to make the frame bigger
	TD.add_sdata( "nbsp" )
	TD.add_sdata( "nbsp" )
	TD.add_sdata( "nbsp" )
	A=TD.add_elem( "A" )
	TD.add_sdata( "nbsp" )	# pad the right too
	TD.add_sdata( "nbsp" )
	TD.add_sdata( "nbsp" )
	TD.add_sdata( "nbsp" )
	TD.add_elem( "BR" )
	# expand on the A element
	A.add_attr( "href", "http://www.gps.uklinux.net/" )
	F=A.add_elem( "FONT" )
	F.add_attr( "SIZE", "+4" )
	F.add_text( "Graham" )
	F.add_sdata( "nbsp" )
	F.add_text( "Swallow" )
	F.add_sdata( "nbsp" )
	F.add_text( "and" )
	F.add_sdata( "nbsp" )
	F.add_text( "Information" )
	F.add_sdata( "nbsp" )
	F.add_text( "Cascade" )
	F.add_sdata( "nbsp" )	# text isnt words - gaps are required!
	F.add_text( "Ltd" )

# ROW Information Casacde
if 1:
	TR=T1.add_elem( "TR" )
	TD=TR.add_elem( "TD" )
	TD.add_attr( "align", "center" )
	TD.add_elem( "BR" )
	A=TD.add_elem( "A" )
	A.add_attr( "href", "cascade" )
	# A.add_attr( "href", "http://www.information-cascade.co.uk" )
	A.add_text( "Cascade" )
	
	TD=TR.add_elem( "TD" )
	TD.add_attr( "align", "center" )
	H2=TD.add_elem( "H2" )
	H2.add_text( "Information Cascade Ltd")

# ROW OPEN SOURCE 
if 0:
	TR=T1.add_elem( "TR" )
	TD=TR.add_elem( "TD" )
	TD.add_attr( "align", "center" )
	TD.add_elem( "BR" )
	A=TD.add_elem( "A" )
	A.add_attr( "href", "baks/index1.html" )
	IMG=A.add_elem( "IMG" )
	IMG.add_attr( "src", "http://www.opensource.org/trademarks/opensource/web/opensource-110x95.png" )
	IMG.add_attr( "border", "0" )
	IMG.add_attr( "width", "110" )
	IMG.add_attr( "height", "95" )
	IMG.add_attr( "alt", "Snippets and libraries" );
	
	TD=TR.add_elem( "TD" )
	TD.add_attr( "align", "center" )
	H2=TD.add_elem( "H2" )
	H2.add_text( "LGPL Project Software" )
	H2.add_elem( "BR" )
	H2.add_text( "available for download" )

# ROW CV
if 1:
	TR=T1.add_elem( "TR" )
	TD=TR.add_elem( "TD" )
	TD.add_attr( "align", "center" )
	TD.add_elem( "BR" )
	H2=TD.add_elem( "H2" )
	TD.add_elem( "BR" )
	A=H2.add_elem( "A" )
	A.add_attr( "href", "http://www.Information-Cascade.co.uk/CV/CV_resume.html" )
	A.add_text( "CV" )
	
	TD=TR.add_elem( "TD" )
	# TD.add_elem( "BR" )
	TD.add_attr( "align", "center" )
	H2=TD.add_elem( "H2" )
	H2.add_text( "CV_resume - Graham" )

# ROW Blood
if 1:
	TR=T1.add_elem( "TR" )
	TD=TR.add_elem( "TD" )
	TD.add_attr( "align", "center" )
	TD.add_elem( "BR" )
	A=TD.add_elem( "A" )
	A.add_attr( "href", "http://www.blood.co.uk" )
#	A.add_attr( "href", "http://www.blood.co.uk/SessionSearcher/search.aspx" )
	IMG=A.add_elem( "IMG" )
	IMG.add_attr( "src", "nbs_heart.png" )
	IMG.add_attr( "border", "0" )
#	google.co.uk # shows ALT text - first words of search from that page!
#	IMG.add_attr( "alt", "You know you dont want to" );
	
	TD=TR.add_elem( "TD" )
	TD.add_attr( "align", "center" )
	H2=TD.add_elem( "H2" )
	H2.add_text( "When" )
	H2.add_elem( "BR" )
	H2.add_sdata( "nbsp" )
	H2.add_text( "and" )
	H2.add_elem( "BR" )
	H2.add_text( "Where" )

# ROW Freshmeat
if 1:
	TR=T1.add_elem( "TR" )
	TD=TR.add_elem( "TD" )
	TD.add_attr( "align", "center" )
	# TD.add_elem( "BR" )
	A=TD.add_elem( "A" )
	A.add_attr( "href", "http://freshmeat.net" )
	IMG=A.add_elem( "IMG" )
	IMG.add_attr( "src", "http://images.freshmeat.net/img/logo.gif" )
	IMG.add_attr( "border", "0" )
	IMG.add_attr( "alt", "Where I go to find stuff" );
	
	TD=TR.add_elem( "TD" )
	TD.add_attr( "align", "center" )
	H2=TD.add_elem( "H2" )
	# H2.add_elem( "BR" )
	H2.add_text( "Freshmeat.net" )
	H2.add_elem( "BR" )
	H2.add_text( "Open Source" )
	H2.add_elem( "BR" )
	H2.add_text( "Public Library" )

# ROW email address
if 1:
	TR=T1.add_elem( "TR" )
	TD=TR.add_elem( "TD" )
	TD.add_attr( "colspan", "2" )
	TD.add_attr( "align", "center" )
	
	TD.add_text( "gps @ gps.uklinux.not" )

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

