
#include "lex_yacc_LEX_PUNCT_NAME.h"
using namespace PSG;

#define OK( S ) 

// why PSG:: required ?? // extern in .h ??

bool PSG:: print_LEX_punct_name( buffer1 & buff, u8 chr )
{
 const char * label = NULL;

 #define PAIR( VAL, STR ) case VAL: label = STR ; break ;

 // if( chr < 0 ) return FAIL("char negative");
  switch( chr ) {
   PAIR( '+', "PLUS" )

   PAIR( 0x00, "NUL" )
   PAIR( 0x07, "BEL" )
   PAIR( 0x09, "TAB" )
   PAIR( 0x0D, "CR" )
   PAIR( 0x0A, "LF" )
   PAIR( 0x0C, "FF" )
   PAIR( 0x1C, "ESC" )
   PAIR( 0x20, "SP" ) // alias SPACE

   PAIR( 0x21, "NOT" ) // aliad bang shriek not exclamation
   PAIR( 0x22, "Q2" ) // 
   PAIR( 0x23, "HASH" ) // 
   PAIR( 0x24, "DOLLAR" ) // 
   PAIR( 0x25, "PERCENT" ) // 
   PAIR( 0x26, "AMP" ) // alias ampersand
   PAIR( 0x27, "Q1" ) // 
   PAIR( 0x28, "L_PAR" ) // alias PAR_L PARENTHESIS_LEFT LPAR
   PAIR( 0x29, "PAR_R" ) // alias PARR // allow _ in sublex
   PAIR( 0x2A, "STAR" ) // alias "ASTERISK"; // 
   PAIR( 0x2C, "COMMA" ) // 
   PAIR( 0x2D, "MINUS" ) // alias DASH
   PAIR( 0x2E, "DOT" ) // 
   PAIR( 0x2F, "SLASH" ) // 

#if 0
   PAIR( 0x30, "ZERO" ) // 
   PAIR( 0x39, "NINE" ) // DIGIT_NINE
#endif

   PAIR( 0x3A, "COLON" ) //
   PAIR( 0x3B, "SEMICOLON" ) //
   PAIR( 0x3C, "LT" ) // LESSTHAN
   PAIR( 0x3D, "EQUAL" ) // EQUALS
   PAIR( 0x3E, "GT" ) // GREATERTHAN
   PAIR( 0x3F, "QUESTION" ) // QUESTION MARK
   PAIR( 0x40, "AT" ) // 

#if 0
   PAIR( 0x41, "UPPER_A" ) // 
   PAIR( 0x5A, "UPPER_Z" ) // DIGIT_NINE
#endif

   PAIR( 0x5B, "L_SQUARE" ) // L_SQUARE_R SQUARE_BRACKET_LEFT BRACKET_LEFT 
   PAIR( 0x5C, "BACKSLASH" ) // 
   PAIR( 0x5D, "SQUARE_R" ) // aliases ...
// PAIR( 0x5E, "CIRC" ) // aliases CIRCUMFLEX HAT POWER
   PAIR( 0x5E, "CARET" ) // aliases CIRCUMFLEX HAT POWER CARET
   PAIR( 0x5F, "UNDERLINE" ) // aliases underscore
   PAIR( 0x60, "BACKQUOTE" ) // QB

#if 0
   PAIR( 0x61, "LOWER_a" ) // aliases ...
   PAIR( 0x7A, "LOWER_z" ) // aliases ...
#endif

   PAIR( 0x7B, "L_CURLY" ) // aliases ...
   PAIR( 0x7C, "PIPE" ) // aliases ...
   PAIR( 0x7D, "CURLY_R" ) // aliases ...
   PAIR( 0x7E, "TILDE" ) // aliases ...
   PAIR( 0x7F, "DEL" ) // aliases ...

  }
  if( label) {
  	if(!buff.put_plain( label )) // print to what was there
		return FAIL_FAILED(); // Never happens ENOMEM
	return true;
  }
  return WARN("char 0x2.2X", chr); // false // buff == '' //
  return FAIL("char 0x2.2X", chr);
}



