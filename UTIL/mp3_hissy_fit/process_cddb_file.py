#!/usr/bin/env python3
#!/usr/bin/env python2

# breakpoint()

# mp3_hissy_fit.py # read CD-music into a tree of taged files
# LGPL # Graham Swallow # 2007
# this is a batch file, look at the scripts in ./eg_rip_here/

# USAGE
#
#	cd $HOME/tmp/ # same disk as $HOME/audio/wav/
#
#	hissy_fit --rip_here
#
#	hissy_fit --burn_cd ./audio.cddb
#
# OPTIONALLY # edit and correct audio.cddb before it is used # find copies?
#
#	hissy_fit --process_wav_to_wav ./audio.cddb
#
#	hissy_fit --process_wav_to_mp3 $HOME/audio/.cddb_recent/cddb_*
#
# RE-RUN to change ID3 strings
#
#	hissy_fit --process_wav_to_mp3 ./audio.cddb
#
#	The wav/artist/album/* must be present and correct
#	The mp3 files can be there and the ID3 tag will be replaced
#	This allows change of names, even dirnames, without reprocessing
#


# TODO # split parse_cddb_file and generally refactor the code
# TODO # make 8859 / UTF8 autodetect ? add tag to output .cddb file
# TODO # sudo (or not for those who dont want it)
# TODO # rethink box-sets, maybe an extra database to retain 'learned' stuff
# TODO # allow cddb files to be edited, used and reused # 
# TODO # MOVE FILES BETWEEN OLD AND NEW .cddb FILENAMES # **** # <<----=((
# TODO # parse cdindex, not cddb #
# TODO # cddb_fetch # and with/out a gui # local versions
# TODO # .cddb_post # particularly for a moderator, or batch via moderator
# TODO # generate .cddb for unknown CD or disconnected 
# TODO # cross-test against grip for compattability # so what if not ?
# TODO # rename old tree layout to new tree layout # ...
# TODO #
# this parses a cddb text file and stores it in an object
# note spelling of DISC, etc
# TODO # python print # cmd # silent !!

# SETUP / INSTALL

# check that python is found on PATH and has ID3.p3 (see /_borrowed/)

# check that this script (and neighbours) is found, with the name you want
# ln -s ~/2000/src/mp3_hissy_fit/mp3_hissy_fit.py ~/bin1/hissy_fit

# edit Config_Settings.py
# edit ALL_SCRIPTS.py (for options you want to change not in Config_Settings)

# create the directories:
#
# HOME/.cddb/
# HOME/audio/.cddb_recent
# HOME/audio/wav/
# HOME/audio/mp3_V0/


# TODO:
#
# SEARCH AUDIO PATH
#
#	that would allow each archive DVD to be its own subdir
#	but still searched + found for various uses
#	the layour would probably put MP3's and WAV's miles apart
#	but each dir-of-files must be intact
#	Alternatively - some database tracks dirs of objects
#	Already - the cddb file is repeated everywhere, and is a KEY

# TODO:
#
# Subdir for boxed-set, group, ...
#
#	each boxed set can have a subdir
#	groups/categories can have groups
#	shame that GENRE field is often wildly naff
#	maybe database overrides cddb, look in /not_yet_set/

# TODO
#
# Something with musicbrainz
#
#	cddb seems so primative (but universal)

import Config_Settings
config_settings = Config_Settings.Config_Settings()

import CDDB_File
from maps_grip_filter_fs_name import grip_filter_fs_name
from fs_util import *
import fs_util
import dgb
# from ID3 import *
import eyed3
# from eyed3.core import Date


#############################################################################

#


def id3_genre( id3, strng ):
	if strng == "X_misc":
		strng = "Blues"
	# absurd categories system
	g = id3.find_genre( strng ) # does not fail!
	if g == 255:
		strng = "misc"
	try:
		g = id3.find_genre( strng )
		id3.genre = g
	except:
		print(( "Genre not recognised:", strng ))
		pass
	print(( "# GENRE #" + strng + " " + str( g )))

#############################################################################

def utf8_from_8859( str ):
	uni = str( str, "iso-8859-1" )
	utf = uni.encode( "UTF-8" )
	return utf

