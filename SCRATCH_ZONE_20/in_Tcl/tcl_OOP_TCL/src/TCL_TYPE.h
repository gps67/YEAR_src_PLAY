#ifndef TCL_TYPE_H
#define TCL_TYPE_H

// #include <tcl.h> // done by TCL_PTR.h
#include "TCL_STUBS.h"
// #include "TCL_TYPE.h"
#include <tcl.h> // Tcl_Obj typePtr

	// typedef __u32__ u32;
	typedef unsigned int u32;
	typedef unsigned char u8;
	typedef  u8  u8_idx;
	typedef u32 u32_lohi;

#define ASCII_NUL 0x00;


/*!
	TCL_TYPE type_decoder;

	This is the think that "knows" if an Obj is one of ours
	if so // and how it provides an api // api_surface_t api_surface;

	If we wanted to create a lookup table of one of our own api names

		anyname_api %s_api %s.api %s // else is under %s as ASCII_PLUS

	ASCII_PLUS_t	SCRIPT; // STREAM // SESSION // API // FD 

		"ASCII SCRIPT" // auto gen UTF8 _with_ FILTERS TWO WAY

		"UDEF_%2X" // name of type must be ASCII_PLUS
		// and of IMPLY test first 32 bits of == "UDEF"
		// and usually "_%2X" // "_" "LEX_EDGE" // GAP_OPTIONAL("_")
		// PARSE REQUIRE BYTE == "_"
		// PARSE REQUIRE BYTE BYTE == %s // must be a valid identifier
		// PARSE REQUIRE BYTE BYTE == %X // must be a valid HEX HEX %2X
		// PARSE REQUIRE BYTE == NUL
		// PARSE REQUIRE all_of_the_above // checklist // run //

		obj -> type -> name 
		val as SPEC DATA

		u32 str_UDEF = "UDEF" // = UDEF_as_u32 // CODE POINT data = "UDEF";
		// "U" "D" "E" "F" // in lohi looks like FFEEDDUU f_e_d_u_udef
	
	PARSE STREAM_is_PARSED_BY_PARSER { SPEC CODE DATA }

	STREAM_is_PARSED_BY_PARSER
	ID = u32_reserved_bytes_in_stream "UDEF"
	PARSER LEX_EDGE means "%s_%s" may be "%s%s" or { %s lhs }  or %s rhs
	{ SPEC CODE DATA }
	HYPNOTIC { LEX } // name // name is EXPR CIDENT // UDEF_CIDENT_LEX_EXPR
	DETECT { LEX }
	DETECT { PSG LEX }
	PHRASE { PSG PHRASE }
	PARSE { explain %s as CIDENT_PLUS Name TypeName }
	TOKEN { EXPR } // SUBLEX += NAME_of_GENRE "TypeName"
	TOKEN { EXPR } // SUBLEX += NAME CIDENT_PLUS // utf8_ASCII

		// "U" "D" "E" "F" // in lohi looks like FFEEDDUU f_e_d_u_udef
		// "U" "D" "E" "F" // in lohi looks like "FEDU_%2X" "_%2X%b NUL
		// what is printf for u8_literal_byte_u8 "_" HEX HEX NUL

		FFEEDDUU f_e_d_u_udef_

		FFEEDDUU_t
			f_e_d_u_udef_

		FEDU_u32 
			"udef_AB"
			75 64 65 66 5f 41 42 0a 
		UDEF_AB
			"UDEF_%2X" 

		 "UDEF_AB" | hd
		55 44 45 46 5f  41  42 0a  
		 U  D  E  F  _ HEX HEX NL 
		 U  D  E  F  _ HEX HEX LEX_EDGE // SHARP_OPTION // WORD_PART_USAGE
	
	if( TypeName _starts_with_ u32_UDEF )
	{
		CLAIM entire u32 abcd u32 second word 

			abcd == "UDEF"

		CLAIM entire second u32 word

			_AB == "_AB"  use " HEX HEX " HEX = HEX_DIGIT_builtin

		CLAIM 256 points of the u32_word

			u8_FMT_2X "_%2X" // followed by a NUL // USUALLY

		USUALLY provides GENERIC API though DIALECT

			PARSE u8 = u8_idx // WITH FAST %2X byte from UDEF_AB
			// u8_idx = UDEF[u8_idx] // array u8 of UDEF_t

		We are ENCODING u8_idx as "%2X" as " HEX HEX " 

			BYTE "%2X" // byte_fmt_2X "%2x"

		This is one of our groups TypeName UDEF_%s // %s == "UDEF"

			u32_UDEF = 55 44 45 46 [0] [1] [2] [3]
			u32_UDEF = 55 44 45 46
			u32_UDEF = 46 45 44 55 [3] [2] [1] [0] _lohi
			u32_UDEF = 46 45 44 55 "F" "E" "D" "U" 
			u32_UDEF = 0x46454455; // "F" "E" "D" "U" 

			u32 u32_UDEF = WORD_from_STR4 "UDEF"

			That only takes a subrange "UDEF_XX_u8_idx"

				UDEF[u8] as %s[%2X] UDEF u8_idx

		u8_idx_t u8_idx = PARSE_BYTES_FROM_STREAM.PARSE_BYTE

			ASCII strings like %2X eat up our 64_bit name
			char Name[8]; // 7 + NUL
			
		u8_idx

			PRINT and PARSE u8_idx as "{ UDEF_%2X }" BYTE

		u8_idx

			USAGE ITEMS[idx_SPEC_u8] // FULL_PAGE_of_IDX_u8

		SPEC = SPECS[u8_this_SPEC]

			BENCH ALLOC HERE[ u8_BYTE ] // as IDX_NAME[u8_idx]
			PARSE PRE_PHRASE "%sSPEC_%s "SPEC_" %2X
			PARSE PRE_PHRASE "%sSPEC_%s "SPEC" "_" %2X LEX_EDGE_NUL
			SUBLEX "%2X" u8_idx_of_NAME("%2X") // if N > 256 ... IDX

		NAME = CHAR[8]

			BYTE_EIGHT # unused option array [ u3 ] of u8_BYTE

		NAME = u32 u32 // condition byte[7] == NUL // so dont check

			GROUP / u8_idx_ITEM

			GROUP = "UDEF_"
			u8_idx = u8_idx_ITEM
			u8 = u8_idx // BENCH BUILDS THESE // RT can use them

			DECODE_STEPS
			%s_%s GROUP ITEM
			%s GROUP
			%s ITEM

			// PARSE detect repeat DECL MATCHING DIALECT

			ENCODE_BYTE_in_STREAM_in_NAME_in_SCRIPT
			// SUBLEX_2X "%2X" // actually str1 setting //
			// PARSE detect repeat DECL MATCHING DIALECT

			GROUP "%s_%s" 
	}



	RES

	typedef TCL_TYPE_UDEF_V99 TCL_TYPE; // ALIAS // 

	METHOD_ZERO of course it is (check anyway)
	+= PRE_CHECK_NOW_CONST optimisation

	METHOD_ONE // METHOS // ctxt->obj->type
	// VAR ctxt
	// VAR ctxt.obj // PARSE . INTO -> CONSISTENTLY check_filter_batch RUN

	METHOD_TWO // ctxt->obj->type.SPEC API // LINK API api_LIBR DIALOG
	// DIALOG.psg // PSG is MINI_MACHINE API is PSG .YARN .THREAD .SESS API
	// PHRASE.psg SESS DATA
	// PARSE WORDS in LISTS // SPEC DATA //

	METHOD_THREE // STREAM TEXT // SESS API //
	// CXX_%s // CXX_Layer // This decl line = NEW TOKEN { TOKEN
	// CXX_%s // CXX_Layer // This decl line = NEW { TOKEN } // declare on use
	// CXX_%s // CXX_Layer // { GEN } // VARNAME_("_CXX_Layer_")
	// CXX_%s // CXX_Layer // { GEN } // VARNAME_("_%s_") _CXX_Layer_
	// CXX_%s // CXX_Layer // { GEN } // VARNAME_("_%s_") "%s" // CXX_Layer
	// CXX_%s // CXX_Layer // { GEN } // KNOW Layer = "%s" // ("_%s_") "%s" // CXX_Layer
	// CXX_%s // CXX_Layer // { GEN } // KNOW CXX = "%s" // ("_%s_") "%s" // CXX_Layer
	// CXX_%s // CXX_Layer // { GEN } // KNOW "_%s_"// "%s" // CXX_Layer
	// %s_%s CXX Layer // (*TUPLO*) BASE_FORM %s CXX %s Layer SUBLEX

	foreach <class> of %s Name // DICTIONARY_or_FILTER_PARSER
	 PERMIT SOURCE_POINT to export 

	typedef struct Tcl_ObjType {
		const char *name;
		Tcl_FreeInternalRepProc *freeIntRepProc;
		Tcl_DupInternalRepProc *dupIntRepProc;
		Tcl_UpdateStringProc *updateStringProc;
		Tcl_SetFromAnyProc *setFromAnyProc;
	} Tcl_ObjType;

       
              typedef struct Tcl_Obj {


		// EA = ALLOC from clever TCL POOL N == FIXED_NBYTES_OF_STRUCT

                  int refCount;

		  // STRN
                  char *bytes;
                  int length;

		  // TYPE_t TYPE // can extend own STRCT with extra fields
                  const Tcl_ObjType *typePtr;

		  // PTR_PAIR PTR1 PTR2 BITFIELDS // u48_payload
		  //
		  // PTR1
		  // PTR2

		  // END OF FIXED SIZE STRUCT 
		  // CAN have ASSOC EA or ASSOC PARSE STR API

                  union {
                      long longValue;
                      double doubleValue;
                      void *otherValuePtr;
                      Tcl_WideInt wideValue;
                      struct {
                          void *ptr1;
                          void *ptr2;
                      } twoPtrValue;
                      struct {
                          void *ptr;
                          unsigned long value;
                      } ptrAndLongRep;
                  } internalRep;

		  // obj -> refCount
		  // obj -> bytes // length // always NUL  in STR0
		  // obj -> typePtr // NULL free to be hijacked from str
		  // obj -> twoPtrValue.PTR1 // PTR1 //
		  // obj -> twoPtrValue.PTR2 // PTR2
		  // 
		  // obj -> get_VAL1
		  // obj -> get_VAL1.u8_SPEC
		  // obj -> get_VAL1.u8_SPEC
		  // obj -> get_VAL1.u16_idx

		  //	get_u24() { VAL1 >> 8 } ; // consts abound (TODO)


		  // obj -> get_VAL2
		  // obj -> get_VAL2.u24
		  // obj -> get_VAL2

		  // BITFIELD union of
			
		//	u32_u32

		//	u16_u8_u8 DECODE_VAL1

		//	u64_u64 	// NOT HERE u48_PTR u16_AB_opcode


              } Tcl_Obj;



*/
class TCL_TYPE_UDEF
{
 public:
 	TCL_TYPE_UDEF()
	{
	}
 	~TCL_TYPE_UDEF()
	{
	}
	bool test_TCL_TYPE_UDEF( Tcl_Interp * interp )
	{
//		return hash.test(interp);
		return false;	// MIN STUB AUTO_STUB_NULL { return false }
	}

