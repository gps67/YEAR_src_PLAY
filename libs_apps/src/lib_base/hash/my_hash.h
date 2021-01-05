#ifndef my_hash_H
#define my_hash_H

#include "ints.h"
#include "p0p2.h"
#include "blk1.h"

/*!
	my_hash is an INT used by strings and hash keys,
	and a method of adding more things in.

	A zero value usually means "is_uncalculated()",
	but most code knows that its half way through.

	Hash tables will reduce the value, probably by truncating
	bits, or modulus.
*/
struct my_hash : public GRP_lib_base
{
	//! The calculated (stored) value (non-zero !)
	uint hash;

	//! constructor -
	my_hash()
	{
		init();
	}

	//! The initial value is multi-bit
	void init()
	{
		hash = 0xdeaf;
		//
		// hmmm - actually Id like it to be zero now!
		// the non-zero-ness is only useful if you detect ZERO+OV
		//
	}

	//! add a byte or int to the hash sum (but I use unsigned u8 bytes)
	void incl( int h )
	{
		hash += h;
		hash *= 0x10509; // bit 0 is set
		// although it is not prime nor anything special
		// factor 13365: 3 3 3 3 3 5 11
		//
		// echo 'ibase=16 ; 10309' | bc | factor
		// 66313: 13 5101

		// it is nicely scattered
		// and useable as (h % 32) or (h%256) or 31,255,...
		// particularly when it will overflow 32 bits in 2 or 64 in 3

// no point //	hash += hash >> 16;
	} 

	/*
		hash computation should be centralised
		for different forms of string (and other)
		so they come here for their shared definition

		DO NOT hash the trailing NUL
		(its less to forget, and it allows these to be called over
		multiple segments of strings)
		BUT it leaves nul-string the same as ininitialised?

		NB a tuple of strings is not a concatenation
		You can have a separator hash.incl( ',' + 0x1234 )
		or save a few cycles by reusing the existing hashes of
		the components (a,b,c) hash.incl( a.hash.hash ) 
			hash.incl( a.get_hash() ) 

		Strings do not have the trailing NUL included,
		since technically it isnt part of the string,
		just the container. blk1 and P0p2 likewise,
		except they may include NUL data bytes, if you wish
		(nb blk1.trailing_null() is _OUTSIDE_ of the blk1 core)

		The hash used is intended to quickly distinguish
		strings within a hash table, with minimum computation.
		The hash table is expected to deal with collisions,
		possibly even with long chains triggering a second
		hash using a DIFFERENT MODULUS of the SAME hash.

		Long chains can be searched quickly, provided the MMAP
		pages are all in cache, because the 32-bit hash
		is different from the u32%table_size_64 (but do beware 
		of non-string data, eg pointers all ending with 3 zero bits)

		The hash is one-size-fits all, and is intended to be
		experimented with (recompile all inline users),
		but then named and frozen for persistant data.

		You probably can switch between 32 and 64 bit,
		as long as you truncate to 32 bit, as the math
		uses addition + multiplication, which moves left,
		not division or any right shift.
	*/

	/*!
		include every byte of string into the hash
		(not the terminating NUL)
	*/
	void incl_str( const u8 * s ) {
		while( u8 c = *s++ ) { // unsigned is significant
			incl( c );
		}
	}

	/*!
		include every byte of string into the hash
		(include embedded NULs but not trailing NUL beyond nbytes)
	*/
	void incl_str( const u8 * s, int len ) {
		while( len > 0 ) {
			len--;
			u8 c = *s++; // unsigned
			incl( c );
		}
	}

	/*!
		include every byte of string into the hash
		(include embedded NULs but not trailing NUL beyond nbytes)
	*/
	void incl_str( const p0p2 zone ) {
		u8 * s = zone.p0;
		int l = zone.str_len();
		incl_str( s, l );
	}

	/*!
		include every byte of string into the hash
		(include embedded NULs but not trailing NUL beyond nbytes)
	*/
	void incl_str( const blk1 & buf )
	{
		incl_str( buf.buff, buf.nbytes_used );
	}

};

#endif
