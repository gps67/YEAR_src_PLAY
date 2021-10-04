#!/usr/bin/env tclsh
#!/tools/CTXT/tcl/bin/tclsh

lappend auto_path [pwd]/fns_tcl

	# using symb link to /tmp/ca
	set dir_priv /tmp/ca
	file mkdir $dir_priv
	file attributes $dir_priv -permissions 0700

## LIBR above ## LIBR below ##

proc crt_to_text { CERTFILE {TEXTFILE {}} } {
	x509_to_text $CERTFILE $TEXTFILE
}

proc csr_to_text { CERTFILE {TEXTFILE {}} } {
	x509_to_text $CERTFILE $TEXTFILE
}

proc x509_to_text { CERTFILE {TEXTFILE {}} } {
	if {{}==$TEXTFILE} {
		set TEXTFILE $CERTFILE.text
	}
	# convert csr to txt
	set cmd2 {}
	lappend cmd2 openssl 
#	lappend cmd2 req ;# csr is supposed to have this, seems to know
	lappend cmd2 x509
	lappend cmd2 -in $CERTFILE
	lappend cmd2 -text
	lappend cmd2 -out $TEXTFILE
	# say_do $cmd2
	if ![say_do $cmd2] return
}

# CA issues CN
# 
proc CA_mk_CN  {CA_CN CN RSA_nbits RSA_days phrase} {

	puts "\n##### ##### ##### CA_mk_CN  $CA_CN $CN \n"

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


	set dir_priv [DIR_NAME_CN_key $CN]
	file mkdir $dir_priv
	file attributes $dir_priv -permissions 0700


	# the filename of the private key for CN
	set file_CN_key [FILENAME_CN_key $CN]
	set file_CN_csr [FILENAME_CN_csr $CN]
	set file_CN_crt [FILENAME_CN_crt $CN]

	# CA to sign
	set file_CA_key [FILENAME_CN_key $CA_CN]
	set file_CA_crt [FILENAME_CN_crt $CA_CN]

	# these are named in the config
	# config is setting all CA=FALSE
	# somehoe pick up stanza within config
	# or go back to C++
	# and sort out it#s DB and SERIAL
	set ca_index ca/index.txt
	set ca_serial ca/serial
	set ca_config openssl.cfg ;# not in ca/ that gets deleted rebuilt

	# create the private key (pair in 1 file)
	# if we dont do it here, we can do it later
 if 1 {
  if [file exists $file_CN_key] {
  #	puts "# file_CN_key already exists $file_CN_key"
  } else {
	set cmd {}
	lappend cmd openssl 
	lappend cmd genrsa
#	puts "phrase '$phrase' TEST='{$phrase!=""}'"
# TODO	# phrase work later # C_API has that ability # openssl not so much
#	# this uses keyboard inout of phrase ?
	if {$phrase!={}} { lappend cmd -aes256 }
	lappend cmd -out $file_CN_key
	lappend cmd $RSA_nbits
	lappend cmd 

	# say_do $cmd
	if ![say_do $cmd] return
  }
 }

 	puts "# create the REQ"
 	# create the REQ
 if 1 {
	set cmd {}
	lappend cmd openssl 
	lappend cmd req 
	lappend cmd -new ;# -x509 

  if 1 {
	lappend cmd -config $ca_config
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


  	# the RSA either exists or we create it
	# add opts to cmds that do that
  if [file exists $file_CN_key] {
  	puts "# file_CN_key already exists $file_CN_key"
	lappend cmd -key $file_CN_key
  } else {
	lappend cmd -keyout $file_CN_key
	lappend cmd -newkey rsa:$RSA_nbits 
	lappend cmd -days $RSA_days
	if {$phrase=={}} {
		# for us it is always nodes
		lappend cmd -nodes
	} else {
		# somehow use phrase to protect + access key
		# TODO # use phrase
	}
  }

	# opts specific to CN on cms line
	lappend cmd -subj "/CN=$CN"
	lappend cmd -out $file_CN_csr 

	if ![say_do $cmd] return 
#	csr_to_text $file_CN_csr ;# NO its not an x509

	# sign the request with the CA
	# https://nodejs.org/api/tls.html
	# _sign

	set cmd {}
	lappend cmd touch $ca_index
	if ![say_do $cmd] return 

	if ![file exists $ca_serial] {
	 puts "# writing $ca_serial"
	 set fd [open $ca_serial w]
	 puts $fd "07"
	 close $fd
	}

	if {$is_self_sign} {
		puts "# self sign cmd"
		set cmd {}
		lappend cmd openssl 
		lappend cmd x509 
		lappend cmd -req
		lappend cmd -in $file_CN_csr
		lappend cmd -signkey $file_CA_key
		lappend cmd -out $file_CN_crt
	} else {
		puts "# CA sign CN cmd"
		set cmd {}
		lappend cmd openssl ca
		lappend cmd -cert $file_CA_crt
		lappend cmd -keyfile $file_CA_key
		lappend cmd -in $file_CN_csr
		lappend cmd -out $file_CN_crt
		lappend cmd -config $ca_config ;# maybe ca cfg
	}
	if ![say_do $cmd] return 

	crt_to_text $file_CN_crt
#	x509_to_text [FILENAME_CN_csr $CN]

	# apply AES256 PHRASE to .key after all done

	# chmod 600 .key now
	# repeat runs are OK with that as 6=rw writable
	file attributes $file_CN_key -permissions 0600
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
	puts "# pwd # [pwd]"
	if ![CA_mk_CN  CA_ZERO  CA_ZERO  4096 1600 {}] return
	if ![CA_mk_CN  CA_ZERO  CA_ONE   2048  800 {}] return
	if ![CA_mk_CN  CA_ONE   CA_ZONE  2048  800 {}] return
	if ![CA_mk_CN  CA_ZONE  CA_VPN   2048  800 {}] return
	if ![CA_mk_CN  CA_VPN   GW_ONE   2048  365 {}] return
	if ![CA_mk_CN  CA_VPN   PC_101   2048  365 {}] return
	if ![CA_mk_CN  CA_VPN   PC_102   2048  365 {}] return
	if ![CA_mk_CN  CA_VPN   PC_103   2048  365 {}] return
}
main

	say_do {tree ca}

