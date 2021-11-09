#!/bin/bash

#  # this script calls
#  mutool run txt_2_png.js "$@"
#  # then it views to output
#  # so both .sh and .js compute the .png filename from the .txt arg
#  # both independently

# Import
	. fns_VARS	# var_add PATH $DIR_bin
	. fns_DIRS	# cd_or_FAIL
# PATH
#	the mupdf binary is ... installed ? built ?
#	pre installed or installed 
	var_add PATH $HOME/G//prefix/bin
#	var_add PATH $HOME/YEAR/src_build/JS/mupdf/build/release


# Testfile

	# default value for testing, last wins
	TXT=test1.txt
	TXT=test1_az.txt
	TXT=test1.txt # SYMB_LINK dir/file where .txt file is found 
	TXT=test1_u32_Decoder.txt # SYMB_LINK dir/file where .txt file is found 

# ARGV # TXT

	# the TXT file that will be converted to PNG
	TXT="${1:-$TXT}"

# symb linked test1.txt -> USE THE SECOND NAME

	[ -L $TXT ] && TXT=`readlink $TXT` ## improves filename.txt

# FILENAME # PNG # PDF #

##	.js recomputes this too :-)
##	we need PNG to view its result
	PNG=`basename $TXT .txt`.png
	PDF=`basename $TXT .txt`.pdf

## to allow -ANYSTR- ensure forewarning VAR( FILENAME ) VAR( PNG }

# OLD	false && \
# OLD	# cd_or_FAIL /$HOME/YEAR/src/PLAY/GFX_mutool/view_page_of_diag_as_pixmap

	# TXT to PDF # kills PDF # ie overwrites it ' or deleted it
	# TXT to PNG # kills PNG
	# TXT to GUI # needs no files # keep interm

	# this makes a syntax error failure more visible
	# but at the risk of deleting something you then cannot create
	# it also deletes .pdf that included .png by same name !
#	rm $PDF # so dont # residual from TXT_to_PDF script usage
	rm $PNG

	# log action
	echo >&2 "# mutool run txt_2_png.js TXT # $TXT"

# generate
#	mutool run txt_2_pdf.js $TXT # $PDF # reminder of other functionality
	mutool run txt_2_png.js $TXT # $PNG # generate PNG from TXT

# view a file we did not create ?
 false && \
	[ -f $PDF ] && mupdf $PDF # plain viewer
#	[ -f $PDF ] && mutool run view_pdf.js $PDF # scripted viewer ?

# view
	# [ -f $PNG ] && gm display $PNG
	[ -f $PNG ] && m $PNG ## // local media m

