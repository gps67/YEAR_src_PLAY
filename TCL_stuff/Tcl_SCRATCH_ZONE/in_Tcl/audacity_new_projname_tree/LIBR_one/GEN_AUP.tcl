

proc GET_VALUE_of_VAR_NAME { VAR_POOL_NAME VAR_NAME } {
	upvar $VAR_POOL_NAME VAR_POOL
	set VAR_POOL($VAR_NAME)
}

# macro replacements over simplified ARGV_of_WORDS
# each word is a SINGLE symbol eq    Q2
# or is TEXT to put to through FILTER out
# better build an array KEY Q2 value TEXT
#
# note VAR_NAME returns::$VAR_NAME
# note VAR_* is reserved for this cannot have VAR_anything !!
# a livable subset #
#
# note we want to have a fixed list of VAR_NAME VALUE
#
proc GET_VALUE_of_SYMBOL { VAR_POOL_NAME symbol } {

	upvar $VAR_POOL_NAME VAR_POOL

	set ASCII_Q2 {"} ;# "
	set ASCII_Q1 {\'} ;# "
	switch -glob $symbol {
	    EOLN { return "\n" }
	    SQUARE_BRACKET_LEFT { return "\[" }
	    SQUARE_BRACKET_RIGHT { return "\]" }
	    CURLY_BRACE_LEFT { return "\{" }
	    CURLY_BRACE_RIGHT { return "\}" }
	    Q2 { return $ASCII_Q2 } 
	    Q1 { return $ASCII_Q1 } 
	    SP { return " " }
	    PIPE { return "\|" }
	    INDENT { return "  " }
	    REC_* { return [GET_VALUE_of_VAR_NAME VAR_POOL $symbol] }
	    VAR_* { return [GET_VALUE_of_VAR_NAME VAR_POOL $symbol] }
	    default { return $symbol }
	}
}

# macro replacements over simplified ARGV_of_WORDS
# each word is a SINGLE symbol eq    Q2

proc GEN_ADDS { VAR_POOL_NAME OUTVAR args } {
	upvar $VAR_POOL_NAME VAR_POOL
	upvar $OUTVAR OUT

#	puts "ARGS $args"
	# without Q2 "$args" gets evaluated as LIST differently
	# that then complains about the contents not proper having Q
	foreach list "$args" {
#	 show_var list
	 foreach item "$list" {
	    append OUT [GET_VALUE_of_SYMBOL VAR_POOL $item]
	 }
	}
}

proc GEN_aup_file {} {
 set OUT {}

#	build array of local values from global vars
#
 array set VAR_POOL [list 	\
 	REC_PROJ   $::REC_PROJ	\
 	REC_ARTIST $::REC_ARTIST \
 	REC_ALBUM  $::REC_ALBUM	\
 	REC_TRACK  $::REC_TRACK	\
 	REC_YEAR   $::REC_YEAR  \
 ] ;# 
 set VAR_PROJ 2025-08-33_Sun_americana_data
 GEN_ADDS VAR_POOL OUT {

	  <? xml
	  SP version="1.0"
	  SP standalone="no"
	  SP ?>
	  EOLN
	  <! DOCTYPE
	  SP project SP PUBLIC SP Q2
	  -//audacityproject-1.3.0//DTD//EN
	  Q2 SP Q2
	  http://audacity.sourceforge.net/xml/audacityproject-1.3.0.dtd
	  Q2 SP >
	EOLN 
	EOLN < project
	EOLN SP xmlns="http://audacity.sourceforge.net/xml/"
 } ; GEN_ADDS VAR_POOL OUT {
	EOLN SP projname= Q2 REC_PROJ Q2
 } ; GEN_ADDS VAR_POOL OUT {
	EOLN SP version="1.3.0"
	EOLN SP audacityversion="2.0.6"
	EOLN SP sel0="0.0000000000"
	EOLN SP sel1="0.0000000000"
	EOLN SP vpos="0"
	EOLN SP h="0.0000000000"
	EOLN SP zoom="86.1328125000"
	EOLN SP rate="44100.0"
	EOLN SP snapto="off"
	EOLN SP selectionformat="hh:mm:ss"
	EOLN >
	EOLN <tags>
	EOLN
        EOLN SP <tag SP name="ARTIST" SP value= Q2 REC_ARTIST  Q2 />
        EOLN SP <tag SP name="ALBUM"  SP value= Q2 REC_ALBUM  Q2 />
        EOLN SP <tag SP name="TITLE"  SP value= Q2 REC_TRACK  Q2 />
        EOLN SP <tag SP name="YEAR"   SP value= Q2 REC_YEAR  Q2 />
        EOLN </tags>
	EOLN </project>
	EOLN
 }
 # it is not called TRACK it is called TITLE
 # puts $OUT
 VFS_file_text $::VFS_file_aup $OUT ;# rewrite entire file

 set cmd "ask_do audacity $::VFS_file_aup"
 set file $::VFS_file_sh  
 VFS_file_text $file $cmd
 file attribute $file -permissions 0775
}

