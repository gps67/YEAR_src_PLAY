#!/usr/bin/env tclsh

set date_zero 2022-04-27
set time_zero [clock scan $date_zero]
set days_plus -270
set dt_1_day [expr "60*60*24"]
set dt [expr $days_plus*$dt_1_day]
set time_two [expr $time_zero + $dt ]
set date_two [clock format $time_two -format "%Y-%m-%d"]
puts "$date_two"
