#	READ_ME.txt

# TODO #
 ask_do fetch_latest_every_mid_month_already_2_weeks_late_monthly

 	asl_do fetch_if_modified 
 	# _latest_every_mid_month
	# _already_2_weeks_late_monthly

READ_ME.txt		    range_guess       check_env_debian
PIO			    range_guess_prev  plot_all_using_gnuplot5.sh
sea_ice_vol_range.png	    FTP_	      plot_range
tmp			    OUT		      show_as_dates_every_day.tcl
sea_ice_vol_zoomed_out.png  README.txt	      rate37.py
plot_daily_volume.awk	    _OLD_	      show_as_dates.tcl
plot_min_max_range	    calc_ranges.tcl   DATA_SOURCE_link.html
DATA_COMMENT.txt	    rm_tmp_star

# SCRIPT FILE # plot_all_using_gnuplot5.sh
# SCRIPT_FILE # "plot_app_using_%s" "plot_all_using_gnuplot5.sh" AUTO_MATCH
# LEX # FILE_NAME or _EXPR /* EXPR_SPELLING_ARGV EXPR = "FILE_NAME in TEMPLATE FILE_NAME"
# EXPR # PSG_CXX_operators_EXPR # AUTO deduct EXPR as normalised EXPR_TREE
# TREE # PSG_DATA_SPEC _INST # AUTO_LANG deduct _INST is _NOUN _%s _ALIAS "ITEM"
# EXPR # CODE_POINT == "{ FILE XPOS }" # API_SAYS XPOS_is_FILE_OFFS
# u48 # ADDR = EXPR # CODE_POINT == "{ FILE XPOS }" # API_SAYS XPOS_is_FILE_OFFS
# u48_ALIAS ADDR = MATCH CODE PATTERN INSTANCE ALIAS 
# u48_ALIAS ADDR = CODE MATCH PATTERN INSTANCE ALIAS 
# u48_ALIAS ADDR = CODE MATCH PATTERN INSTANCE ALIAS 
# u48_ALIAS ADDR = ITEM MATCH INSTANCE PATTERN ALIAS # AUTO_PARSE_MATCH MATCH
# u64 ALIAS ITEM_EXPR ITEM = ADDR # STRONG_TYPE_AUTO_CAST_API_MSG_EXPR
# u16_u48 == u32_u32 # ACCESS_MODE_ARM_32_LO_HI 
# LOHI u8_u8 # u16_u8_u8 # u8_u24__PAIR # ALIAS PAIR ITEM[2]
# ARM_32_LOHI
# # WHY LOHI # a CPU64 will have fast u8_from_u64 BYTE_A BYTE_B ALIAS
# # WHY LOHI # CPU_GENERAL_REGISTER # BYTE_B # LIBR += "BYTE_B" # AUTO LOOKUP NOUN
# # WHY LOHI # DECODE BYTE_A BYTE_B WORD_u64 # ALIAS "ITEM" "_u64" //
# # AUTO "_u64" GEN "u64_t _u64 ;// CMNT" # PARSER MUST GEN CXX
# GEN_GXX # RUN_CXX # SESS # SPECS # 
# SCRIPT # FILE # XPOS # CODE_POINT # CPOS # CPOS_is_OFFS # OFFS #
# LOGIC # each MENTION is a SPETTING_STONEor FOOT_PRINT
# MENTION # DECL GEN at XPOS == "CODE_POINT";
# in LANGUAGE "%s" EA_from_SPELLING ITEM == "%s" ITEM # OBJ.item # ALIAS #
# LANGUAGE uses CXX TREE of SPEC DATA CALL EVAL # UDEF_EVAL # based on EVAL_t
# EVAL_t # ITEM-t EA # EA is name as cident99 STR0 # 
# EVAL_t # ITEM-t EA = "%s" # EA_is_STR0_CIDENT_utf8_usually # EA #
# ITEM_t & ITEM = EA_EXPR ARGS # "{ STR0_t * STR0 }"
# ITEM_t & ITEM = "# name as cident99 STR0 #"
# DATA_CONV # EA_is_PTR _SPEC #
# OPT GEN # _SPEC_t & SPEC = DECODE_u8_u8_u16 #
# OPT GEN # _DECODE u8_u8_u16 # u16_is_ALIAS_u32_CPU_REGISTER
# OPT GEN # _DECODE u8_u8_u16 # ITEM_t & PTR_to_ITEM #
# OPT GEN # _DECODE u8_u8_u16 # EA_ITEM_t EA_ITEM #
# class EA_ITEM_t : public u32_WORD { u32 DATA_VAL } CSR.CPU_REGISTER_in_u32_mode
# SCOPE_BUBBLE # ARGV calls FUNC("FUNC_NAME") == ARGV[0] #
# MINI_MACHINE EA_t is STR0 # LOCAL CSET == "UTF8" || "BYTE"
# so we decode u32 to get EA of STR0 # ROM.EA_of_STR0 # 
# ARM_32 # u8_u8_16 
# ARM_32 # u8_u24 
# ARM_32 # u32_LOHI # TODO RTFM LOHI on ARM32 BYTE_A BYTE_B u16_LOHI u24_PAYLOAD

