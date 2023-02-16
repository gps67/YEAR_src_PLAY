#!/usr/bin/env tclsh
#!/tools/x86_64/tcl/bin/tclsh

package require sqlite3


puts "seems there is no .dump command, use sqlite3 command"

puts [exec sqlite3 /tmp/testdb .dump]
