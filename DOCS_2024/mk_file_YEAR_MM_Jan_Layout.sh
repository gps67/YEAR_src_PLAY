# ! /bin/bash 
# ${HOME}/YEAR/DOCS/mk_YEAR_MM_Jan_Layout.sh

function T1()
{
	echo -ne "# T1 # "
	ls -ld $HOME/YEAR
}

# setup PATH
# load FUNCTIONS

# setup PATH to find useful dirs, and to FILTER PATH ## no dupes # drop list

# expect caller has PATH that works
# including . fns_XXXX for bash

# fns_var_add is a quick example of BASH calling PATH_TOOL.exec to set a VAR
# technique limited to 200 bytes of data_space ## on old UNIX DOS_BOX
# technique limited to 4K of data_space ## dunno on API connected CALLER // ME
# technique extended to API_STREAM with STO at both sides ENQ_ACK to SYNC_EDGE
# SYNC_EDGE is when we have all parameters, online access, cpu time, NOW
# TIME_BAND is freom EDGE_START to EDGE_STOP // option use of LHS_RHS EDGES;

# include "fns_var_add" // .sh AUTO_ADD AUTO_TRIM OMIT DIALECT OMIT
# include FILENAME //  ~/bin_LGX # because PATH_TOOL is a binary

. fns_VARS # UDEF and present on my system
# . fns_var_add # avail for others

var_add PATH \
	${HOME}/bin_NFS_down/HOME/bin \
	${HOME}/bin_NFS_down \
	/home/gps/bin_NFS_down/HOME/bin \
	/home/gps/bin_NFS_down \
#
#	/home/gps/2017/bin_NFS/HOME_bin \
#	/home/gps/2018/bin_NFS/HOME_bin \

## IMPORT LIST ## AUTO_MAINTAINED ## todo obvs
## IMPORT_LIST {
## 	## AUTO_MAINTAINED ## BASE_CLASS_INCL_LIST ##
##	## AUTO_INDENT ## was kept BOLN_NOSP flat_left
##	## AUTO_DATA_SOURCE ## GET AUTH {DONE} // SUBLEX USAGE {DONE} // DIALECT
## }

## load FUNCTIONS ## LIBR ##

. fns_FAIL
. fns_DIRS
. fns_DATE
. fns_ASKS
. fns_EDIT
. fns_BASE

fn_DATE_show_VARS() { # V A R S # BASH function # extends LIBR for now

	local VAR_LIST_ALL="
		DATE_YEAR
		DATE_MM
		DATE_DD
		DATE_Jan
		DATE_Tue
		TIME_HH
		TIME_MM
		TIME_SS
	"
	local VAR_LIST="${@:-$VAR_LIST_ALL}"
	for var_NAME in $VAR_LIST
	do
		eval local VALUE="\$$var_NAME"
		[ -z "$VALUE" ] && VALUE="''"
		echo "# var_NAME # $var_NAME # $VALUE #"

	done
}

# can change date -d if required #

# TEST # fn_DATE_get_VARS 2020-01-15 # _get_ new values for VARS using "$@"

	# fn_DATE_show_VARS 2020-01-15 # set date from ARGS
	# fn_DATE_show_VARS 
	# show the value we have set as current DATE
	echo "# DATE # $DATE_year_mm_dd $DATE_Jan $DATE_Tue DATE_STAMP_VAL"
	# echo "# TEST" fn_DATE_show_VARS DATE_year_mm_dd
	# fn_DATE_show_VARS # ALL
	false && \
	fn_DATE_show_VARS \
		DATE_year_mm_dd \
		DATE_Jan \
		DATE_Tue \
	#
# exit

	# PWD == ~/YEAR/DOCS/
	# PWD == ~/YEAR/DOCS_%s/ // %s YEAR / %d but might be "_YEAR_"

	TOPIC="DOCS"

# ~/YEAR/ is a convenience for scripts and me

# taking this as a common block, of DATE_EXPR [s]
# set -x
YEAR_MM_Jan="${DATE_YEAR}_${DATE_MM}_${DATE_Jan}"
TOPIC_YEAR_MM_Jan="${TOPIC}_${YEAR_MM_JAN}"
TOPIC_YEAR="${TOPIC}_${DATE_YEAR}"
# _year is the word "_YEAR", but _YEAR is reserved for _2001
year=YEAR
TOPIC_year_="${TOPIC}_${year}" 
HOME_YEAR="${HOME}/${DATE_YEAR}"
HOME_year="${HOME}/${year}" # already ~/YEAR
HOME_year_TOPIC_YEAR="${HOME}/${year}/${TOPIC_YEAR}"

# MOVE THIS to ANNUAL SETUP SCRIPT #

	# this is where you PATH YEAR
	ln_s YEAR_offline/ $HOME_YEAR
	# T1

	# update link to year change
	#cd_mkdir_FAIL ${HOME}/${DATE_YEAR} 
	cd_mkdir_FAIL ${HOME_year}
	# T1
	cd
	# echo "# or KEEP YEAR => YEAR_offl/"
	# ln_s_ask "${DATE_YEAR}/" "${HOME}/$year" # YEAR -> 2020/
	# ln_s_ask "YEAR/" "${HOME}/${DATE_YEAR}/" # 2020 => YEAR -> YEAR_offl

	cd_mkdir_FAIL ${HOME_year_TOPIC_YEAR} 

	# ~/201X/ is on NAS2 # some bounces to get to it

	# FAIL HERE

	cd_or_FAIL $HOME
	# cd_or_FAIL ${HOME_YEAR} 
	cd_or_FAIL ${HOME_year} 

	# ~/2018/TOPIC_2018/
	cd_mkdir_FAIL ${HOME_year_TOPIC_YEAR} 

# ~/2018/TOPIC_2018/TOPIC_2018_01_Jan/
	rmdir $TOPIC_YEAR_MM_Jan 2>/dev/null # if empty allows script debug
# cd_mkdir_FAIL $TOPIC_YEAR_MM_Jan
	cd_mkdir_FAIL $YEAR_MM_Jan

# ln_s_ask $TOPIC_YEAR_MM_Jan/ TOPIC_ 
# ln_s_ask $TOPIC_YEAR/TOPIC_ ../TOPIC_
# ln_s_ask ${TOPIC}_${DATE_YEAR}/ $HOME/$year/TOPIC_YEAR
# ln_s_ask ${TOPIC}_${DATE_YEAR}/ $HOME/$year/${TOPIC}
# ln_s_ask YEAR/TOPIC_/ ${HOME}/TOPIC_


# NO # not set there # fn_BASE_filename $TOPIC_YEAR_MM_Jan/.varfile
# YES $PWD/.varfile # so depends on above cd_mkdir_FAIL ${HOME_year_TOPIC_YEAR}

# BASH you idiot, "bash: local: can only be used in a function"
# OK it is not effective, but not unusable! So correct, it is wrong!
# local
BASE_filename=`fn_BASE_filename ""`
fn_BASE_edit "$BASE_filename" TASK TaskName
filename="${DATE_YEAR_MM_DD}_${TASK}.txt"
function mk_file()
{
	local filename="${1:-filename}"
	[ -f $filename ] && return
	DIR1=`dirname $filename`
	DIR1="$YEAR_MM_Jan"
	FILE1=`basename $filename`
	echo -e "\t${DATE_YEAR_MM_DD}\n\n\t${DIR1}\n\t${FILE1}\n\n" >> "$filename"
}

mk_file "$filename"
vi -c 'set fileformat=dos' ${filename}






exit