def utf8_from_uni( uni ):
	utf=uni.encode( "UTF-8" )
	# print type(uni),
	# print( utf )
	return utf
	# return utf8_from_8859( str )

def str_from_utf8_from_uni( uni ):
	return uni
	dgb.obj_type( "uni", uni )
	utf=uni.encode( "UTF-8" )
	dgb.obj_type( "utf", utf )
	# print type(uni),
	# print( utf )
	return utf
	# return utf8_from_8859( str )

#############################################################################
class cddb_split_title_artist:

 def __init__( self ):
 	pass

#############################################################################

def audio_munger_get_action_map():
	map1 = {}
	## NORMALISE ## is broken ## 
	## broken also in already happens in load / save / summary
	## manually edit ?
	L1 = [
		"cddb_summary",
		"cddb_normalise", 
		"process_wav_to_wav",
		"process_wav_to_mp3",
		"check_saved_cddb_file",
		"burn_cd"
	]
	for name in L1:
		argname = "--" + name
		map1[ argname ] = name
	return map1

#############################################################################


class audio_munger:

 def __init__( self ):
 	self.setflag_file_names_changed()
 	self.set_Album( None )
 	self.set_lame_quality_V( 2 )
 	self.keep_cdp_names = 1
 	self.set_action( "fail_action_unset")

 def set_Album( self, Album ):
 	self.Album = Album
 	self.setflag_file_names_changed()

 def set_lame_quality_V( self, lame_quality_V ):
 	self.lame_quality_V = lame_quality_V
 	self.setflag_file_names_changed()

 def setflag_file_names_changed( self ):
 	self.file_names_changed = True

 def need_file_names(self):
 	if self.file_names_changed:
 		self.calc_file_names()

 def calc_file_names( self ):

 	print( "calc_file_names" )
 	self.file_names_changed = False # recalc done
 	global config_settings
 	HOME_DIR = config_settings.HOME_DIR
 	WAV_DIR1 = config_settings.AUDIO_DIR
 	WAV_dir2 = "wav"
 	WAV_dir3 = grip_filter_fs_name( self.Album.DTITLE_ARTIST )
 	WAV_dir4 = grip_filter_fs_name( self.Album.DTITLE_ALBUM )

 	print(( "WAV_dir4:", WAV_dir4))

 	MP3_DIR1 = WAV_DIR1
 	MP3_dir2 = "mp3_V%d" % self.lame_quality_V
 	MP3_dir3 = WAV_dir3
 	MP3_dir4 = WAV_dir4

 	self.WAV_DIR4 = WAV_DIR1 + "/" + WAV_dir2 + "/" + WAV_dir3 + "/" + WAV_dir4 
 	self.MP3_DIR4 = MP3_DIR1 + "/" + MP3_dir2 + "/" + MP3_dir3 + "/" + MP3_dir4 
 	self.CDDB_recent = WAV_DIR1 + "/.cddb_recent"
 	self.HOME_DIR = HOME_DIR
 	self.check_files_exist()

 def check_files_exist(self):
 	return # CANT DO THIS HERE ... refactor ?
 	self.check_dir_exists( self.WAV_DIR1 ) # MUST exist
 	self.check_dir_exists( self.WAV_DIR4 ) # ONLY after wavs have been read in

 def get_grip_track_year( self, idx ):
 	year = "1998"
 	try:
 		year = self.Album.DTITLE_ALBUM 
 	except:
 		year = "1997"
 	return year

 def get_grip_track_name( self, idx ):
 	trk = self.Album.TRACK[ idx ]
 	TT = self.get_TT01_of_idx0( idx )
 	if trk.TTITLE:
 		base = trk.TTITLE
 	else:
 		base = "UNKNOWN"
 	# maximum file size
 	# base = base[:20]
 	base = grip_filter_fs_name( base )
 	name = TT + "_" + base
 	return name

 def lame_wav_to_mp3( self, idx ):
 	self.need_file_names()
 	## trk = self.Album.TRACK[ idx ]
 	name = self.get_grip_track_name( idx )
 	year = self.get_grip_track_year( idx )
 	TT = self.get_TT01_of_idx0( idx )
 	# should maybe look on disk to see what is available, try both
 	print(( "self.keep_cdp_names", self.keep_cdp_names))
 	if self.keep_cdp_names:
 		pathname_wav = self.WAV_DIR4 + "/" + "audio_" + TT + ".wav"
 	else:
 		pathname_wav = self.WAV_DIR4 + "/" + name + ".wav"
 	pathname_mp3 = self.MP3_DIR4 + "/" + name + ".mp3"
 	pathname_tmp = self.MP3_DIR4 + "/" + name + ".mp3.tmp"
 
 	check_file_exists( pathname_wav, True )
 	if check_file_exists( pathname_mp3 ): return

 	# grip uses: -h -b %b # wav mp3 == %w %m

 	cmd = ''
 	cmd = cmd + 'nice '
 	cmd = cmd + "lame -h --vbr-new -V '%s' --tt '%s' --ty '%s' '%s' '%s'" % (
 		self.lame_quality_V,
 		name,
 		year,
 		pathname_wav,
 		pathname_tmp 
 	)
 	cmd = ''
 	cmd = cmd + 'nice '
 	cmd = cmd + "lame -h --vbr-new -V '%s' '%s' '%s'" % (
 		self.lame_quality_V,
 		pathname_wav,
 		pathname_tmp 
 	)
 	ret = call_system( cmd )
 	if ret == 0 :
 		ret = move_file(
 			pathname_tmp,
 			pathname_mp3 
 		)
 		return True
 	else:
 		raise Exception("command failed: " + cmd )
 		return None

 def get_TT01_of_idx0( self, idx ):
 	# get track number as two digits
 	return "%2.2d" % (1 + idx)

 def set_id3_tags_OLD( self, idx ):
 	self.need_file_names()
 	trk = self.Album.TRACK[ idx ]
 	name = self.get_grip_track_name( idx )
