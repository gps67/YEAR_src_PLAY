#ifndef my_hash_H
#define my_hash_H

/*!
	Common hash function, and storage variable

	The 32-bit value can be folded into less

	Not for use in mixed width environments (files) though
	truncating it to 32 bits should be OK

	The value IS non-zero (except where overflow chops to zero),
	and zero is recommended as a value-flag.
*/
struct my_hash {
	//! The calculated (stored) has value (non-zero !)
	int hash;
	//! constructor -
	my_hash()
	{
		init();
	}
	//! The initial value is multi-bit
	void init()
	{
		hash = 0xdeaf;
	}
	//! add a byte or int to the hash sum
	void incl( int h )
	{
		hash += h;
		hash *= 0x10509; // bit 0 is set
// no point //	hash += hash >> 16;
	} 
};

#endif
