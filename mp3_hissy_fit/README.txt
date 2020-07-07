mp3_hissy_fit is a python script to convert many CDs to MP3

	cdda2wav converts CD to wav files, and does .cddb lookup/fetch
	script parses .cddb files (and maintains consistent tree of files)
	script moves wavs into correct dir (HOME/audio/artist/title/)
	lame converts wav to mp3, at various quality levels
	script tags mp3's with ID3 tags (edit to your pref style)
	cdrecord writes out CD's from the .cddb file (and the tree of wavs)
	script is called from the command line, no GUI

mp3_hissy_fit

	mp3_hissy_fit is where python swallows a box of music CD's
	and converts a tree of files to MP3 files with ID3 tags

	too much LAME compression causes a dulling of the high tones.
	too little compression, and not enough tracks fit on the device.

	You can produce multiple trees, at different LAME compression levels,
	so that you can balance how much will fit onto your player,
	against the quality level you want. Then if you change your mind,
	you can build-up a different tree.

	It is driven by the .cddb file, and the script / parameters,
	so it produces the same results every time (with ID3 tags).

	TODO: figure out ways of adjusting the filename mangling + rework

	like most of my scratch scripts, I edit it each time I use it,
	eg so that box-sets appear in a common subtree (or not),
	but it is migrating towards a stable thing

	TODO: use sqllite to add missing BOXSET field (key=CDDB_ID)

	I want it to re-run, with the same results every time,
	all driven by the cddb file (cdda2wav fetches that from freedb).
	Its quite good at re-applying ID3 tags to existing MP3 files.

	EG ACTION: recode a tree of MP3's at quality 0, to get less hiss
	EG ACTION: recode a tree of MP3's at quality 4, to get the optimum
	EG ACTION: recode a tree of MP3's at quality 6, to get more tracks on
	EG ACTION: recode a tree of MP3's at quality 9, to get headaches
	EG ACTION: recode with different use of .cddb text in ID3 tags

	This tool isnt much use without an internet connection. (yet)

	TODO: various steps down the submit-new-CDDB route

COPYING:

	this code is L-GPL
	Copyright Graham Swallow 2006, 2007, 2008, 2012
	http://www.information-cascade.co.uk/

INSTALL:

	ensure that you have Python, lame, cdda2wav, ... installed ..

		type cdda2wav
		type lame
		type python

	ensure that you have ID3.py installed (see the _borrowed subdir)

		$prefix/lib/python2.4/site-packages/ID3.py
		$prefix/lib/python2.7/ID3.py

	untar this anywhere (or use midnight commander)

		cd $HOME
		tar -zxvf /tmp/mp3_hissy_fit-2007-01-01.tgz

	creating a link on your PATH to the main script

		ln -s $HOME/mp3_hissy_fit/hissy_fit /usr/local/bin/
		# note that python is clever, and figures out symb-links
		# so that it finds adjacent scripts and imported modules
		# when running cdrecord, you may need to be root, /usr/bin/ ?

	EDIT any default settings (maybe OK as-is)

		# default lame_V is 4
		# ID3 tags can be varied to suit you
		# default cdrecord options, speed 32 means 20 for me

	create a directory (symb link) where the big files will go

		mkdir $HOME/audio # or link, or link subdirs
		mkdir $HOME/audio/wav
		mkdir $HOME/audio/mp3_V4

	create the subdirs where .cddb files will be kept

		mkdir $HOME/audio/.cddb
		mkdir $HOME/audio/.cddb_recent

		NOTE hissy_fit uses long filenames in .cddb_recent
		the files are similar, the names have the title added

	create an empty dir where the initial rip happens (same disk)

		# TODO # maybe I should put this in the script
		# as well as clearing out the residue

		mkdir $HOME/audio/tmp/cdscratch

	test it now with

		cd $HOME/audio/tmp/cdscratch
		hissy_fit --rip_here
		ls -l
		hissy_fit --process_wav_to_wav ./audio.cddb	
		ls -l # here almost empty
		hissy_fit --process_wav_to_mp3  ./audio.cddb
		ls -l # and over in $HOME/audio/ ... look around

rip a cd to wav and convert to mp3 at quality level 4:

	create an empty dir ; cd  somewhere $HOME/tmp/cdscratch
	read in cd to wav:


USAGE:

	see further down

	NB: hissy_fit --rip_here calls cdda2wav which uses an Internet
	connection to get the .cddb file from freedb.freedb.org
	This tool isnt much use without the .cddb file
	This tool isnt much use without an internet connection 
	internet ISNT used after the .cddb file is available
	TODO: code a system to batch fetch, or proxy with a stack of CDS
	TODO: after offline cdda2wav, go online and upgrade to freedb/cdindex
	NOTE: cdda2wav also uses cdindex files, BEST WITH NET CONNECTED

USES COMMANDS WITH OPTIONS:

	#
	# cdrtools
	# http://cdrecord.berlios.de/
	#
	cdda2wav \
		dev=/dev/cdrom \
		-vall \
		cddb=0 \
		-B \
		-Owav \
		cddbp-server=freedb.freedb.org

	#
	# http://lame.sourceforge.net
	#
	lame -h --vbr-new -V 4 f.wav f.mp3.tmp

	#
	# http://id3-py.sourceforge.net/ 
	#
	ID3.py (GPL - but its normally fitted to Python, so ... ?)

	#
	# http://www.python.org
	#
	Linux, Python

		WIN32 might not work out-of-the-box, but not far off