#	pathname_wav = self.WAV_DIR4 + "/" + name + ".wav"
 	pathname_mp3 = self.MP3_DIR4 + "/" + name + ".mp3"
 	pathname_tmp = self.MP3_DIR4 + "/" + name + ".mp3.tmp"
 	check_file_exists( pathname_mp3, True )
 	if trk.TTITLE:
 		id3_title = utf8_from_uni( trk.TTITLE ) # as utf8 ...
 	else:
 		id3_title = "UNKNOWN"
 	print( "id3_title", id3_title )
 	if trk.TARTIST:
 		id3_artist = utf8_from_uni( trk.TARTIST )
 		# instead of "various" as DTITLE_ARTIST, I use "box-set-name"
 		id3_comment = utf8_from_uni( self.Album.DTITLE_ARTIST )
 	else:
 		# most albums have single artist
 		id3_artist = utf8_from_uni( self.Album.DTITLE_ARTIST )
 		id3_comment = " # "
 	id3_genre_str = self.Album.DGENRE
 	try:
 		id3 = ID3( pathname_mp3 )
 		id3.title = id3_title
 		id3.album = utf8_from_uni( self.Album.DTITLE_ALBUM )
 		id3.artist = id3_artist
 		if self.Album.DYEAR:
 			id3.year =  utf8_from_uni( self.Album.DYEAR )
 		id3.track = idx + 1 # int(self.get_TT01_of_idx0( idx ))
 		id3.comment = id3_comment
 		# id3.genre = genre
 		id3_genre( id3, id3_genre_str )
 		id3.write()	# also called butomatically y dtor
 		print()
 		print( id3 )
 		print()
 	# except ( InvalidTagError, message ):
 	except InvalidTagError as message :
 		print(( "Invalid ID3 tag:", message ))

 def set_id3_tags( self, idx ):
 	self.need_file_names()
 	trk = self.Album.TRACK[ idx ]
 	name = self.get_grip_track_name( idx )
