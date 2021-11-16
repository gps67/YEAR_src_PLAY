#!/usr/bin/env python

# This is main() script
# find subscripts and get start time

import sys
from sys import stdout, stderr, argv, path
p0 = path[0]
path.insert(1, p0+"/../lib_base/" )
path.insert(1, p0+"/../lib_plus/" )

from time import time
t1 = time()

# load the spin_py module
# NB the subscript knows where spin_py.so is (when not installed)

from spin_eh_num import spin_py, SP_EH_NUM
# from sgml_tree_print import SGML_Tree_Printer 

# TOP holds everything together
# create the kit
# set any options
# including search path

name = "spin2"

# pick a file

file="test1.sg"
ofile="test1.sgm"

if len(argv) == 2:
	file = argv[1] 
else:
	if len(argv) == 3:
		file = argv[1] 
		ofile = argv[2] 

# SP_EH is a generic base class
# SP_EH_NUM adds HTML numbering of H1
# it is a handler of SPIN events
# it activates spin_py, and gets called back
# spin_eh.py contains the /etc/sgml/catalog parameters

EH = SP_EH_NUM( name )

# top.EH = EH
# EH.top = top # circular reference - OK-ish
# top.elem_info = EH.elem_info

# run SP over it
# the result is placed by DocumentEnd
# into EH.tree

files = [file]
t2 = time()
ret = EH.Run( files )
t3 = time()

# print "--HERE--"
# print EH
# print EH.tree

# dump/print the EH.tree to stdout
verbose=0
if 1:	
	if verbose:
		print "top.run( file ) returned: ", ret
		print "#"
		print "# The input tree"
		print "#"
		print "# --------------------------------------"
		print "# A rather un-sophisticated tree printer"
		print "# But it does have a tree to print ..."
		print "# View this file with MC and no-wrap-long-lines!"
		print "# --------------------------------------"
	import sys
	fd = sys.stdout
	# fd = open( "output1.ht", "w");
	fd = open( ofile, "w");
	# ratio 0.16:0.12 for lo,hi changes
	if 1:
		lo = 10*1024
		hi = 300*1024
	else:
		lo = 1
		hi = 2
	# add the new buffer ...
	fd = spin_py.SPIN_write_buffer( fd, lo, hi )

	# need the DTD to write at the top of the file 
	fd.write( EH.dtd_line+"\n" )

	# pr = SGML_Tree_Printer( fd, 0 )
	lo = 10240
	hi = 102400
	pr = spin_py.SPIN_tree_print( fd, lo, hi )
	# print "--HERE--"
	# print EH
	# print EH.tree
	pr.print_tree( EH.tree )
	# pr.flush2()
	# either is ... vital ! 
	# should be auto on destructor # fd.flush2()
	if verbose:
		print "#"
		print "# --------------------------------------"
		print "# That was the Input Tree "
		print "# --------------------------------------"
		print "#"
t4 = time()

# all done calculate the times

def fmt( str, t1, t2 ):
	dt = t2 - t1
	s = " %s - %4.2f \n" % (str, dt)
	stderr.write( s )

print "# How long did it take?"
stdout.flush()
fmt( "setup", t1, t2 ) # usually 0.00
fmt( "load", t2, t3 )
fmt( "dump", t3, t4 )

