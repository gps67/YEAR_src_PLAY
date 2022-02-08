#!/usr/bin/env tclsh

	lappend auto_path \
	[pwd] \
	[pwd]/MENU \
	;#

# /home/gps/YEAR/src/PLAY/SCRATCH_ZONE_20/in_Tcl_Module/tcl_Bench/lib/

# package require tcl_Bench # using MENU atm
	package require Tk

# [pwd] # == ... # ${HOME}/audio/mp3_V4/.

	set dir_home		$env(HOME)
	set dir_audio		$dir_home/audio
	set dir_audio_mp3_V4	$dir_audio/mp3_V4
	set dir_audio_wav	$dir_audio/mp3_wav
	set dir_audio_cddb	$dir_audio/cddb

# [pwd] # == album_avail

	set d_artist the_creole_choir_of_cuba
	set d_album santiman

	set d_artist rodrigo_y_gabriela
	set d_album 11_11

# [pwd] local layout # where .mp3 files keps # or .wavs # etc #
# parse $var in path/naame.ext # optional ${var} 
# Layout.MACRO recalc(dir_album) # move to CACHE of layout macro

	set dir_album $dir_audio_mp3_V4/$d_artist/$d_album

# . / steps / . # .mp3 # .vfs media_file.mp3 and nearby README.html
# # local Layout 

# ALBUM_dir # TRACK_FILE.mp3 # .cddb list

# sorted list L_ of files, filter FILENAME.mp3 # maybe other media run_pkg
# NOTE hissy_fit lists %2.2d_%s.mp3 track_no track_filename 
# NOTE sort_order track_no track_filename .mp3 PLUS

	set L_files [glob $dir_album/*.mp3 ]
	set L_files [glob -tails -directory $dir_album *.mp3 ]
	set L_files [lsort $L_files]

# optional 

	EXIT_BUTTON

# text_out panel # only per node
# shared, tagged, syslog, screen_view .text_out and .scroll
# MENU/text_out.tcl

	mk_text_out_global ;# uses h_pack tho

# refresh this every REPL

	puts $L_files

	text_out_ln - {}
	text_out_ln - $dir_album
	text_out_ln - {}
	foreach file $L_files {
		text_out - "\t"
		text_out_ln - $file
	}



