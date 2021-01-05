#ifndef CB_get_phrase_H
#define CB_get_phrase_H

#include "CB_get_phrase_base.h"
#include "dgb.h"
#include "buffer2.h"

class key_holder;

class CB_get_phrase_blk1 : public CB_get_phrase_base 
{
 public:

	blk1 phrase;

	CB_get_phrase_blk1( const EVP_CIPHER * _enc = NULL )
	: CB_get_phrase_base( _enc )
	{
		// add key to memory scrub area list for fast exit
	}

	~CB_get_phrase_blk1()
	{
		// scrub key from memory
	}

	void set_phrase( const char * _phrase ) {
		phrase.set(_phrase);
	}

	int get_phrase( u8 * buf, int maxlen ); // memcpy from blk1 key
	bool is_null_phrase();

};

class CB_get_phrase_key_holder : public CB_get_phrase_base
{
 public:
	key_holder & key;

	CB_get_phrase_key_holder( key_holder & _key );
	int get_phrase( u8 * buf, int maxlen ); // memcpy from key.blk_key
	bool is_null_phrase();
};

#endif
