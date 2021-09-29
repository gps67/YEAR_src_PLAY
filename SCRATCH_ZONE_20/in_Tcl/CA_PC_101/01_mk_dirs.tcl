#!/usr/bin/env tclsh
#!/tools/CTXT/tcl/bin/tclsh

proc say_do {cmd} {
	# cmd is a list # ARGS
	# need to add exec as first arg and call eval on that
	puts "# RUN # $cmd"
	set cmd2 [linsert $cmd 0 exec] ;# NAFF tcl lxxxx should use varname
	# TODO catch #
	set ret [eval $cmd2]
	puts "# RET # $ret"
	return ret
}

proc mk_C_dirs {CN} {
	#
	# C_SERVER
	# C_CLIENT
	# C_CA CA ;# z2q111111
	#
	#  idx = lookup $CN
	#
		mk_POOL_ITEM_CN $CN	;# the CTOR that actually makes dirs
	
	# STYLE += return last expr # it returns idx 
	#
	# CODE_VIEW #
	# CTOR mk_ITEM_from 
	# PARSE mk_%s

	# DATA += already in DB KEYS += CN
	# DATA += load from ROM PREBUILT_ROM
	# DATA += lookup( CN )
	# there that ran it # JIT AOT # 
}


# in C++ I would call ITEM_CTOR from mk_ITEM( CN ) ;# CTOR ( %s NAME )
#
# idx = lookup CN # mk POOL_ITEM from { %s CN } # symbol tax
#
#
proc mk_POOL_ITEM_CN {CN} {
 #
 # mk_dirs
 #  ca
 #   CN
 #    private
 #
 # symb link
 #  ../.. private/CN # ca is SOMEWHERE AROUND # HINT but IDX_DATA not needed
 # HERE simplifies THIS and THAT WHEN HOW
 # ../../private ./CN/. X509 RSA_SECRET KEYPHRASE MEDIA_FILE # META KEYS
 #
 # VFS # parametised ITEM = GET from VFS
 #  RSA_SECRET_HOLDER # will ask access as id == HOLDER_OF RSA_SECRET
 # RSA_SECRET_HOLDER = HOLDER_RSA_SECRET
 # RSA_SECRET_HOLDER += RSA_SECRET_private_copy_nbytes_or_PAGE_TEXT_REMINDER
 #  ABSENT RSA_SECRET means CHECK_LATER_at_RUNTIME
 #  ABSENT RSA_SECRET means NEVER_CALLED # cache predict unused pathway was dreamt up
 #  ABSENT RSA_SECRET means DONE # CONFIRMED # PASS #
 # 
 # RSA_SECRET
 #  X509 of where it is used # must be VALIDATED # option: Q or DECL #
 #  RSA_KEY_PRIV_2048 256 bytes
 #  AES_256_KEY0 32 bytes u64_WORDS[4] four_words
 #  RSA_SECRET_UNDER_AES_256 NBYTES( DATA bytes[256] )
 #  AES_SECRET ( DATA bytes[32] )
 #  AES_SECRET_under passphrase ( DATA bytes[32], INFO_ABOUT_PICK_AES )
 #  USR_SECRET_PASSPHRASE( STR0 as_str ) // NBYTES( str ) // P0P2 // offs offs
 #  KNOW is plain STR0 KNOW NUL dont go past NUL 
 #  ENV_SECRETS list_see_TREE_of_ASK_KEY binary and text and methods and PLUS
 #  ASSOC_LINK_ABOVE ...
 #  KEY = decoded_from NBYTES via VAR_POOL # and STBL_POOL #
 #  TEXT_FRAGMENT = AES256_SECRET_as_base64 # 32 //4 = 8 incl space for 2 byte
 #  32 * 4/3 # 2 + 30 == 10 x 3 byes into 10 x 4 
 #  33 = 32 + 1 byte OPCODE == 11 x 4 ASCII no terminator = P2 required
 #  33 => P2 known 
 #  TEXT_FRAGMENT = AES256_under_PHRASE_as_base64

 #	OPTION AES256 BINARY_SECRET  # BINARY
 #	EXPOSED AES256 BINARY_SECRET # BINARY
 #	PHRASED_SECRET # same N on nbytes # BINARY
 #	RETAINED_BLOB_AS_BASE64 #
 #	OPCODE BYTE 33 from base64 of 32 # unique unused payload space # used
 #	IGNORE OPCODE option

 

 #  RSA_SECRET # that identifies RSA_SECRET_HOLDER
 #  RSA_SECRET # that unlocks MESSAGE
 #  SCOPE # POOL appears in SCOPE # ... is ...
 #   HERE %s # auto filter cident(%s) # 
 #    PARSE prefer %s NAME as cident // varname
 #    PARSE prefer %s NAME as "anystr"
 #
 #
 #

	# set dot_dot = ".. # comment EOLN # BOLN # ATTR '..' # NOTE # CODE_POS "
	# set d0 "d1" # n_bytes_t item # "d1" # THIS_t # OBJ_t # DOC_t # TREE # 
 #	d0_t d1;
# 	d0_t d1;
#	# { SCRIPT }

#	ca/PC_101/private # KEYS # CRL ?
#	POOL = ca PC_%s
#	ITEM = PC_%s == PC_101_at_home
#	CERT = HERE += CN # KEPT RSA[0] == CA_ZERO of_this_SCOPE # DB_ITEM # 
#	CERT = HERE += CN # KEPT RSA[1] == CA_of_this_SCOPE # DB_MATCH_DATA # 
#	CERT = HERE += CN # KEPT RSA[2] == CA of this C # CA_TWO = SUBS _list
#	CERT == CA of this C # CA_TWO = POOL # ITEM = CN # or_PARSE_CN_etc
#	CERT = label name for holder of RSA_SECRET ## 
#	CERT claims it holds own RSA(SECRET)

	set d1 "$CN" ;# CMNT # 
	set d2 [file join ca $d1 private] ;# 

	lappend cmd mkdir -p $d2
	# say_do $cmd
	# say_do [list chmod 700  $d2]
	file mkdir $d2
	file attributes $d2 -permissions 0700
	say_do [list ls -lad $d2]
}
proc UNUSED_mk_ZONE_ITEM_CN {CN} {
	# a ZONE is POOL("POOL") of ITEM_t ;# SUBLEX ITEM_t ITEM;
	# alias POOL
	# alias ITEM
	# alias CTOR CN ;# or ITEM # or THIS # or THAT # %s # new FUNC( ARGV )
	mk_POOL_ITEM_CN $CN 
}

