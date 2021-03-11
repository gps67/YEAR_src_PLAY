
# convert a naff string to a usable filename namepart
# src/mp3_hissy_fit
# maps_plausible_list.py 
# list of ASCII from accented
# list of TEXT from PUNCT

proc filename_from_text TEXT {
	# anything that isnt plain ASCII glyphs # multichar # is a stone
	# the output stone is "_"
	# drop leading _
	# drop trailing _

	set stones {[^a-zA-Z0-9]+} ;# easier to see whats allowed NOT
	set stone "_"
	regsub -all $stones $TEXT $stone TEXT
	regsub -all "^$stone" $TEXT "" TEXT
	regsub -all "$stone\$" $TEXT "" TEXT
	return $TEXT
}
 
 # regsub RTFM
 # regsub -all $exp $TEXT subSpec var_name
 # takes text from $TEXT put it into var TEXT
 # regsum finds all $exp and substitutes $subSpec
 # subSpec expands & \0  also \1 .. \9

 # subSpec
 # subSpec expands & \0  also \1 .. \9
 # subSpec
 #
 #	 & \0 replaced with matched TEXT
 #	\1 .. \9 nth parametised matching text
 #
 
 # OPTION var_name absent
 #  MAYBE find it, search for ABBR
 #  MAYBE set NULL or "-" or UNSET
 #  MAYBE UNSET # caller can detect well known SUBLEX name ABBR
 #  UNSET means remote accepts default VFS TREE STUFF
 #  UNSET means we do it on optimised C_PLUS TOKENISED db_SCRIPT


proc test1 {fn data} {
	set out [ $fn $data ]
	text_out_ln - "# $fn # $out <--- {$data}"
}

proc filename_from_text_run_some_tests {} {
	set fn filename_from_text 
	text_out_ln - "# $fn # START ..."

	test1 $fn "cident99"
	test1 $fn "one space"
	test1 $fn "  j  o  i  n  "
	test1 $fn "minus-minus"
	test1 $fn "so{}()[]!"

	text_out_ln - "# $fn # ... END"
	text_out_ln - ""
}
