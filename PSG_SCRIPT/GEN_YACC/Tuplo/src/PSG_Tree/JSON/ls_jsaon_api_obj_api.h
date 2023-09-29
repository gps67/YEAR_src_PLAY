#ifndef ls_json_H
#define ls_jsaon_api_obj_api_H
//
// ls a DIR
// send back LIST of FILES 
// in JSON text format
// include full UNIX STAT
// API to FAKE_UNIX from FULL_DOS STAT_from_DIR_ls
// idx_t file = lookup(filename)
// idx_t file = lookup("HERE") // __dunder__ __HERE__
// COMPILER runs SCRIPT and PRE_ALLOCATES "{ idx = N++ }"
// APPEND ITEM to LIST // ITEM.STR0 // ITEM.CODE_POINT_BINDING //
// file = lookup("FILENAME") // FILENAME == known_FILENAME_EXPR_CACHED_VALUE[idx]

// FLEX SCRIPT // TOKENISE SOURCE using PARSER // ARGV0 in_CAPS //
// API SCRIPT // TOKENISE SOURCE using PARSER // ARGV0 in_CAPS //
//
// WE somehow generate a STRUCT which is PAIR { TOKEN DATA }
// idx = lookup(nbytes)
// idx = lookup("STR0")
// idx = lookup("CSR") // move CSR so return more than first visible // AUTO 
// idx = lookup("XPOS") // PARSE from XPOS to get STR0 // expecting cindent97
// idx = lookup("XPOS") // EVAL from XPOS to get PARSED STR0 // CODE_BLOCK
// (table,idx) = lookup("%s") // USAGE requires API to also get SESS CTXT etc
// API calls fields from here, CACHE keeps them, REMOTE has CACHE and PREDICT
// 
// FILENAME NAME
// FILESTAT STAT
// MIMECODE MIME
// ITEMPLUS UDEF
//
// lifecycle of VAR_POOL includes some moments to PRE_COMPILE stuff
// that also means finding PRE_COMPILED that matches what we knew, soon knew
// soon_knew //
// on_EVENT = COMPILER_OUTPUT_SAYS_BULK_LOAD_TABLE
// multi_attr layer u8_u24_u16_u16 i24_OPCODE u16_lhs u16_rhs
// u8_u8_t i16_LHS
// u8_u8_t i16_RHS
// i16_LHS lhs
// i16_RHS rhs // STYLE DIALECT TYPE "i16_RHS" not using _t VALUE "rhs"
// DECODE_i16_EVAL_in_SESS // MINI_MACHINE // VAR_POOL = globals //
//
// 	DECODE_BYTE_A_t * PTR = lookup u8 and eval GET for that EIGHT_SLOT
//
//		TABLE = BASE_of_NBYTES_of_TABLE
//		TABLE = TABLE_NAME("DECODE_BYTE_A"] // in curr CTXT
//		TABLE_ONE = ARRAY[ 37 ] of STR0
//		TABLE_OUT = ARRAY[ 37 ] of OBJ_PTR
//		TABLE_OUT = ARRAY[ 256 ] of OBJ_PTR // AUTO_INIT_ZERO //
//
//	so we allocate all 256 of TABLE for speed
//	EIGHT_t & EIGHT = ARRAY_of_EIGHT[idx
//	interpret ZERO as ABSENT entry in table // u8_u8_u48 //
//	interpret ZERO as PLUS entry in table // u8_u8_u48 //
// //	interpret ZERO as NULL // OFF
//	interpret PLUS as ARRAY[u8_idx] // GET_EA_EIGHT
//	u8_u8_u48 //
//	u8_u8 == " xFF xFF " // DECODER knows what that means // BYTE_PAIR
//	u8_u24_u32 // _u16_u16 lhs rhs // OPCODE = OPCODE_FROM u8_u24
//
// //	recursively define EIGHT points to EIGHT
// 	outer layer being DECODED inner layer is an item found
//	inner EIGHT must use one of our CT_SCRIPT allocated items
//	for speed need to reduce inner range to FEW [-128[-1[0[N[128
//	N can be +128 top would be +127 x8F 
//
//	FEW TYPES, allocated from ZERO, REBASE ZERO is -37 // table size
//	[ LO [ HI 	[-99 [0 // all negative are second table
//	WASTED TABLE MINUS PLUS PLUS_HIGH // MINUS and PLUS_HIGH repeat
//
//	EXPR is ROM CODE CACHE can resolve it down to JIT_VALUE
//
//	u8_u8 BYTE_A_ITEM_is_PAGE says all_in_page_DECODE_using_PAAMETER
//	EIGHT_t EIGHT = V[BYTE_A] ;// COPY or in_SITU code linke PTR STAR
//	EIGHT_t & EIGHT = V[BYTE_A] ;// COPY or in_SITU code linke PTR STAR
//	EIGHT_t * EIGHT = V[BYTE_A] ;// V is SESS.PTR_PAGE0 + OFFS
//	OPTION OBJECT can live in 4 layers // alloc to that SEGMENT 
//	OPTION OBJECT layer ROM RW SESS CTXT OBJ ARGS_dor
//	
//
//
//
//
//	LO == ZERO_
//	ie TOKEN is -83 
//
//
//	if EIGHT 
//		u6_idx 
//		EIGHT_t & EIGHT = TABLE[idx]
//
// send back LIST of FILES 
// send back NAME STAT PLUS // PLUS == "CT_" // ALIAS "SOURCE_USES" "CT_KNOWS"
// ITEM += { NAME STAT CT KNOW UDEF }
// EA += "{ EA = lookup IDX }"
// EA += "{ EA = lookup IDX via SCRIPT }"
// EA += "{ EA = know idx_t idx = EXPR }" // BIND EXPR // AUTO CMNT CODE_POINT
// XPOS of a MACRO is where it was DECL (in ROM)
// XPOS of a MACRO is where it was USED (in SCRIPT)
//
// expect IDX_t == "u8_idx_t" // u48_idx fits u8 fits u3 // OPTIMISER INFO
// expect just_wider_than_IDX_t for { VAR["N"] = 0 ; BIND ++ GET LOCK ROM_LOCK
// expect just_wider_than_IDX_t is IDX_t is u32 // u5_idx // 
// MATCH "CXX_DECL u5 idx = IDX" 
// MATCH "CXX_DECL u5 idx = %s" 
// MATCH "CXX_DECL u5 idx = N++" "N++" "N ++" " N ++ " "{ N ++ }" KNOW += ALLOC
// ASSOCIATE SCRIPT with XPOS.as_SCRIPT // XPOS == PTR // 
// PTR = { STRUCT NAME STAT UDEF } // usage a lot like TCL SCRIPT
// co-evolve both together as VIEW as CXX as TCL as DIAG_TWO
// use { BRACES } like tcl does // Q2_from_BRACES //
//
// to what extent should we limit the data format from REMOTE untrusted
// CT_RT // is the infrastructure that notices SCRIPT ITEM PHRASE EXPR 
// so we call GET_JSON_DATA_from_FSON_TEXT ...
// we get LIST of ITEM // ITEM is FS_ITEM_PLUS // ALIAS FS_FILENAME_PLUS
// 
//