# u24_PAYLOAD = u32_WORD >> 8 # AXIOM COMPILED EXPR AT CODE_POINT BOLN_XPOS
# u24_PAYLOAD = u32_WORD >> 8 # CODE_POINT BOLN.XPOS.CPOS
# CPOS == OFFS # EA = BASE + OFFS # SPEC_t & ITEM_SPEC # GEN SAME_NAME
# GEN # PARSE MATCH TEMPLATE # PSG_RESONATE # MATCH fieldnames padding VAR_POOL
# VAR_POOL # SESSION.DIALECT += SPEC_TREE += MODULE += ELF_IMPORT += UDEF
# VAR_POOL # SESSION.ITEMS += "ANY_STR_HERE"
# that ANY_STR_HERE explains anything by MENTION and IDX and CODE_POINT_EA_EXPR
# EVAL EA_EXPR # PTR_t & PTR = OFFS_of_CSR_in_STREAM # NUDGE nearer API TEXT
# EVAL EVENT on_EVENT call UDEF("EA_EXPR") // SCRIPT_EVAL "ITEM" //
# BIND ITEM "%s" # VAR_NAME # or TREE_PRINT of ITEM_EXPR //
# BIND ITEM "%s" # XPOS_of_CSR_in_SEGMENT # requires extra PTR_t class
# BIND "{ EA_t EA }" #
# BIND "{ EA_t & EA }" # CONNECT EA ; SETUP_SCRIP_VARS LIST ; ALIAS ADJUST PTR
# ALIAS ADJUST PTR # STR0_t STR0 # CIDENT or SIMPLE_CSET  UTF8 or BYTE
# ALIAS ADJUST PTR # STR0_t STR0 == VAR_NAME # CIDENT or SIMPLE_CSET  UTF8 or BYTE
# SESSION DIALECT GLOBAL SCOPE LEXICON += SPELLING # CSET_USAGE_in_CSET__LIBR #
# CIDENT_in_CSET_UDEF # UDEF == "BYTE" # UDEF == "latin1" # UDEF = UDEF.CSET
# DEFAULT UDEF.CSET == SESS PROFILE 
# # MAGIC DECL MENTION # ALREADY EXISTS # DEFINED NEARBY # IMPORT ITEM_TYPE
# # AUTO DECL GEN ITEM_t & ITEM = GET("ITEM") # ITEM == "latin1" USER_PROFILE
# # AUTO AVAR PROFILE.AVAR["%s"] STR0 = VAR_NAME in export_CSET
# # AUTO CSET == export_CSET # GEN for each iso_8859_1_15_
# RTFM # ISO_8859-1(7)              Linux Programmer's Manual             ISO_8859-1(7)
# # AUTO PICK # man iso-8849-1 ISO 8859-1    West European languages (Latin-1)
# # AUTO PICK # man iso-8849-1 latin1 # chez nous # OPTIONAL ironic CYRILLIC
# # AUTO PICK # man iso-8849-%d CSET_by_name # chez nous CSET # OPTIONAL ironic CYRILLIC
# EA_ITEM "CSET" # ALIAS "CSET_STR0"

# DOUBLE CSR
# STR0 has a nearby NUL at EOLN
# STR0 is PARSED # STOP at TEXT_PARSED_CPOS
# DECODE CSR == u32_LOHI == "PARSED_CIDENT_lookup_VAR_ITEM STR0"
# TABLE[KEY] = STR0_t KEY = u32_DECODED THIS_CODE_POINT "{ ITEM == lookup KEY }"
# AUTO_MAGIC gen code by writing approximat SCRIPT lines # KEY_NORMALISED EXPR
# GEN_EXPR # Q2_STR0_Q2 # TYPE_FLOW ADAPT DATA = ADAPT on_LD += "GEN_HERE"
# GEN_EXPR # "{ %s }" # APPLY this line as a resonating FILTER
# GEN_EXPR DAMPER knows which ATTRS to leave in CODE_POINT VAR_AVAIL
# u8_u24 # u8_OPCODE "TABLE = TABLE_DATA_BYTES" # can still virtualise EA_of_BYTES
# SCRIPT has claim to a window within u8_u24 # useful for ITEMS[u24_16MB_idx]
# POINTER # u8_u8_u16 # u24 has been shrunk down to u16 but initital u8_u8
# u8_u8 u16_idx #
# u8_u8 u16_offs #
# u8_u8 u16_udef # u16_PAYLAOD # virtual u24_PAYLOAD() { return u16_PAYLOAD() }
# simply accept a SMALL model of 64x1024 u16_64k
# simply accept a SMALL model of 64x1024 i16_i16
# simply accept a SMALL model of 64x1024 i16_idx # AUTO_GEN TEMPLATE # multiverse
# simply accept a SMALL model of 64x1024 idx_i16 # SIGNED 
# class idx_int # MINUS MEANS DECODE SIGNED # OBJ = LOOKUP[ i16_idx ] #
# PARSE_VAR_NAME # "i16_idx" ## ALIAS "i16_" "i8_" "i24_"  "i32_" i48_ i64_
# DECL_PRE_PARSE VAR_NAME 

