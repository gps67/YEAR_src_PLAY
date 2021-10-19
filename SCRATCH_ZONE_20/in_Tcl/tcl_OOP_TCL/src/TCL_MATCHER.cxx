#include "TCL_MATCHER.h"
#include <string.h>

extern
bool CMP( Tcl_Obj * obj, const char * str )
{
	// this gets string rep of Literal // also anystr
	const char * str_word = Tcl_GetString( obj );
	return 0 == strcmp( str, str_word );
}

bool LITERAL_MATCHER:: MATCHES_fn( Tcl_Obj * obj )
{
	fprintf(stderr,"# CALL # LITERAL_MATCHER(%s).MATCHES_fn # %s \n",
		Tcl_GetString( match_one ),
		Tcl_GetString( obj ));

	if( obj == match_one ) return true;
	if( obj == match_two ) return true;

	// there should be an ASM for this
	// plain searches though N can be blindingly fast
	// but setup overhead makes it less so
	// plough through NULL values

	for( int i = 0; i<N_different; i++ ) {
		Tcl_Obj * ITEM =  differents[i];
		const char * s = NULL;
		if(ITEM)
			s = differents[i]->bytes; // uses operator ->  .PTR ->
		else
			s = "ITEM (NULL)";

		if(!s) s = "(NULL)";
		const char * s2 = match_one->bytes;
		fprintf(stderr,"# LITERAL_MATCHER(%s) TRY_NOT '%s' \n", s2, s );
		if( obj == differents[i] ) {
			fprintf(stderr,"# LITERAL_MATCHER(%s) CONFIRMED_NOT '%s' \n", s2, s );
			// add to counters
			return false;
		}
	}

	// obj not found in cached items
	// do the strcmp compare
	// maybe add it to cache 

	// some sanity checks
	if( !(match_one && match_one->bytes )) {
		// match_one is supposed to exist with string
		fprintf(stderr,"**** // NULL match_one\n");
		fprintf(stderr,"**** // NULL match_one bytes\n");
		return false;
	} // else OK stay happy

	// match_ONE is "GET" or "array_get"
	// do the strcmp compare
	if( CMP( obj, match_one->bytes )) {
		// match_two is rare, second route to "Literal"
		if(match_two) { // already have a second, this is 3rd
			// do not cache // _one _two but not _three
			fprintf(stderr,"**** // match_two OVERWRITE\n");
		}
		match_two = obj; // never third
		fprintf(stderr,"**** // match_two used ///////////\n");
		return true;
	} // else strcmp differs

	// add obj to differents[i] overwrite NULL or FULL
	for( int i = 0; i<N_different; i++ ) {
		if( differents[i] ) {
	fprintf(stderr,"**** keeping differents[%d] = %s \n",
			i, differents[i]->bytes );
		}
		if( !differents[i] ) {
			differents[i] = obj;
	fprintf(stderr,"**** stored differents[%d] = %s \n",
			i, differents[i]->bytes );
			// add to counters
			return false;
		}
	}
	// no space for another, N_different used
	fprintf(stderr,"**** differents[N] FULL ///////////\n");
	return false;
}

