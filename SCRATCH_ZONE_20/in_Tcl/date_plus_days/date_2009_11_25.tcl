#!/usr/bin/env tclsh

set date_zero 2009-11-25
set time_zero [clock scan $date_zero]

set date_curr 2022-06-06
set time_curr [clock scan $date_curr]

set dt_1_day [expr "60*60*24"]

set dt_days [expr ($time_curr - $time_zero)/$dt_1_day]
set dt_weeks [expr $dt_days/7]
set dt_years [expr $dt_days/365.25]
set p10 [expr 0.1 * $dt_days]

proc V varname {
	upvar $varname var
	puts [format "%9s %7.2f" $varname $var]
}

V dt_days
V dt_weeks
V dt_years
V p10

