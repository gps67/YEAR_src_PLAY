namespace eval ::OBJ_FIELD {
 namespace export *
 variable db 

 set db [dict create ]

 # k1 is OBJ name
 # k2 is FIELD name
 # v is value 

 proc OBJ_FIELD_GET {k1 k2} {
	variable db
 	dict get $db $k1 $k2
 }

 proc OBJ_FIELD_SET {k1 k2 v} {
	variable db
 	dict set  db $k1 $k2 $v
 }

 proc OBJ_exists {k1} {
	variable db
 	dict exists $db $k1
 }

 proc OBJ_FIELD_exists {k1 k2} {
	variable db
 	dict exists $db $k1 $k2
 }

 proc OBJ_del {k1} {
	variable db
	dict unset  db $k1 ;# and all k2
 }

 proc OBJ_FIELD_UNSET {k1 {k2 {}}} {
 	if { $k2 == {} } {
		variable db
		dict unset  db $k1 ;# and all k2
	} else {
		variable db
		dict unset  db $k1 $k2
	}
 }

# TCL FAIL # lappend must have single k1 #
# TCL FAIL # append must have single k1 #
# even though they write the updated value to the var
# they dont do that nested k1 k2
# probably because of expectation to have a list of v1 v2 v3

}

lappend auto_path [pwd]

namespace import ::MK_ID::mk*
namespace import ::OBJ_FIELD::*

 proc db_test1 {} {
 	set k1 OBJ
	set k2 FIELD
 	OBJ_FIELD_SET $k1 $k2 v1
	set v1 [OBJ_FIELD_GET $k1 $k2]
	puts "# got v1 $v1"
	OBJ_FIELD_UNSET $k1 $k2
	puts "# expect throw no field"
	set v1 [OBJ_FIELD_GET $k1 $k2]
	puts "# got v1 $v1"
 }

# db_test1


