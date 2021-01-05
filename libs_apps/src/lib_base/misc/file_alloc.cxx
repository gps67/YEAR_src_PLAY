
#include "file_alloc.h"

#include "dir_list.h"
#include <stdio.h>
#include <errno.h>

#include <string.h>
#include <ctype.h>
#include <stdlib.h> // atoi

	File_Alloc_base::~File_Alloc_base()
	{
	}

	File_Alloc_base::File_Alloc_base(
		const char * _dir,
		const char * _pfx,
		int _d_fmt
	)
	: buf( 80 )
	{
		dir = _dir;
		pfx = _pfx;
		d_fmt = _d_fmt;
		hi = -1;
	}

	int File_Alloc_base::find_hi_unused()
	{
		if( d_fmt > 0 )
			return find_hi_unused_int();
		else
			return find_hi_unused_chr();
	}

	const char * File_Alloc_base::mk_base( int val )
	{
		if( d_fmt > 0 )
			return mk_base_int( val );
		else
			return mk_base_chr( val );
	}

	const char * File_Alloc_base::mk_pathbase( int val )
	{
		buf.clear();
		buf.print( "%s/%s", dir, mk_base( val ) );
		return str0( buf );
	}

	int File_Alloc_base::find_hi_unused_chr()
	{
		int pfx_len = strlen( pfx );

		if(!d.open( dir )) return errno;
		while( d.next() )
		{
			const char * name = d.name();
			if(0!=strncasecmp( name, pfx, pfx_len )) continue;
			name += pfx_len;

			int val = 0; // no value is 'a'
			const char * letters = name;
			char c1 = *letters;
			if ( islower( c1 ) )
			{
				val = c1 - 'a';
				letters ++;
			}
			else if( isupper( c1 ) )
			{
				val = c1 - 'A';
				letters ++;
			}
				else val = 0;
			if( hi < val )
				hi = val;
			printf( "%s.png -- %s\n", mk_base_chr( val ), d.name() );
		}
		d.close();
		// return hi ; // reuse last filename
		return hi + 1;
	}

	int File_Alloc_base::find_hi_unused_int()
	{
		int pfx_len = strlen( pfx );
		int digits_max = 10;
		char buf[ digits_max + 1 + 10 ];
		if(!d.open( dir )) return errno;
		while( d.next() )
		{
			const char * name = d.name();
			if(0!=strncasecmp( name, pfx, pfx_len )) continue;
			name += pfx_len;
			const char * digits = name;
			while( isdigit( *name ) ) name++;
			if( name == digits ) continue;
			const char * digits_end = name;
			int digits_len = digits_end - digits;
			if( digits_len > digits_max ) continue;
	
			for(int i=0; i< digits_len; i++ )
			{
				buf[i] = digits[i];
			}
			buf[digits_len] = 0;
			int val = atoi( buf );
			if( hi < val )
				hi = val;
			printf( "%s.png -- %s\n", mk_base_int( val ), d.name() );
		}
		d.close();
		// return hi ; // reuse last filename
		return hi + 1;
	}

	const char * File_Alloc_base::mk_base_int( int val )
	{
		static char buf[80];
		sprintf( buf, "%s%*.*d", pfx, d_fmt, d_fmt, val );
		return (const char *)buf;
	}

	const char * File_Alloc_base::mk_base_chr( int val )
	{
		char letter_lo = 'a'; // 'A'
		static char buf[80];
		if( val )
			sprintf( buf, "%s_%c", pfx, val + letter_lo );
		else
			sprintf( buf, "%s", pfx );
		return (const char *)buf;
	}

