#! /usr/bin/env tclsh9.0

proc make_png_from_txt { file_png file_txt } {

#	puts "make_png_from_txt $file_png $file_txt "

	exec </dev/tty >/dev/tty 2>/dev/tty ask_do txt_2_png/txt_2_png.sh $file_txt 
	
}

set list [glob 2*.txt]
set list [lsort $list]
foreach file_txt $list {

	# this wont make sense if it isn't png, eg jpg
	set file_png [regsub .txt $file_txt .png]
	# get mtime of file_txt
	set t1 [file mtime $file_txt ]
	# s1 unused # for debugging
	set s1 [clock format $t1 -format { %Y %m %d %H %M %S } ]
	set s2 {UNSET}

	set need_make 0

 #	puts "$file_txt $s1 ;# NO PNG"

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
