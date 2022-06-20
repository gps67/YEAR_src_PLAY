#!/usr/bin/env tclsh 

proc TRIER { code } {

	upvar var var

	set catch_ret  [catch $code retvar opts] 

	if $catch_ret {
		set word "FAIL"
	} {
		set word "PASS"
	}
	# pass #
	puts "-TRIER-"
	puts "CODE {$code}"
	puts "$word retvar {$retvar}"
	# retvar {can't read "unknown": no such variable}
	puts "$word opts {[dict get $opts]}"
}

TRIER "set unknown"
TRIER "set known ok"
TRIER { set key "a b" ; set var($key) val ; puts "VAR == $var($key)" }
TRIER { set key "a b" ; set var($key) val ; set var($key) }
TRIER { set key "a b" ; set var($key) val ; array get var }
TRIER { set key " a b " ; set var($key) val ; set var($key) }
TRIER { set key {[ a ] b } ; set var($key) val ; set var($key) }

TRIER {array get var}

# its the last one that has retvar {b"}
# maybe its the parser
# I THINK I had this - not from data in source
# maybe that was passaged through back-via-source-and-reparse somehow
# my aim is to have "completely safe" spaces in KEYS # and other chars
TRIER { set key "a b" ; set var($key) val ; set var("a b") }
