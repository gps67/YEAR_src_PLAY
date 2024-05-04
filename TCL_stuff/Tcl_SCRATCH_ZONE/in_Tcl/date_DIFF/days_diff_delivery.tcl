#!/usr/bin/env tclsh

set dates {
	2024-03-19 {Ordered}
	2024-05-04 {Delivered}
}

proc days_between {d1 d2} {

	set t1 [clock scan $d1]
	set t2 [clock scan $d2]
	set dt1 [expr "60*60*24"]

 #	puts "t1 = $t1"
 #	puts "t2 = $t2"
	set dt [expr $t2-$t1]
	set dt_days [expr $dt/$dt1]
	set dt_days [format "%3d" $dt_days]
 #	puts "Days diff: $dt_days"

	return $dt_days
}

set date1 {}
foreach {date2 desc2} $dates {

	if { $date1 == {} } {
		set date1 $date2
	}
	set days12 [days_between $date1 $date2]
	set date1 $date2
	puts "$days12 $date2 $desc2"
	
}

