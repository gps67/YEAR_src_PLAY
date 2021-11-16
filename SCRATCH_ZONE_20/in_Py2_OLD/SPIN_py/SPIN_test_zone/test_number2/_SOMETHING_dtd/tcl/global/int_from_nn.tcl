# "08" and "09" are invalid octal numbers, all over 2 digit decimals are OK

proc int_from_nn {dd} {
	switch $dd {
	 08 { set dd 8 }
	 09 { set dd 9 }
	}
	incr dd 0
}
