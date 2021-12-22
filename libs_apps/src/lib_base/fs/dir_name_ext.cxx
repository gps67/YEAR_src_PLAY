
#include "dir_name_ext.h"
#include "buffer2.h"

bool dir_name_ext::get_cwd()
{
	int len = 2048;
	char buf[len];
	// should fetch this via some channel, eg THIS_PID_CAN _preloaded_
	// PID PPID cwd uid gid USER HOME TTY // child_list // MMAP // fd //
	// man getcwd // absolute pathname not ending in / except when it does
	if( !getcwd( buf, len ) )
	{
		perror( "getcwd()" );
		return false;
	}
	cwd.set( buf );
	return true;
}

bool dir_name_ext::dos_drive_prefix( str0 path )
{
	if(!path) return false;
	uchar c1 = path[0];
	if(!c1) return false;
	uchar c2 = path[1];
	if(!c2) return false; // reject "A" require "A:"
	if(c1 != ':') return false;
	if( ('a'<=c1) && ( c1<='z' ) ) return true;
	if( ('A'<=c1) && ( c1<='Z' ) ) return true;
	return false;
}

/*!
	quick + dirty

	.profile gets reported as .ext
*/
bool dir_name_ext::decode_filename( str0 _path )
{
	// clear all results

	ext.clear();
	name.clear();
	name_ext.clear();
	dir.clear();
	orig_path_name.set( _path );

	// check NULL

	if(!_path || (!*_path)) 
	{
		return false;
	}

	// working buffer buf

	buffer2 buf(50);
	buf.put( _path );
	buf.trailing_nul();

	// [start [end // end is extra NUL 
	// parse from RHS 

	uchar * start = buf.buff;
	uchar * end = buf.buff + buf.nbytes_used    ; // the NUL

	uchar * pos = end; // the NUL or . or /
	uchar * dot = end; // the NUL or . or /
	// [start name [dot .ext [end
	// search end to start, right to left, for .ext for dir/file
	// pos points to the char OUTSIDE the string
	// pos points to a NUL
	// pos-- ; required

	// search for .ext, leave pos at '.'
	while( pos > start )
	{
		pos --;
		uchar c1 = *pos;
		if( c1 == '.' )
		{
		//	pos . P1 <= end
			uchar * p1 = pos + 1;
			ext.set( p1, end - p1 ); // N = P2-P0
			dot = pos;
			break;
		} else if( (c1 == '/') || (c1 == '\\') )
		{
			// . not seen
			// ext is NULL
			dir.set( start, pos-start ); // exclude /
			uchar * p1 = pos + 1;
			name.set( p1, end - p1 );
			name_ext.set( p1, end - p1 );
			return true;
		}
	} 

	if( pos == start ) {
		name.set( pos, dot - pos );
		name_ext.set( pos, end - pos );
		return true;
		// dir is absent
		// maybe .bashrc as .ext
		// maybe abcde // no . poss 
	}
	// . was found (no detection of .tar.gz only .gz)
	// no detection of .bashrc

	// search for name
	while( pos > start )
	{
		pos --;
		uchar c1 = *pos;
		if( (c1 == '/') || (c1 == '\\') )
		{
			uchar * p1 = pos + 1;
			name.set( p1, dot - p1 );
			name_ext.set( p1, end - p1 );
			dir.set( start, pos-start ); // exclude /
			return true;
		}
	}

	name.set( pos, dot - pos );
	name_ext.set( pos, end - pos );

	return true;
}

void dir_name_ext::mk_BOTH_path_name()
{
	mk_full_path_name();
	mk_near_path_name();
}
/*!
	This is a bit lame
*/
void dir_name_ext::mk_full_path_name()
{
	full_path_name.clear();
	if( dir ) 
	{
		if( dir.buff[0] == '.' ) 
		{
			if( !cwd ) get_cwd();
			full_path_name.put( cwd );
			full_path_name.put( '/' );
		}
		full_path_name.put( dir );
		full_path_name.put( '/' );
	} else {
		if( !cwd ) get_cwd();
		full_path_name.put( cwd );
		full_path_name.put( '/' );
	}
	if( name ) 
	{
		full_path_name.put( name );
	}
	if( ext ) 
	{
		full_path_name.put( '.' );
		full_path_name.put( ext );
	}
	full_path_name.trailing_nul();
}

/*!
	This is slightly better - relative near path
*/
void dir_name_ext::mk_near_path_name()
{
	near_path_name.clear();
	if( dir ) 
	{
	/*
		if( dir.buff[0] == '.' ) 
		{
			if( !cwd ) get_cwd();
			near_path_name.put( cwd );
			near_path_name.put( '/' );
		}
	*/
		near_path_name.put( dir );
		near_path_name.put( '/' );
	} else {
	/*
		if( !cwd ) get_cwd();
		near_path_name.put( cwd );
		near_path_name.put( '/' );
	*/
	}
	if( name ) 
	{
		near_path_name.put( name );
	}
	if( ext ) 
	{
		near_path_name.put( '.' );
		near_path_name.put( ext );
	}
	near_path_name.trailing_nul();
	// INFO(" near_path_name '%s'", (STR0) near_path_name );
}

void dir_name_ext::test_print()
{
	mk_BOTH_path_name();
	e_print("\n" );
	e_print(" cwd      '%s'\n", (STR0) cwd );
	e_print(" dir      '%s'\n", (STR0) dir );
	e_print(" name     '%s'\n", (STR0) name );
	e_print(" ext      '%s'\n", (STR0) ext );
	e_print(" name_ext '%s'\n", (STR0) name_ext );
	e_print(" orig_path_name  '%s'\n", (STR0) orig_path_name );
	e_print(" near_path_name  %s\n", (STR0) near_path_name );
	e_print(" full_path_name  %s\n", (STR0) full_path_name );
}

bool dir_name_ext::test_on( str0 path )
{
	decode_filename( path );
	test_print();
	return true; // hmmm
}

// STATIC
void dir_name_ext::test1()
{
	dir_name_ext named;
	named.test_on( "/dir1/dir2/file.ext" );
	named.test_on( "/dir1/dir2/file" );
	named.test_on( "/dir1/dir2/" );
	named.test_on( "file.ext" );
	named.test_on( ".ext" );
	named.test_on( "./.file.ext" );
	named.test_on( "../.file.ext" );
	named.test_on( "../../.file.ext" );
	named.test_on( "./.file." );

	named.decode_filename( "/tmp/t/name.ext" );
	dir_name_ext named2 = (str0) named.full_path_name;
	named2.test_print();

#if 0
	named.decode_filename( "/tmp/t/name" );
	dir_name_ext named3;
	named3 = (str0) named.full_path_name;
	named3.test_print();
#endif

	named.decode_filename( "/renamed/ext/maybe/image.aaa" );
	named.ext = (str0) "png";
	named.test_print();

	named.decode_filename( "t/name.ext" );
	named.test_print();

}

str0 dir_name_ext::abs_filename( str0 filename )
{
	static dir_name_ext named;
	named.decode_filename( filename );
	named.mk_BOTH_path_name();
	return named.full_path_name;
}



