#ifndef BIO_mem_H
#define BIO_mem_H

#include "buffer2.h"

// TODO use in X509_cert:: PEM_in _out
// DONE use in EVP_P_KEY

class BIO_mem
{
	BIO * bio_mem;
 public:

	BIO_mem()
	{
		bio_mem = BIO_new(BIO_s_mem());

	}

	operator BIO * () {
		return bio_mem;
	}

	~BIO_mem()
	{
		if(bio_mem) BIO_free( bio_mem );
	}

	bool write_to_buffer( blk1 & buf )
	{
		BUF_MEM * bptr;
		BIO_get_mem_ptr(bio_mem, &bptr);
		return buf.put_nn_bytes( bptr->length, bptr->data );
	}

};

class BIO_mem_read
{
	BIO * bio_mem;
 public:

	BIO_mem_read( buffer2 & mem_in )
	: bio_mem( NULL )
	{
		// buffer2 must continue to exist
		int len_max = mem_in.nbytes_used;
		const unsigned char *in = uSTR0_from_STR0( mem_in );
		bio_mem = BIO_new_mem_buf( in, len_max );
		// READONLY not deleted

	}

	operator BIO * () {
		return bio_mem;
	}

	~BIO_mem_read()
	{
		if(bio_mem) BIO_free( bio_mem );
	}

};

#endif