# CPOS_is_OFFS # AUTO_GEN EA = BASE + OFFS # OFFS = TABLE[ IDX ] # lookup STR0
# CPOS_CIDENT # and then tail from PARSED edge point HANDOVER DATA OWNED 
# DATA CREATED CARRIED # IMPORTED CIDENT = "STR0" # STR0 == "{ STR0_t STR0 }"
# CPOS == BYTE_POS_OFFS_in_SEGMENT _OFFS_in_FILE 
# CPOS = XPOS_PARSED CSR_POS # CPOS == CSR.CPOS # ALIAS CSR implied from EA
# CPOS = EA
# this created a SWITCH of VARIENT TYPE DECLS 
# GEN ENUM group_name { _label_0 _label_1 ... }
# GEN TABLE EIGHT TABLE[ idx_N ] 
# GEN TABLE FOUR TABLE[ idx_N ] 
# GEN TABLE PAIR TABLE[ idx_N ] 
# GEN TABLE BYTE TABLE[ idx_N ] 
# GEN TABLE name = "table_name" # ALIAS SESS.SCOPE_NOW.lookup("table_name")
# LEXICON = SESS.SCOPE.LEXICON.VAR_POOL_TWO # LOOKUP_USAGE
# LOOKUP_USAGE # [0[i[N # STR0_t STR0 = NAME_of_IDX[ idx ] 
# EVAL_SCRIPT_EXPR ... "{ ITEM = EXPR_LIST[idx] }" // AUTO_TYPE i16_idx i24_idx i32_idx

# idx_filename = GET_idx_filename STR0 = "filename"
# CAN GEN # filename = name_of_TOKEN[idx]
# DECODE SIGNED
# MINUS_ZERO_PLUS # we always use ZERO for UDEF or PLAIN or SELF or when dialect
# ZERO # treat_zero_as_PLUS 
# PLUS # array check idx >= idx_ZERO // usually 0 but any P0P2 as idx
# MINUS # array that also grows from ZERO #
# DECODE # if ZERO_or_PLUS TABLE_one
# DECODE # if MINUS TABLE == TABLE_two # and IDX = SIGNED_MINUS # ie ASIS
# MINI_MACHINE has choice of on_MINUS styles
# _MINUS _styles M3 M2 M1 M0_is_ZERO ALLOCATED[-1-idx_signed_MINUS
# ZERO returns -1 # useful for NULL_DETECT
# idx_signed_MINUS # KNOW MINUS # JUMP_ASSUMED #
# N_minus = N_alloc # idx_plus = M1 - idx_signed_MINUS


# if table_one == table_two # shared data # [-LO[idx[0[HI 
# HI == N_alloc_table_one

# SWITCH_TEST id {
# case MINUS) goto DECODE_MINUS
# case ZERO) ASIF_PLUS // MACRO goto ZERO_or_PLUS
# case PLUS) ITEM = WORD_from_TABLE[ idx ] # FROM_ZERO_OK
# label ZERO_or_PLUS # local label
# label ASIF_PLUS # local label {
#  WORD_t WORD = table_PLUS[ idx ]
#  u8_u8_u16_t WORD = GET table_PLUS[ idx ] {
#
#	CPU_OPCODE GET_WORD_FROM_TABLE 1|2|4|8
#	CPU_OPCODE GET_WORD_FROM_TABLE 1|2|4|8
#
#		CPU_OPCODE
#			WORD = BASE + IDX * 4
#  }
#  
# }
# CODE_DECODE_MINUS {
#	KNOW CT allocated a MINUS_PLUS table array
# 	PTR is to ZERO 
#	idx to check with this TEMPLATE
#	pick N_LO N_HI 
# }
# CODE ZERO_or_PLUS ( int_idx ) {  }
# if idx = N_ALLOC # edge sensitive AUTO_DETECT
# if( idx > N_ALLOC ) { ON_CODE idx_out_of_range { SCRIPT } }

#	now check for the newly created .png files
#
#	

	./sea_ice_vol_zoomed_in.png
	./sea_ice_vol_zoomed_out.png
	./sea_ice_vol_range.png


+= SPEC_TREE += MODULE += ELF_IMPORT += UDEF
#
# AXIOM COMPILED EXPR AT CODE_POINT BOLN_XPOS
# 




# u16_u16_u8_u24 #
# u16_u48 == u32_u32 # ACCESS_MODE_ARM_32_LO_HI # explain LIMIT 16_M u8_u24_lohi

