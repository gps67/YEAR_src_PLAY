#/tools/2001/py/bin/python
#!/usr/bin/env python

"""
	zflt data in blobs is a raw array of floats zlib compressed
	That is incompattible with gzip because there is no gzip header
	Hopefully you wont need this in your debugging tools.
	(you still have to fprintf %f convert the output file to ASCII)
"""

import zlib 
def zlib_unz( filename, zfilename ):
	fd = open( filename )
	zdata = fd.read()
	fd.close()
	data = zlib.decompress( zdata )
	fd = open( zfilename, "w" )
	fd.write( data )
	fd.close()

loid = 29875
loid = 237488
filename = "/tmp/segs/LO_%d" % loid
filename = "/tmp/segs/LO_%d" % loid
zfilename = filename+".unz"
zlib_unz( filename, zfilename )
	
	
	
