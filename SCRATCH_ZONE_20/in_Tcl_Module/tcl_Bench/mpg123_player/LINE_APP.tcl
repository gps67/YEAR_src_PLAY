#!/usr/bin/env tclsh

# /home/gps/YEAR/src/PLAY/SCRATCH_ZONE_20/in_Tcl_Module/tcl_menu/MENU

	lappend auto_path \
	[pwd] \
	[pwd]/MENU \
	;#

# LIBR uses a few namespaces
	namespace import ::MK_ID::mk*

# package require tcl_Bench # using MENU atm
	package require Tk



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
	set w1 {}
	EXIT_BUTTON $w1
	mk_text_out_global $w1 ;# uses h_pack tho
	wm_raise $w1

	set f1 $w1.f1
	frame $f1
	h_pack $f1

	set line1 $f1.line1
	set line2 $f1.line2
	global str_line1
	entry $line1 -textvariable str_line1
	entry $line2 -textvariable str_line1
	v_pack $line1
	v_pack $line2

	# usual Return not keypad Return #
	bind $line1 <Return> {
	#	text_out is PS not monospaced
	#	text_out_ln - "Return W==%W"
	#	text_out_ln - "str==$str"
		set str [%W get]
		global global_fd
		set t [puts $global_fd $str]
		flush $global_fd

		text_out_ln - "t '$t' IN: $str"
		%W delete 0 end
	} ;# OK
	# focus I-beam csr to first field # default is fuzzy no focus yet
	focus $line1

	btns_b_text_cmd $f1 - "CLOSE w" {
		global global_fd
		# half close fd, writing done, flushes through
		close $global_fd w
		# set global_fd {}
	}

	# ERRM # need to kill mpg321 # close w # meh

	foreach line {
		"LOAD file1.mp3" 
		"LOAD file2.mp3" 
		"PAUSE"
		"STOP"
		"QUIT"
		"GAIN 100"
		"GAIN 70"
		"GAIN 10"
		"JUMP +720"
		"JUMP -720"
		"F"
	} {
		set cmd_str "
			global global_fd
			puts \$global_fd {$line}
		"
		btns_b_text_cmd $f1 - $line $cmd_str
	}

	# RTFM open tcl COMMAND PIPELINE
	set cmd { tr [a-z] [A-Z] }
	set cmd { echo hello } 
	set cmd { sleep 2 } 
	set cmd { tty } ;# why does this fail ? 
	set cmd {false}
	set cmd {false2}
