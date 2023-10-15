# 2022-11-27 Nov Sun 03:29 # 53 #  PARSE_at_line.tcl


namespace import ::RE_CODES::*

namespace eval fd_mpg123  {
proc PARSE_at_line {line} {
  	set RE_str {}


	# speeds up CXX slows down TCL #
	set byte0 [string index $line 0]
	set byte1 [string index $line 1]

	if { $byte0 != "@" } {
		return [WARN "expected @ got '$byte0' "]
	} 
	# is ident
	if { $byte1 < "A" } {
		return [WARN "expected [A-Z] got byte1 %s "]
	} 
	if { $byte1 > "Z" } {
		return [WARN "expected [A-Z] got byte1 %s "]
	} 

	set BOLN "^"
	set EOLN "\$"
	set SP " "
	set cident99 [RE_CODES::get_RE_phrase_cached cident99]
	set numeric  [RE_CODES::get_RE_phrase_cached numeric]

	# TODO # set_RE_phrase_var $varname # AUTO_ADD re_

	RE_clear RE_str 
	RE_append RE_str         $BOLN     ;# regex compile will optimise
	RE_append RE_str         @         ;# regex compile will optimise
	RE_addvar RE_str R       $cident99 ;# at_R_line 
	RE_append RE_str         $SP       ;# actual GAP SP1
	RE_addvar RE_str to_eoln ".*"      ;#
	RE_append RE_str         $EOLN     ;# regex compile will optimise


	# str val #
	set RE_at_P_line $RE_str

	puts "{ RE_str } { $RE_str }"
 }
} ;# namespace
