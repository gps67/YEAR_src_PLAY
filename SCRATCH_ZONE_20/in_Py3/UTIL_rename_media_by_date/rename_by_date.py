#!/usr/bin/env python

# rename_by_date
# rename photo files to their timestamp from the camera
# this requires the camera is accurate
# this requires a single source and no 2 files have the same mtime second
# EG multi-shot takes 5 in rapid succession
# the timezone is taken into account - so if it is wrong ...

# ALL files in the current directory are at risk
# .JPG .AVI are the current file types

# if the camera date is older than 5 months


# do not touch files if they are older than (date_recent_ish)
# that includes things this year (maybe automate to make it time relative?)

import os
import time 

fmt2 = "%a, %d %b %Y %H:%M:%S +XXXX"
fmt1 = "%Y-%m-%d_%a_%H%M_%S"
fmt = fmt1

class match_filename_filter:
 def __init__( self ):
 	self.tail_dict = {}
 	tails = [
 		"jpg",	# J700
 		"JPG",
 		"jpeg",
 		"JPEG",
 		"avi",
 		"AVI",
 		"mp4",
 		"MP4",	# SAMSUNG Camera
 		"3gp",
 	]
 	for tail in tails:
 		tail_low = tail.lower()
 		self.tail_dict [ tail ] = tail_low
 		self.tail_dict [ tail_low ] = tail_low
 	# print( "#", self.tail_dict )

 def match_filename( self, name ):
 	l = name.rsplit(".",1)
 	ext = l[-1]
 	try:
 		seen = self.tail_dict[ ext ]
 	except:
 		seen = None
 	# print( "seen = ", seen, "ext = ", ext )
 	return seen

 def get_valid_ext( name ):
 	l = name.rsplit(".",1)
 	ext = l[-1:]
 	


def local_str_from_time_int( fmt, time_int ):
	# my camera has %s == 12, it is 2012, but its not python
	# year_secs = 365 * 24 * 60 * 60
	# time_int = time_int - year_secs
	""" YEAR-MM-DD Tue HH:MM:SS from time_int """
	# convert int to struct 
 #	struct_time = time.gmtime( time_int )
	struct_time = time.localtime( time_int )
	# print( string from struct )
	return time.strftime(fmt, struct_time )

# NOTE: these return structs not ints
# print( time.strftime(fmt2, time.gmtime()) )
# print( time.strftime(fmt2, time.localtime()) )


days_since = 1
days_since = 0.5
days_since = 99 # limit age of files
days_since = 199 # limit age of files
days_since = 365*2 # limit age of files
days_since = 365*40 # limit age of files
time_since = days_since * 24 * 60 * 60 # units must match time()
time_now = time.time()
time_ago = time_now - time_since

# try not to rename files that have just been rotated and time_changed
# if its newer than when we got home, drop it
#
days_recent = 0.01
days_recent = -2 # clocks wrong
secs_recent = days_recent * 24 * 60 * 60 # units must match time()
time_recent = time_now - secs_recent

print( local_str_from_time_int( fmt, time_now ) )
print( "# MINIMUM", local_str_from_time_int( fmt2, time_ago ) )


D1="."

filter = match_filename_filter()

L = os.listdir(D1)
L.sort()
for f in L:
	ext2 = filter.match_filename( f )
	if ext2 == None:
		print( "# SKIPPING", f )
		continue
	s = os.stat( f ) # FAIL if deleted since listdir (etc)
	t = s.st_mtime
	if t > time_recent:
		print( "#", f, local_str_from_time_int( fmt1, t ), "# TOO NEW" )
		continue
	vn_str = ""
	if t < time_ago:
		print( "#", f, local_str_from_time_int( fmt1, t ), "# TOO OLD" )
		continue
	vn_str = ""
	vn = 0
	looking = 1
	while looking:
		# move this forward - to end 
		vn += 1
		vn_str = "-{0}".format( vn )
		f2 = local_str_from_time_int( fmt1, t )
		filename2 = f2.lower() + vn_str + "." + ext2
		if filename2 == f:
			# rename file as itself = OK ish
			looking = 0
			break
		try:
			os.stat( filename2 )
			print( "# ALREADY EXISTS -", filename2 )
		except:
			looking = 0
			break
	if filename2 == f:
		print( "# SAME", filename2 )
	else:
		print( "mv -n", f, filename2 )
		os.rename( f, filename2 )

# STAT:
# 2012-07-28-0014.jpg
#  posix.stat_result(
#  st_mode=33188,
#  st_ino=66199286,
#  st_dev=2056L,
#  st_nlink=1,
#  st_uid=1016,
#  st_gid=1000,
#  st_size=221919,
#  st_atime=1343656131,
#  st_mtime=1343585892,
#  st_ctime=1343659618
# )

