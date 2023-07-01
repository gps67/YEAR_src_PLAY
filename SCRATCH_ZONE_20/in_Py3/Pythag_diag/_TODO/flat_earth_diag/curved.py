#! /usr/bin/env python

import math

pi = math.pi
miles_per_km = 0.621371
km_per_mile = 1.0 / miles_per_km 
feet_per_mile = 5280.0

def radius_from_circumference( circ ):
	return circ / ( pi * 2.0 )

earth_circumference_miles = 24901
# earth_radius_miles = 6371
earth_radius_miles = radius_from_circumference( earth_circumference_miles )
earth_radius_ft = feet_per_mile * earth_radius_miles

# earth_circumference_km = 40075
# earth_radius_km = 6371

rads_per_360 = 2.0 * pi

# flat-earth claim
# camera on beach - say 25 feet above sea level
# can see lighthouse 13.8 miles_away (direct or curved - not specified)
# if via map was curved but not much difference
# want to have 80 feet of the base hidden
# but the lighthouse is built on top of a rock 
# What is the obscured height?
# shimmer obviously messes up reality too

# have to make 2 triangles
# with a 90' angle at the horizon
# convert height (lens above sea) to horizon distance
# then 13.8 - M1 is M2 the other distance
# which gives the height at that side 

# also usable for cliffs at 200 ft
# also usable for airplanes at 30,000 ft


miles_away = 13.8 # miles # 
angle = miles_away / earth_circumference_miles * rads_per_360 # unused

maybe_20_feet_above_sea = 20 # viewer is maybe 40 ft above
claim_80_feet_above_sea = 80 # unused

# +---B---+
# |      /h  # CONCEPT += KNOW sphere surface flat earth here
# |     / # ie MENTION invokes DRAW_CIRCLE on DIAG with auto_labels
# A    / # ie create CIRCLE from BOZ_BOX_EDGE_OUTLINE _to_CIRCLE as_BOX
# |   C 
# |  / C == A + h
# | / # CONFIRMS h as VERTICAL wrt SPHERE at POLARITY + ANGLE + XPOS + XYZ_POS
# |/ # XYZ # XYZ_and_XPOS_csr # HERE == "ASSOC XYZ XPOS HERE # 
# # XPOS ZERO #
# SPEC_is_one_of_{ struct XPOS_t } # MK VALUE with XPOS _t #
# IMPORT SPEC #
# LOAD LIBR #
# RUN SESSion #
# OPCODE #
# RUNTIME #
# SCRIPT # ENV # CODE += VARS #
# SCRIPT # ENV # CODE += SPEC #
# SCRIPT # ENV # CODE += ITEM # ID # CTOR ID # OBJ_t == "ITEM_t" # AUTO_MATCH #
# SESS # SCRIPT #
# SCRIPT # ENCODER # DECODER # SESS DATA ARGS CTXT # AUTO_MK_VAR_on_MENTION
# MENTION # CTOR MENTION # STRUCT FIELDS # 
# MENTION # CTOR fields # STRUCT FIELDS_t # _t #
# SCRIPT FIELDS += FIELD # FIELD_t FIELD IDX IDX_ALLOC IDX_CTOR
# SCRIPT_is_JS #
# SCRIPT_is_ARGS # "ARGS" is a dialect with API 
# SCRIPT_is_Python # SCRIPT_FRAGMNENT # SCRIPT_API RUN_TIME # LIBR #
# SCRIPT_is_CXX # ARGS == "ARGV" # api("argv[argc]") += PICK_ARGV_ITEM
# SCRIPT_is_JS # THE_VALUE to create a SCRIPT VAR for # it_exists # set_to #
# DIAG += "{ DRAW STEP } "
# // MAGIC SYNTAX // SCRIPT is ARGS // DRAW == "$1" STEP == "$2" // CTOR PAIR "$@"
# // MENTION "$@" as ARGS as ARGV as AVAR "{ AVAR_t AVAR }"
# // DIALECT MK_ITEM "%s" MK_LIST "$@" // DIALECT PLURAL // PRE_GEN_CODE_HERE XPOS
# // ITEM INIT SAVE LOAD CTOR DTOR API
# // API ITEM_t 
# // API IDX_t // optional _t // DIALECT ACCENT += AVAIL("_t") // WORD_EDGES //
# // "%s" // "_t" // LEX "%s" ASIF " %s " MK_TYPE_SPEC("_t") // PRE_PARSED
# // GEN_is_PARSE_VAR //
# // PARSE_VAR PARSE GEN VAR SPEC NEAR STRUCT VAR_POOL LEXER PARSER RUNTIME CSR
# // DRAW SCRIPT // fade_in flat_earth

