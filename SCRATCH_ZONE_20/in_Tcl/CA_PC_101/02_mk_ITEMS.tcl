#!/tools/CTXT/tcl/bin/tclsh

lappend auto_path [pwd]/fns_tcl

	# using symb link to /tmp/ca
	set dir_priv /tmp/ca
	file mkdir $dir_priv
	file attributes $dir_priv -permissions 0700

## LIBR above ## LIBR below ##

proc mk_CN_RSA {CA_CN CN RSA_nbits RSA_days phrase} {

	set is_self_sign 0
	if { $CA_CN == $CN } { # self sign # CA == CA_CN
		set is_self_sign 1
		puts "# SELF SIGN"
	}
	set is_CA 0
	if { [string equal -length 3 "CA_" $CN] } {
		set is_CA 1
	}
	puts "# is_CA $is_CA"

	puts "# pwd # [pwd]"

	set dir_priv [DIR_NAME_CN_key $CN]
	file mkdir $dir_priv
	file attributes $dir_priv -permissions 0700
	

	set RSA_priv_key [FILENAME_CN_key $CN]
 if 1 {
  if [file exists $RSA_priv_key] {
  #	puts "# RSA_priv_key already exists $RSA_priv_key"
  } else {
	set cmd {}
	lappend cmd openssl 
	lappend cmd genrsa
#	puts "phrase '$phrase' TEST='{$phrase!=""}'"
	if {$phrase!={}} { lappend cmd -aes256 }
	lappend cmd -out $RSA_priv_key
	lappend cmd $RSA_nbits
	lappend cmd 

	# say_do $cmd
	if ![say_do $cmd] return
  }
}

 if 1 {
	set cmd {}
	lappend cmd openssl 
	lappend cmd req 
	lappend cmd -new -x509 

  if 1 {
	lappend cmd -config openssl.cfg
	if {$is_CA} {
		puts "# is_CA # NOW $is_CA"
		lappend cmd -extensions easyrsa_ca
	} else {
		# org is not -extensions valid
		# lappend cmd -extensions org
		lappend cmd -extensions basic_exts
		puts "# is_CA # NOW $is_CA"
	}
  }

  if [file exists $RSA_priv_key] {
  	puts "# RSA_priv_key already exists $RSA_priv_key"
	lappend cmd -key [FILENAME_CN_key $CN]
  } else {
	lappend cmd -keyout [FILENAME_CN_key $CN]
	lappend cmd -newkey rsa:$RSA_nbits -days $RSA_days
	if {$phrase=={}} {
		lappend cmd -nodes
	} else {
		# somehow use phrase to protect + access key
	}
  }

	lappend cmd -subj "/CN=$CN"
	lappend cmd -out    [FILENAME_CN_csr $CN]

	if ![say_do $cmd] return 

	# convert csr to txt
	set cmd2 {}
	lappend cmd2 openssl 
#	lappend cmd2 req 
	lappend cmd2 x509
	lappend cmd2 -in [FILENAME_CN_csr $CN]
	lappend cmd2 -text
	lappend cmd2 -out [FILENAME_CN_csr $CN].txt
	# say_do $cmd2
	if ![say_do $cmd2] return

	# apply AES256 PHRASE to .key after all done

	# chmod 600 .key now
	# repeat runs are OK with that as 6=rw writable
	file attributes [FILENAME_CN_key $CN] -permissions 0600
 }
 # PHRASE to add passphrase to key
 # openssl rsa -des3 -in server.key -out server.key.new 
 # PHRASE to view csr
 # openssl req -in $1 -text -noout
 # PHRASE to view x509
 # openssl x509 -in $1 -text -noout

	# NOW SIGN CSR

	return true
}

proc main {} {
	if ![mk_CN_RSA CA_ZERO CA_ZERO 4096 1600 {}] return
	if ![mk_CN_RSA CA_ZERO  CA_ONE 2048 800 {}] return
	if ![mk_CN_RSA CA_ONE   PC_101 2048 365 {}] return
	if ![mk_CN_RSA CA_ONE   PC_102 2048 365 {}] return
	if ![mk_CN_RSA CA_ONE   PC_103 2048 365 {}] return
}
main

	say_do {tree ca}

