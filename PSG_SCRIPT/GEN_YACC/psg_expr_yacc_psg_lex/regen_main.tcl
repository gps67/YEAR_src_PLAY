#!/usr/bin/env tclsh

set d .
set e1_yacc $d/e1.y
set e1_lex  $d/e1_lex.lex

set d0 [pwd]/.
set d0 [pwd]/..

lappend auto_path $d0/..
lappend auto_path $d0/gen
lappend auto_path $d0/obj
lappend auto_path $d0/psg

namespace eval regen {}
namespace eval regen::psg {}

# cant get rid of this
source $d0/regen_libr.tcl
source $d0/regen_psg.tcl

::regen::init
# ::regen::gen_yacc_top_code ::regen::text_yacc_top_code 
# ::regen::gen_str_of_token  ::regen::text_yacc_top_code 


proc write_buffer_to_file {outvarname filename} {
	upvar $outvarname text
	set fd [open $filename w+]
	puts -nonewline     $text
	puts -nonewline $fd $text
	close $fd
}



set out {}
namespace eval regen {
 LINE out "%{"
 gen_lex_top_code out
 LINE out "%}"
 LINE out ""
 LINE out "%option noyywrap"
 LINE out 
 LINE out "%%"
 LINE out 
 lex_GAP_list out
 lex_EXPR_list out
 lex_RW_list out
 lex_PUNCT_list out
 LINE out 
 LINE out {. printf("Unknown token!\n"); yyterminate();}
 LINE out 
 LINE out "%%"
}
write_buffer_to_file out $e1_lex 

set out {}
namespace eval regen {
 LINE out "%{"
 gen_yacc_top_code out
 gen_str_of_token  out
 LINE out 
 LINE out "%}"
 LINE out 
 gen_union out
 LINE out 
 gen_tokens out
 LINE out 
 gen_types out
 LINE out 
 gen_left_right out
 LINE out 
 LINE out "%start top"
 LINE out 
 LINE out "%%"
 LINE out 
 gen_yacc_rules_code out
 LINE out "// # EOT #"
}
write_buffer_to_file out $e1_yacc 



