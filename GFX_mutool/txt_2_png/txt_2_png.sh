#!/bin/bash


. fns_VARS
# var_add PATH $HOME/YEAR/src_build/JS/mupdf/build/release
var_add PATH $HOME/G//prefix/bin

# PATH=$PATH:/home/gps/YEAR/src_build_2019/JS/mupdf/build/release

#  mutool run txt_2_png.js "$@"

TXT=test1.txt

# Import
	. fns_DIRS	# cd_or_FAIL

# Testfile

	# TXT=test1_a.txt
	TXT=test1_az.txt
	TXT=test1.txt # SYMB_LINK dir/file where .txt file is found 

# ARGV # TXT

	TXT="${1:-$TXT}"

# test1.txt -> USE THE SECOND NAME

	[ -L $TXT ] && TXT=`readlink $TXT` ## improves filename.txt

# FILENAME # PNG

##
	PNG=`basename $TXT .txt`.png
	PDF=`basename $TXT .txt`.pdf

## to allow -ANYSTR- ensure forewarning VAR( FILENAME ) VAR( PNG }

	false && \
	# cd_or_FAIL /$HOME/YEAR/src/PLAY/GFX_mutool/view_page_of_diag_as_pixmap

	# all PNG's in this dir are recreated overnight (archive)
	# only test1 
	

	# TXT to PDF # kills PDF
	# TXT to PNG # kills PNG
	# TXT to GUI # needs no files # keep interm

	rm $PDF
	rm $PNG

	echo >&2 "# mutool run txt_2_png.js TXT # $TXT"
#	mutool run txt_2_pdf.js $TXT # $PDF # it computes itself 
	mutool run txt_2_png.js $TXT # $PDF # it computes itself 
	[ -f $PDF ] && mupdf $PDF
#	[ -f $PDF ] && mutool run view_pdf.js $PDF
	# $PNG # $OBJ_PIXMAP # TODO shared mmap file pixmap objects .sh
	# $OBJ_PIXMAP # TODO shared mmap file pixmap objects link_name -> store`
	# [ -f $PNG ] && gm display $PNG
	[ -f $PNG ] && m $PNG ## // local media m

	# [ -f $PNG ] && gm display $PNG
