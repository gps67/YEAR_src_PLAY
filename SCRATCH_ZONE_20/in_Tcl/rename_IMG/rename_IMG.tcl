#!/usr/bin/env tclsh

proc rename_IMG_main {dir} {
	puts "dir $dir"
	set L1 [glob -tails -directory $dir *]
	set L1 [lsort $L1]
	puts "L1 $L1"
}

if {$argc != 1} {
	puts "expected argc 1 got $argc"
}

rename_IMG_main [lindex "$argv" 0]
