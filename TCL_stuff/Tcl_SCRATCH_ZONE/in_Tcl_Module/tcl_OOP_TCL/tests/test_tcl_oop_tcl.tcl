#!/usr/bin/env tclsh9.0
#!/usr/bin/env tclsh
puts "argv0 is $argv0"

proc TT {ARGV} {
#	set ret [eval $ARGV]
	set ret [uplevel $ARGV]
	puts "TT {$ARGV} -> '$ret'"
}

proc test_VECT {} {
	puts "# test_VECT #################"
	# want this imperative set V [VECT] # [VECT NEW A R G S ]
#	set VECT [VECT]
TT {	set VECT [set b b] }
TT {	set VECT [VECT] }
TT {	set VECT [OBJ NEW VECT] }
# TODO soon
#	set VECT [OBJ + VECT]
#	set VECT [OBJ - VECT]
#	set VECT [OBJ _ VECT]
#	set VECT [OBJ {} VECT]
#	set VECT [OBJ NEW VECT]
TT {	OBJ $VECT ADD A ;# OPTION ADD A B C	}
TT {	OBJ $VECT ADD B	}
TT {	OBJ $VECT ADD C	}
TT {	OBJ $VECT ADD D ;# OPTION ADD A B C }
TT { 	OBJ $VECT GET 2	}
TT {	set AS_LIST [OBJ $VECT array_get]	}

TT {	set DICT [DICT]	}
TT {	OBJ $DICT array_set $AS_LIST	}
TT {	set AS_LIST2 [OBJ $DICT array_get]	}
TT {	puts "$AS_LIST2"	}
TT {	set DICT [OBJ]	}
TT {	OBJ $DICT SET A A ;# 	}

}

proc test2 {} {
	puts "# This is test2 #################"
	OBJ - GET fieldname 
	OBJ {} GET fieldname 
	OBJ {{}} SET fieldname value
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

proc fn3 {} {
	puts [OBJ - LIST_ALL_OBJ_TYPE]
}

#	load ./tcl_oop_tcl.so
#	load ./tcl_oop_tcl.so Optical
	lappend auto_path \
	. 	\
	../obj/	\
	/home/gps/YEAR/src/PLAY/SCRATCH_ZONE_20/in_Tcl/tcl_OOP_TCL/obj \
;	#
	# load the CXX module from a known file path
	# todo LOAD from PATH search, or TRAX("purpose")
	# todo LOAD from PATH search, or TRAX("TCL_CXX_MODULE tcl_oop_tcl")
	# search PATH for %s.so file(s) { .so .db .rc .sess }
	puts "# KNOW # load ../obj/tcl_oop_tcl.so optical"
	load ../obj/tcl_oop_tcl.so optical
#	load tcl_oop_tcl.so optical

if 0 {
	# these 2 show that the use of LITERAL LIST_ALL_OBJ_TYPE
	# outside of any proc, creates a LEX2
	# even after the use inside fn3 used the LEX1
	fn3
	puts [OBJ - LIST_ALL_OBJ_TYPE]
}

	puts "# ==== # test_VECT #"
	test_VECT ;# want to see ARGV0 typePtr
	puts "# ==== # test_DICT #"
	test_DICT ;# second call # see not LEX 1 ified

if 1 {

	# already said k1 k2 k3 test from C
	test2
	test2
	fn1 A
	fn1 B
	fn2 C
	OBJ obj_id ONE

	puts "# TEST # SET and GET outside of proc become LEX2"
	OBJ obj_id SET fieldname value
	OBJ obj_id GET fieldname 
	OBJ obj_id 
	puts test1
	test1
}
