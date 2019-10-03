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




