#!/usr/bin/env tclsh
# record20220522135748.3gpp
# recordYYYYMMDDHHMMSS.3gpp

# BUG # _ASTR # when already had different name
# BUG # _ASTR .mp4 => .jpg # when already had different name # slightly more serious
# BUG # SS == 00 #
#
# so LOSE multiple photos taken in same minute

# BUG # .ext #

puts "ASTR and lose MP4 to JP"
# fn_ask_do exit

set dir_default "/currently/unused/"
set dir_default .

# all these photos were taken at an  event
# event_name ZONE_NAME
set ZONE_NAME WOODEN_ROOTS
set ZONE_NAME WOMAD
set ZONE_NAME HCTS
set ZONE_NAME Djemboree
set ZONE_NAME DRUM_CAMP
set ZONE_NAME TANKATA
set ZONE_NAME HansCamp
set ZONE_NAME MoGueyeSalisbury_Jan
set ZONE_NAME IyaSako_Salisbury

set ZONE_NAME Iya_Sako_Salisbury
set ZONE_NAME border_line
set ZONE_NAME BH6
set ZONE_NAME RSAC_NYE
set ZONE_NAME XMAS_Wed
set ZONE_NAME Tue_BH6
set ZONE_NAME Wed_BH6
set ZONE_NAME Mon_EVE
set ZONE_NAME RSAC_WITS
set ZONE_NAME Babara_Bangura
set ZONE_NAME BH6_Christchurch



proc list_pop {listname itemname } {
	upvar $listname list
	upvar $itemname item
	if { $list == {} } {
		return 0
	}
	set item [lindex $list 0]
	set list [lrange $list 1 end]
	return 1
}

# push might use VAL or by_VAR # I like by_VAR except slower

proc my_main {list} {
	global ZONE_NAME
	set seen_one 0
	set name_one 0
	while {[ list_pop list item ]} {
		puts "# item # $item # list '$list' #"
		if { $item == "--NAME" } {
			list_pop list ZONE_NAME
			set name_one 1
			puts "# ZONE_NAME # $ZONE_NAME #"
			continue
		}
		if {!$name_one} {
			puts "# ERROR # I really expect --NAME zonename"
			return 
		}
		set seen_one 1
		rename_IMG_dir $item
	}
	if {!$seen_one} {
		rename_IMG_dir .
	}
	return
}



# BASH # . fns_EDIT
# BASH # fn_EDIT_var ZONE_NAME


puts ". fns_ASKS"

proc list_dir {dir} {
	puts "# dir $dir"
	set L1 [glob -tails -directory $dir *]
	set L1 [lsort $L1]
}

proc Tue_of_YEAR_MM_DD {YEAR MM DD} {
	set t1 [clock scan "$YEAR-$MM-$DD"]
 	set Jan [clock format $t1 -format "%b"]
	set Tue [clock format $t1 -format "%a"]
	return $Tue
}

proc Jan_of_YEAR_MM_DD {YEAR MM DD} {
	set t1 [clock scan "$YEAR-$MM-$DD"]
 	set Jan [clock format $t1 -format "%b"]
	set Tue [clock format $t1 -format "%a"]
	return $Jan
}

proc ask_do args {
	set cmd ask_do
	set cmd say_do
	foreach arg $args {
		lappend cmd $arg
	}
	# puts "# { $cmd } #"
	puts "$cmd"
}

proc macro_offer_rename {} {
 uplevel {
	if { $ext == "3gpp" } {
		if { $VID == "AUD" } {
		} else {
			puts "# WANT # set VID AUD # SUGGEST # manual fix"
			# AUD_2022-05-21_Sat_1029_11.3gpp: #
			# ISO Media, MPEG v4 system, 3GPP #

		}
	}
 	ask_do mv $f1 $f2
 }
}

proc macro_offer_set_date {} {
 uplevel {
 	set STAMP "$YEAR$MM$DD$hh$mm.$ss"
 	ask_do touch -t $STAMP $f1 
 }
}

proc macro_calc_name_XY {} {
 uplevel {
 	set NAME1 $NAME
	set NAME {}
# 	append  NAME $MS_XY "_" $NAME1
 	append  NAME $NAME1 "_" $MS_XY
	# TODO # ontain time of day from file mtime use filename for YEARMMDD
	macro_calc_name_one 
	macro_rewrite_ext 
 	set NAME $NAME1
 }
}

