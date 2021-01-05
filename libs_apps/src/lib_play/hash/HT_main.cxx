
#include "HT_table.h"
#include "HT_main.h"

#include "str2.h"

int HT_main()
{
	HT_table_str_obj tbl;

	int N = 50;
	for( int i = 0; i < N; i++ )
	{
		if( i== 40 ) continue;
		char buf[20];
		sprintf( buf, "item %d", i );
		str2 * key = new str2( buf );
		tbl.add( key, key );
		if( i== 44 ) 
		{
			tbl.remove( key );
		}
		if( i== 45 ) 
		{
			tbl.remove( key );
		}
		if( i== 46 ) 
		{
			tbl.rename( key, new str2("item 45") );
		}
		if( i== 48 ) 
		{
			// target key already exists
			tbl.rename( key, new str2("item 47") );
		}
	}

	for( int i = 0; i < N; i++ )
	{
		char buf[20];
		sprintf( buf, "item %d", i );
		str2 * key = new str2( buf );
		obj_hold<str2> s2 = key; // ensure deleted
		str2 * val = (str2*) tbl.lookup( key );
		if( !val )
			e_print( "KEY(%s) VAL(NULL)\n",
				(STR0) *key
			);
		else
			e_print( "KEY(%s) VAL(%s)\n",
				(STR0) *key,
				(STR0) *val
			);
	}

	return 0;
}