	// future inline // currently fn
	bool is_one_of_my_types_fn( Tcl_Obj * obj )
	{
		// TYPENAME UDEF_%2X // PARSE _%2X elsewhere
		// does typename begin with "UDEF" // extract u8 possible
		return type_name_UDEF( obj );
	}

	bool type_name_UDEF( Tcl_Obj * obj ) { // only check first 4 bytes;

		// NULL means not our UDEF_%s %2X u8_SPEC_idx
		// NULL TYPE is common STR0 "bytes[length]"
		// good to convert to us, but not us yet
		if( !obj->typePtr ) {
			return false;	// plain "bytes[length]" // not UDEF
		}

	  	// TCL_TYPE_NAME "UDEF_%s"
		const char * type_name_str =

			obj->typePtr->name; 

	  	// TYPE_NAME (NULL) // this should never happen // TYPE exists
		if( !type_name_str ) {
			return false;
		}

		// typename -> "ABCD EFGH ..." happens to stop with H==NUL
		// OPTION require E to be "_" or "." or "/" or CIDENT_PLUS
		// u32_u32 lhs rhs "UDEF" "_%2X" // NUL common _ possible

		// pointer type cast
		u32_lohi * type_name_STR4 = (u32*) type_name_str;

		// ASM COMPARE_WORD u32_WORD u32 * PTR
		// hovver over ADDR, COMPARE from here
		// by asking other side to do it
		// other side can keep its secret
		// upto sucessful guess

	  // NULL_TYPE // common STRN obj == "bytes[length]" // expect STR9
	  // expect CIDENT_PLUS // "UDEF_FF"
	  // "UDEF_%s" "%2X" FF
	  // "UDEF_FF"
	  // "UDEF_FFF"
	  // "UDEF_FFFF"
	  // "UDEF_u8_2X"
	  // "UDEF_u16_4X"
	  // "UDEF_u24_6X" // unusual u24_payload = WORD >> 8;
	  // "UDEF_u32_8X"
	
	  // MATCH STR4 prefix of type_name_UDEF
		// PLUS code allocs BINARY not ASCII subset // both work

	 // hand written BYTES for STR4 "UDEF"

		u32 
		 u32_UDEF = 
		   0x46454455; // "F" "E" "D" "U"  // "UDEF"

		// load first 4 bytes of type name
		u32 u32_STR4 = * type_name_STR4; // STR4_WORD_one
	
	  // FASTEST POSSIBLE MATCH

		if( u32_STR4 != u32_UDEF ) {
			return false;
		}
#if 0
#endif

	}

