# fs_util.py
# could be recoded, to stat, link, copy, ...
# currently implemented as os.system

import os

def test_uid_is_root():
	return test_uid_is_zero()
def test_uid_is_zero( default=True ):
	try:
		uid = os.getuid()
	except:
		# FAILED: os.getuid
		# return True
		# mabe OS does have "uid" and user is "admin"
		# or at least the request will work
		return default # OS without getuid
	if uid == 0:
		return True
	return False


def test_uid_is_non_zero( default=True ):
	return not test_uid_is_zero()
	return not test_uid_is_zero( default )

def must_be_root(reason = None):
	if test_uid_is_zero():
		return True
	if reason:
		print "# must_be_root('%s') " % reason
	raise "Must be root - please dont run as a plain user"

def must_not_be_root():
	if not test_uid_is_zero():
		return True
	raise "Must not be root - please run as a plain user"

def stat_file_or_None( filename ):
	try:
		stat = os.stat( filename )
	except:
		stat = None
	return stat

def check_file_exists( filename, throws=False ):
	stat = stat_file_or_None( filename )
	if stat:
		print "# found  #", filename
	else:
		print "# ABSENT #", filename
		if throws:
			raise filename
	return stat

# upgrade with something that can call sudo
# without lookin at syntax of provided command
# upgrade with ... something that cant run shell command junk
def call_system( cmd ):
	print "# RUN # " + cmd
	ret = os.system( cmd )
	if ret != 0 :
		print "# EXIT CODE #" + str(ret)
	return ret

def make_dir( pathname ):
	if not check_file_exists( pathname ):
		cmd = "mkdir -p '%s'" % pathname 
		call_system( cmd )

def move_file_repeatable( old, new ):
	if check_file_exists( new ):
		return
	move_file( old, new )

def move_file( old, new ):
	cmd = "mv '%s' '%s'" % (
		old,
		new 
	)
	# should exist or else ... ? untested!
	# cmd = "echo " + cmd
	ret = call_system( cmd )
	return ret

def copy_file( src, dst ):
	cmd = "cp -p '%s' '%s'" % ( src, dst )
	ret = call_system( cmd )
	return ret

def copy_file_if_missing( src, dst ):
	if not check_file_exists( dst ) :
		copy_file( src, dst )

# dst is a link that points to src
# ensure the link is good relative to its own dir!
def symb_link_file_if_missing( self, src, dst ):
	if not check_file_exists( dst ) :
		cmd = "ln -s '%s' '%s'" % ( src, dst )
		call_system( cmd )



def sync_request():
	# Today I'm moving files to a USB-1 HDD
	# calling sync takes forever
	# really I just want a good effort at sync
	call_system( "nohup sync >/dev/null &" )
