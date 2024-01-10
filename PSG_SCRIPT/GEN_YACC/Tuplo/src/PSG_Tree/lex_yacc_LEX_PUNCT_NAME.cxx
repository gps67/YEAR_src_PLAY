
#include "lex_yacc_LEX_PUNCT_NAME.h"
using namespace PSG;

#define OK( S ) 


// buff is optional for TEMP1 
// fn can return STR0 from ROM or from BUFF
// fn { on_MATCH switch(chr) ; label = STR0 ; return label; } 
// after inline OPTIM //

// REWORK array[u8] of "STR0"
// MMAP local ROM_SEGMENT BASE NBYTES u16_OFFS  u8_IDX
// get_EA = BASE + OFFS
// BASE = PAGE0 + OFFS_BASE 
// OFFS = IDX * _1234

bool PSG:: print_LEX_punct_name( buffer1 & buff, u8 chr )
{
 const char * label = NULL;

 #define PAIR( VAL, STR ) case VAL: label = STR ; break ;

 // if( chr < 0 ) return FAIL("char negative");
  switch( chr ) {

   PAIR( 0x00, "NUL" )	//
// PAIR( 0x01, "SOH" )	//
// PAIR( 0x02, "STX" )	//
// PAIR( 0x03, "ETX" )	//
// PAIR( 0x04, "EOT" )	//
// PAIR( 0x05, "ENQ" )	//
// PAIR( 0x06, "ACK" )	//
   PAIR( 0x07, "BEL" )	//
// PAIR( 0x08, "BS" ) 	//
   PAIR( 0x09, "TAB" )	//
   PAIR( 0x0A, "LF" )	//
   PAIR( 0x0B, "VT" )	//
   PAIR( 0x0C, "FF" )	//
   PAIR( 0x0D, "CR" )	//
// PAIR( 0x0E, "SO" ) 	//
// PAIR( 0x0F, "SI" ) 	//
// PAIR( 0x10, "DLE" ) 	//
// PAIR( 0x11, "DC1" ) 	//
// PAIR( 0x12, "DC2" ) 	//
// PAIR( 0x13, "DC3" ) 	//
// PAIR( 0x14, "DC4" ) 	//
// PAIR( 0x15, "NAK" ) 	//
// PAIR( 0x16, "SYN" ) 	//
// PAIR( 0x17, "ETB" ) 	//
// PAIR( 0x18, "CAN" ) 	//
// PAIR( 0x19, "EM" ) 	//
// PAIR( 0x1A, "SUB" ) 	//
// PAIR( 0x1B, "ESC" ) 	//
// PAIR( 0x1C, "FS" ) 	//
// PAIR( 0x1D, "GS" ) 	//
// PAIR( 0x1E, "RS" ) 	//
// PAIR( 0x1F, "US" ) 	//
   PAIR( 0x20, "SP" )		// alias SPACE
   // SP is both CTRL and GLYPH
   PAIR( 0x21, "NOT" )		// aliad bang shriek not exclamation
   PAIR( 0x22, "Q2" )		// 
   PAIR( 0x23, "HASH" )		// 
   PAIR( 0x24, "DOLLAR" )	// 
   PAIR( 0x25, "PERCENT" )	// 
   PAIR( 0x26, "AMP" )		// alias ampersand
   PAIR( 0x27, "Q1" ) 		// 
   PAIR( 0x28, "L_PAR" )	// alias PAR_L PARENTHESIS_LEFT LPAR
   PAIR( 0x29, "PAR_R" )	// alias PARR // allow _ in sublex
   PAIR( 0x2A, "STAR" )		// alias "ASTERISK"; // 
   PAIR( 0x2B, "PLUS" )
   PAIR( 0x2C, "COMMA" )	// 
   PAIR( 0x2D, "MINUS" )	// alias DASH
   PAIR( 0x2E, "DOT" )		// 
   PAIR( 0x2F, "SLASH" )	// 

#if 0
   PAIR( 0x30, "ZERO" )		// 
   PAIR( 0x39, "NINE" )		// DIGIT_NINE
#endif

   PAIR( 0x3A, "COLON" )	//
   PAIR( 0x3B, "SEMICOLON" )	//
   PAIR( 0x3C, "LT" )		// LESSTHAN
   PAIR( 0x3D, "EQUAL" )	// EQUALS
   PAIR( 0x3E, "GT" )		// GREATERTHAN
   PAIR( 0x3F, "QUESTION" )	// QUESTION MARK
   PAIR( 0x40, "AT" )		// 

#if 0
   PAIR( 0x41, "UPPER_A" )	// 
   PAIR( 0x5A, "UPPER_Z" )	// LETTER Z
#endif

   PAIR( 0x5B, "L_SQUARE" )	// L_SQUARE_R SQUARE_BRACKET_LEFT BRACKET_LEFT 
   PAIR( 0x5C, "BACKSLASH" )	// 
   PAIR( 0x5D, "SQUARE_R" )	// aliases ...
   PAIR( 0x5E, "CARET" )	// aliases CIRCUMFLEX HAT POWER CARET
   PAIR( 0x5F, "UNDERLINE" )	// aliases underscore
   PAIR( 0x60, "BACKQUOTE" )	// QB

#if 0
   PAIR( 0x61, "LOWER_a" )	// aliases ...
   PAIR( 0x7A, "LOWER_z" )	// LETTER z ...
#endif

   PAIR( 0x7B, "L_CURLY" )	// aliases ...
   PAIR( 0x7C, "PIPE" )		// aliases ...
   PAIR( 0x7D, "CURLY_R" )	// aliases ...
   PAIR( 0x7E, "TILDE" )	// aliases ...
   PAIR( 0x7F, "DEL" )		// aliases ...

  }
  if( label) {
  	if(!buff.put_plain( label )) // print to what was there
		return FAIL_FAILED(); // Never happens ENOMEM
	return true;
  }
  return WARN("char 0x%2.2X", chr); // false // buff == '' //
  return FAIL("char 0x%2.2X", chr);
}



