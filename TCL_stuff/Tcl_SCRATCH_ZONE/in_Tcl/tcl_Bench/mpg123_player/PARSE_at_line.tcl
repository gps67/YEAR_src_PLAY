# 2022-11-27 Nov Sun 03:29 # 53 #  PARSE_at_line.tcl


namespace import ::RE_CODES::*

namespace eval fd_mpg123  {

proc build_RE_s {} {
	
  	set RE_str {}

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
	# variable RE_at_P_line
	set ::fd_mpg123::RE_at_P_line $RE_str
}

proc PARSE_at_line {line} {

	# speeds up CXX slows down TCL #
	set byte0 [string index $line 0]
	set byte1 [string index $line 1]
	set byte2 [string index $line 2]
	if { $byte2 != { } } {
		text_out_ln "# ERROR # EXPECTED SPACE # $line"
	}
	set tail  [string range $line 3 end]
	# text_out_ln - "# $byte1 # $tail # AT line tail"

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
	switch -exact $byte1 {
	 R { # @R MPG123 (ThOr) v10
		 text_out_ln - "$tail ## welcome version tail"
	 }
	 I {
	 	switch -exact $tail {
		 \{ {
			set ::fd_mpg123::ID_TEXT {}
			lappend ::fd_mpg123::ID_TEXT "ID3_TEXT"
			return
		 }
	 	 \} {
			text_out_ln - $::fd_mpg123::ID_TEXT 
			return
		 }
		 default {
			lappend ::fd_mpg123::ID_TEXT $tail
		 #	text_out_ln - "'$tail' ## ID3 info tail"
		 }
	 	}
	 }
	 P {
	 	foreach { PLAY_STATE } $tail {}
		switch -exact $PLAY_STATE {
		 0 {
		 	text_out_ln - "# KNOW # STOP # TODO # start next track"
		 }
		 1 {
		 	text_out_ln - "# KNOW # PAUSE"
		 }
		 2 {
		 	text_out_ln - "# KNOW # PLAY"
		 }
		 default {
		 	text_out_ln - "# ERROR # PLAY code $P not recognised"
		 }
		}
	 }
	 S {
		 text_out_ln - "# $tail ## S stereo tail"
	 }
	 F {
	 	foreach { seq frame time remain } $tail {}
	 # TODO # send to GUI to display
	 #	text_out_ln - "seq $seq frame $frame time $time remain $remain " 
	 }
	 default {
		 text_out_ln - "# FAIL # $line ## unrecognised line ####"
	 }
	}

set x {
  @I { }
  @I ID3:Hello                         Adele                         25                                 #                            Blues 
  @I ID3.genre:0 
  @I ID3.track:1 
  @I
  @P 2 
  @S 1.0 3 44100 Joint-Stereo 2 104 2 0 0 0 32 0 1 
  @F 0 11313 0.00 295.52 
  @F 0 11313 0.00 295.52 
}

	# puts "{ byte0 byte1 tail } { $byte0 $byte1 $tail }"
 }
} ;# namespace