#	pathname_wav = self.WAV_DIR4 + "/" + name + ".wav"
 	pathname_mp3 = self.MP3_DIR4 + "/" + name + ".mp3"
 	pathname_tmp = self.MP3_DIR4 + "/" + name + ".mp3.tmp"
 	check_file_exists( pathname_mp3, True )
 	if trk.TTITLE:
 		id3_title = str_from_utf8_from_uni( trk.TTITLE ) # as utf8 ...
 	else:
 		id3_title = "UNKNOWN"
 	if trk.TARTIST:
 		id3_artist = str_from_utf8_from_uni( trk.TARTIST )
 		# instead of "various" as DTITLE_ARTIST, I use "box-set-name"
 		id3_comment = str_from_utf8_from_uni( self.Album.DTITLE_ARTIST )
 	else:
 		# most albums have single artist
 		id3_artist = str_from_utf8_from_uni( self.Album.DTITLE_ARTIST )
 		id3_comment = " # "
 	id3_genre_str = self.Album.DGENRE
 	try:
 		mp3file = eyed3.load( pathname_mp3 )

 		# when file has no id3 tags, then .tag is None 
		# then tag = TAG() maybe ?
 		# see lame call --tt title #
 		# dgb.obj_type( "mp3file.tag", mp3file.tag )
 		# dgb.obj_type( "id3_title", id3_title )

		# how does field tag know filename or object_addr
 		if mp3file.tag is None:
 			print("SETTING tag away from None")
 			mp3file.tag = eyed3.id3.Tag()
 		# mp3file.tag = eyed3.id3.Tag()
 		mp3file.tag.title = id3_title
 		mp3file.tag.album = str_from_utf8_from_uni( self.Album.DTITLE_ALBUM )
 		mp3file.tag.artist = id3_artist
 		mp3file.tag.album_artist = id3_artist

		# this stopped working !!
		# it started when Date # maybe version of id3 records #
		# or was it when lame set the fields # IDK #
		# but this module is not what it should be #
 		if self.Album.DYEAR:
 			YYYY_str =  str_from_utf8_from_uni( self.Album.DYEAR )
 		else:
 			YYYY_str =  "1999"
 		YYYY =  int(YYYY_str)
 		print("Setting YYYY", YYYY, "and yet NOT" )
 		mp3file.tag.year = YYYY
 	 #	mp3file.tag.recording_date =  eyed3.core.Date(YYYY)
 		mp3file.tag.recording_date =  YYYY_str # OK but as "recording_date"
# 		mp3file.tag.year =  eyed3.core.Date(YYYY) # nowt
# recording date is in # eyeD3 -V $FILE # but not year
 			
 		mp3file.tag.track_num = idx + 1 # int(self.get_TT01_of_idx0( idx ))
 		# TEST # id3_comment = "HISSY_FIT_COMMENT"
 		# id3_comment = "HISSY_FIT_COMMENT"
 		# print("id3_comment",id3_comment)
 		# NO # mp3file.tag.comment = id3_comment # did not work
 		mp3file.tag.comments.set(id3_comment)

 		# print("id3_genre_str",id3_genre_str)
 		mp3file.tag.genre = id3_genre_str
 		# mp3file.tag.genre = eyed3.id3.Genre(id3_genre_str)

 		mp3file.tag.save()	# also called butomatically y dtor
 		print()
 		print( mp3file )
 		print()
 	# except ( InvalidTagError, message ):
 	# except InvalidTagError as message :
 	# except Exception as message :
 	except TagException as message :
 		print(( "EXCEPTION:", message ))

 def check_saved_cddb_file( self ):

 	self.need_file_names()
 	# self.cddb_input_filename = filename
 	audio_cddb = "audio.cddb"
 	fs_DTITLE = grip_filter_fs_name( self.Album.DTITLE_ALBUM )
 	cddb_number_name = "cddb_" + self.Album.DISCID + "_" + fs_DTITLE

#	copy
#	copy_file_if_missing( self.Album.cddb_input_filename, home_cddb_number )
#	copy_file_if_missing( self.Album.cddb_input_filename, self.WAV_DIR4 + "/" + audio_cddb )
#	copy_file_if_missing( self.Album.cddb_input_filename, self.WAV_DIR4 + "/" + cddb_number_name )
#	copy_file_if_missing( self.Album.cddb_input_filename, self.MP3_DIR4 + "/" + audio_cddb ) 

#	makedir
 	make_dir( self.MP3_DIR4 + "/" )

	# regenerate # needs testing, and guard file - for when it fails
# SAFE	self.Album.write_as_file( self.HOME_DIR + "/.cddb/" + self.Album.DISCID )
 	regen_dir =  self.HOME_DIR + "/audio/.cddb/_regen" 
 	make_dir( regen_dir + "/" )
 	make_dir( self.CDDB_recent + "/" )
 	filename =  regen_dir + "/" + self.Album.DISCID
 	self.Album.write_as_file( filename )