	bool type_name_UDEF_u8( Tcl_Obj * obj, u32 & u8_FF ) { // only check first 4 bytes;
		if(! type_name_UDEF( obj )) { // only check first 4 bytes;
			return false;
		}

		// new access to repeat use of str
		const char * type_name_str =

			obj->typePtr->name; 

		// check stray syntax _ '_'  HEX HEX NUL

		if( type_name_str[4] != '_' ) {
			// PARSE FAIL DEMANDING '-'
			// return false;

			// or log badly formed NAME_%s
			// but let it happen

			fprintf(stderr,"EXPECTED %%s_%%s GOT %%s.%%s # '%s'\n",
			 type_name_str );
		 	return false; // DEMAND '_ in %s_%s
		}

		if( type_name_str[7] != ASCII_NUL ) {
			// we want to fit into 2 words
			// but we dont have to
			// u8 can overflow to u16 or _u32_idx 
			// u24_IDX
			// complain and stay
			// in future u8 breach upgrades to u32
		}
		
		// check NN is valid HEX HEX *
		// convert NN from HEX * // str_to_long

		u32 NN = strtol( type_name_str[5] , NULL,  16 );

		// check range // reject out of domain or convert

		if( NN == 0 ) {
			// check for 0 else it was a wrong blind parse
		 if( '0' == type_name_str[5] ) {
			// all good // really is a 0
			 fprintf(stderr,"GOT_UDEF_00 quite unusual # '%s'\n",
			  type_name_str
			 );
		 } else { // FAIL // DEMANS %2X
			 fprintf(stderr,"GOT 'UDEF_%%s' expected ""2X # '%s'\n",
			  type_name_str
			 );
		 	return false;
		 }
		} // else not TYPE UDEF_00

		if( NN > 255 ) {
			 fprintf(stderr,"GOT 'UDEF_%d' from ""2X # '%s'\n",
			  NN,
			  type_name_str
			 );
		}

		u8_FF = NN;

		return true; // no more questions // UDEF_NN
	
	  // only compared the first 4 chars // BENCH KNOWS //

	  	// that's it // stop there //
		// above did NOT match the FAST_MATCH
		// subclass can override
		// eg look at "_%2X" NUL // u8_idx // DECODE HEX HEX
	
	  // caller calls another MACRO that does
	  //
	  // ASM
	  //	u8_SPEC = u32_WORD.BYTE_A
	  //
	  // CXX
	  //	u8 u8_spec == VAL1.BYTE_A
	  //
	  // MACRO 
	  //	SPEC_t * SPEC = SPECS[u8_SPEC] / u8_idx == u8_SPEC
	  //
	  // DECODE
	  //	HERE applied as FILTER as TREE_WALK as STREAM as BITFIELDS
	  //	eg obtain_that_promised "_u8_"
	  //	eg obtain VAL1.BYTE_A
	  //	eg obtain VAL1.BYTE_B
	  //	eg obtain VAL1.HIGH_CD 
	  //	eg obtain VAL1.u24_payload; // from BYTES BCD_lohi
	  //	eg obtain VAL1.u32_payload; // default first WORD[0]
	  //	eg obtain VAL1.u47_payload; // ONLY on 64 BIT
	  //
	  //

	  // DERIVED SUBCLASS

	  	// MATCH A B C D // SPEC == A // u8_spec == u8_idx == BYTE_A
		// 

		// to know that SPEC is there requires to KNOW os_one_of_ours
		// 

		return false;
	}
};

