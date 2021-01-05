
#include "zfile_line_in.h"
#include "dgb_fail.h"

/*!
	constructor - no file yet
*/
zfile_line_in::zfile_line_in() : linebuff( 0 ) // can be any value ?
{
	f = NULL;
}

/*!
	Open the compressed text file - which can also be a plain text file 
*/
bool zfile_line_in::open( const char * filename )
{
	close();
	f = gzopen( filename, "rb" );
	if(!f) {
		return FAIL( "open(%s)", filename );
	}
	return TRUE;
}

/*!
	close the file
*/
void zfile_line_in::close( )
{
	if( f ) {
		gzclose(f);
		f = NULL;
	}
}

/*!
	Keep reading until a complete line has been loaded, retain LF

	returns NULL on EOF or ERROR, else pointer to a writable buffer,
	with a guaranteed LF and NUL at the end.

	Caller must NOT free the returned pointer.

	Line length is generally unknown.  nbytes_used is not for external use,
	its probably 0, but is definitely used for long lines re-iterating
	the loop.

*/
char * zfile_line_in::getline()
{

	if( !f ) return NULL;
	linebuff.nbytes_used = 0;
	/*
		There has to be some limit on line length,
		but do remember that many HTML generators write
		a tonne on one line. When this is exceeded, (probably
		by 30%) the line is cut
	*/
#	define LINE_LEN_MAX_ERROR (1024*99)
	while( linebuff.nbytes_alloc < LINE_LEN_MAX_ERROR )
	{
		// nb p[-2] requires at least 2 ! Could then grow by 1 though
		if(!linebuff.get_space( 20 ))
		{
			return (char *)NULL_dgb_fail(
			" zfile_line_in::getline() linebuff cannot expand"
			);
		}
		// obtain the unwritten available buffer
		uchar * gap = linebuff.gap_addr();
		int gaplen = linebuff.space_avail();
		uchar * p = gap + gaplen; // POINTS OUTSIDE
		// plant markers to see if [-2] gets overwritten
		p[-1] = 0; // last null (fgets guarantees)
		p[-2] = 0; // prelast - last part of string (not null)
		/*
			ON ERROR
				fgets returns NULL
			AT EOF
				fgets returns NULL
			ON LAST LINE WITHOUT LF AND EOF
				fgets writes what it gets and \0
				which looks like insufficient space
				so the loop loops with nbytes_used will be >0
				then a NULL will be returned
			ON COMPLETE LINE
				fgets has written it, and the \n and \0
			ON INCOMPLETE LINE - INSUFFICIENT SPACE
				fgets has written as much as it can and \0
				
		*/
		char * s = gzgets( f, (char *) gap, gaplen );
		if( !s ) {
			if( linebuff.nbytes_used ) { // and wanted
				/*
					fixup lack of LF at EOF
					previous loop got the line,
					this iteration got NULL,
					next iteration also gets NULL
				*/
				e_print( "WARNING zfile_line_in::fgets last line of file had no LF !" );
				// yes there is 2 of 20 space!
				linebuff.buff[ linebuff.nbytes_used++ ] = '\n';
				linebuff.buff[ linebuff.nbytes_used++ ] = 0;
				// NB	Next loop makes gzgets hit eof a SECOND time 
				return (char *) linebuff.buff;
			}
			// else // normal EOF or ERROR
			close();
			f = NULL;
			return NULL;
		}
		/*
			// p[0] is outside of buffer
			// p[-1] is \0 (definition of gets)
			// p[-2] is last of loaded string or untouched NUL
		*/
		if( !p[-2] ) {
			// untouched NUL - buffer contains a full line
			// LF is in buffer, or (rare) last line without LF + EOF
			return (char *) linebuff.buff;
		}
		if( p[-2] == '\n' ) {
			// exact fill
			return (char *) linebuff.buff;
		}
		/*
			p[-2] was overwritten and not with a LF
			The line is longer than the buffer (plus NUL byte).
			Retain .nbytes_used, and re-iterate to get more
			// possible EOF, last line without LF and exact fill of buffer !
		*/
		// printf("GROW: '%s'\n", linebuff.buff); fflush(0);
		linebuff.nbytes_used = linebuff.nbytes_alloc-1;
	}
	/*
		This function returns NULL on EOF or ERROR
	*/
	e_print( "ERROR buffer overflow, used %d of %d",
		linebuff.nbytes_used,
		linebuff.nbytes_alloc );
	return NULL;
}