proc macro_calc_name_one {} {
 uplevel {
		macro_rewrite_ext 
	  	set YEAR_MM_DD "$YEAR-$MM-$DD"
	  #	set hh_mm_ss "$hh-$mm-$ss"
	  	set hh_mm_ss [format %s%s_%s $hh $mm $ss]
		# precision is fragile # %s -vs- %S # %s == time_t t
		set fmt_clock_scan "%Y-%m-%d %H %M %S"
		set val_clock "$YEAR-$MM-$DD $hh $mm $ss"
		set time [clock scan $val_clock -format $fmt_clock_scan ]
		set Tue [clock format $time -format "%a"]
		set time_str [clock format $time]
	#	set Tue [ Tue_of_YEAR_MM_DD $YEAR $MM $DD]
		if { $ASTR != {} } {
			set ASTR "_$ASTR"
		}
		set f2 [format "%s_%s_%s_%s%s.%s" $YEAR_MM_DD $Tue $hh_mm_ss $NAME $ASTR $ext]
		set f2 "$dir/$f2"
	if 0 {	puts "# val_clock $val_clock"
		puts "# time_str $time_str" 
		puts "# f2 $f2" 
		puts "# ext $ext" 
	}
 }
}

proc macro_date_from_unit_time { timestamp } {
	uplevel "
	foreach { YEAR MM DD hh mm ss } {[
	 clock format $timestamp -format { %Y %m %d %H %M %S }
	 ]} {}
	"
	macro_know_date_came_from_mtime
}

proc macro_know_date_from_stat_file { filename } {
 uplevel {
	file stat } $filename { stat_field
	#  atime, ctime, dev, gid, ino, mode, mtime, nlink, size, type, uid. 
	set mtime $stat_field(mtime)
	set fmt { %Y %m %d %H %M %S  }
	set list [ clock format $stat_field(mtime) -format $fmt ]
	foreach { YEAR MM DD hh mm ss } $list {}
	macro_know_date_came_from_mtime
 }
}

proc macro_know_date_came_from_filename {} {
 uplevel {
 	set date_came_from_filename 1
 	set date_came_from_mtime 0
 }
}

proc macro_know_date_came_from_mtime {} {
 uplevel {
 	set date_came_from_filename 0
 	set date_came_from_mtime 1
 }
}

proc rewrite_ext { ext } {
	return [ string tolower $ext ]
	switch -exact $ext {
	 WAV { set ext wav }
	}
	return $ext
}

proc macro_rewrite_ext {} {
	uplevel {
		set ext [rewrite_ext $ext]
	}
}