class TCL_TYPE : public TCL_TYPE_UDEF
{
 public:
 	TCL_TYPE()
	{
	}
 	~TCL_TYPE()
	{
	}
	bool test( Tcl_Interp * interp )
	{
//		return hash.test(interp);
		return false;	// MIN STUB AUTO_STUB_NULL { return false }
	}


	bool is_one_of_my_types( Tcl_Obj * obj ) {
		u32 & u8_FF = 0x00;
		// only check first 4 bytes;
		// which 
		if( ! obj->typePtr ) {
			return false; // plain string start off as NULL type
			// could attempt to PARSE to upgrade to ours
			// but that is only for BIND decoders
		}
		if( type_name_UDEF_u8( Tcl_Obj * obj, u8_FF ) {
			return true; // FAST_MATCH HERE
		}
		// INLINE catches built_in // look for other things
		return is_one_of_my_types_fn( obj );
	}

	bool get_u8_HEX_HEX_from_STR4_2X_typename( Tcl_Obj * obj, u8_idx & ret ) {
		if(! is_one_of_my_types( obj ) { return false; }
		// KNOW NOW typePtr NOT NULL

		u32 * str0_words = obj->typePtr->name ;
		u32 * u32 EFGH = /* & */ str0_words[1];
		u8 * EFGH = (u8*) EFGH;
		u8 A = EFGH[0];

		// if( EFGH.A == '_' ) 

		typedef u8 EIGHT_BYTES_t[8];
		typedef u8 FOUR_BYTES[4];

		return true;
	}


	// //
	// STAY there may be other reasons this IS on of ours
	// TODO // CT GEN and RUN // u32_udef = CT_EVAL "STR4"
	// OK the compiler adds an overflow word after the u32
	// but it then allows u32 word_from_lohi str_ptr word_from_lohi_STR4
	// but it then allows u32 word_from_lohi
	// str_ptr
	//	u32 read_WORD at_PTR  // KNOW_is_aligned_u32 // FAST_MATCH
	//   
	// word_from_lohi_STR4

};

extern
int declare_OBJ_functions( Tcl_Interp * interp, TCL_TYPE * decoder );

#endif
