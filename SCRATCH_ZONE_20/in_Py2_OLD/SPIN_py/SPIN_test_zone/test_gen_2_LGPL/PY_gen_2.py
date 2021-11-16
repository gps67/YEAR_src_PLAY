#!/usr/bin/env python

# PY_gen_2.py
#
# CALLED BY:
#	R
#	this is the top python script of this directory
#	it is called by R which also sets PATH
#	also normalises, and uploads the generated file
#
# OUTPUTS:
# 	http://www.information-cascade.co.uk/LGPL/index1.html
#	a list of Open Source Software to download
#	on uklinux - an absent index.html makes the dir browsable!
#
# INPUTS:
#
#	PY_gen_2.py	- text is in this script
#	app_list_1.py	- text for items as Python script
#	app_list_2.py	- text for libs_apps sub-items as Python script
#
#	app_item.py
#	gen_base.py
#	gen_here.py
#	PI_PHP_holder.py
#	re_cache.py
#	vfs.py

# Keep track of time
from time import time
t1 = time()

#
# set PATH so that scripts are found in ../lib_base/
# why wasnt PATH finding the files ?
#
import sys
from sys import stdout, stderr, argv, path
p0 = path[0]
path.insert(1, p0+"/../lib_base/" ) # used

# GEN holds the generated HTML file
#
# put some of the jigsaw pieces together ...
# gen_here as a 10 line local wrapper on top of gen_base 
# gen_here is a scripted class to hold a generated HTML file
# gen_here uses SP_EH but doesnt have any input document (DTD)
# (TODO: move DTD and allow XML)
# it holds HTML, HEAD + BODY and prints to a named file
#
from gen_here import gen_here

# PHP is unused
# from PI_PHP_holder import PI_PHP_holder


# pick input and output files from argv
# NB there is no ifile! This is a common code snippet
# ofile defaults to index1.html, changing that requires specifying ifile!!!

ifile="test1.sg"
ofile="index1.html"
if len(argv) == 2:
	ifile = argv[1] 
else:
	if len(argv) == 3:
		ifile = argv[1] 
		ofile = argv[2] 

# create the output file
# set its CSS DTD title etc
# the file will be opened LATER (for now its a name)

title = "Information Cascade - Downloadables"

GEN = gen_here( ofile, title )
el_BODY = GEN.el_BODY

#	app_list:
#	app_list_sorted_by_date
#	from app_item import app_item, libs_apps_item, app_list, app_dict, app_list_sorted_by_date

#	from app_item import app_list, app_list_sorted_by_date

# each OSS app is an item with a file/name/desc/...
# each app_item has its own format instructions (in the code)
# they could be loaded from data, but are actually loaded from script
# it would be better to allow sub-trees per item, but currently plain strings
# it would be better to interpret some simple XML in the scripts strings
# it would be better to have polymorphics,
#	DESC is plain_string
#	DESC is ML_subtree with fake TOPNODE <app_item_desc>
#	DESC is ML_subtree with fake TOPNODE <P>
#	DESC is ML_subtree without TOPNODE -- ie its a list for ... SPECIFY
#	DESC is python_string_8859_that_needs_ml_interpretation <B>allowed</B>
# for now each item is a simple thing

# TODO: move this to data_store_app_list
# app_item as a module also brings a GLOBAL list of app_items, and funcs
#	app_item.app_list
#	app_item.app_dict
# so you can sort the list by date or whatever
# the data is loaded into app_item. as a side effect of the following imports
#
import app_item # MODULE with a similarly named class - and 
import app_list_1 # side effect - adds data to app_list - most apps
import app_list_2 # side effect - adds data to app_list - libs_apps

# setup done, now time to generate the output
t2 = time()

