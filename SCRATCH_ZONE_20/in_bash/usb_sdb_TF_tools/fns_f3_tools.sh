#  fns_f3_tools.sh

log_ask_do()
{
	# -a LOGFILE # append
	local A=""
	if [ -a = "$1" ] 
	then
		A="-a"
		shift
	fi

	# LOGFILE must come first # after -a
	local file="$1"
	shift

	if yes_no RUN "$@"
	then
	 (
		echo
	 	date
	 	set -x
		"$@"
	 	date
	 ) 2>&1 | tee $A $file
	fi
}

say_do()
{
	echo >&2 "# RAN # $*"
	"$@"
}

check_mounted_as_label()
{
	echo "# media_me_label ${media_me_label}"
	echo "# sdb1_of_card /dev/${sdb1_of_card}"
	if mount | grep ${media_me_label}
	then
		echo 
		return 0
	fi

	if is_mounted_card
	then
		echo >&2 "# FAIL # card alrady mounted elsewhere" 
		return 1
	fi

	ask_do \
	mount /dev/${sdb1_of_card} ${media_me_label}

	if mount | grep ${media_me_label}
	then
		echo "# OK #"
		return 0
	else
		echo "OVERWRITE DISK must be mounted as $media_me_label"
		return 1
	fi
}

is_mounted_card()
{
	mount | grep ${dev_sdb1_of_card} > /dev/null # any dir
}

umount_card()
{
	if is_mounted_card
	then
		say_do umount ${dev_sdb1_of_card}
		sync
	else
		echo >&2 "# OK # not mounted # ${dev_sdb1_of_card}"
		sync
	fi
}

eject_card()
{
	umount_card
	say_do eject ${dev_sdb_of_card}
	sync
}

check_label_11()
{
	local LEN=`expr length "$G64_media_label"`
	if [ $LEN -lt 1 ]
	then
		# nb EMPTY str is refused # 
		echo >&2  "# FAIL # label is '$G64_media_label' len is $LEN"
		return 1
	fi
	if [ $LEN -gt 11 ]
	then
		# longer than 11
		echo >&2  "# FAIL # label is $G64_media_label len is $LEN # max 11"
		return 1
	fi
	# TODO # regexp [0-9A-Za-z_]{1,11}
	echo >&2  "# OK # label is $G64_media_label len is $LEN"
	return 0
}

load_media_label()
{
	fn_BASE_rd default G64_media_label
	check_label_11
	# echo "# LABEL # $G64_media_label"
}

edit_media_label()
{

	echo "#"
	echo "# The media label is in the .varfile"
	echo "# The media label is in the VFAT header"
	echo "# The media label is also the mount point /media/me/LABEL/"
	echo "# The media label is upto 11 chars, UPPER, OK _, ... unchecked"
	echo "#"
	
	while :
	do
		# VARFILE default == .varfile # ./.varfile
		# VARNAME G64_media_label # is a shell_env_var #
		# to provide a default VAR_VAL add it as extra args or in VAR
		# value found in file will override ENV_VAR_VAL
		# edited value is placed in FILE # BEFORE CHECKING IT # ! #
		fn_BASE_edit default G64_media_label
		check_label_11 && return 0
		# error message hints already from check #
		# just loop editing until happy
		# nb EMPTY str is refused # 
	done
}
