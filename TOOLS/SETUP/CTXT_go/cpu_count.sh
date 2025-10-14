# . go__fns_2025
# man nproc

set_CPU_COUNT_J() # MINUS 1 usually 
{
	F=/proc/cpuinfo	# count them
	# SIDE EFFECT set_AVAR wxports GLOBAL variable as_far_as fork_exec
	# certainly not LOCAL
	export CPU_COUNT_J=1
	
	CPU_COUNT_J=$(
		# todo # rewrite to use BASH regexp parsing #
		# rewrite gawk # one sharp tool # readable #
		# LINE_SPLIT_on_SP1 # or_GAP #
		# $0 == "processor"
		# $2 == ":"
		# $3 += INT_CARDINAL_1_PLUS # N_CPU # THREAD #
	gawk -- '
	# { #
	 BEGIN {
	 	N_CPU = 0
	 }

	 $1 == "processor" {
		N_CPU ++
	 }

	 END {
		if(0) {
			N_CPU --
		}
	 	if( N_CPU < 1 ) {
			N_CPU = 1
		}
		printf( "%d\n", N_CPU )
	 }
	 # } #
	' $F # -- SCRIPT_TEXT TEXT_DATA_FILE_NAME

	#	grep "-c processor" $F # count 
	#	# OK # grep "processor" $F | tail -1 | gawk '{ print $3 }'
	)
	# avoid -j 0
	# MINUS 1 #

	CPU_COUNT_J=$(( $CPU_COUNT_J - 1 ))
	# LIMIT MINIMUM at least 1
	[ $CPU_COUNT_J < 1 ] && CPU_COUNT_J=1

	# reduce -j 3 from 4 # also add NICE # get from Kernel
	# grep tail and gawk are a lot when LIBR_AVAIL
	# OK RTFM someone write a BASH and ZSH SCRIPT LINE
	# currently LIMIT 1 MINIMUM 1 # OPTION SWITCH_VAL
	# case 0: # AUTO ALIAS %s_one
	# case 1: # AUTO ALIAS %s_one %s_two DIALECT USAGE
	# USAGE # on GROW shuffle idx map keeping to the RULES
	# USAGE MATCH # MIRROR # run LOCAL code for REMOTE CLIENT
	# SECURE MODEL # CACHE can do all the work from INIT_VAL DECL SPEC
	# DECL SPEC # C # int_enum # auto enum # SPEC_DECL_t SPEC;
	# DIALECT _t DEPTH crosses a threshold # MINI WORLD EVENT DECL DATA
	# EVENT # on_EVENT_FFFF( ARGS ) { SCRIPT } # GEN_C_CODE 
	# case:2: %s
	# only keep 1 back from 2 on
}


set_j_JOBS() {
	set_CPU_COUNT_J # ALIAS for JOBS but daft and unique
	j_JOBS="-j $CPU_COUNT_J"
}


set_j_JOBS


echo j_JOBS "'$j_JOBS'"
