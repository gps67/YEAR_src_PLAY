#!/usr/bin/env tclsh

# /home/gps/YEAR/src/PLAY/SCRATCH_ZONE_20/in_Tcl_Module/tcl_menu/MENU

	lappend auto_path \
	[pwd] \
	[pwd]/MENU \
	;#

# LIBR uses a few namespaces
	namespace import ::MK_ID::mk*
	namespace import ::fd_mpg123::*

	# namespace tree_walk
	# NB # in mk_btns $cmd # cmd needs either import or absolute NS::func
	# wanted to be able to forget current NS name # somehow #
	# eval $eval_ctxt ; 

# package require tcl_Bench # using MENU atm
	package require Tk

# namespace for triptic compound widget

namespace eval tree_walk {

	variable D1 {}

	variable btns_artists
	variable btns_albums
	variable btns_tracks
	variable btns_play ;# its actually a PANEL where buttons might go

	variable single_mpg123_fd

 proc build_three {w2} { # 3 panels # data later

	variable single_mpg123_fd
	set varname "::fd_mpg123::global_fd"
	set varname "::tree_walk::single_mpg123_fd"
	set $varname [::fd_mpg123::mpg123_open]

	# either this or than not both tho
	variable btns_artists
	variable btns_albums
	variable btns_tracks
	variable btns_play ;# must be imported to this function

	set btns_artists $w2.btns_artists
	frame $btns_artists
	h_pack $btns_artists

	set btns_albums $w2.btns_albums
	frame $btns_albums
	h_pack $btns_albums

	set btns_tracks $w2.btns_tracks
	frame $btns_tracks
	h_pack $btns_tracks

	set btns_play $w2.btns_play
	frame $btns_play
	h_pack $btns_play

 }

 # DICT # TODO #
 # ( OVER D2 idx )
 # ./.tcl.data_per_dir.tdb
 # KEY_PER_APP { zone } ;# zone is k1 v1 k2 v2 ;#
 #
 # ./.tcl.sqlite_data_per_dir .tdb .sqlite3 .MMAP .UDEF_elf
 # SEGMENT # NBYTES # MIME_TYPE ELF_SEGMENT 
 # LOADER # LINK_XPOS IDX OFFS SEGMENTS NAMES VAR_POOL LANG_LOADER
 # LOAD Python SEGMENT
 # LOAD Tcl SEGMENT ROM CACHED NBYTES from ROM OFFS NBYTES // COPY DATA // N_Bytes // N_bytes_BOTH //
 # BOTH _nbytes _spec_name _spec API UDEF
 # UDEF # DECL name spec # DATA # nbytes # SPEC_t & SPEC # run_as_this
 # UDEF # DECL name spec # DATA # nbytes # SPEC_t * SPEC # run_as_this
 # UDEF provides SPEC for SEGMENT via elf_api
 # ELF # LOAD_SHARED_OBJECT # RELOCATED RESOLVE _on_LOAD( API_event )
 # API # SCRIPT provides API_event as XPOS
 # XPOS # FILE CPOS #
 # XPOS # FILE XPOS #
 # XPOS # FILE EA_EXPR # EA_t & EA = EVAL EXPR with CTXT via FILTER[s]
 # XPOS Y_LINE X_CPOS # ZERO is BOLN # X_XPOS # BYTE_POS = EVAL FILTER_FUNC XPOS
 # XPOS XPOS # Once upon a time, everthing was one byte, one XPOS now char_bytes
 # XPOS -SELF- # ie byte_pos is not char_pos #
 # SPEC # MATCH # ACTIVATE MANAGE CONTROL VAR_POOL of UDEF_TYPE_SPEC _n_STO
 # STO # nbytes in sqlite3 blob with DECODE_elf_SEGMENT _as_SQLITE_FILE
 # STO # N BYTES ALLOC
 # ALLOC # obj_hold<STO_NBYTES_see_SPEC>
 # sqlite3 select item where PAIRS EXPR CXX_SCRIPT
 # CXX_ARGV # FORK TRACK PID CXX ARGV #
 # CXX_ARGV # FORK TRACK PID CXX ARGV # LIBR # LOAD # DECL #
 # CXX_ARGV_ELF # FORK TRACK PID CXX ARGV # LIBR # LOAD # DECL # SPEC #
 # CXX_ARGV_ELF # FORK TRACK PID CXX ARGV # LIBR # LOAD # DECL # SPEC # SCRIPT #
 # CXX_ARGV_ELF # ALIAS ANYSTR # IDENT was CIDENT now ANYSTR # MATCH ANYSTR BYTES NBYTES nested_nbytes_SCRIPTED_DATA

