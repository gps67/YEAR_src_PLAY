#!/usr/bin/env tclsh

namespace eval ::MK_ID {
 namespace export *

 # store counter N for each prefix used
 #
 # pfx_N(OBJ) # 10 # OBJ10 #
 # pfx_N(PFX) # 21 # PFX21
 # pfx_N(ONE) # 32
 # pfx_N(TWO) # 43
 # pfx_N(g_ITEM_) # 1
 #
 variable pfx_N 

 # single default prefix # usable as .w1.frame2.g_ITEM_123.subfield
 # window names cannot start with an uppercase # g_ITEM_123

 variable G_counter_NAME   g_ITEM_ 

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

}

# caller must somehow do
# namespace import ::MK_ID::mk*

# OK # ::MK_ID::test1 ;#
