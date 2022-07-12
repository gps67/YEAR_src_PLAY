#!/usr/bin/env tclsh
# record20220522135748.3gpp
# recordYYYYMMDDHHMMSS.3gpp

set dir_default Iya_Salisbury

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
	foreach arg $args {
		lappend cmd $arg
	}
	# puts "# { $cmd } #"
	puts "$cmd"
}

proc macro_offer_rename {} {
 uplevel {
 	ask_do mv $f1 $f2
 }
}

proc macro_offer_set_date {} {
 uplevel {
 	set STAMP "$YEAR$MM$DD$hh$mm.$ss"
 	ask_do touch -t $STAMP $f1 
 }
}

proc macro_calc_name_one {} {
 uplevel {
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
		set f2 [format "%s_%s_%s_%s.%s" VID $YEAR_MM_DD $Tue $hh_mm_ss $ext]
		set f2 "$dir/$f2"
	if 0 {	puts "# val_clock $val_clock"
		puts "# time_str $time_str" }
		puts "# f2 $f2" 
		puts "# ext $ext" 
 }
}

proc rename_IMG_main {dir} {
	set L1 [list_dir $dir]
	set D {[0-9]}
	set DD "($D$D)"
	set DDDD "($D$D$D$D)"
	set re_ext "(.*)"

	append re_zero VID_ $DDDD "-" $DD "-"  $DD  "_"  $DD $DD "_"  $DD "\." $re_ext
	append re_one record $DDDD $DD $DD    $DD $DD $DD "\." $re_ext
	append re_two VID_ $DDDD $DD $DD  "_"  $DD $DD $DD "\." $re_ext

	# end use $DD

	puts "# re_one == {$re_one}"

	foreach f $L1 {
	  set f1 "$dir/$f"
	  foreach {YEAR MM DD hh mm ss ext} { 1999 12 31 23 59 00 ext } {}
	  if {! [file isfile $f1]} {
	  	puts "# NOT # $f1"
		continue
	  } elseif [regexp $re_zero $f all YEAR MM DD hh mm ss ext] {
	  	# macro_calc_name_one
	  } elseif [regexp $re_one $f all YEAR MM DD hh mm ss ext] {
	  	# macro_calc_name_one
	  } elseif [regexp $re_two $f all YEAR MM DD hh mm ss ext] {
	  	# macro_calc_name_one
	  } else {
	  	puts "# NO # $f1"
		continue
	  }
	  # not continue
	  macro_calc_name_one
	  macro_offer_set_date ;# before rename
	  macro_offer_rename
	}
}

if {$argc != 1} {
	puts "# expected argc 1 got $argc"
	rename_IMG_main $dir_default
	exit
}

rename_IMG_main [lindex "$argv" 0]
