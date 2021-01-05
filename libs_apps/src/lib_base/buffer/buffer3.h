#ifndef buffer3_H
#define buffer3_H

#include "buffer2.h"
#include "str1.h"
#include "dgb_fail.h"

/*!
	buffer3 adds offs_front, a left gap in a head+tail buffer

	The circ is not really circular - but it might be for subclasses?

	The front_gap IS included in the data (eg get_space)
	but if you regulrly clear it down, there isnt one ;-)
*/
struct buffer3 : public buffer2
{
	unsigned int offs_front;

	/*!
		constructor - init empty
	*/
	buffer3( int N ) : buffer2( N ) { offs_front = 0; }

	/*!
		constructor - init empty
	*/
	buffer3() : buffer2() { offs_front = 0; }

	/*!
		destructr - plain
	*/
	~buffer3() { /* dtor */ }

	void circ_copy_down_fn();
	/*!
		reduce the front gap to zero
	*/
	void circ_copy_down()
	{
		if(offs_front) circ_copy_down_fn();
	}

	/*!
		the number of bytes USED in the circular buffer (may move buffer)
	*/
	int circ_len()
	{
		if( !offs_front )
			return nbytes_used;
		else {
			int l = nbytes_used - offs_front;
			if(!l) circ_clear();
			return l;
		}
	}

	/*!
		differs from blk1
	*/
	operator bool()
	{
		return circ_len();
	}

	/*!
		return the pointer to the front

		Always call this last, as others might move it
	*/
	uchar * circ_addr()
	{
		return buff + offs_front;
	}

	/*!
		return the pointer to the front of the data - READ
	*/
	uchar * circ_addr_rd()
	{
		return buff + offs_front;
	}

	/*!
		return the pointer to the front of the gap - WRITE
	*/
	uchar * circ_addr_wr()
	{
		return buff + nbytes_used;
	}

	/*!
		return the number of (bytes) available
	*/
	int circ_space_avail()
	{
		// the space above the circ mid part
		// there is additional space before off_front
		// but thats only available after copy_down - circ_check()
		return space_avail();
	}

	/*!
		return the number of (bytes) available
	*/
	int circ_data_avail()
	{
		return circ_len();
		// return nbytes_used - offs_front;
	}

	/*!
		return the number of (bytes) _NOT_ available

		mostly for debugging, when circ copy down is not being called
	*/
	int circ_lost_space_unavail()
	{
		return offs_front;
	}

	void dgb_show( const char * s1=NULL, const char * s2=NULL );

	/*!
		advance the front pointer
	void circ_advance( int by )
	{
		offs_front += by;
		circ_check();
	}
	*/

	// ALIAS // void circ_removed( int used ) { circ_used( used ); }
	//
	/*!
		data has been taken from the circ buffer
	*/
	void circ_used( int used )
	{
		if( used < 0 ) THROW_dgb_fail( "circ_used negative sign - see circ_added" );
		// circ_advance( used );
		offs_front += used;
		// circ_check();
	}

	/*!
		data has been added to the circ buffer
		put bytes also does this ...
	*/
	void circ_added( int added )
	{
		if( added < 0 ) THROW_dgb_fail( "circ_added negative sign - see circ_used" );
		nbytes_used += added;
	}

	void circ_check_fn();
	void circ_check()
	{
		if( !offs_front ) return;
		circ_check_fn();
	}

	/*!
		advance the tail pointer
		// now circ_added( addedbytecount )
	void gap_advance( int by )
	{
		nbytes_used += by;
	}
	*/

	/*!
		clear() with added offs_front
	*/
	void circ_clear()
	{
		STEP("ZMPTY");
		buffer2::clear();
		offs_front = 0;
	}

private:
	void clear()
	{
		FAIL("clear called on buffer3");
	}

};

#undef Put
#undef Get_space

#endif

