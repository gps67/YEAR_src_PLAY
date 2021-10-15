#!/usr/bin/env tclsh

proc fn1 {X} {
	OBJ obj_id SET fieldname value
	OBJ obj_id GET fieldname 
}

#	load ./tcl_oop_tcl.so
#	load ./tcl_oop_tcl.so Optical
	load ./tcl_oop_tcl.so optical
	fn1 A
	fn1 B

if 0 {
	OBJ obj_id SET fieldname value
	OBJ obj_id GET fieldname 
	OBJ obj_id 
	puts test1
	test1
}
