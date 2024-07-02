#!/usr/bin/env tclsh9.0

lappend auto_path [pwd]


namespace import ::MK_ID::mk*

namespace eval ::TESTER_ZONE {
 namespace export * 

 proc x_run_ret args {
  # throw if absent args ?
  set RET_EXPECT [lindex $args 0]
  set args [lremove $args 0]
 }

 proc run_ret { RET_EXPECT cmd_line {desc {}} } {

  if [catch $cmd_line ret_var] {
  	puts "# FIAL # THREW EXPECTED $RET_EXPECT # GOT $$ret_var # $cmd_line $desc"
	return 0 ;# falsy
  }
  if { $RET_EXPECT == $ret_var } {
	  puts "# PASS # $ret_var # $cmd_line # $desc"
	  return $ret_var
  } else {
  	  if { $desc == {} } { set desc '' }
  	  puts "# <==> #"
  	  puts "# FAIL # TEST # $desc # "
  	  puts "# FAIL # EXPECTED $RET_EXPECT # "
  	  puts "# FAIL # GOT $ret_var # "
  	  puts "# FAIL # RAN $cmd_line # "
	  return $ret_var
  }
 }

}


namespace eval ::MK_ID {
#  namespace export * ;# not needed # already set in first ?
 namespace import ::TESTER_ZONE::*

 proc test1 {} {
 	run_ret {PFX1} {mk_id  PFX } "prefix PFX item_zero counts from ONE"
 	run_ret {PFX2} {mk_id  PFX } "expect to PASS as PFX2 is really expected"
 	run_ret {PFX9} {mk_id  PFX } "expect to fail as PFX3 is really expected"
 	run_ret {4}    {mk_idx PFX } "prefix PFX with item count 3"
 	run_ret {PFX5} {mk_id  PFX } "prefix PFX with item count 4"
 	run_ret {TWO1} {mk_id  TWO } "prefix TWO with item count 1 - note each PFX N"
 	run_ret {TWO2} {mk_id  TWO } "prefix TWO with item count 2 - note each PFX N"
 	run_ret {3}    {mk_idx TWO } "counter TWO no PFX return idx from 1"
 	run_ret {TWO4} {mk_id  TWO } "prefix TWO with item count 4"
 	run_ret {1}    {mk_idx     } "counter DEFAULT first use"
 	run_ret {g_ITEM_2} {mk_id      } "prefix DEFAULT first use"
 }
}

::MK_ID::test1 ;#
