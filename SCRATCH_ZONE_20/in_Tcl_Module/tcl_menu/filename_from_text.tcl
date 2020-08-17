
proc filename_from_text TEXT {
 puts "text_into_filename $TEXT"
 set stones {[^a-zA-Z0-9]+} ;# easier to see whats allowed NOT
 set stone "_"
 regsub -all $stones $TEXT $stone TEXT
 regsub -all "^$stone" $TEXT "" TEXT
 regsub -all "$stone\$" $TEXT "" TEXT
 return $TEXT
 
 # regsub $exp $TEXT subSpec var_name
 # subSpec
 #  & \0 replaced with matched TEXT
 # \1 .. \9 nth parametised matching text
 
 # OPTION var_name absent
 #  MAYBE find it, search for ABBR
 #  MAYBE set NULL or "-" or UNSET
 #  MAYBE UNSET # caller can detect well known SUBLEX name ABBR
 #  UNSET means remote accepts default VFS TREE STUFF
 #  UNSET means we do it on optimised C_PLUS TOKENISED db_SCRIPT
}


proc test1 {fn data} {
	set out [ $fn $data ]
	text_out_ln - "# data $fn # {$data} # $out # ..."
}
proc filename_from_text_tests {} {
	set fn filename_from_text 
	text_out_ln - "# $fn # data {\$data} # \$out #\n# ... MULTI_LINE ..."
	test1 $fn "cident99"
	test1 $fn "one space"
	test1 $fn "  j  o  i  n  "
	test1 $fn "minus-minus"
	test1 $fn "so{}()[]!"
}
