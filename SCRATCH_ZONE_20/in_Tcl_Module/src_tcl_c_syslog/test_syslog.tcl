#!/usr/bin/env tclsh

	load ./tcl_syslog.so Syslog
	syslog::syslog TCL_PROG 0 USER INFO "Tcl message without pid"
	syslog::syslog TCL_PROG 1 USER INFO "Tcl message with pid"
