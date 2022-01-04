#ifndef map_file_H
#define map_file_H

#include "ints.h"	// u8 GRP_
#include "p0p2.h"	// ger P0P2

/*!
	MMAP memory-mapped file holder

	map_file is more general mmap_file
	which might include SEGMENT elf dlopen ld_load

	When in ROM we have to be able to detect EOF with no NUL in sight
	When in MMAP we MIGHT have a trailing NUL beyonf EOF upto PAGE_EDGE
	When in SEGMENT use P0P2 

	MMAP can also borrow  pages from elf_LOADED_SEGMENTS incl RW_sections!
	MMAP_DATA asks HEAP_CTRL for access to PAGES within SHARED FILES
	MMAP_DATA asks FILE_CTRL for PAGES of MMAP as if from filename.ext
*/
struct mmap_file : public GRP_lib_base // NO BASE CLASS // TODO ref0 
{
	mmap_file( void );
	~mmap_file();

	int fd;		/* fd of file */

	int nbytes;	/* file length from inode */
	char * page0;	/* mmapped */ /* NOT alloc'ed */
	bool writable;	/* was opened RDWR */
	// compiler KNOW that this function returns a complex WORD_TRIPLE
	// compiler FIND which side of the API provided the data_buffer
	// we are collecting our own copy INIT_VALUE 
	// we are optimising CACHE over own data //
	// we can init then forget callers // noone is moving stuff (yet)
	// caller provided memory WORD_TRIP WORD_PAIR WORD_ITEM WORD_FLAG
	// caller provides it's P0P2 var, which this script CTOR's into
	// scope callers p0p2 * data_buffer
	// scope callers p0p2 & data_buffer
	// scope callers p0p2   data_buffer = operator p0p2 () 

        // INLINE // p0p2( u8 * P0, unsigned int len )
        // INLINE // {
        // INLINE //         p0 = P0;
        // INLINE //         p2 = p0 + len;
        // INLINE // }
	// GEN
	// GEN 	CALLERS_P0P2 . p0 = page0
	// GEN 	CALLERS_P0P2 . p2 = p0 + nbytes
	// GEN  EA = data_buffer // where result is placed // being CTOR'd
	// CTOR of data_buffer 
	// operator p0p2 () { return p0p2( page0, nbytes ); } INLINE
	// p0 = page0
	// p2 = p0 + nbytes
	// CALLERS	data_buffer .p0 = page0
	// CALLERS	data_buffer .p2 = p0 + nbytes
	// BRIDGE 	compiler_word p0 
	// COMPILER sees it immediately

	// operator p0p2 () { return p0p2( page0, nbytes ); }

	operator p0p2 () {
	  return p0p2(
		  page0,	// INLINE // callers p0 = page0
		  nbytes	// INLINE // callers p2 = p0 + nbytes
		 );
	 }

	/*
		CALLER
		 ALLOC
			p0p2 p0p2_file;
		 CTOR
		 	p0p2_file = (p0p2) mmap_file
		 CODE
		 	p0p2_file = (p0p2) {
			  return p0p2(
				  page0, // INLINE // p0p2_file p0 = page0
				  nbytes // INLINE // p0p2_file p2 = p0 + nbytes
			 );
			}
			// KEYWORD p0p2_file // HIGHLIGHT ALL USING // SUBST //
			// view "p0p2_file" // but without Q2 // layering //
			
	*/
	/*	WHY THIS VIEW
		
		caller provides

			data_buffer =
			"// CXX // p0p2 p0p2_file // = INIT_CODE // = SET_GET"

		VAR { p0p2 data_buffer } // caller provided data_buffer
		CACHE GET
			SET GET directly to data_buffer
			// data_buffer owned by "caller"


		SCOPE
		 GEN	p0p2 p0p2_file // = INIT_CODE // = SET_GET"

	*/


//	operator p0p2 () { return p0p2( page0, nbytes ); }

	// COMPILER KNOWS HOW TO RETURN WORD_PAIR_TRIP // A A_MORE
	// EG caller provides WORD_PAIR of registers CTOR leaves them set
	// EG caller provides data_buffer CTOR writes into memory // not reg
// C	p0p2 GET_P0P2_VAL( mmap_file * mmap ) { ... }
	// GEN_AVAIL_DATA // CTOR data_buffer ...
	p0p2 GET_P0P2_VAL() { // EITHER SIDE CAN PROVIDE p0p2 data_buffer
		// the prototype "looks like" a copy during calls return value
		// the compiler want CTOR DIRECT to EA // COPY to EA
		// that copy gets optimised out as CTOR data_buffer // DIRECT
		return (p0p2) this; // see operator p0p2 //
	}

	// COMPILER KNOWS HOW TO RETURN WORD_PAIR_TRIP // A A_MORE
	bool GET_P0P2_VAR( mmap_file * mmap, P0P2 * P0P2_retval ) { // COPY
		* P0P2_retval = (p0p2) this; // write output to callers retval
		return true; // goto next_step
	}

	// compiler CTOR direct to VAR or WORD_PAIR // and provide PTR origin
	// memory written, WORD_PAIR returned as floating CURR_VAR p0p2
	p0p2 & get_P0P2( mmap_file * mmap, P0P2 & P0P2_retval ) { // EA
		return P0P2_retval = (p0p2) mmap ; // NOTE DIALECT of "&"
	}
	// OPTION RETVAL bool
	// OPTION RETVAL p0p2
	// OPTION RETVAL p0p2 * PTR ; // either side //
	// bool GET_P0P2( mmap_file * mmap, P0P2 & P0P2_retval ) { SCRIPT }

/*
	TODO GET_P0P2( mmap_file, P0P2 ) {
		SEGMENT is LOCK RDWR RO 
		PARSER wants RO
		STO wants RW
		SCRIPT wants RO SNAPSHOT

		STO wants to relocate the MMAP as it grows
		STO wants to flip between ODD EVEN CYCLES
		STO wants RDWR
		ROM wants RO

		scan_nl wants, unix specifies, trailing LF within file
		STR0 has at least NUL as last byte
		MMAP probably adds a NUL, but might not // 4K next is random

		P0P2 detects EOF // checks every EOLN NL NEWLINE LF 
	}
*/

	// callers call this, it holds operator GET (p0p2)

	int map_in_file( const u8 * filename, int wwritable );
	int map_in_file( const char * filename, int wwritable )
	{
		return map_in_file( (const u8 *) filename, wwritable );
	}

	// remap does the MMAP and FLASGS

	void mmap_unmap( );		/* unload file */
	int mmap_remap( );	/* eg after grow file */

	int fsync( );		/* calls ::fsync() */
	int sync( )		{ return fsync(); }

	bool close();

	int grow_file( int new_size );	/* appends to file */

	int test1(void);

	bool check_nl_at_eof();
	bool check_nul_after_eof();

};
#endif
