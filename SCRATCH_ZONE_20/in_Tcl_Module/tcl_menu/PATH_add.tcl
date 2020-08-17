
proc PATH_add_gvim {} {
	set DIR1 {C:/Program Files (x86)/Vim/vim82}
	append ::env(PATH) ";$DIR1"
	puts $::env(PATH) 
}
# PATH_add_gvim 

