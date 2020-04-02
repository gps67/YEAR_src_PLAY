#!/bin/bash

echo >&2 CMD $0 ARGS "$@"

 # LOAD VARS #

 # local # if you got em # bash FAILS when used in global scope by accident

 # local # to this PID.ENV # and_then_forked # FILTER OFF ON=mk_ENV_for_fork
 
 # local to this PID	<-- vars evaporate on exit # [ SCRUB ]
 # local to this DIR	<-- CSR in DIR of TREE
 # local to this SESS	<-- SOCKET_to_and_from_API CACHE_VARS $CTXT_idx
 # pre_load_CTXT SESS = CTXT.SESS # SESS is API
 # SESS is API # MUX_FD_STREAM_SCRIPT_LINE _PSG _P0P2_MMAP
 # API has STO P0P2 in MMAP
 # MMAP SEGMENT N_BYTES FRAGMENT BUFFER and other ALIASES
 # above list are CT # RT adds a LIST of MORE(" ... ")
 # 

	USB_iso=USB_iso
	USB_iso='${ISO}'

# uncomment says you cant
#
#	echo "'${ USB_iso }'"
#	# ./test_SP_in_BRACES.sh: line 24: '${ USB_iso }': bad substitution
#
# so BASH could be a tad more relaxed
# so VIEW could add a relaxed "^ # SP %s WS \n"
# nb SCAN_BLEX Q2 ^ # SP %s WS \n" // ^ MATCHES BOLN // WS == SP || WS

#	echo "'${ USB_iso }'"
	echo "'\${ USB_iso }' # FAILS #"
	echo "'${ USB_iso }'"
#	# ./test_SP_in_BRACES.sh: line 24: '${ USB_iso }': bad substitution

#	echo "'${USB_iso}' # PASSES #"
set -x
	echo "'${USB_iso}'"
 
