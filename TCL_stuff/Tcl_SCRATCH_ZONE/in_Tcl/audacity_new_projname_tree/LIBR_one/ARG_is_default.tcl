
proc ARG_is_default ARG {
	switch $ARG {
	 {''} {
	 	return 1
	 }
	 {""} {
	 	return 1
	 }
	 {{}} {
	 	return 1
	 }
	 {} {
	 	return 1
	 }
	 {-} {
	 	return 1
	 }
	 {default} {
	 	return 1
	 }
	}
	return 0
}

