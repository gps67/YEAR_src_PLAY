#!/usr/bin/env python2.7

# p2.py -- normalise an SGML file

# why wasnt PATH finding the files ?

import sys
from sys import stdout, stderr, argv, path
p0 = path[0]
path.insert(1, p0+"/../lib_base/" ) # used


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

EH = SP_EH( "spin2" )

# run SP over it
# the result is placed by DocumentEnd
# into EH.tree

files = [file]
t2 = time()
ret = EH.Run( files )	## the MAIN part of the program ##
t3 = time()
print "ret val ", ret
print "--"

# SP has now disappeared, and the document is loaded as EH.tree

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

EH.tree = None
EH.top = None
EH = None

# all done calculate the times

def fmt( str, t1, t2 ):
	dt = t2 - t1
	s = " %s - %4.2f \n" % (str, dt)
	stderr.write( s )

print "# How long did it take?"
fmt( "setup", t1, t2 ) # usually 0.00
fmt( "load", t2, t3 )
fmt( "dump", t3, t4 )