// NO VALUE REQUIRED // USAGE IFDEF ls_jsaon_api_obj_api_H GOTO INDENTED_BLOCK
// CALL_CODE_PHRASE using already unique name dot field // CT KNOW // RT KNOW
// STYLE used in CXX // rephrase as MODULE_LABELED_ITEM on_EVENT
// 
// ls_jsaon_api_obj_api_H
// ls_jsaon
//	_api
//	_obj_api
//	_H_api  // causes BASE_ITEM to alreasdy be UTF8_JSON_of_dir_ls
// api += "FS ITEM_TYPES"
// api += "FS ITEM_api"
// api += "FS LOCN_api" // CODE_POINT[idx] // TABLE_TWO // not SELF maybe FIRST
// SELF idx may be ZERO // idx_0 // [0[N // ZERO_means_NULL _or_not_special_PLUS

// api += "FS api"
// api += "OBJ api"
// OBJ_api += FILENAME // FULL or PART of FILENAME_EXT_expr_in_CACHE
// OBJ_api += FS_ITEM_t
// FS_ITEM_t += "FILENAME FILE_STAT GUESS_MIME_from_FILENAME"

// DIR FILE SYMB_LINK HARD_LINK FIFO FIFO_active_SESS DEV OTHER
// TOKEN = one of line above "DIR FILE RPC FIFO DEV CDEV BDEV DOS_DEV URL"
// TOKEN = parsed URL // TOKEN == VALUE of SPELLING of URL STR0
// TOKEN == "http://SITE/ITEM/field" // API into PARSED SCRIPT // LEX_VAR_NAME
// TOKEN == "ANYSTR" // but usually cident97_in_utf8 // sort_as_bytes

// STR0 == "{ XPOS }" // strong builting usage ; claims MATCH STR0 _ish
// STR0 == A little bit of the source XPOS within file or some buffered segment

// api += "FD

// COMPILING BENCH LOADER //
// The original SOURCE SCRIPT is available in ROM // possibly offline

#endif

