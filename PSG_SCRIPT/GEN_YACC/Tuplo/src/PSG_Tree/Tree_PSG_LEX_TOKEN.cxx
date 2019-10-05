#include "Tree_PSG_LEX_TOKEN.h"

#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"


 LEX_TOKEN_DECL:: LEX_TOKEN_DECL() {}
 LEX_TOKEN_DECL:: ~LEX_TOKEN_DECL() {}

 LEX_TOKEN_GROUP:: LEX_TOKEN_GROUP() {}
 LEX_TOKEN_GROUP:: ~LEX_TOKEN_GROUP() {}

 bool
 LEX_TOKEN_GROUP::
 lookup_by_Name( LEX_TOKEN_DECL * TOK, STR0 Name ) {
	
   int n = LIST_Token.N();
   for( int i =0; i<n; i++ ) {
	LEX_TOKEN_DECL * tok = LIST_Token[ i ];
	if( !tok ) return FAIL("NULL ITEM IN LIST");
	
	// STRING Compare -via- 
	if( tok-> Name == Name ) {
		TOK = tok;
		return true;
	}
	return FAIL("GROUP('%s')ITEM('%s')", (STR0) PFX, (STR0) Name );
   }
 }

 bool
 LEX_TOKEN_GROUP::
 lookup_by_Value( LEX_TOKEN_DECL * TOK, STR0 Value ) {
	
	int n = LIST_Token.N();
	for( int i =0; i<n; i++ ) {
		LEX_TOKEN_DECL * tok = LIST_Token[ i ];
		if( !tok ) return FAIL("NULL ITEM IN LIST");
		
		// STRING Compare
		if( tok-> Value == Value ) {
			TOK = tok;
			return true;
		}
	}
	return FAIL("GROUP('%s')ITEM('%s')", (STR0) PFX, (STR0) Value );
 }


 bool
 LEX_TOKEN_GROUP::
 add_PUNCT(
   STR0 punct  // "==" ); // STR0 tail,  // "_EQUAL_EQUAL", // 
 ) {
  // buff .put( PFX ); // PRF_PUNCT is added later

  buffer1 buff; // TAIL = _PLUS_PLUS
  buffer1 NAME_of_char; // PLUS
  
  STR0 Z1 = "OK";
  const char * p = punct;
  while( char c = *(char *)p ) {
	p++; // but leave at NUL
	NAME_of_char.clear();
	if( !print_LEX_punct_name( NAME_of_char, c ) ) {
		// not all defined :-)
		return FAIL_FAILED();
	}
	// add it to tail
	buff .put_byte( '*' ); // PUNCT_
	buff .put_byte( '_' ); // PUNCT_
	buff .put( NAME_of_char ); // PUNCT_PLUS
  }
  STR0 _PLUS_PLUS = buff; 
  return add_PUNCT( _PLUS_PLUS, punct );

 }