#	DONT LOSE THE ORIGINAL FROM cdda2wav, but do add cddb_number_name
#	self.Album.write_as_file( self.WAV_DIR4 + "/" + audio_cddb )
#	EVEN IF THAT MEANS latin1 problems ?
# but do write it to the current dir which WAS the cdda2wav copy!
 	self.Album.write_as_file( self.WAV_DIR4 + "/" + cddb_number_name )
 	self.Album.write_as_file( self.MP3_DIR4 + "/" + audio_cddb ) 
 	self.Album.write_as_file( self.CDDB_recent + "/" + cddb_number_name ) 
 	self.Album.write_as_file( ".test_out_cddb" )
 	#
 	# dont write normalised audio_cddb in .
 	# this is because cdda2wav sometimes leaves on behind ?
 	# or delete when calling cdda2wav
 	self.Album.write_as_file( audio_cddb )

 def set_action( self, name ):
 	# check that action is valid ... or let python notice
 	self.action = name

 def run_action( self ):
 	CALLABLE = getattr( self, self.action)
 	return CALLABLE()
 	return getattr( self, self.action)()

 def process( self ):
 	self.process_wav_to_mp3()

 def process_wav_to_mp3( self ):
 	self.need_file_names()
 	make_dir( self.MP3_DIR4 + "/" )

 	# if cdparanoia in, call mkae_dir - but not for this
 	check_file_exists( self.WAV_DIR4 + "/", True ) # wavs must exist too
 	check_file_exists( self.MP3_DIR4 + "/", True ) # must exist
 	# checking for MP3_DIR4 avoids mishaps - disk full

 	# after splitting TTITLE into TITLE and ARTIST

 	# write ../.cddb/cddb_DISCID_title
 	self.check_saved_cddb_file()

 	# return # dont copy just check file names

 	N = len( self.Album.TRACK )
 	for idx in range( 0, N ):
 		self.lame_wav_to_mp3( idx )
 		self.set_id3_tags( idx )
 		#
 		# eg after fixing the id3 parsing, rerun, relabel each, no lame

 def cmd_list_all_wav_files_without_dir( self ):
 	cmd = ""
 	N = len( self.Album.TRACK )
 	for idx in range( 0, N ):
 		TT = self.get_TT01_of_idx0( idx )
 		file_wav = "audio_%s.wav" % TT
 		print( file_wav )
 		# cmd = cmd + " '%s'" % file_wav
 		cmd = cmd + " %s" % file_wav
 	return cmd

 def check_files_exist_all_wav( self ):
 	check_file_exists( self.WAV_DIR4, True ) # where WAVS are
 	N = len( self.Album.TRACK )
 	for idx in range( 0, N ):
 		TT = self.get_TT01_of_idx0( idx )
 		file_wav = "audio_%s.wav" % TT
 		print( file_wav )
 		dir_file_wav = self.WAV_DIR4 + "/" + file_wav
 		check_file_exists( dir_file_wav, True ) # force stop

 def burn_cd( self ):
 	global config_settings
 	# man cdrecord says:
 	# cdda2wav dev=2,0 -vall cddb=0 -B -Owav
 	# cdrecord dev=2,0 -v -dao -useinfo -text  *.wav

 	self.need_file_names()
 	self.check_files_exist_all_wav()

 	cmd = ""
 	cmd = cmd + "cd '%s';"	% self.WAV_DIR4
 	if config_settings.use_sudo_pfx:
 		cmd = cmd + config_settings.use_sudo_pfx
 	else:
 		fs_util.must_be_root("I prefer to call cdrecord as root")
 	# cmd = cmd + " echo" # TESTING
 	# cmd = cmd + " echo" # TESTING
 	cmd = cmd + " cdrecord"
 	cmd = cmd + " -speed=%s"	% config_settings.cdrecord_speed
 	cmd = cmd + " -dev=%s"	% config_settings.cdrecord_dev
 	cmd = cmd + " -driveropts=burnfree"
 	cmd = cmd + " -v -eject -dao -useinfo -text "
 	cmd = cmd + self.cmd_list_all_wav_files_without_dir()
 	# this is optional - but I want it
