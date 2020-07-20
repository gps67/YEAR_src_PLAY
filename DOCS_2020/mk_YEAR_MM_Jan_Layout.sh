#/bin/bash 
# ${HOME}/YEAR/DOCS/mk_YEAR_MM_Jan_Layout.sh


# expect caller has PATH that works
# including . fns_XXXX for bash

# fns_var_add is a quick example of BASH calling PATH_TOOL.exec to set a VAR
# technique limited to 200 bytes of data_space ## on old UNIX DOS_BOX
# technique limited to 4K of data_space ## dunno on API connected CALLER // ME
# technique extended to API_STREAM with STO at both sides ENQ_ACK to SYNC_EDGE
# SYNC_EDGE is when we have all parameters, online access, cpu time, NOW
# TIME_BAND is freom EDGE_START to EDGE_STOP // option use of LHS_RHS EDGES;
. fns_var_add # ~/bin_LGX # because PATH_TOOL is a binary
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

. fns_FAIL
. fns_DIRS
. fns_DATE
. fns_ASKS
. fns_EDIT
. fns_BASE

# can change date -d if required #

# TEST # fn_DATE_get_VARS 2020-01-15 # _get_ new values for VARS using "$@"

	# fn_DATE_show_VARS 2020-01-15 # set date from ARGS
	# fn_DATE_show_VARS 
	echo "# DATE # $DATE_year_mm_dd"
	fn_DATE_show_VARS DATE_year_mm_dd
# exit

	# PWD == ~/YEAR/DOCS/
	# PWD == ~/YEAR/DOCS_%s/ // %s YEAR / %d but might be "_YEAR_"

	TOPIC="DOCS"

# ~/YEAR/ is a convenience for scripts and me
YYYY=${DATE_YEAR}

# taking this as a common block, of DATE_EXPR [s]
# set -x
TOPIC_YEAR_MM_Jan="${TOPIC}_${YYYY}_${DATE_MM}_${DATE_Jan}"
TOPIC_YYYY="${TOPIC}_${YYYY}"
TOPIC_year_="${TOPIC}_YEAR" 
HOME_YYYY="${HOME}/${YYYY}"
HOME_year_="${HOME}/YEAR"
HOME_YEAR_TOPIC_YYYY="${HOME}/YEAR/${TOPIC_YYYY}"

# TREE ROOTED $HOME/YEAR/
HOME_YEAR="${HOME}/YEAR"



# update link to year change
cd_mkdir_FAIL ${HOME}/${YYYY} 
cd
ln_s_ask "${YYYY}/" "${HOME}/YEAR" # YEAR -> 2020/

cd_mkdir_FAIL ${HOME_YEAR_TOPIC_YYYY} 

# ~/201X/ is on NAS2 # some bounces to get to it

# FAIL HERE

cd_or_FAIL $HOME
cd_or_FAIL ${HOME_YEAR} 

# ~/2018/TOPIC_2018/
cd_mkdir_FAIL ${HOME_YEAR_TOPIC_YYYY} 

# ~/2018/TOPIC_2018/TOPIC_2018_01_Jan/
rmdir $TOPIC_YEAR_MM_Jan 2>/dev/null # if empty allows script debug
cd_mkdir_FAIL $TOPIC_YEAR_MM_Jan

# ln_s_ask $TOPIC_YEAR_MM_Jan/ TOPIC_ 
# ln_s_ask $TOPIC_YYYY/TOPIC_ ../TOPIC_
# ln_s_ask ${TOPIC}_${YYYY}/ $HOME/YEAR/TOPIC_YEAR
ln_s_ask ${TOPIC}_${YYYY}/ $HOME/YEAR/${TOPIC}
# ln_s_ask YEAR/TOPIC_/ ${HOME}/TOPIC_


# NO # not set there # fn_BASE_filename $TOPIC_YEAR_MM_Jan/.varfile
# YES $PWD/.varfile # so depends on above cd_mkdir_FAIL ${HOME_YEAR_TOPIC_YYYY}

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
	DIR1="$TOPIC_YEAR_MM_Jan"
	FILE1=`basename $filename`
	echo -e "\t${DATE_YEAR_MM_DD}\n\n\t${DIR1}\n\t${FILE1}\n\n" >> "$filename"
}

mk_file "$filename"
vi -c 'set fileformat=dos' ${filename}






exit
