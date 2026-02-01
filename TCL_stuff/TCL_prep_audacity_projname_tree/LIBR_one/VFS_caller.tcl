# this does not provide a VFS
# this uses a VFS, or actual real FS
# VFS_client # all that this project needs from VFS

 proc VFS_mkdir { DIR1 } {
 	if [file isdirectory $DIR1 ] {
	#	puts "# ok # $DIR1"
		return 1
	} {
		puts "file mkdir $DIR1"
		file mkdir $DIR1
	}
 }

 proc VFS_file_text { FILE TEXT } { ;# rewrite entire file
	# wb for binary write
 	set fd [ open $FILE w ]
	puts $fd $TEXT
	close $fd
 }

