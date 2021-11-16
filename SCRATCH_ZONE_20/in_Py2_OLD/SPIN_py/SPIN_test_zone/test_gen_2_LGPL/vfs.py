
# to handle VFS as an extra parameter, make 'pathname' an object (this)

import commands
import string
import os

class vfs_name:

	def __init__( self, dir, name=None ):
		self.vfs = None # UNIX
		self.dir = dir
		self.name = name

	def ls1( self ):
		cmd = "cd " + self.dir + "&& ls -1"
		( ret, out ) = commands.getstatusoutput( cmd )
		if ret:
			print out
			return []
		else:
			# print "out:", out
			lines = string.split( out, "\n" )
			# print "lines:", lines
			return lines

	def list( self ):
		return os.listdir( self.dir )

def os_listdir( dir ):
	vfs = vfs_name( dir )
	return vfs.list()

# saves a second by not runnin ls -1 20 times (each item in page)
os_listdir_cache = {}

def os_listdir_cached( dir ):
	try:
		l = os_listdir_cache[ dir ]
	except:
		l = os_listdir( dir )
		os_listdir_cache[dir] = l
	return l