# DRAW_ITEM is DIAG_VIEW as CODE_DATA in SEGMENT_LAYOUT
# AUTO_SCRIPT matched "ITEM" as "%s" ITEM // RT_from_CT // += SCRIPT
# SCRIPT += AUTO_SCRIPT_PLUS # { load_CTOR_IDX _DATA _IDX } " %s %s " # AUTO_SP
# SP # over_use of SP in " %s " tells PARSER to NOTICE LEX_EDGE_of_ITEM "%s" 
# ITEM # CTOR # LOAD_DATA # CALL_CODE # SCRIPT PERMIT ARGS # 
# PARSE STREAM ... ITEM ... PUNCT ... ALIAS ... SPEC ... API ...
# PARSE LIBR PARSER EXPECT PHRASE_BOOK # EXPR_t expr = " %s "; // CMNT
# STREAM ITEM #
# STREAM EXPR #
# STREAM PHRASE #
# STREAM DATA # STREAM # ALIAS would find it but say it out loud # option used #
# STREAM PHRASE # PHRASE # MK_VAR NAME == "PHRASE" # NO_SP_auto_LEX_EDGE

# TODO # DECL PSG_SCRIPT and LEXER and API #
# TODO # DECL PSG_SCRIPT and LEXER and API # GEN #
# TODO # DECL PSG_SCRIPT and LEXER and API # PARSE #

# TODO # TODO # PARSE_VAR_NAMES recognise "_and_API" # LHS="_" RHS="_and_%s" "API"
# TODO # TODO # SCRIPT == " BENCH STEP " # DIALECT BENCH # DIALECT STEP #
# TODO # TODO # PARSE_SPEC_NAMES recognise "_and_API" # LHS="_" RHS="_and_%s" "API"
# TODO # TODO # PARSE_SPEC_NAMES recognise "/* enable SUB_LEX over TEXT_STREAM */"

# STREAM DATA # LINES_of_ARGS # SCRIPT_WORLD works this way MATCH PARSED
# SESS += KEEP_CSR # PTR or BOOL or STRUCT or CODE or DATA #
# SESS += SESS_DATA #
# BENCH_LANG # LANG += SYNTAX_of_ARGS
# BENCH_LANG # LANG += SYNTAX_of_ARGS ALLOC IDX VARS API
# LANG += API # API += MK_VAR "%s" with SPEC #
# BLUFF is the value of the IDENT of VAR # ALIAS # PARSED_ALIAS #
# when that is called return "{ STR0_t VAR_NAME } // CMNT // "
# when that is called return "{ STR0_t VAR_NAME }" // AUTO_VAR CMNT // VAR_NAME //
# ANIM STEP # DRAW_DIAG horizon_distance_at_h
# PARSE Python # FILTER on_MK_VAR # grow_SCRIPT # repeat_py_line_as_DIALECT
# our builtin parser for python
# GEN CODE_BASE EXPR_POOL PY_SCRIPT
# KNOW from EXPECTATIONS ARGS_EXPRS_as_PY_VARS
# KNOW can use a simple subset
# IMPLEMENT as GEN_Python_API MK_VAR _t _API 
# PARSE ARGS # ARGV == "ARGV" # /* pick VAL as VAR */
# ALIAS "VAL" # or %s # LEXER_HELPER MK_VAR_on_MENTION += "VAR_NAME" // VAR == "VAL"
# AUTO "{ ALIAS %s }" # recognise VAR_NAME # CMNT after "SCRIPT" is also SCRIPT
# CMNT # parse as ARGV_SCRIPT 
# CMNT # parse as PSG_SOUP into gen ARGV_SCRIPT 
# CMNT # parse as PSG_SOUP into gen ARGV_OPCODE += CODE_POINT # AUTO_CODE_POINT #
# CMNT # AUTO_CODE_POINT XPOS EXPR TEXT DATA CSR STREAM of CODE_POINTS
# XPOS # XYZ_POS # PARSED LEX_WELL_KNOWN # PARSED == "NORMALISED == PARSED"
# XPOS # XYZ_POS # "{ XYZ POS }"
## "{ XY POS }" ## MATCH "{ %s %s }"
## "{ XY POS }" ## MATCH " { %s %s } " // NUDGED_ADJUST_MATCH_SAME VIEW PICK
## MKVAR XY ## MKVAR POS ## EXPECT EOS ## MATCHED_P0P2 ## MATCHED_P1 ##
## MKVAR "%s" ## DIRECT_ASIS ## WEAVE this word porridge into TREE with PSG
## MK_VAR mk_SPEC mk_VAR mk_EXPR ## CALL_API ## SCRIPT_API ##
## MK_WORLD
## MK_API
## MK_STEP
## BENCH.MK_var SCRIPT_VAR("A") VAR_TYPE_Python 
## BENCH.MK_var SCRIPT_VAR("C") VAR_TYPE_Python ## HINT type_FLOAT ##
## BENCH.MK_var SCRIPT_VAL("C") VAR_TYPE_VAR_NAME ## "C" ## "%s" ##