#	fs_util.must_be_root("I prefer to call cdrecord as root")
 	ret = call_system( cmd )
 	if 0 != ret:
 		raise Exception("command failed: " + cmd )
 	return ret

 def cddb_summary( self ):
 	print( "### SUMMARY ## #" )
 	txt = self.Album.txt_cddb_summary()
 	print(( txt.get_unicode_text() ))

 def cddb_normalise( self ):
 	print( "### normalise ## #" )
 	txt = self.Album.txt_cddb_summary()
 	print(( txt.get_unicode_text() ))

 def process_wav_to_wav( self ):
 	# copy '.' to WAV_DIR4
 	self.need_file_names()
 	make_dir( self.WAV_DIR4 + "/" )
 	make_dir( self.MP3_DIR4 + "/" )

 	# if cdparanoia in, call make_dir - but not for this
 	check_file_exists( self.WAV_DIR4 + "/", True ) # wavs must exist too
 	check_file_exists( self.MP3_DIR4 + "/", True ) # must exist
 	# checking for MP3_DIR4 avoids mishaps - disk full

 	# after splitting TTITLE into TITLE and ARTIST

 	# write ../.cddb/cddb_DISCID_title
 	self.check_saved_cddb_file()

 	N = len( self.Album.TRACK )
 	for idx in range( 0, N ):
 		TT = self.get_TT01_of_idx0( idx )
 		name = self.get_grip_track_name( idx )
 		src_wav = "./audio_%s.wav" % TT
 		src_inf = "./audio_%s.inf" % TT
 		if self.keep_cdp_names:
 			dst_wav = self.WAV_DIR4 + "/" + src_wav
 			dst_inf = self.WAV_DIR4 + "/" + src_inf
 		else:
 			dst_wav = self.WAV_DIR4 + "/" + name + ".wav"
 			dst_inf = self.WAV_DIR4 + "/" + name + ".inf"
 		if not check_file_exists( self.MP3_DIR4 + "/", False ):
 			print(( "# NOT copied # " + src_wav ))
 			continue
 		# print( "# YES copied # " + src_wav )
 		# continue
 		move_file_repeatable( src_wav, dst_wav )
 		move_file_repeatable( src_inf, dst_inf )

 	# return # dont copy just check file names
 	copy_file_if_missing( "audio.cddb", self.WAV_DIR4 + "/audio.cddb.IN" )
 	copy_file_if_missing( "audio.cddb", self.WAV_DIR4 + "/audio.cddb" )
 	move_file_repeatable( "audio.cdindex", self.WAV_DIR4 + "/audio.cdindex" )
 	# move_file( "audio.cddb", "audio_OLD.cddb" )
 	copy_file( "audio.cddb", "audio_OLD.cddb" )
 	# move_file( ".test_out_cddb", "../OLD_.test_out_cddb" )
 	# repeatable means the command isnt repeated when the call is !
 	# move_file_repeatable( "audio.cddb", "../OLD_audio.cddb" )
 	# move_file_repeatable( ".test_out_cddb", "../OLD_.test_out_cddb" )

#############################################################################

