#!/usr/bin/env tclsh

#	load ./tcl_oop_tcl.so
#	load ./tcl_oop_tcl.so Optical
	load ./tcl_oop_tcl.so optical
	test1
	optical::OBJ obj_id SET fieldname value
	optical::OBJ obj_id GET fieldname 
