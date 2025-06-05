#! /usr/bin/env tclsh9.0

proc make_png_from_txt { file_png file_txt } {

	puts "# TCL # make_png_from_txt $file_png $file_txt "

	exec </dev/tty >/dev/tty 2>/dev/tty ask_do ../txt_2_png/txt_2_png.sh $file_txt 
	
}

# ARGV_ because intention is in_SITU edit #
# EDIT_ARGV_ITEM # SPLICE OLDVAL_new_val # like GIT_SWITCH
proc ARGV_edit_file_ext { file_ext ext_old ext_new } {
	# WARN # will subst .txt ANYWHERE in filename #
	# WANT add $ to expr .$ext_old
	regsub .$ext_old $file_ext .$ext_new ;# RET_VAL #
}

set list [glob *.txt]	;# list all .ext_old files on VFS_DOT
set list [lsort $list]  ;# sort list for sanity
foreach file_txt $list { ;# foreach file_txt in list 

	# TODO # avar_edit_filename_ext_1_to_ext_2 file_ext 
	# ARGV edit_file_ext ext_old ext_new
	# this wont make sense if it isn't png, eg jpg
	#
	set file_png [ ARGV_edit_file_ext $file_txt txt png  ]
# OLD #	set file_png [regsub .txt $file_txt .png]

	# get mtime of file_txt
	set t1 [file mtime $file_txt ]
	# s1 unused # for debugging
	set s1 [clock format $t1 -format { %Y %m %d %H %M %S } ]
	set s2 {UNSET}

	set need_make 0

 #	puts "$file_txt $s1 ;# NO PNG"

	# set t1 [file mtime $file_txt ]
	# got t1 = mtime of file_txt
	# get t2 = mtime of file_png
	if {[catch { file mtime $file_png } t2  ] } {
		# file mtime threw # eg no such file (yet)
		# catch returned TRUE did catch
		set need_make 1

	} { 
		# else # catch returned FALSE # no catch was required
		# so compare t1 and t2
		# maybe already up to date

		# s2 unused # for debugging # for information
		set s2 [clock format $t2 -format { %Y %m %d %H %M %S } ]

		# compare the 2 timestamps, do we need to re-make the png
		if { $t1 > $t2 } {
			set need_make 1
		}
	} 

	if $need_make {
		make_png_from_txt $file_png $file_txt 
	} else {
		# reassuring line for skipped files
		puts "# OK # $file_png 	# $s2"
	}
}