proc rename_IMG_dir {dir} {
	global NAME

	set L1 [list_dir $dir]

	# each FIELD has (FIELD) # OK if no collect used

	set D {[0-9]}
	set DD "($D$D)"
	set DDD "($D$D$D)"
	set DDDD "($D$D$D$D)"
	set re_D13 "($D{10,15})" ;# 13 DIGITS
	set re_ext "(.*)"
	set re_tue "(...)"
	set re_MSXY {([MSXY][MSXY])}
	set re_VID_IMG {([VIDIMG][VIDIMG][VIDIMG])[-_]}
	set IMG_ {IMG[-_]}


	
	# A_STR added at the end just before dot_ext
	append re_ASTR \
		"\[_-\]?" \
		"(\[^.\]*)"

	append re_dot_ext "\\." $re_ext

	append re_ASTR_dot_ext \
		$re_ASTR \
		$re_dot_ext 

	append re_HDR "(_HDR)?"

	append re_zero \
		VID_ $DDDD \
		"-"  $DD \
		"-"  $DD  \
		"_"  $DD $DD \
		"_"  $DD \
		$re_ASTR_dot_ext
	append re_one \
		record $DDDD $DD $DD "_"  $DD $DD $DD $re_ASTR_dot_ext
	append re_two \
		$re_VID_IMG $DDDD $DD $DD  "_"  $DD $DD $DD $re_ASTR_dot_ext
	append re_two_ \
		VID_ $DDDD $DD $DD  "_"  $DD $DD $DD $re_ASTR_dot_ext
	append re_ZOOM_03d_WAV \
		ZOOM $DDDD $re_ASTR_dot_ext
		# happens on ZOOM1234.WAV H2N audio recorder with .WAV

	append re_VID_YYYYMMDD_HHMMSSXX \
		VID_ $DDDD $DD $DD "_"  $DD $DD $DD $DDD $re_ASTR_dot_ext
	append re_IMG_YYYYMMDD_HHMMSSXX \
		$IMG_ $DDDD $DD $DD "_"  $DD $DD $DD $DDD $re_HDR $re_ASTR_dot_ext
	
	append re_myrec \
		myrec $re_D13 $re_ASTR_dot_ext

# myrec1663179554257.mp3
#      1234567890123.mp3
	
	# probably an iphone or something
	# VID-20230410-WA0038.mp4

	# or maybe WhatsApp date is date of phone fetch
	# which could be miles away from when shot

	append re_VID_YYYYMMDD_WA4D \
		VID- $DDDD $DD $DD "-"  WA $DDDD $re_ASTR_dot_ext
	append re_IMG_YYYYMMDD_WA4D \
		IMG- $DDDD $DD $DD "-"  WA $DDDD $re_ASTR_dot_ext
	
	# probably an iphone or something
	# VID-20230410-WA0038.mp4

	append re_SR_3D_MSZY_WAV \
		SR $DDD $re_MSXY $re_ASTR_dot_ext
	
	append re_NEWNAME \
		$DDDD "-" $DD "-" $DD \
		"_"  ".*" "_"  \
		$DD $DD "_" "\[^.\]*" $re_dot_ext
		

	# puts "# re_one == {$re_one}"

	# 2024_08_04_10_16_46.m4a
	append re_Voice \
		     $DDDD \
		"_"  $DD \
		"_"  $DD  \
		\
		"_"  $DD  \
		"_"  $DD  \
		"_"  $DD  \
		$re_ASTR_dot_ext

	global ZONE_NAME
	foreach f $L1 {
	  macro_know_date_came_from_filename ;# INT default
	  set NAME $ZONE_NAME
	  set VID VID
	  set ss 00
	  set f1 "$dir/$f"
	  # INIT M1 1999-12-31 23:59.00 ASTR ext
	  foreach {YEAR MM DD hh mm ss ASTR ext} { 1999 12 31 23 59 00 aztr JPG } {}
	  # this is where ext gets a dummy value # not overwritten #

	  if {! [file isfile $f1]} {
	  	puts "# NOT # $f1"
		continue

	  } elseif [regexp $re_zero $f all YEAR MM DD hh mm ss ASTR ext] {
	 #	macro_know_date_came_from_filename ;# DONE in LOOP init
		continue

	  } elseif [regexp $re_one $f all YEAR MM DD hh mm ss ASTR ext] {
	 #	macro_know_date_came_from_filename ;# DONE in LOOP init
	  	# macro_calc_name_one
		# record # means audio #
		if { $ext == "3gpp" } {
			set VID AUD
		} else {
			puts "# record BUT ext == $ext # $VID AUD or VID #"
		}

	  } elseif [regexp $re_two $f all VIDIMG YEAR MM DD hh mm ss ASTR ext] {
	  	puts "# re_two # $f"
	 #	macro_know_date_came_from_filename ;# DONE in LOOP init

	  } elseif [regexp $re_ZOOM_03d_WAV $f all DDD ASTR ext] {
	  	# DATE to come from MTIME not filename
		if { $ext == "WAV" } {
			set VID AUD
		} else {
			puts "# ZOOM BUT ext == $ext # $VID AUD or VID #"
		}
		puts "# ext is '$ext'"
		# need to STAT file to get YEAR MM DD
		macro_know_date_from_stat_file $f1
	  	# DATE came from MTIME not from filename # done by stat above

	  } elseif [regexp $re_two $f all VID_IMG YEAR MM DD hh mm ss ASTR ext] {
	  	# macro_calc_name_one

	  } elseif [regexp $re_VID_YYYYMMDD_HHMMSSXX $f all YEAR MM DD hh mm ss XXX ASTR ext] {
	 #	macro_know_date_came_from_filename ;# DONE in LOOP init
	  } elseif [regexp $re_IMG_YYYYMMDD_HHMMSSXX $f all YEAR MM DD hh mm ss XXX HDR ASTR ext] {
	 #	macro_know_date_came_from_filename ;# DONE in LOOP init

	  } elseif [regexp $re_VID_YYYYMMDD_WA4D $f all YEAR MM DD d4 ASTR ext] {
	  	append NAME "_" $d4
	 #	macro_know_date_came_from_filename ;# DONE in LOOP init
	  } elseif [regexp $re_IMG_YYYYMMDD_WA4D $f all YEAR MM DD d4 ASTR ext] {
	  	append NAME "_" $d4
	 #	macro_know_date_came_from_filename ;# DONE in LOOP init

	  } elseif [regexp $re_SR_3D_MSZY_WAV $f all D3 MS_XY ASTR ext] {
		macro_know_date_from_stat_file $f1
		macro_calc_name_XY
		macro_offer_rename
		continue

	  } elseif [regexp $re_myrec $f all D13 ext] {
		set F 1000
		set T_1000 [expr $D13 / $F]
		macro_date_from_unit_time $T_1000
	        macro_calc_name_one
	        macro_offer_set_date ;# before rename
	        macro_offer_rename
		continue

	  } elseif [regexp $re_Voice $f all YEAR MM DD hh mm ss ASTR ext] {
		puts "# $YEAR $MM $DD $hh $mm $ss $ASTR $ext"
		# without any local macro the outer loop presumes ... OK

	  } elseif [regexp $re_NEWNAME $f all YEAR MM DD hh mm ] {
		puts "# ALREADY # $f"
		# continue


	  } else {
	  	puts "# NO # $f"
	  	puts "# NO # $f1"
		continue
	  }

	  # not continue
	  macro_calc_name_one
#	  macro_offer_set_date ;# before rename
	  macro_offer_rename
	  continue
	}
}

#if {$argc != 1} {
#	puts "# expected argc 1 got $argc"
#	rename_IMG_dir $dir_default
#	exit
#}

# rename_IMG_dir [lindex "$argv" 0]
my_main $argv