class munger_caller:

	def __init__( self ):
		# restore from older .wav archive
		self.common_use_cdp_names = 0
		# self.lame_V = 0
		# most cddb files are 8859
		# beware second run finds normalised cddb file!
		self.utf8 = False
		#
		self.action_map = audio_munger_get_action_map()

	def rip_here( self ):
		global config_settings
		""" cddb file doesnt exist yet ..."""
		# remove any old one, in case cdda2wav leaves it untouched
		move_file( "audio.cddb", "audio.cddb_PREV" )
		move_file( ".test_out_cddb", ".test_out_cddb_PREV" )
		if 0!=call_system( "eject -t" ):
			call_system( "sleep 2; eject -t" )

		# SUDO cdda2wav dev=... # as a multiline STR0
		cmd = """
%s %s \
dev=%s \
-vall \
cddb=0 \
-B \
-Owav \
-paranoia \
cddbp-server=%s \
#
		""" % (
			config_settings.use_sudo_pfx,
			config_settings.cdda2wav_cmd,
			config_settings.cdda2wav_dev,
			config_settings.cdda2wav_cddbp_server
		)

		# scratch the above, SUDO not needed ?
		cmd = """
%s \
dev=%s \
-vall \
cddb=0 \
-B \
-Owav \
-paranoia \
cddbp-server=%s \
speed=8 \
#
		""" % (
			config_settings.cdda2wav_cmd,
			config_settings.cdda2wav_dev,
			config_settings.cdda2wav_cddbp_server
		)
		# dont believe the speed=8 # but added it 
		# TODO # add as config_settings.cdda2wav_speed # -1 ?

		cmd2 = " sudo chown %s * . ;  # sync " % config_settings.USER
		if test_uid_is_root():
			pass
		else:
			if config_settings.use_sudo_pfx:
				cmd = config_settings.use_sudo_pfx + cmd
			else:
				fs_util.must_be_root("You might get cdda2wav: Operation not permitted. Cannot send SCSI cmd via ioctl")
		print(( "# cmd #", cmd ))
		ret = call_system( cmd )
		if ret == 0 :
			# eject before sync # persuade operator to kick the cable !
			call_system( "eject" )
		ret2 = call_system( cmd2 )
		if ret == 0 :
			# DONE # call_system( "eject" )
			print( "RIP WORKED" )
			return True
		else:
			# dont eject - make user wake up + do something
			print(( "command failed with exit code: %d" % ret ))
			# also eject -t might need a second to load tray
			# so leave it ready for next attempt
		return False

	def main_sys_argv( self, argv ):

		if len(argv) > 1:
			argv = argv[1:]
		else:
			argv = 'audio.cddb'

		munger = audio_munger()
		munger.set_lame_quality_V( 4 ) # my default
		self.PDB_on = True
		self.PDB_on = None  # default OFF

		for arg in argv:

			if arg == "--PDB":
				# argv = argv[1:]
				self.PDB_on = True
				continue

			# cdda2wav uses simple filenames, it provides cdinfo, so keep for now
			# check this comand line .. review, rework all filename stuff
			# cdda2wav dev=/dev/cdrom -vall cddb=0 -B -Owav  

			if arg == "--use_cdp_names":
				self.common_use_cdp_names = 1
				# supposed to look on disk to see whats there - but force CLI solution
				munger.keep_cdp_names = 1
				continue

			if arg == "--lame_V=0":
				# self.lame_V = 0
				munger.set_lame_quality_V( 0 )
				continue

			if arg == "--lame_V=4":
				# self.lame_V = 4
				munger.set_lame_quality_V( 4 )
				continue

			if arg == "--utf8":
				self.utf8 = True
				continue

			if arg == "--8859":
				self.utf8 = False
				continue

			if arg == "--rip":
				munger.set_action( "rip" )
				continue

			# if internet is available ...
			# do --process_wav_to_wav audio.cddb
			# otherwise ... TODO: create half-step phases
			# fetch .cddb with a differenet tools, when online
			# remove -cddb from call to cdda2wav
			# place audio.cddb file when available
			# TOOL: generate .cddb request from FILES
			# TOOL: batch-fetch-now (dial_up)
			# THEN: call --process_wav_to_wav audio.cddb

			if arg == "--rip_here":
				if self.rip_here():
					continue
					argv.append( "--process_wav_to_wav" )
					argv.append( "audio.cddb" )
					# continue
				else:
					raise Exception("rip_here() died" )
					continue


			try:
				print(( "## ARG ##", arg ))
				act = self.action_map[ arg ]
				munger.set_action( act )
				continue
			except:
				pass
				

			parser = CDDB_File.CDDB_Parser()
			parser.load_lines_from_file( arg, self.utf8 )
			parser.parse_lines()

			munger.set_Album( parser.Album )
			if 1:
				munger.run_action()
				# fs_util.must_not_be_root()
				continue

			# convert .wav to 9 different .mp3 compressions
			# munger.set_action( "process_wav_to_mp3" )
			#for lame_quality_V in range( 0, 9+1 ):
			#	munger.set_lame_quality_V( lame_quality_V )
			#	munger.run_action( )

			# after each action on command line, other than args
			# alternative is each action
			sync_request()
		# problems with other activity never completing


#############################

import sys
caller = munger_caller()
caller.main_sys_argv( sys.argv )
# call_system("sync")
# call_system("nohup sync &")
# done # sync_request()

