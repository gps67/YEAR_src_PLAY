#!/usr/bin/env python
# called by httpd or by shell

# INSTALLING require edit PATHS
#	scripts will need to be on path
#	test_number/dtd/base/.
#
#	/etc/sgml/ or some dir must be mentioned in base/spin_eh.py
#
#	The catalog1 file must also work!
#
#	The sgml file being used is also hard-coded

def handler(req):
	req.content_type="text/html"
	req.send_http_header()
	# req.write("Hello World!")
	stderr=req
	return handler2( req, req )
	# forgetting the above return is interesting. Try it!

def handler2( fd_req, req ):

	# where python looks for progs and modules, including spin_py.so
	import sys
	sys.path.append( '../../SP_lib' )
	sys.path.append( '/tools/1999/_src/lib2/SPIN-0.2/SP_lib' )

	# where ELF looks for libsp16.so (referenced by spin_py.so)
	# LURK: this varies with os, so install into site
	# above just failed where it used to work!
	# seems sys.path needed for python and EXTERNAL LD_LIBRARY_PATH for ld.so
	# so put link into /usr/lib or dir into /etc/ld.do.conf

	from os import environ
	# environ['LD_LIBRARY_PATH']='../../SP_lib:/usr/src/sp/SP_lib:/tools/1999/_src/lib2/SPIN-0.2/SP_lib:'

	import spin_py 

	# test scripts are in subdirs
	# find them too

	from sys import path
	# shell sets this to the symb-link-resolved dir where script is
	# ie if script is a symb link to script ...
	# apache sets this to the dir where the directive first applied
	p0 = path[0]
	#
	path.insert(1, p0+"/base/" )
	# this one works from the command line
	path.insert(1, p0+"/../../test_number/dtd/base/" )
	print p0

	# put the loader side of the jigsaw pieces together ...
	import spin_eh # new_top does not create an EH though
	from spin_eh_num import SP_EH_NUM
	from sgml_tree_print import SGML_Tree_Printer 
	# dtd_info also required

	from time import time
	t1 = time()

	# SP_EH is a generic base class of SPIN_LDOC
	# SP_EH_NUM adds HTML numbering of H1
	# it is the SPIN event handler
	# it activates spin_py, and gets called back
	# it contains the /etc/sgml/catalog parameters
	# so if you don't have /usr/src/sp/pubtext/html.soc - edit that back in

	# EH = SPIN_LDOC()
	EH = SP_EH_NUM()

	# pick a file
	# should maybe look at apaches request ?

	file="/hda6/tools/1999/serv2/htdocs/py/test_doc.sg"
	ofile="/tmp/test1.sgm"
	#if len(argv) == 2:
	#	file = argv[1] 
	#else:
	#	if len(argv) == 3:
	#		file = argv[1] 
	#		ofile = argv[2] 

	# create the kit
	# set any options
	# including search path

	top = spin_eh.new_top_with_default_ParserEventGeneratorKit( "spin2" );
	top.EH = EH
	EH.top = top # circular reference - OK-ish
	# EH.elem_info = top.elem_info
	top.elem_info = EH.elem_info

	# run SP over it
	# the result is placed by DocumentEnd
	# into EH.tree

	file = [file]
	t2 = time()
	ret = top.run( file )
	t3 = time()

	# dump/print the EH.tree to fd_req via a write_buffer
	if 1:	
		import sys
		# fd = open( ofile, "w");
		fd = spin_py.SPIN_write_buffer( fd_req, 10*1024, 300*1024 )
		fd.write( EH.dtd_line+"\n" )
		# need the DTD to write at the top of the file 
		pr = SGML_Tree_Printer( fd, 0 )
		pr.print_tree( EH.tree )
		# there is no auto_flush, though print_tree could do one?
		fd.flush()
	t4 = time()

	# not that the tree is loaded its up to you to process it
	# possibly someting like:
	#
	# ldoc = LINUXDOC( EH.tree )
	# ...
	# ldoc.gen_pages( dir, base, ext, helper )

	# all done calculate the times

	def fmt( str, t1, t2 ):
		dt = t2 - t1
		s = " %s - %4.2f \n" % (str, dt)
		# req.write( s )

	# print "# How long did it take?"
	fmt( "setup", t1, t2 ) # usually 0.00
	fmt( "load", t2, t3 )
	fmt( "dump", t3, t4 )

	fd.flush2()
	try:
		return apache.OK
	except:
		pass

class DUMMY:
	def __init__( self ):
		pass

if __name__ == '__main__':
	import sys
	dummy = DUMMY()
	fd = sys.stderr
	# fd = open( "/dev/null", 'w' )
	# fd = open( "test_out.html", 'w' )
	handler2(fd,dummy)
else:
	from mod_python import apache
 