# the HTML file tree already exists
# this could be a component of a larger page
# but here it IS the entire page (except the headers)
if 1:
	# Introduce the page with a Title Heading and a few paragrpahs
	# Later there will be summary table, (line per item)
	# then each section will have a half-page per item
	#
	# many users will be confused - what is OSS ?
	#
	H2 = el_BODY.add_elem( "H2" )
	A = H2.add_elem( "A" )
	A.add_attr( "href", "http://www.Information-Cascade.co.uk" )
	A.add_text( "www.Information-Cascade.co.uk" )
	H2.add_elem( "BR" )
	H2.add_text( "LGPL Software available for download" )

	P = el_BODY.add_elem( "P" )
	P.add_text( """
		This is an informal collection of software written here.
		It ranges from the best to the oldest,
		from libraries to snippets.
		This web page was generated using SPIN_py, whilst the
		desktop mixer adjusts the music/tv volume.
	""" )

	P = el_BODY.add_elem( "P" )
	P.add_text( """
		libs_apps is a large collection of code (libraries and apps).
		This HTML table shows it as several sub-sections, as well as
		the whole. You can tell by the filename.
	""" )

	if 0:
		#
		# interesting aside - SP normalises this differently!
		#
		# the caller script (R) then calls normalise
		# which moves this UL out of the containing P
		# which renders completely differently!
		P = el_BODY.add_elem( "P" )
		P.add_text( """
			All my code is (Library) L-GPL, which allows commercial use
			(In the unlikely event that it is actually useful!).
			I encourage all to consider L-GPL over GPL for three reasons:
		""" )
		UL = P.add_elem("UL")
		LI = UL.add_elem("LI")
		LI.add_text( """
			It is right - our code is ours, yours is yours 
		""" )
		LI = UL.add_elem("LI")
		LI.add_text( """
			Using code helps code. Not using at work, doesnt help.
			Learning, testing, supporting, extending, ...
		""" )
		LI = UL.add_elem("LI")
		LI.add_text( """
			What is an stand-alone application today, might be a module tomorrow.
		""" )
		P.add_text( """
			Simply draw a line around the code, (be fair!), and you have
			an inside and outside.
		""" )
		

	P = el_BODY.add_elem( "P" )
	P.add_text( """
		These packages work best on Linux, or UNIX boxes.
		WIN32 developers will also find them usable (maybe even useful),
		but they aren't .zip files. They are compressed
		.tgz tarballs (.tar.gz). Try http://www.7-zip.org WinZip or other.
	""" )

	P = el_BODY.add_elem( "P" )
	P.add_text( """
		ALL Linux/UNIX users, experienced and novice, will
		get good service from """ )
	# B = P.add_elem( "B" )
	# B.add_text( "'MidnightCommander' " )
	# P.add_text( " available at " )
	A = P.add_elem( "A" )
	URL = "http://www.midnight-commander.org"
	A.add_text( URL )
	A.add_attr( "href", URL )
	# LURKING BUG # above space migrates out, but following space gets lost
	# maybe add_text should not just trim_blanks, but reduce_to_single_space
	P.add_text( ": " )
	P.add_text( """
		a two panel file browser, and all-round swiss-army-knife.
		It helps you view MIME files inside .tgz files.
		I cannot recommend this text-based utility enough, though you
		do have to set a few options: pause_after_run_always,
		lynx_like_motion_yes, confirm_delete_yes, confirm_exit_no,
		internal_editor_maybe_not, save ...
	""" )

	#
	# this demos how code can update different parts of the text
	# you can go back to any part of the tree, and add items to it
	# script variables are used to hold parent node pointers
	# add_item() appends a child to that parents internal list of items
	# you dont specify "end_tag", just "element", attributes and contents
	# elem_info lookup is by name into the same table as the parent
	# Theres no DTD checking (currently), so test with regen/norm.
	#

	el_BODY.add_elem( "HR" )
	TABLE = el_BODY.add_elem( "TABLE" )
	el_BODY.add_elem( "HR" )

	# TABLE header
	TR = TABLE.add_elem( "TR" )
	TR.add_attr( "class", "HDR" )
	TH = TR.add_elem( "TH" )
	TH.add_text( "Name" )
	TH = TR.add_elem( "TH" )
	TH.add_text( "Date" )
	TH = TR.add_elem( "TH" )
	TH.add_text( "Size" )
	TH = TR.add_elem( "TH" )
	TH.add_text( "Lang" )
	TH = TR.add_elem( "TH" )
	TH.add_text( "Desc" )

	# TABLE rows and BODY segments
	# each software item is a row in the table and a linked screen
	# this demo keeps things within one page,
	# others might generate a group of files (a different gen_here).
	for item in app_item.app_list_sorted_by_date():
		print item.str_date, item.name
		item.fmt2_table_row( TABLE )
		item.fmt3_item_screen( el_BODY )

# fmt1 is 
if 0:
	for item in app_item.app_list:
		print item.name
		item.fmt1( el_BODY )

# item as python instance
# SGML_print_tree will ask object to print itself
if 0:
	for item in app_item.app_list:
		print item.name
		el_BODY.add( item )

t3 = time()

GEN.print_tree_to_file()

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

## START_PYTHON_and_load_SPIN_py = not recorded
## setup - 0.04 ## 0.30 first run then 0.04
## load - 0.09 
## dump - 0.01 

