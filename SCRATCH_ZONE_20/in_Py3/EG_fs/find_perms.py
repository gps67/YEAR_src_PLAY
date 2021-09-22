#! /usr/bin/env python

import os
from stat import *

# TEST # os.chdir("ROOT")
# TEST # os.chdir("..")

def walk( dir ):

	L1 = os.listdir(dir)
	L1.sort()

	L_dirs = []
	L_files = []
	L_symbs = []

	for f in L1:
		fullname = os.path.join( dir, f )
		# print fullname
		# lstat does not follow links
		s = os.lstat(fullname)
		m = s.st_mode
		if S_ISLNK(m):
			dest = os.readlink( fullname )
			print( "LINK", fullname, "->", dest )
		elif S_ISDIR(m):
			print( "DIR ", fullname )
			walk( fullname )
		elif S_ISBLK(m):
			print( "BLK ", fullname )
		elif S_ISCHR(m):
			print( "CHR ", fullname )
		elif S_ISFIFO(m):
			print( "FIFO", fullname )
		elif S_ISSOCK(m):
			print( "SOCK", fullname )
		else:
			# not totally clear is a plain FILE #
			print( "FILE", fullname )
		# print dir, f

# walk("../..")
walk("..")
# walk(".")

#for root, dirs, files in os.walk("."):  
#	for filename in files:
#		print(dir, filename)
