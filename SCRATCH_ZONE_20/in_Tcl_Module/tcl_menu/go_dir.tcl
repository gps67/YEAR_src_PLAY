#!/usr/bin/env tclsh

# /home/gps/YEAR/src/PLAY/SCRATCH_ZONE_20/in_Tcl_Module/tcl_menu/MENU

	lappend auto_path \
	[pwd] \
	[pwd]/MENU \
	;#

# package require tcl_Bench # using MENU atm
	package require Tk

# [pwd] # == ... # ${HOME}/audio/mp3_V4/.

	set dir_home		$env(HOME)
	set dir_audio		$dir_home/audio
	set dir_audio_mp3_V4	$dir_audio/mp3_V4
	set dir_audio_wav	$dir_audio/mp3_wav
	set dir_audio_cddb	$dir_audio/cddb

#	TODO # generic dir browser # specific recognised tree ~/audio/
#	overlay known map with find_as_found
#	VIEW customised for known dirs
#	UDEF TREE # GRAFT_at_CSR_ROOT # TREE_TYPE #
#	NOTICE CSR is on TREE # app starts there # 

#
#
#	set L_dirs [glob $dir_audio_mp3_V4/* ]
	set L_dirs [glob -tails -directory $dir_audio_mp3_V4 * ]
##	set L_dirs [glob -tails -directory $dir_audio_mp3_V4  *.mp3 ]
	set L_dirs [lsort $L_dirs]
#
#
#

	set D1 $dir_audio_mp3_V4
	set D2 $D1/_artist_
	set D3 $D2/album

####################################

	namespace import ::MK_ID::mk*

	set w1 .{}
	# $w1.a # .a

	EXIT_BUTTON

	mk_text_out_global ;# uses h_pack tho
	wm_raise .

	set w2 .w2
	toplevel $w2
	raise $w2

	set btns $w2.btns
	frame $btns
	h_pack $btns

	btns_b_text_cmd_Exit $btns

proc set_D1 { D1 } {
}

proc set_D1_d2 { D1 d2 } {
	puts "set_D1_d2 $D1 $d2"
}

proc set_D2_d3 { D2 d3 } {
	puts "set_D2_d3 $D2 $d3"
}


	set D1_first 1
	set D2_first 1
	set D3_first 1

	set L_D1 [glob -tails -directory $D1 * ]
	set L_D1 [lsort $L_D1]
	foreach d2 $L_D1 {
		# test $item is DIR
		set D2 $D1/$d2
		btns_b_text_cmd $btns [mk_id] $d2 {
			set_D1_d2 $D1 $d2
		}
		if { $D1_first } {
			set D1_first 0
			set L_D2 [glob -tails -directory $D2 * ]
			set L_D2 [lsort $L_D2]

			foreach d3 $L_D2 {
				# test $item is DIR
				set D3 $D2/$d3
				btns_b_text_cmd $btns [mk_id] $d3 {
					set_D2_d3 $D2 $d3
				}
				if { $D2_first } {
					set D2_first 0
					set L_D3 [glob -tails -directory $D3 * ]
					set L_D3 [lsort $L_D3]

					foreach d3 $L_D3 {
						# test $item is DIR
						set D3 $D1/$d2
						btns_b_text_cmd $btns [mk_id] $d3 {
							set_D1_d2 $D2 $d3
						}
						if { $D3_first } {
							set D3_first 0
							set L_D3 [glob -tails -directory $D3 * ]
							set L_D3 [lsort $L_D3]
						}
					}

				}
			}
		}

	}



	set menu_l1 .l1
	frame $menu_l1
	pack $menu_l1

	text_out_ln - {}
	text_out_ln - $dir_audio_mp3_V4
	text_out_ln - {}

if 0 {
	foreach file $L_D1 {
		text_out - "\t"
		text_out_ln - $file
	}

}


