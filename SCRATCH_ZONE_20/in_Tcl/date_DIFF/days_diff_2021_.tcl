#!/usr/bin/env tclsh

set dates {
	2020-12-31 {ZERO}
	2021-02-25 {day 56}
}
set dates {
	2016-01-22 {Chris n Bex Godalming}
	2018-07-12 {Andreas wedding}
	2019-10-20 {AMS (flights)} 
	2021-04-04 {Lunch at 1}
	2021-04-15 {Priddy}
	2021-04-22 {CA Flying}
	2021-06-13 {Email lads to visit}
	2021-07-08 {Marianna David Lordship Pub}
	2021-07-10 {Godalming Day}
	2021-07-18 {Carmen 2020 anniv}
	2021-07-29 {Met Chris at Albany}
	2021-07-30 {Chris leaves Albany}
	2021-07-31 {Andrea broadcasts photo}
	2021-10-23 {CA Flying maybe}
	2021-12-02 {Malcolm email}
	2021-12-24 {xmas eve}
	2021-10-17 Order
	2021-12-29 Deliver
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

