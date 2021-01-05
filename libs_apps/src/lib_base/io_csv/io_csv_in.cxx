#include "io_csv_in.h"
#include "dyn_array.h"                                                          
#include "buffer1.h"                                                          

#define DBG2 if(0)

// VIRTUAL
/*!
	destructor - default call to ~z;
*/
io_csv_in::~io_csv_in()
{
}

/*!
	constructor - init null. default call to z()
*/
io_csv_in::io_csv_in()
{
	nfield = 0;
}

/*!
*/
bool io_csv_in::load_file( str0 filename )
{
	return load_file( new str2( filename ) );
}

/*!
*/
bool io_csv_in::load_file( str2 * _filename )
{
	if(!_filename)
	{
		throw "NULL str2 passed";
	}
	str0 filename  = *_filename;

	e_print("io_csv_in::load_file('%s')\n", _filename->sget() );
	if( !z.open( filename )) return FALSE;;

	// scan over each line
	while(1)
	{
		char * line = z.getline();
		if( !line ) break;
		load_line( line );
	}
	z.close();
	return TRUE;
}

/*!
*/
bool io_csv_in::load_line_x( str0 line )
{
	e_print("DEBUGGING io_csv_in::load_line_x( %s )\n",  line.sget() );
	static buffer1 b1;
	b1.clear();
	b1.put_plain( line );
	b1.trailing_nul();
	return load_line( (char *) b1.buff );
}

/*!
	untested
*/
bool io_csv_in::load_text( char * text ) // text is overwritten with NUL's
{
	const char LF = 10;
	const char NUL = 0;

	bool ok = true;
	static buffer1 b1;
	const char * p1 = text;
	unsigned char c1 = *p1;
	while(c1)
	{
		b1.clear();
		unsigned char c2 = NUL;
		while( c2 != LF ) 
		{
			c1 = *p1++;
			c2 = c1 ? c1 : LF;
			b1.put( c2 );
		}
		b1.trailing_nul();
		bool ok_line = load_line( (char *) b1.buff ) && ok;
		if(!ok_line) ok = false;
		b1.clear();
	}
	return ok;
}

#include "CR_LF_NUL.h"
static void trim_trailing_spaces( char * p )
{
	// e_print( "TRIM(%s)\n", p );
	// const char NUL = 0;
	// const char SP = 32;
	// const char TAB = 9;
	char * p_sp = NULL; 	// overwrite or NULL
	while(1)
	{
		char c = *p;
		switch( c ) {
		 case NUL:
			// overwrite first of last
			if( p_sp ) *p_sp = NUL;
			return;
		 case SP:
		 case TAB:
			// remember first of last 
			if( !p_sp ) p_sp = p;
		 break;
		 default:
			// not part of last
			p_sp = NULL;
		}
		p++;
	}

}

/*!
*/
bool io_csv_in::load_line( char * line )
{
	const char CR = 13;
	const char LF = 10;
	const char SP = 32;
	const char NUL = 0;
	const char TAB = 9;
	const char HASH = '#';

	switch( line[0] ) 
	{
		case CR:	// DOS blank line
			this->got_blank_line();
			return TRUE;;
		case LF:
			this->got_blank_line();
			return TRUE;;
		case NUL:
			this->got_blank_line();
			return TRUE;; // this means break ??
		case HASH:
			this->got_comment();
			return TRUE;; // skip comment lines
	}
	char * p1 = (char *) line;

	if(0) fprintf(stderr, "LINE: %s$", line ); // has own LF

	fields.rezero();

	// split line into fields
	nfield = 0;
	bool LF_PENDING = TRUE;
	while(LF_PENDING)
	{
		// keep str for removing trailing spaces
		char * p1_trims = p1;

		// a field is ALWAYS found, even empty line
		// save the start pointer
		fields.extend_to(nfield+1);
		fields[nfield] = p1;
		nfield++;
		// scan over the text and overwrite a nul
		// leave end char in C and pointed to!
		char c = 'X'; // c-prev was (anything)
		while(c)
		{
			c = *p1;
			if(0) {
				fprintf(stderr,"%c", c );
				fflush(0);
			}
			switch(c) {
			 case NUL:
				LF_PENDING = FALSE; // seen something
				trim_trailing_spaces( p1_trims );
				break;
			 case ',':
				// overwrite COMMA with a NUL
				*p1 = NUL;
				p1++;
				// skip any following spaces - OR TABS
				while( ( *p1 == ' ') || ( *p1 == TAB ) ) p1++;
				// p1 points to next field or LF
				// nul signals end of field

				trim_trailing_spaces( p1_trims );
				p1_trims = p1;
				c = NUL;
				break ;
			 case CR:
				if( p1[1] != LF ) 
				{
					// treat like plain char!
					p1++;
					continue;
				}
				// overwrite with a NUL
				*p1 = NUL;
				p1 ++;
				trim_trailing_spaces( p1_trims );
				c = NUL;
				LF_PENDING = FALSE; // seen it
				break;
			 case LF:
				// overwrite with a NUL
				*p1 = NUL;
				// move to next char (should be null)
				p1 ++;
				trim_trailing_spaces( p1_trims );
				c = NUL;
				LF_PENDING = FALSE; // seen it
				break;
			 case SP:
			 case TAB:
				; // dont advance p1_trims through spaces
			 default:
				p1_trims = p1;
				p1++;
				 // point to NEXT as poss first of last space
			}
		}
	}
	got_line_1();
	return TRUE;
}

// ----------------------------------

/*!
*/
void io_csv_in::print_fields()
{
	fprintf(stderr, "\n" );
	for( int i = 0; i<nfield; i++ ) 
	{
		fprintf(stderr, "field[%d] = '%s'\n", i, fields[i] );
	}
}

/*!
*/
// VIRTUAL
void io_csv_in::got_comment()
{
}

/*!
*/
// VIRTUAL
void io_csv_in::got_blank_line()
{
}

/*!
*/
// VIRTUAL
bool io_csv_in::got_line_1()
{
	return TRUE; // continue
}