# 
# THIS CODE is STREAM_PARSED alias PARSED_STREAM
# THIS CODE is STREAM_PARSED 
# THIS CODE is PARSED_STREAM
# 
#	a simple subset of Python that I got the PSG for
#	direct GEN ARGS == "ARGV" # PICK "char * argv[int argc]" # 
#	## porridge_parser ##
#	## PSG ## BASIC_LANG
#
#
# UDEF adds PARSED_STREAM # PSG got the LIBR_API _use_of_UDEF
# UDEF adds ARGS == "ARGV"
# UDEF adds ARGV == "ARGV" # if in doublt RET_VAR is this_identity
# UDEF adds ARGV == "ARGV" # if in doublt RET_VAR is "ARGV" # %s ANYSTR #
#
# SCRIPT adds MK_VAR on_FIRST_MENTION then_on_USE_VAR_VAL
# on_USE_VAR_VAL # OPCODE_MATCH # DECODE densly simple API # ARGV_PLUS
#
#	a simple subset of Python that I got the PSG for
#	direct GEN ARGS == "ARGV" # PICK "char * argv[int argc]" # ; # PARSER
# AUTO_SP # EXPECTED INDENT as REWRITTEN # OFFLINE # HERE # VAR_NAME;
#	STR0_t VAR_NAME = "ARGV"; // expect CIDENT "ARGV" // CT_TOKEN MATCH
#	CT_RT TOKEN = "VAR_NAME" // VAR_NAME == "ARGV" // SAMPLE_VALUE_TOO
# // SAMPLE_VALUE_TOO // MATCH XPOS_in_SCRIPT // XPOS is ...
# XPOS_t XPOS = XPOS_API_OFFS_XPOS_in_FILE _in_SEGMENT _OFFS_EXPR _ETC
# 	An XPOS can be a FIXED POINT # IN_ROM # PTR_from_CODE_POINT
#	PTR = CODE_POINT = XPOS = XPOS_at_start_of_CSR_in_STREAM
#	simply record STREAM as FILE as SCRIPT # GEN_SCRIPT #
# SCRIPT += MK_VAR DECL STEP USE_GET_SET GET_SPEC EA CODE COMPILED as DIAG
#
#		CIDENT ARGV = "ARGV";
#		
#	## porridge_parser ##
#	## PSG ## BASIC_LANG
#
#
#
#	## porridge_parser ##
#	##
#	## SCRIPT is a SIMPLE CALCULATOR with VAR_NAMES
#	## SCRIPT is a LIST_of_ARGV ## external API my_ref_your_ref ##
#	## SCRIPT is STR0 IDENT == "cident99" // DIALECT //
#	## SCRIPT is "{ A R G S }" with added CMNT and CODE_POINT
#	## simply CODE_POINT == OFFS_of_CODE_in_TEXT # u24_OFFS # u8_u24 #
#	## OPCODE u8_u24 OPCODE OFFS
#	## u8_OPCODE ## MK_VAR ALIAS = %s
#	## u8_OPCODE ## u3_u5 ## MINI_MACHINE u3 u5 # u5_idx_of_REGISTER
#	## u8_OPCODE ## u3_u5 ## MINI_MACHINE u3 u5 # u3_idx_of_FUNCT
#	## u8_OPCODE ## u3_u5 ## MINI_MACHINE u3 u5 # u3_idx_of_SUITE
#	## i24_IDX ## 
#	## i24_OFFS ## 


