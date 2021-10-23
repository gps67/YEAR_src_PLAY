#!/usr/bin/env tclsh

proc test2 {} {
	puts "# This is test2 #################"
	OBJ obj_id GET fieldname 
	OBJ obj_id SET fieldname value
	OBJ obj_id array_get
	OBJ obj_id array_get { 1 "a1"  2 "b2" }
}

proc fn1 {X} {
	puts "# This is fn1 #################"
	OBJ obj_id array_get
	OBJ obj_id SET fieldname value
	OBJ obj_id GET fieldname 
}

proc fn2 {X} {
	puts "# This is fn2"
	OBJ obj_id GET fieldname value
	OBJ obj_id GET fieldname 
}

#	load ./tcl_oop_tcl.so
#	load ./tcl_oop_tcl.so Optical
	lappend auto_path \
	. 	\
	../obj/	\
	/home/gps/YEAR/src/PLAY/SCRATCH_ZONE_20/in_Tcl/tcl_OOP_TCL/obj \
	#
	load ../obj/tcl_oop_tcl.so optical
	puts "# FAIL # can find module without ../obj/filename.so"
#	load tcl_oop_tcl.so optical
	puts [OBJ - LIST_ALL_OBJ_TYPE]

	# already said k1 k2 k3 test from C
	test2
	test2
	fn1 A
	fn1 B
	fn2 C
	OBJ obj_id ONE

if 0 {
	OBJ obj_id SET fieldname value
	OBJ obj_id GET fieldname 
	OBJ obj_id 
	puts test1
	test1
}
