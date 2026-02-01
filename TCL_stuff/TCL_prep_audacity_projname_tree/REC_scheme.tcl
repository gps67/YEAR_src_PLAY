
  package require gps_one ;# sets paths to find more stuff and LIBR
# package require REC_one ;# module for REC

# set ::REC_dir_zero /home/gps/REC/REC_2025/REC_AUD_2025/REC_AUD_2025_08_Aug
# set ::REC_dir_zero /home/gps/REC/REC_2025/REC_AUD_2025/REC_AUD_2025_09_Sep
set ::REC_dir_zero /tmp

set ::L1_N 0

proc MODE { M } {
	switch $M {
	 TODO {
	 	set ::SKIP 0
	 }
	 SKIP -
	 DONE {
	 	set ::SKIP 1
	 }
	}
}

proc L0 { dir_over d0 args } {
	if {[ARG_is_default $dir_over]} { set dir_over $::REC_dir_zero }
	set title "$args"
	set d0 [filename_from_text "$d0"] ;# VFS_clean_name "$d0"
	set ::L0_path_dir $dir_over/$d0  ;# inside proc no use of ::REC_
	puts "---- '$d0' $title "
	puts "---- $::L0_path_dir ---- "
}

proc get_L1_NN {} { format %02d $::L1_N }
proc L1 { _LN args } {

	# BOOK BOOK_SECTION CHAPTER SECTION_S1 SECTION_S2 # OPTION SECTION_S3 # often no S3 used
	# L1 H2 T3 # LEVEL HEADING TITLE # L1 L2 L3 # H1 H2 H3 # T1 T2 T3 # DIALECT MIXES L1 H2 T3
	# CODING using VA_NAME "::L1_N" # a global var_name "L1_N" # SCOPE this task count of L1 available
	# a global var_name "H2_N" # N of H2 # ALLOC idx = N ++ # spelling of idx is "H2_idx" N is "H2_N"
	# a global for this TASK # list H2 is specific to each L1 # SESS is global # SESS.H2_idx # curr H2

	# at start of L1 call this function
	# it creates a new L1 as the new global current SESS CTXT
	# at start of next L1 call this function
	# is does it again as L1_idx == 2 # SKIPPED_ZERO for human sense # used as GROUP_identity see M1
	# ZERO_means_NULL # NULL_means_various things, sometimes DEFAULT sometimes EMPTY
	# ZERO_means_PLUS # no special treatment for idx_0 # intentionally vanilla
	# PLUS_means_ITEM
	# MINUS_means_EXPR # possibly ENQ_ACK needing TRACKING

	# VIEW_PLUS # reserve ZERO for ZERO ONE TWO _4
	# AUTO ADD to all paths

	# at start of H2 call that function 
	# on_H2(SESS) # ALLOC H2_idx = H2_N ++ # VIEW idx_zero_based name_ONE_TWO

	# at start of T3 call that function 
	# on_T3(SESS) # ALLOC T3_idx = T3_N ++ # VIEW idx_zero_based name_ONE_TWO

	# this is one way to number sections
	# another would be a class that does that along wi keeping a stack
	# tree builder
	
	incr ::L1_N 
	set  ::L1_NN [get_L1_NN]

	set ::H2_N 0
	set ::T3_N 0 ;# optional

	if {[ARG_is_default $_LN ]} {
		set _LN $::L1_N 
	}

	if { $::L1_N == $_LN } {
		# OK # alignment match
	} else {
	  # OOPS # alignment mis-match
	  puts "expected { ::L1_N == $_LN } got { $::L1_N != $_LN }" 
	  puts "args $args"
	  # change to users number will fail twice
	  # REQUIRE $_LN is a number # above INCR would have failed
	  set ::L1_N $_LN
	  set ::L1_NN [get_L1_NN]
	}
	set title "$args"
	set ::L1_title "$_LN $title"
	set ::L1_dir [filename_from_text "$::L1_title"]

	puts "---- $::L1_dir ---- "
	# puts "N $::L1_NN TITLE $::L1_title"
}

proc H2 { _H2 args } {
	incr ::H2_N 
	set ::T3_N 0
	set L_H [ filename_from_text $::L1_N $::H2_N ]
	set ::H2_title "$args" 
	set ::H2_dir [ filename_from_text $L_H $::H2_title ]
	if $::SKIP { set M SKIP } { set M TODO }
	puts "$M $L_H $::H2_title  ---- "
}

# H2 title is basically ignored -- just H2_N

proc T3 { _T3 args } {
	incr ::T3_N 
	set ::L_H_T [ filename_from_text $::L1_N $::H2_N $::T3_N ]
	set d1 $::L1_dir
	set ::T3_title "$args" 
	set ::T3_dir [ filename_from_text $::L_H_T $::T3_title ]
	if $::SKIP { set M SKIP } { set M TODO }
	puts "${M} ${::L_H_T}_ $::T3_title  ---- "
	if $::SKIP { return }
	calc_file_name
	VFS_mkdir $::VFS_dir_data
	GEN_aup_file
	# exit 2
}

proc P { args } {
	# each L1 has a P
}

proc calc_file_name {} { # maybe pass RET_VAL or CALLERS_ID and CREDS

#
# OPTION # H2 is a subdir #
# OPTION # H2 not a subdir #
#
#   maybe create a file.txt so that it appears in ls
#

# OPTION # T3_title is REC_TRACK
# OPTION # entire .aup3 is one TRACK

	set ::VFS_dir_L0_L1_H2 $::L0_path_dir/$::L1_dir/$::H2_dir
	set ::VFS_file_part $::T3_dir 

	set ::VFS_file_aup $::VFS_dir_L0_L1_H2/$::VFS_file_part.aup
	set ::VFS_dir_data $::VFS_dir_L0_L1_H2/$::VFS_file_part
	set ::VFS_file_sh  $::VFS_dir_L0_L1_H2/$::VFS_file_part.sh
	# caller does this # VFS_mkdir $::VFS_dir_data

	set ::REC_PROJ  $::VFS_file_part ;# T3_dir == VFS_dir_data.tail

	set ::REC_YEAR 2025
if 1 {
#	throw undoable
	set ::REC_ARTIST $::L1_title
	set ::REC_ALBUM  $::H2_title
	set ::REC_TRACK  $::T3_title
} {
	set ::REC_ARTIST $::L1_title
	set ::REC_ALBUM  $::T3_title
	set ::REC_TRACK unused
}

	show_var ::VFS_file_aup 
	show_var ::REC_ARTIST
	show_var ::REC_ALBUM
	show_var ::REC_TRACK 
	show_var ::REC_YEAR
	show_var ::REC_PROJ 
	
}
 
