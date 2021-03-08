#!/tools/CTXT/tcl/bin/tclsh

lappend auto_path [pwd]/fns_tcl

## LIBR above ## LIBR below ##

proc mk_CN_RSA {CA_CN CN RSA_nbits RSA_days phrase} {

	if { $CA_CN == $CN } { # self sign # CA == CA_CN
		puts "SELF SIGN"
	}

	set dir_priv [DIR_NAME_CN_key $CN]
	file mkdir $dir_priv
	file attributes $dir_priv -permissions 0700
	

 if 0 {
	set cmd {}
	lappend cmd openssl 
	lappend cmd genrsa
#	puts "phrase '$phrase' TEST='{$phrase!=""}'"
	if {$phrase!={}} { lappend cmd -aes256 }
	lappend cmd -out $RSA_priv_key
	lappend cmd $RSA_nbits
	lappend cmd 

	say_do $cmd
}

 if 1 {
	set cmd {}
	lappend cmd openssl 
	lappend cmd req 
	lappend cmd -new -x509 
	lappend cmd -newkey rsa:$RSA_nbits -days $RSA_days
	if {$phrase=={}} {
		lappend cmd -nodes
	} else { }
	lappend cmd -subj "/CN=$CN"
	lappend cmd -keyout [FILENAME_CN_key $CN]
	lappend cmd -out  [FILENAME_CN_csr $CN]

	say_do $cmd

	# apply AES256 PHRASE to .key after all done
	# chmod 600 .key now
	file attributes [FILENAME_CN_key $CN] -permissions 0600
 }

}

	mk_CN_RSA CA_ZERO CA_ZERO 4096 1600 {}
	mk_CN_RSA CA_ZERO  CA_ONE 2048 800 {}
	mk_CN_RSA CA_ONE   PC_101 2048 365 {}
	mk_CN_RSA CA_ONE   PC_102 2048 365 {}
	mk_CN_RSA CA_ONE   PC_103 2048 365 {}

	say_do tree

