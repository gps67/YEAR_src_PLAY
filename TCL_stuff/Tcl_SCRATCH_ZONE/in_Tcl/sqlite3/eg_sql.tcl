#!/usr/bin/env tclsh
#!/tools/x86_64/tcl/bin/tclsh
# using absolute tcl path to test that it is used
# when properly installed the default PATH found one should have sqlite3

puts "tcl_version $tcl_version"


# to build did # cd_mkdir obj/tea # $src/tea/configure ... ; make install
# that installs into the tcl tree found (and other to the libr tree!!)
#
# /tools/x86_64/tcl/lib/sqlite3.40.0
#	pkgIndex.tcl
#	libsqlite3.40.0.so

package require sqlite3

# this will then fail usage - but command was there and invoked

set VN [sqlite3 -version]
puts "sqlite3 -version => $VN"

puts https://www.sqlite.org/tclsqlite.html

sqlite3 db1 /tmp/testdb

db1 eval {CREATE TABLE t1(a int, b text)}

# Query results are returned as a list of column values. If a query requests 2 columns and there are 3 rows matching the query, then the returned list will contain 6 elements. For example:

db1 eval {INSERT INTO t1 VALUES(1,'hello')}
db1 eval {INSERT INTO t1 VALUES(2,'goodbye')}
db1 eval {INSERT INTO t1 VALUES(3,'howdy!')}
set x [db1 eval {SELECT * FROM t1 ORDER BY a}]
# The variable $x is set by the above code to
# 1 hello 2 goodbye 3 howdy!

puts "x $x"

db1 .dump
