#!/usr/bin/env tclsh
package require tdom

set xml {
<NODE>
 <field>f</field>
</NODE>
}

proc TRIER {args} {

	# warning tis is a mess
	# upvar saves uplevel
	# saves list manipulation # "asif concat" #
	#
        upvar var var
        upvar doc doc
        upvar root root

#	set code [linsert $code 0 uplevel]
	set code $args
        puts "CODE {$code}"
 #       set catch_ret  [uplevel catch $code retval opts]
        set catch_ret  [catch $code retval opts]

        if $catch_ret {
                set word "FAIL"
        } {
                set word "PASS"
        }
        # pass #
        # puts "$word retval {$retval}"
        # retval {can't read "unknown": no such variable}
        puts "$word opts {[dict get $opts]} retval {$retval}"
	return $retval
}

TRIER puts OK

TRIER dom parse $xml doc
puts "doc $doc"
$doc documentElement root
puts "root $root"
TRIER $root nodeType 
TRIER $root nodeName
TRIER $root nodeValue
foreach child [TRIER $root childNodes] {
	TRIER $child nodeType 
	TRIER $child nodeName
	TRIER $child nodeValue
}