proc mk_CA_dirs {CA_HERE} { 
# HERE = lookup $CA_HERE ;# NAME # CN # PARSE EXPR
# CA_LIST_of_ITEM # uses $CA_ZERO # "%s" # LINK #
# ARGV == "cn" "CA_SUBS" # ca # nickname for local scope # SUBS_t SUBS;

	# PARSER finds CN = (STR0) "CN" # lookup CN
	# CN == "CA_THAT_POOL" # often THIS_POOL

	# DB of NAME knows
	# .is_CA() .is_C() idx_as_C(u8 idx) # FEW CA
	# .is_C() C_from_idx( u8_idx ) # HERE
	# HERE NEAR : CA is C # RSA_SECRET_HOLDER
	# dirname = CN # dirname_from_CN $CN 
	# dirname = CN # dirname_from_CN( CN )
	# dirname = CN # dirname_from_( name ) 
	# CA _of_ SUBSCRIPTION_LIST # uses %s # 
	mk_C_dirs $CA_HERE ;# CA_ONE ;# ZONE_ITEM # SIGNED_BY #
}

proc mk_CA_machine CA_ZERO {
#	mk_CA_dirs ;# CA_LIST_of_ITEM # uses $CA_ZERO # "%s" # LINK #
	mk_CA_dirs $CA_ZERO
}

proc mk_C_Cert CN {
#	mk_C_dirs ;# C_LIST_ITEM # uses $CN # "%s" # LINK #
	mk_C_dirs $CN
}


proc mk_C_Cert_PC {IDX} {
	#
	# "PC_%s" ;# # NOT alias "PC_101_me_at_home"
	# CODE MODE # allow switch 
	# mk_C_Cert_PC    IDX
	# mk_C_Cert PC PC_IDX
	# 
	# mk_C_Cert_PC    101
	# mk_C_Cert    PC_101
	# 
	
	mk_C_Cert "PC_$IDX" ;# "PC_%s" ;# "PC_101" #
}

# _HERE # CTOR_WITHOUT_ARGV # possibly as a BASE class
proc mk_CA_machine_HERE {} {

	#
	# CA_ROOT = CA_ZERO # OFFLINE SECRET
	#
	mk_CA_machine CA_ZERO
	#
	# CA_ONE = CA # OFFLINE ELSEWHERE SECRET # PKI #
	#
	mk_CA_machine CA_ONE ;# += CA_ # obtain %s NAME from SCOPE
	#
	# C_PC_GPS = C # ONLINE SECRET # PROOF OF USE # PKI #
	# C_as_db_KEY # require X509 to be pre-registered with data in DB
	# check C_Cert was produced by CA as proved/indicated by CA_Cert
	# expect ( C_POOL C_ITEM ) as ( CA C ) # of CA SERVER CLIENT subs
	# 
	#
	mk_C_Cert_PC 101 ;# idx of ( POOL ITEM )
	mk_C_Cert_PC 102 ;# idx of ( POOL ITEM )
	mk_C_Cert_PC 103 ;# idx of ( POOL ITEM )
	# above PSG is from AS_FOUND module
	# TUPLE( POOL, ITEM ) ;# CODE_POINT #

}
#
# say_do false ;# shows exec throws when main returns non-zero
# exit
#
# SOME TEST EXPRS # 
# say_do env
# say_do {bash -c alias} ;# OK alias builtin # shows {conv str to list}
# say_do alias ;# OK alias is a /bin/sh builtin # proves exec runs a binary

mk_CA_machine_HERE 
say_do tree
