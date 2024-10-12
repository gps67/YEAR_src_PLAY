#!/usr/bin/env tclsh

proc log_error msg {
	puts stderr $msg
}

proc log_noise msg {
	if 0 { puts stderr $msg }
}

proc file_mtime { retvar filename } {
	upvar $retvar t1
	set t1 -1
	set t1 [file mtime $filename]
 if 0 {
 	if [ catch {
		set t1 [file mtime $filename]
	 }] {

		log_noise "file_mtime $filename $t1 FAILED"
		log_noise "::errorCode $::errorCode"
	#	log_noise "::errorInfo $::errorInfo"
		return 0 ;# FALSE failed
	}
 }
	log_noise "file_mtime $filename $t1"
	return 1
}

proc my_main {argc argv} {
	if { $argc != 2 } {
		log_error "argc $argc argv $argv"
		return 1 ;# NOT is_older
	}
	file_mtime t1 [lindex $argv 0]
	file_mtime t2 [lindex $argv 1]
	# is_older # t2 > t1
	set ret [expr $t2 > $t1 ]
	log_noise "ret $ret"
	if $ret {
		log_noise is_older
	} else {
		log_noise NOT_is_older
	}
	return $ret ;# TRUE_FALSE is_older
	return 1
}

my_main $argc $argv