TODO:

	add an extra parameter/data into the .cddb file?
	box_set has different layout of filenames
	put data into a pickle db
	add attributes (layout) to cddb file
	add sleeve-notes, opinions, fluff, db extras not in .cddb
	add try:catch: to process many files unattended
	finish work on utf/python (noticed the need here, code evolving)
	moving files is not a part of --rip_here
	front-end (other than xterm)
	RATIO (-V4 is 10:1)

	http://wiki.musicbrainz.org/PythonMusicBrainz2

	First sight of box-set, disk,
		DISCID=bc0b340f
		DTITLE=Ruben Gonzales / ORIGINAL DE CUBA - 5 Leyendas
	In this case, the pattern is correct, and followed aby all 5
	The artist is the artist
	The album name is the boxset name
	but we want the album name to be modified (#1 #2 )
	and subsequent items in the boxset, with similar strings match
	and use the same (selected) pattern.
	Other patterns might be ...


BUGS:

	too many to mention

	cdda2wav leaves .test_out.cddb
	or does it leave audio.cddb
	delete that first, test when .cddb is found, look !

FILES:

	README.txt
	hissy_fit

	CDDB_File.py
	fs_util.py
	maps_grip_filter_fs_name.py
	maps_plausible_list.py
	parse_cddb_file.py
	process_cddb_file.py
	re_cache.py

	_borrowed/ID3.py	# in case you need it in a hurry

	go_rename_dir	# ignore symb link to a centralised file
	go_tarball	# ignore symb link to a centralised file
	go_upload	# ignore symb link to a centralised file

hissy_fit

	MAIN SCRIPT - currently a symb link

CDDB_File.py

	parses a CDDB file
	stores the data
	generates a new CDDB text file (but allows long lines)
	DOESNT: calculate the disk id, or fetch the file

maps_grip_filter_fs_name.py
maps_plausible_list.py

	how to convert strings to filenames
	started off as grip compat, but now not tested

	In future, you might be able to import a tree of files
	prepared by GRIP (a much loved tool), and build your
	own layout of files, with a file that isnt cddb (extra fields)

	today, a game of tag, tomorrow a jukebox (?)

fs_util.py
	generic bits, make dir, getuid, ...

re_cache.py
	generic RE class, used by the parser


process_cddb_file.py


	MAIN SCRIPT (called from shell)
	run action on each track of each named cddb file

	USAGE:
	hissy_fit --rip_here	# current directory should be empty + temp

	USAGE:
	hissy_fit --burn_cd  ./audio.cddb # SINGLE cddb file!, wavs required!

	USAGE:
	hissy_fit
	--process_wav_to_wav	# move .wavs from here to proper place in tree
	./audio.cddb		# name the file that came from --rip_here

	USAGE:
	hissy_fit
	--8859			# default INPUT for .cddb opposite of --utf8
	--utf8			# input -avoid double encoding- always 8859 out
	--check_saved_cddb_file	# only needed when things arenty working
	./audio.cddb		# or any list of files

	USAGE:
	hissy_fit
	--process_wav_to_mp3	# make them .mp3's
	--lame_V=4		# edit to your needs
	cddb_recent/cddb*	# list of files

	USAGE OPTION:
	--utf8			# input is utf8 (output is 8859)
	--8859			# switch back if processing several files ?

	USAGE OPTION:
	--use_cdp_names		# default - need --use_grip_names_for_wav

	USAGE:
	files are processed when seen, so flags can change
	complex command line parsing, often results in complex command lines,
	which go into shell scripts. Python is itself a scripting language ...

FILENAMES:

	$HOME		# taken from UNIX env
	$HOME/audio/	# must already exist

	.../wav/artistname/albumname/audio.cddb		# PRESERVED ORIGINAL
	.../wav/artistname/albumname/audio.cdindex
	.../wav/artistname/albumname/audio_01.wav
	.../wav/artistname/albumname/audio_01.inf	# cdda2wav gives this

	.../mp3_V4/artistname/albumname/01_track_name.mp3
	.../mp3_V4/artistname/albumname/02_name_of_track.mp3

	.../mp3_V0/artistname/albumname/01_track_name.mp3
	.../mp3_V0/artistname/albumname/02_name_of_track.mp3

	.../wav/artistname/albumname/01_the_track_name.wav # grip style was once a good idea

	TODO:
	$HOME/audio/wav/BOXSETNAME/artistname/albumname/audio.cddb # TODO

8859:

	cddb uses 8859 file by default
	Thats OK (in a european environment),
	but cddb says it blindly allows UTF8 format files,
	there should be an indicator to say its UTF8.
	SUGGESTION: use emacs/vi/C heading line in a comment

	ID3 tags use UTF8, so everything is converted 8859->UTF8
	but if the input is already UTF8, (and you didnt know)
	you get double encoded text, which is not right.

	You can detect LIKELY selection of 8859/UTF8, and
	even automatically correct double encoding,
	as very few UTF8 bytes occur in normal 8859 text,
	but technically, its still guessing (and fails for
	non-8859, where the few become many)