#	set cmd "echo $cmd"
	set cmd {tr_to_upper}
	set cmd {bash -c pwd}
	set cmd {bash -si} ;# needed -si # immediate EOF
	set cmd {sftp mobile 2222} ;# needed -si # immediate EOF
	set cmd {tclsh}
	set cmd { cat -}
	set cmd {mpg321 -R fake.mp3 }

	global global_fd
	set global_fd [open "| $cmd &" w+b]
	fconfigure $global_fd -blocking 0 
	fconfigure $global_fd -buffering none
	set pid [pid $global_fd]
	puts  "cmd $cmd"
	puts  "global_fd $global_fd"
	puts  "pid $pid"
	text_out_ln - "cmd $cmd"
	text_out_ln - "global_fd $global_fd"
	text_out_ln - "pid $pid"

	fileevent $global_fd readable {
		text_out_ln - "READABLE"
		# { CHECK 
		# on readable called when data is available
		# on readable called on eof
		# on readable called .. maybe thats it
		#
		# there are two functions to use, gets and read
		# gets is line based
		# read is block based
		# libr is api based
		# var # sess # STO # CALLER_PROVIDES # DECL mechanism
		#
		# HERE
		#
		#  PSG_DIALECT 
		#   PARSER
		#	...
		#    PARSE at_%s_parse_line( STR0 line )
		#	at_idx mpg321 -R fake.mp3 # BIND ALIAS INSTANCE
		# 	# implement as { { at_%s_%s %s %s } } {...}
		# LEX "{...}" # MATCH FILTER # FILTER_MATCH # ALIAS_ARGS == "FILTER_MATCH"
		# DECL ident_ANYSTR "{...}" # // ASIF from " { ... } ' // MATCHER
		#	 at_%s_%s %s %s  ...
		#	 at_%s_%s %s %s  at_XPOS.R("R") // AUTO_REPEAT // "R" // 
		#	 at_%s_%s %s %s  at_XPOS.%s("R") // AUTO_REPEAT // "R" // 
		#	 at_%s_%s %s %s  at_XPOS.%s("R") // AUTO_LINK_API // VAR
		#	 at_%s_%s %s %s  at_XPOS.DECL("{ APP_NAME MPG123 }"") 
		#	 at_%s_%s %s %s  SUBLEX "fake2.mp3"
		#	 at_%s_%s %s %s  SUBLEX "idx"
		#	 at_%s_%s %s %s  SUBLEX "idx_mpg321" idx_%s APPNAME mpg321
		# 	# implement as { { at_ %s_%s %s %s } } { ...}
		# 	# implement as { { at_ R %s %s } } {  SAME_LOCN LEX_STEP }
		# LEX_STEP == "SAME_LOCN" 
		#
		#  KNOW ID_STR0 == "R"
		#
		#	OPTIMISE KNOW ID_str == "R" || "Permit_ANYSTR"
		#	# RED PILL ANYSTR # %s with ANYSTR you can get away with
		#	# if in doubt, sto in ROM, and call { IDENT A_Z PLUS_PLUS }
		# PLUS_PLUS x%2x # A_Z # A B C ... # DIAGS #
		#	
		#	@R ...
		#
		#	PSG += CHAR_or_STR0 # ASSIGN MEANING TO [VAL]
		#	u8_at_line
		#	... MATCH CODE
		#	 at_%s_%s %s %s  at_XPOS.X("R") 
		#	 at_R "@R MPG123" # it volunteers its own PROG_NAME
		#    PARSE FIELD("MPG123") # MATCH HERE #
		#	fd_SESS { VAR FIELD } # PSG {} PARSER LEX SUBLEX
		#	at_I OUT
		#	 @I ID3:%s BYTE_ZONE_ID3_fields # RTFM
		# PSG_FOR "@I ID3 : { FM_MP3_ID3_fields ...}"
		#	
		# SUBLEX_COBOL_FIELDS_as_u8_idx # A_Z 
		#	
		# 	PARSE_LINE AT SINGLE_LETTER_NAME("I")
		#	SP(" ") # ALSO_MATCHES_TABS EOLN INDENT PAGE_LAYOUT
		#	APP_NAME("ID3")
		#	PUNCT ":" # AUTO_ALLOW_SP_AS_IF_NOSP_WS_PUNCT
		#	
		# ...EXPRS...
		#	at_R PARSE("@R ") usually single letter "R"
		#	 ar_R "@R %s" { APP_NAME "MPG123" } // LOGIN_CONNECT
		#	NEW SESS 
		#	 at_R "@R MPG123" # it volunteers its own PROG_NAME
		#	 at_I "@I ID3:%s" BYTE_ZONE_upto_EOLN # ID3_DECODER
		#	 at_I "@I ID3:%s" ID3_api BYTE_ZONE # PARSE EOLN #
		#	 at_S "@S 1.0 3 44100 Joint-Stereo 2 320 2 0 0 0 128 2"
		#	 at_P "@P 1" # EOLN_expected # ALL_DONE #
		#	 at_P "@P 2" # EOLN_expected # AUTO_REASONS_WHY_FAST_MATCH
		#	 at_P "@P 3" # EOLN #
		#	 at_P "@P 3 # EOLN # "
		#	 at_P "@P %s NEW_NEVER_SEEN_BEFORE_IDX_ALIAS
		#	 at_P "@P %s STARTUP_NEW_IDX_ALIAS
		#
		# LOADLIST %d idx # { %d pos_in_list } # 
		# LOADLIST %s url # { %s url } # 
		# EXPLAIN get PLAYLIST from URL # BASIC_API_GET #
		# API %s VAR_NAME more_specifically %s EXPR # CMNT
		# PSG collects LEX NOUNS ALIAS SCRIPT 
		# TOKENISER STR0 "url" # VAR_NAME #
		# VAR `mk_VAR { VAR RET_VAL }`
		# `{ VAR RET_VAL }` # REDUCING MID LINES # FADE BACK_QUOTES
		# { VAR RET_VAL } # AUTO_ALIAS VIEW_with_BACKQUOTES #
		#
		# { %s %s } # %s # more
		# { OPCODE_VAR _NAME %s } { STR0 %s } // MENTION ANYWHERE in MODULE
		# { OPCODE_ARG _NAME %s } { STR0 %s } // ALTERNATE MENTION
		# { OPCODE_OBJ _ALLOC_ %s } { STR0 %s } // NULL means "ITEM_idx"
		# // non NULL gets PARSED // empty_means_empty //
		#
		# { VAR ARG_ONE }
		# { VAR ARG_TWO }
		# { VAR ARG_EXPR }
		# { API VAR_as_SIGNAL_as_VAR }
		# 
		# PARSE MPG321 #
		#
		#	LIBR
		#	CODE
		#	DATA near CODE
		#	DATA near ROM
		#	DATA near ROM INIT DATA_VALUE # in _utf
		#	DATA near LOGIN
		#
		# PARSE at_lines
		#
		#	"@%s %s"
		#	BOLN AT LETTER "@%s %s" LETTER 
		#
		#
		#
	#	if {[gets $global_fd line] >= 0} {}
	#	set OUT [read $global_fd 4000] ;# also ERR ?
		#
		# CHECK } 

		if {[gets $global_fd OUT] > 0} {
		#	text_out_ln - "{OUT} $OUT"
			text_out_ln -       "$OUT"
			PARSE_at_line $OUT
			set OUT "OUT: $OUT"
		} else {
			if {[ eof $global_fd ]} {
				text_out - {## EOF TODO WAIT LESS ##}
				text_out_ln - {<<ADDED EOLN>>}
				update idletasks
			#	fileevent AUTO on close
			#	fileevent $global_fd readable {}
				set t [close $global_fd]
				set $global_fd {}
			}
			# RTFM says an incomplete line returns -1
			text_out_ln - "read returned {$OUT} close returned '$t'"
		}
	#       update idletasks
	}

# tty exited, this faulted #
# write after close # possible
#	puts $global_fd "# FIRST LINE" ; flush $global_fd


#	set w2 .w2
#	toplevel $w2
#	raise $w2
#	EXIT_BUTTON $w2
#	tree_walk::build_three $w2
#	tree_walk::set_D1 $D1

}
RUN_MAIN

