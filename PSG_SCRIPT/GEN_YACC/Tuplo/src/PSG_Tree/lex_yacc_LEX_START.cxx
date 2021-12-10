#include "lex_yacc_LEX_START.h"

#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"
using namespace PSG;


 LEX_START_DECL:: LEX_START_DECL() {}
 LEX_START_DECL:: ~LEX_START_DECL() {}

 LEX_START_GROUP:: LEX_START_GROUP() {}
 LEX_START_GROUP:: ~LEX_START_GROUP() {}

 bool
 LEX_START_GROUP::
 add_START(
   STR0 name,	// "name1"
   bool excl,	// true
   STR0 desc	// "within a Q2 mode"
 ) {
	if( name == "INITIAL" ) return true;
	LEX_START_DECL * token = new LEX_START_DECL();
	token -> Name = name;
	token -> Desc = desc;
	if( excl )
		this->LIST_Token_x.append( token );
	else
		this->LIST_Token_s.append( token );
	INFO("added %s", name );
	int ns = LIST_Token_s.N();
	int nx = LIST_Token_x.N();
	INFO("START ns %d nx %d \n", ns, nx);
	return true;
 }

 bool
 LEX_START_GROUP::
 print_lines( buffer1 & buf ) {

   INFO("START called");

   int ns = LIST_Token_s.N();
   int nx = LIST_Token_x.N();
    INFO("ns %d nx %d \n", ns, nx);

   if( ns || nx ) {
   } else {
    buf.print(" // no LEX START Conditions\n");
    buf.print(" // ns %d nx %d \n", ns, nx);
    buf.print("\n");
    return true;
   }

   if(ns) {
    buf.print(" // START Conditions\n");
    buf.print("\n");
    buf.print("%%s");
    for( int i =0; i<ns; i++ ) {
	LEX_START_DECL * tok = LIST_Token_s[ i ];
	if( !tok ) return FAIL("NULL ITEM IN LIST");
	
    	buf.print(" %s", (STR0) tok->Name ); // leading SP
    }
    buf.print("\n");
   }

 if(0)
   if(nx) {
    buf.print(" // START Conditions - exclusive\n");
    buf.print("\n");
    buf.print("%%x");
    for( int i =0; i<nx; i++ ) {
	LEX_START_DECL * tok = LIST_Token_x[ i ];
	if( !tok ) return FAIL("NULL ITEM IN LIST");
	
    	buf.print(" %s", (STR0) tok->Name ); // leading SP
    }
    buf.print("\n");
   }

   if(nx) {
    buf.print(" // START Conditions - exclusive\n");
    buf.print("\n");
    for( int i =0; i<nx; i++ ) {
	LEX_START_DECL * tok = LIST_Token_x[ i ];
	if( !tok ) return FAIL("NULL ITEM IN LIST");
	
    	buf.print("/* %s */\n%%x %s\n\n",
		(STR0) tok->Desc, 
		(STR0) tok->Name 
		); // leading SP
    }
    buf.print("\n");
   }

   buf.print("\n");
   return true;
 }




