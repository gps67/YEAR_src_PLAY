
#include "str0.h"
#include "buffer2.h"
#include "dir_name_ext.h"
#include <unistd.h> // system


/*
	this should be in dir/file/ext
	and it should check for .ext = ".pdf"
	and it should have a check for file_exists
	and it should also respond to VFS path remapping
	and it should have list of dirs allowed in <set>, += subdirs
*/
bool name_looks_bad( str0 filename )
{
	bool ok = true;
	const char * s = (STR0) filename;
	char bad_char = 0;
	while( char c = *s++ )
	{
		switch( c ) {
		 case '\\':	// backslash is common WIN32, target unix crack
		// system comand should be replaced with argv one 
		 case ' ':	// WIN32 uses SPACE, but many unix tools dont
		 case '[':	// tcl is sensitive to these
		 case ']':	// tcl is sensitive to these
		 case '`':	// back quote
		 case '"':	// double quotes
		 case '\'':	// single quotes
		 case '(':	// parenthese - sub shell
		 case ')':	// parenthese - sub shell
		 case '#':	// parenthese - sub shell
		 case 127:	// DEL
#ifdef WIN32
			; // OK allow all sorts of things with WIN32
#else
			bad_char = c;
#endif
		 break;
		 default:
			if( c < ' ' ) bad_char = c;
		}
		if( bad_char )  {
		 // e_print can cope with CR LF BS ^S ... I hope
		 e_print( "# BAD CHAR %2.2X '%c' IN FILENAME '%s'\n", bad_char, bad_char, (STR0)filename );
		}
	}
	if( bad_char != 0 ) ok = false;
	return ! ok;
}

bool call_system( const char * cmd ) {

	int t = system( (STR0) cmd );
	if( t==-1 ) return FAIL("CMD %s", (STR0) cmd );
	if( t==0 ) return true;
	return FAIL("CMD %s RETVAL %d", (STR0) cmd, t );
}

bool fork_xpdf_page( str0 filename, int pageno )
{
	if( name_looks_bad( filename ) ) return false;
	buffer2 cmd;
#ifdef WIN32
	cmd.print("'C:/Program Files/Adobe/Acrobat 4.0/Reader/AcroRd32.exe' %s &", (STR0) filename );
#else
//	cmd.print("xpdf %s %d&", (STR0) filename, pageno );
//	cmd.print("mupdf %s %d&", (STR0) filename, pageno );
	cmd.print("mupdf -r 120 %s %d&", (STR0) filename, pageno );
#endif
//	blk1 & p1 = (blk1&) cmd;
//	blk1 p1b;
//	str0 st0a =        p1b;
//	str0 st0b = (str0) p1b;
//	e_print( "Running: %s\n", (STR0)p1b );
	e_print( "Running: %s\n", (STR0)cmd );
	if(!call_system(cmd)) return FAIL_FAILED();

	return true;
}

bool fork_xpdf( str0 filename )
{
	if( name_looks_bad( filename ) ) return false;
	buffer2 cmd;
#ifdef WIN32
	cmd.print("'C:/Program Files/Adobe/Acrobat 4.0/Reader/AcroRd32.exe' %s &", (STR0) filename );
#else
	cmd.print("xpdf %s &", (STR0) filename );
	if(!call_system(cmd)) return FAIL_FAILED();
#endif
	return true;
}

bool fork_netscape( str0 filename )
{
	if( name_looks_bad( filename ) ) return false;
	buffer2 cmd;
#ifdef WIN32
	cmd.print("'C:/Program Files/Adobe/Internet Explorer/iexplore.exe' %s &", (STR0) filename );
#else

// http://www.mozilla.org/unix/remote.html`
// OK	cmd.print("netscape -remote 'openFILE(%s)' &",
// OK		(STR0) dir_name_ext::abs_filename(filename) );

	cmd.print("netscape -remote 'openFILE(%s,new-window)' &",
		(STR0) dir_name_ext::abs_filename(filename) );
#endif
	if(!call_system(cmd)) return FAIL_FAILED();
	return true;
}

bool fork_make( str0 dir, str0 target )
{
	if( name_looks_bad( dir ) ) return false;
	if( name_looks_bad( target ) ) return false;

	buffer2 cmd;
	cmd.print(" cd '%s' && make '%s' ",
		(STR0) dir,
		(STR0) target
	);
	e_print( "Running: %s\n", (STR0) cmd );
	if(!call_system(cmd)) return FAIL_FAILED();
	// sleep_secs(1);
	return true;
}
