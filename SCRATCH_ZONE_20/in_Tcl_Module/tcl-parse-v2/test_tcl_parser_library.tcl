#!/usr/bin/env tclsh

proc step args {
	puts "# call:  $args"
	if [catch { eval $args } msg] {
		puts "# error: $msg"
		flush stdout
		return $msg
		error $msg
	} else {
		puts "# retval: $msg"
		flush stdout
		return $msg
	}
}


namespace eval tokeniser {
# heres a way of avoiding "global tcl_patchLevel" or need for uplevel
step set ::tcl_patchLevel
step load ./tcl_parser_library.so Parser_library
# no error, no action
step load "" Tokeniser
step load "" Tokeniser
# but name was recognised
step load "" nonexist
# step info commands

# step tokeniser::new parser1
# step tokeniser_new parser1

# step tokeniser::new parser1	;# probably a global!
  step tokeniser::new tokeniser::parser1
# step new parser1		;# didnt load into namespace?
# step tokeniser_new parser1

proc pp_word {word indent} {
	foreach {type text subs} $word {
		if {$type=="TCL_TOKEN_SIMPLE_WORD"} {
		 	set text "(( $text ))"
		}
		if {$type=="TCL_TOKEN_WORD"} {
		 	set text "(( $text ))"
		}
		puts "$indent $type $text"
		set indent2 "$indent |  "
		foreach part $subs {
			pp_word $part $indent2
		}
	}
	flush stdout
}
proc pp_script script {
	foreach command $script {
		foreach {cmnt cmd args } $command {
			puts "cmnt = $cmnt"
			puts "cmd = $cmd"
			puts "word\[0..[llength $args]] "
			set i -1
			foreach arg $args {
				incr i
				# puts "word\[$i] "
				set indent " | "
				pp_word $arg $indent
			}
		}
	}
	flush stdout
}
proc demo script {
	pp_script [ parser1 parse_script $script ]
}

puts closed

demo {
	# simple 3 words
	set a 0

	# what happens to the quotes?

	set x "some string"
}
demo {
	# command name is quoted using braces
	{a}
}
demo {
	# surrounding double quotes go
	# and contents is still a "simple" word
	
	"{{a}b{c}{d{e}f}}"
	"\{a\}b\{c\}\{d\{e\}f\}"
	"\{a\}b\{c\}[added $ignored]\{d\{e\}f\}"
}
demo {
	set x $a.b.c.d
}
demo {
	set x $a
}
demo {
	set x "$a"
}
demo {
	set x "$b(xxx)"
}
demo {
	set c "{$a} $b $c(xxx) $d($e)"
}
demo {
	set c [list {$a} $b $c(xxx) $d($e)]
	# NB first EOLN kept, following dropped
}
demo {	set a(b) ${pfx_$c}(xxx) }
## namespace ##
}