 proc set_D1 { D1 } { # TREE ROOT 
 	# globs a tree to 3 levels aurtis/album/filename
	# needs dirname_when_not_filename
	# needs ext_mime_empty when filename

	# TODO keep idx_pos_in_LIST of selected subdir || file || ___
	# TODO incr idx_pos over LIST then maybe next dir ??
	# TODO event mechanisms

	# D1 is $dir_audio_mp3_V4 / but could also be ...
	#       $dir_audio_wav/
	#
	set tree_walk::D1 $D1

	set LIST [glob -tails -directory $D1 * ]
	set LIST [lsort $LIST]
	set LIST [lrange $LIST 0 39] ;# NEED SCROLLABLE btns
	foreach d2 $LIST {
		# test $item is DIR
		set DIR $D1/$d2
		set cmd [list tree_walk::set_D1_d2 $D1 $d2]
		btns_b_text_cmd $tree_walk::btns_artists [mk_id] $d2 $cmd
	}
	set_D1_d2 $D1 [lindex $LIST 0]
 }

 proc set_D1_d2 { D1 d2 } { # ROOT artist
	# puts "set_D1_d2 $D1 $d2"

	forget_all_packed_content $tree_walk::btns_albums

	set DIR $D1/$d2
	set LIST [glob -tails -directory $DIR * ]
	set LIST [lsort $LIST]
	foreach d3 $LIST {
		set cmd [list tree_walk::set_D1_d2_d3 $D1 $d2 $d3]
		btns_b_text_cmd $tree_walk::btns_albums [mk_id] $d3 $cmd
	}
	set_D1_d2_d3 $D1 $d2 [lindex $LIST 0]
 }

 proc set_D1_d2_d3 { D1 d2 d3 } { # ROOT artist album
	# puts "set_D1_d2_d3 $D1 $d2 $d3"

	forget_all_packed_content $tree_walk::btns_tracks

	set DIR $D1/$d2/$d3
	set LIST [glob -tails -directory $DIR * ]
	set LIST [lsort $LIST]
	set d4_first [lindex $LIST 0]

	foreach d4 $LIST {
		set cmd [list tree_walk::play_track $D1 $d2 $d3 $d4]
		btns_b_text_cmd $tree_walk::btns_tracks [mk_id] $d4 $cmd
	}
	tree_walk::play_track $D1 $d2 $d3 $d4_first
 }

 proc play_track { D1 d2 d3 d4 } { # ROOT artist album track
	# we have to get $fd from somewhere
	variable single_mpg123_fd
	set mpg123_fd  $single_mpg123_fd

	set filename [file join $D1 $d2 $d3 $d4]
	set cmd [list fd_mpg123::play_track $mpg123_fd $filename]
	btns_b_text_cmd $tree_walk::btns_play [mk_id] $d4 $cmd
 }


} ;# namespace eval

####################################

proc RUN_MAIN {} {
	# globals 
	upvar env env

	set dir_home		$env(HOME)
	set dir_audio		$dir_home/audio
	set dir_audio_mp3_V4	$dir_audio/mp3_V4

	set dir_audio_wav	$dir_audio/mp3_wav
	set dir_audio_cddb	$dir_audio/cddb

	# triptic is specific to my layout from  dir_audio_mp3_V4
	# a wider scheme would { KNOW dir_is_subdir_subdir }
	# a wider scheme would { KNOW dir_is_subdir_artists }
	# a wider scheme would { KNOW dir_is_artist_albums }
	# a wider scheme would { KNOW dir_is_album_tracks }
	# a wider scheme would { KNOW dir_is_bunch_of_DVD_iso } // video
	# a wider scheme would { KNOW dir_is_bunch_of_DVD_iso } // FS_BOOT
	# scan tree find tracks, work backwards, leave ATTR dot_is_dirtype X
	# merge multiple dir usages # eg notes about album lyrics scrapbook
	#
	set D1 $dir_audio_mp3_V4

	# the main app window, and text_out panel
	# . already exists
	set w1 .

	EXIT_BUTTON $w1

	mk_text_out_global $w1 ;# uses h_pack tho
	wm_raise $w1

	# $w1 is the logfile
	# $w2 is the tree_menu and play buttons

	# the 3-panel ARTISTS   ALBUMS   TRACKS
	# 4th panel BUTTONS
	set w2 .w2
	toplevel $w2
	raise $w2

	EXIT_BUTTON $w2

	tree_walk::build_three $w2	;# widgets
	tree_walk::set_D1 $D1		;# D1 artist albums tracks into widgets

}
RUN_MAIN