#	SCRIPT REPLAY #
#	SCRIPT REPLAY # CACHE PRE_GEN OPTIMISED LAZY_EVAL
#	SCRIPT REPLAY #

#
#	FILTER says GEN repeat of each Python STEP in Python
#	CONNECT is the remote calling APi from "SCRIPT_TOKEN[S]"
#	FILTER match optimised PHRASE CODE_POINT XPOS
#

#	DRAW_DIAG 
#	 VIEW

#	for each STMT there is a DIAG FRAG FRAGMENT EXPR
#	for each variable there is DRAW_DIAG ## PSG_recursion_used_to_depth_2"
#	for each XPOS there is DRAW_DIAG ## CSR_HERE visitor_api
#	for each XPOS there is DRAW_DIAG ## CSR_HERE stream_api
#	

#	STREAM_API STREAM CSR API
#	LINE = ARGV with CMNT and CODE_POINT
#	LIST = of_LINE
#	LIST = of_ZONE
#	LIST = of_TEXT # however GEN got it into PSG via TEXT # it did #
#	# that is quite a sophisticated script of a MINI_MACHINE
#
#
#	DECODE u3 u5 i24 # can directly get i24 during shift off u8_opcode
#	DECODE i24 = i32_WORD >> 8; # ONE_CLOCK_CYCLE # now have SIGNZ
#	DETECT ZERO
#	DETECT MINUS
#	DETECT ERROR # exception DROP_HERE # search_NEXT #
#	DETECT EOT # PARSED_EOT #
#	u3_PREFIX u5_IDX 
#	u3_GROUP u5_OPCODE # dialect GROUP OPCODE CODE_POINT
#
#	TEXT_API += get XPOS of this VAR get_LIST
#	u3_GROUP=="GROUP"  u5_EXPR_in_LIST
#	u3_GROUP=="GROUP2" u5_ITEM # EA # u5_EA # u5_IDX #
#	u3_u5 = ALLOC LINE OF CODE in MINI_MACHINE
#
#	ITEM_is_OPCODE 
#	ITEM = lookup u8 # OPTIMISE isolate_u8_from_u32_WORD # BYTE_A #
#	ITEM is GET_VAR EA_var EA; // type has extra type SPEC varname core EA
#	ITEM = TABLE[u8] # PICK TABLE # u8 # u8_from_WORD #
#	
#	MINI_MACHINE # RUNNING REMOTELY # DISPLAY #
#	MINI_MACHINE # STEP #
#	MINI_MACHINE # DECL #
#	MINI_MACHINE # STEP #
#	
#	DIAG for each 256 OPODE and (future) decode_24_too
#	DIAG for u3_u5_i24 # SIGNED i24 if you want it #
#	DIAG for u8_i24 # -ve means second table VAR not ROM
#	DIAG for u16_i48
#
#	MINI_MACHINE can decode the u8_i24
#	MINI_MACHINE can decode the u8 u24 # drive SIGNED tests
#	MINI_MACHINE can decode the u8_u8_i16
#	MINI_MACHINE can decode the u8_u8_u16
#
#	u32_ASM as u32_DECODER
#	+= get_i24
#	
#	u64_ASM as u64_DECODED
#	+= MANAGED_CODE_using_own_TOKEN_POOL
#	+= fits in u8_u24_EA_EXPR
#
#	u8 BYTE_B # HAND_CRAFTED to suit AMD64 then ARM32 then C
#	# VIA our own ALIAS # BENCH compiles to make effect happen #
#	# BENCH PRE_GEN CXX # u8 BYTE_B = (u32_WORD << 24) >> 24; // NOT SIGNED
#	# BENCH PRE_GEN CXX # i8 BYTE_B = (i32_WORD << 24) >> 24; // YES SIGNED
#	# CORRECT code as required #
#
#
#
#	MAXI_MACHINE
#	u32 #
#	u32_u32 #
#	CSR_STEP TEXT OFFS XPOS CSR
#	MENTION creates those VARS
#	MENTION writes SCRIPT fragments
#	SCRIPT CACHE # RUN_at_CT # EVAL_is_quick_enough at EDIT_time #
#
#	u8_u8_i1_i16
#	u8_u8_i1_u15 # EG 0 NOT signed
#	u8_u8_i1_i15 # EG -1 YES signed # ALIAS -1 1 # unique CODE_POINT
#
#	Because a MINI_MACHINE is so elemental, it can be PRE_GEN
#	fast DECODE and EVAL of OPCODE # of COMPILED_SCRIPT # TOKENISED #
#	`
#	BEHIND mini_machine is a LIBR of CODE for 256 items
#	BEHIND maxi_machine is a LIBR of DECODE( "u8" )
#	BEHIND maxi_machine is a LIBR of DECODE( "u8_u8" )
#
#	FAIR_MINI_MACHINE
#	u8_i24 WORD_EDGE_maybe_not_OPCODE_EDGE
#	u8_u8 u16 u32 # u16 UNUSED_so_ZERO or_M1
#	u8_u8_i16 # u32
#	u8_u8_i48 # u64
#	BYTE_B as i8 # W>>8 <<24 >>24 # 3 clock cycles #
#	BYTE_A as i8 # W    <<24 >>24 # 2 clock cycles #
#	LOHI_PAIR
#	LOHI
#
#	DECODE STREAM over MINI_MACHINE _MAXI_
#	eg UDEF_OPCODE u16_UDEF = u16_BITFIELD
#	++ UDEF_NEXT_WORD u32_UDEF = u32_BITFIELD
#	++ MK DECODE_ABCD_EFGH as u32_u32 or u32
#
#	PTR_DATA = DATA[u8] # u64 from u8 #
#	
#	MOXI_BOX
#	256 registers addressable from OPCODE # more from LIBR
#	256 POINTS on a DIAG
#	256 COMPONENTS on a DIAG # exact FILL PAGE without CHECK
#	still gets checked but no SOURCE required (managed code)
#	MINI_MACHINE u8_u8_u8_u8
#	MINI_MACHINE u8_u8_u16
#	MINI_MACHINE u8_u24
#	MINI_MACHINE ABCD
#	MINI_MACHINE EFGH
#	MINI_MACHINE i64_lohi_ABCD_EFGH
# 	DECL OPCODE as BITFIED as [-optionally-] SIGNED
# 	DECL OPCODE as BITFIED as [-optionally-] UNSIGNED
#
#	ITEM = POOL_ALLOC
#
#	MINI_MACHINE_i8_STREAM
#	MINI_MACHINE_i32
#	MINI_MACHINE_u32
#
#	COMPILE this "Python"-like code // def idioms //
#	ANIMATE the DATA_FIELDS # DATA_VIEW # STEP_REPLAY # get to state
#	


def ft_horizon_away_from_height_ft( height_ft ):
	# horizon is DIRECT distance not curved distance
	A = earth_radius_ft
	C = earth_radius_ft + height_ft
	B = math.sqrt( C*C - A*A )
	return B

def ft_height_from_horizon_away_ft( horiz_away_ft ):
	# again the directline from eye to horizon, not curved distance
	B = horiz_away_ft
	A = earth_radius_ft
	C = math.sqrt( A*A + B*B )
	extra_height = C - earth_radius_ft
	return extra_height

# height of camera lens above sea level in ft
H1 = 20
H1 = 30
H1 = 25
D1 = ft_horizon_away_from_height_ft( H1 )
M1 = D1 / feet_per_mile

print( "%f FEET ft_horizon_away_from_height_ft(%f)" %( D1, H1 ))
print( "%f MILES ft_horizon_away_from_height_ft(%f)" %( M1, H1 ))

M2 = miles_away - M1
D2 = M2 * feet_per_mile
H2 = ft_height_from_horizon_away_ft( D2 )

print( "" )
print( "%f FEET ft_height_from_horizon_away_ft(%f FEET)" %( H2, D2 ))
print( "%f FEET ft_height_from_horizon_away_ft(%f MILES)" %( H2, M2 ))
