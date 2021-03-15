#!/usr/bin/env tclsh

namespace eval ::MK_ID {
 namespace export *
 variable pfx_N 
 variable G_counter_NAME G_ITEM_ 
#  set pfx_N(G_counter_N) -1 ;# is an array ;# TODO dict

 proc mk_id {{pfx {}}} {
 	if {$pfx == {} } { set pfx $::MK_ID::G_counter_NAME }
	return "$pfx[mk_idx $pfx]"
 } 

 proc mk_idx {{pfx {}}} {
 	if {$pfx == {} } { set pfx $::MK_ID::G_counter_NAME }
#	variable pfx_N 
#	incr pfx_N($pfx) ;# return value
	incr ::pfx_N($pfx) ;# return value
 } ;# it is actually a str %d idx 99

 proc test1 {} {
 	puts "[mk_id PFX]"
 	puts "[mk_id PFX]"
 	puts "[mk_idx PFX]"
 	puts "[mk_id PFX]"
 	puts "[mk_id TWO]"
 	puts "[mk_id TWO]"
 	puts "[mk_idx TWO]"
 	puts "[mk_id TWO]"
 	puts "[mk_idx ]"
 	puts "[mk_id ]"
 }
}

# caller must somehow do
# namespace import ::MK_ID::mk*

# OK # ::MK_ID::test1 ;#
