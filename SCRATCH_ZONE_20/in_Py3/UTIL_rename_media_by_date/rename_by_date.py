#!/usr/bin/env python

# rename_by_date # ie the mtime of the file 
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
import stat
import time 
import re_cache
RE = re_cache.re_cache()

fmt2 = "%a, %d %b %Y %H:%M:%S +XXXX"
fmt1 = "%Y-%m-%d_%a_%H%M_%S"
fmt = fmt1

def os_rename( filename1, filename2 ):
		print( "mv -n", filename1, filename2 )
		os.rename( filename1, filename2 )
		return None

class match_filename_filter:
 def build_tails( self ):
 	self.tail_dict = {}
	# only need to list UPPER CASE 
 	tails = [
 		"JPG",
 		"JPEG",
 		"AVI",
 		"MP4",	# SAMSUNG Camera
 		"MP3",
 		"3GP",
 	]
 	# convert all exts to lower
 	for tail in tails:
 		tail_low = tail.lower()
 		self.tail_dict [ tail ] = tail_low
 		self.tail_dict [ tail_low ] = tail_low
 	# print( "#", self.tail_dict )
 def build_re( self ):
 	d = "[0-9]" # digit # 
 	re_decimal = d + "+" # timestamp as long decimal # 1663447650623
 	re_sep = "[-_]"
 	re_YEAR = "[0123]" + d + d + d 
 	re_MM = "[01]" + d 
 	re_DD = "[0123]" + d

 	# R matches 1999-12-31
 	# R matches 1999_12_31
 	# which will matche files we have already renamed
 	# or files that are already good enough
 	# most cameras dont use re_sep
 	#
 	self.re_YEAR_MM_DD = re_YEAR + re_sep + re_MM + re_DD

 	R3 = ""
 	R3 += RE.wrap_group_re( re_YEAR )
 	R3 += re_sep 
 	R3 += RE.wrap_group_re( re_MM )
 	R3 += re_sep 
 	R3 += RE.wrap_group_re( re_DD )
 	self.re_YEAR_MM_DD_3 = R3

 	self.re_myrec_STAMP = "myrec" + RE.wrap_group_re( re_decimal )

 	# print( "self.re_YEAR_MM_DD_3", self.re_YEAR_MM_DD_3 )

 def __init__( self ):
 	## applied to filename keep this
 	self.found_YYYY = 1999
 	self.found_MM = 12
 	self.found_DD = 31

 	self.build_tails()
 	self.build_re()


 def match_filename_ext( self, filename ):
 	# only match .ext in list # skip over others
 	ext = self.get_valid_ext( filename )
 	# l = filename.rsplit(".",1)
 	# ext = l[-1]
 	try:
 		seen = self.tail_dict[ ext ]
 	except:
 		seen = None
 		print( "returning seen = ", seen, "ext = ", ext )
 	return seen

 def get_valid_ext( self, filename ):
 	l = filename.rsplit(".",1)
	# l is a list # l =  ['P2110001', 'JPG'] 
	# unless it isnt
 	ext = l[-1]
 	# print("l = ", l, "ext = ", ext )
 	return ext
 	
 def match_filename_uses_YEAR_MM_DD( self, filename ):
 	if RE.run( filename, self.re_YEAR_MM_DD_3 ):
 		if 0:
 			print( "group(0)", RE.group(0) ) # YEAR-MM-DD #
 			print( "YEAR", RE.group(1) )
 			print( "MM", RE.group(2) )
 			print( "DD", RE.group(3) )
 		self.found_YYYY = RE.group(1)
 		self.found_MM = RE.group(2)
 		self.found_DD = RE.group(3)
 		return "uses YEAR_MM_DD_3"
 	if RE.run( filename, self.re_YEAR_MM_DD ):
 		print( "uses YEAR_MM_DD" )
 		return "uses YEAR_MM_DD"
 	else:
 		return None
 	

 def match_filename_already_renamed( self, filename ):
 	re_done_1 = ""
 	return None # false


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

# try not to rename files that have just been rotated and time_changed
# if its newer than when we got home, drop it

days_too_old = 365*2 # limit too old to be true
days_too_new = 0.01 # limit too new to be true

# or pick some other value

days_too_old = 1
days_too_old = 0.5
days_too_old = 99 # limit age of files
days_too_old = 199 # limit age of files
days_too_old = 365*2 # limit age of files
days_too_old = 365*40 # limit age of files
days_too_old = 99 # limit age of files

days_too_new = -2 # clocks wrong

def secs_from_days( days ):
	return days * 24 * 60 * 60 

time_now = time.time()

time_too_old = time_now - secs_from_days( days_too_old )
time_too_new = time_now - secs_from_days( days_too_new )

print( "# MIN #", local_str_from_time_int( fmt1, time_too_old ) )
print( "# NOW #", local_str_from_time_int( fmt1, time_now ) )
print( "# MAX #", local_str_from_time_int( fmt1, time_too_new ) )


D1="."

filter = match_filename_filter()

# filter.match_filename_uses_YEAR_MM_DD( "1999-12-31" )
# filter.match_filename_uses_YEAR_MM_DD( "1999-12-31ABC" )
# filter.match_filename_uses_YEAR_MM_DD( "LHS1999-12-31ABC" )

L = os.listdir(D1)
L.sort()
for filename1 in L:
	# skip if filename already contains YEAR-MM-DD
	reason = filter.match_filename_uses_YEAR_MM_DD( filename1 )
	if reason:
		print( "# SKIPPING filename #", reason, "#",  filename1 )
		continue
	
	if RE.run( filename1, filter.re_myrec_STAMP ):
		decimal = RE.group(1)
		print("# REC #", filename1, decimal )
		continue;

	s = os.stat( filename1 ) # might FAIL if deleted since listdir (etc)

	# skip dirs dev fifo symblink
	if not stat.S_ISREG(s.st_mode):
		print( "# SKIPPING not a file # ", filename1 )
		continue

#	# not a file includes DIR
#	# skip dirs dev fifo symblink
#	if stat.S_ISDIR(s.st_mode):
#		print( "# SKIPPING dir # ", filename1 )
#		continue

	# skip if .ext not on the list
	ext2 = filter.match_filename_ext( filename1 )
	if ext2 == None:
		print( "# SKIPPING ext2 == None # ", filename1 )
		continue

	# skip if mtime too_old or too_new
	file_mtime = s.st_mtime
	if file_mtime > time_too_new:
		print( "#", filename1, local_str_from_time_int( fmt1, file_mtime ), "# TOO NEW" )
		continue
	if file_mtime < time_too_old:
		print( "#", filename1, local_str_from_time_int( fmt1, file_mtime ), "# TOO OLD" )
		continue

	# print("# ??? #", filename1 )
	# continue;
	# compute new filename, version vn is clash from same second
	vn_str = ""
	vn = 0
	looking = 1
	while looking:
		# move this forward - to end 
		vn += 1
		vn_str = "-{0}".format( vn )
		# fmt1 = "%Y-%m-%d_%a_%H%M_%S"
		# f2 = "1999-12-31-Fri_2359_59"
		f2 = local_str_from_time_int( fmt1, file_mtime )
		filename2 = f2.lower() + vn_str + "." + ext2
		if filename2 == filename1:
			# rename file as itself = OK ish
			looking = 0
			break
		try:
			os.stat( filename2 )
			print( "# ALREADY EXISTS -", filename2 )
		except:
			looking = 0
			break
	
	# and do the rename
	if filename2 == filename1:
		print( "# SAME", filename2 )
	else:
		# func does print or call
		os_rename( filename1, filename2 )

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

