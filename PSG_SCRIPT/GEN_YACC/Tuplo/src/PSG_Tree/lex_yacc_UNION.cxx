#include "lex_yacc_UNION.h"

using namespace PSG;

bool 
union_field_t::
print_with_semicolon( buffer2 & out)
{
	return out.print(" %s ;\n", (STR0) union_field_line );
}

union_field_t *
union_field_list_t::
lookup_add( STR0 name, STR0 line )
{
	int N = union_field_list.N();
	for( int i = 0; i< N ; i ++ ) {
		if( union_field_list[i]->union_field_name == name ) {
			INFO("already found %s - %s", name, line );
			return union_field_list[i]; // pointer 
		}
	}
	union_field_t * new_line = new union_field_t( name, line );
	union_field_list.append( new_line );

	N = union_field_list.N();
	INFO(" added N == %d %s - %s", N, name, line );
	return new_line;
}

bool 
union_field_list_t::
print_lines( buffer2 & out)
{
	int N = union_field_list.N();
 //	out.print(" // N == %d \n", N );
	for( int i = 0; i< N ; i ++ ) {
 //		INFO("field[%d] %s { %s }",
 //			i,
 //			(STR0) union_field_list[i]->union_field_name,
 //			(STR0) union_field_list[i]->union_field_line
 //		);

		// if NOT return FAIL FAILED
		union_field_list[i]->print_with_semicolon( out );
	}
	return true;
	
}

