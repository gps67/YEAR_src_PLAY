#!/usr/bin/env tclsh

proc fn1 {X} {
	puts "This is fn1"
	OBJ obj_id array_get
	OBJ obj_id SET fieldname value
	OBJ obj_id GET fieldname 
}

proc fn2 {X} {
	puts "This is fn2"
	OBJ obj_id GET fieldname value
	OBJ obj_id GET fieldname 
}

#	load ./tcl_oop_tcl.so
#	load ./tcl_oop_tcl.so Optical
	load ./tcl_oop_tcl.so optical
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
