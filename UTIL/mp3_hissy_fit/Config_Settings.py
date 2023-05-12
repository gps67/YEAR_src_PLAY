
# Config_Settings.py # things that I would change more often than scripts
# other mp3_hissy_fit things are in the scripts, and still changable
# you will be expected to adjust these things 
# This might be overruled by some sort of database

# usage:
#
# import Config_Settings
# config_settings = Config_Settings.Config_Settings()
# .... config_settings.cdrecord_speed ...

import os
import fs_util

class Config_Settings:
 def __init__( self ):
 	#
 	# chown the files to ... USER
 	#
 	self.USER = os.environ['USER']
 	#
 	# where is $HOME/.cddb/ going to be found
 	#
 	self.HOME_DIR = os.environ['HOME']
 	#
 	# but when running as root ...
 	# maybe use SUDO_USER
 	# or LOGNAME or ...
 	# also detect NEW files and chown them, but not if already existed
 	# but that is not done - TODO - so run as user, and sudo cdda2wav
 	#
 	if self.USER == "root":
 		# when run as root (to get cdrecord pri perms) act for 'gps'
 		# NOW no need for root - sudo is used
 		self.USER = "gps"
 		self.HOME_DIR = "/home/gps"
 	#
 	# store tree of files here
 	#
 	self.AUDIO_DIR = self.HOME_DIR + "/audio"

 	#
 	# cdda2wav command # re-reads from CD-media to file
 	# other options are in the scripts
 	#
 	self.cdda2wav_cmd = "/tools/2000/media/bin/cdda2wav"
 	self.cdda2wav_cmd = "/usr/bin/cdda2wav"
 	self.cdda2wav_cmd = "cdda2wav"
 	self.cdda2wav_dev = "/dev/cdrom"
 	self.cdda2wav_cddbp_server = "freedb.freedb.org"
 	self.cdda2wav_cddbp_server = "http://gnudb.gnudb.org:80/~cddb/cddb.cgi"
 	self.cdda2wav_cddbp_server = "gnudb.gnudb.org"
 	#
 	# cdrecord command # writes tracks to CD media
 	# it used to use dev=0,2,0  Now it accepts /dev/hdd
 	# Other options are in the scripts
 	# Different batches of media, writers and machines means a lower speed
 	#
 	self.cdrecord_dev = "/dev/cdrom"
 	self.cdrecord_speed = "4"
 	self.cdrecord_speed = "32"

 	# quick hack to allow me to use sudo
 	# and allow you to switch it off, or use other
 	# self.use_sudo_pfx = ""
 	if fs_util.test_uid_is_zero():
 		self.use_sudo_pfx = " "
 	else:
 		self.use_sudo_pfx = "/tools/2000/admin/bin/pseudo --chroot_users_media "
 		self.use_sudo_pfx = "sudo -- "
 		self.use_sudo_pfx = "AS_ROOT "
 	
 	self.submit_submit_cddbp_server = "gnudb.gnudb.org"
 	self.submit_submit_cddbp_server = "127.0.0.1"
 	self.submit_submit_cddbp_port = "80"
 	self.submit_submit_cddbp_port = "8080"
 	self.submit_submit_cddbp_uri = "8080"

