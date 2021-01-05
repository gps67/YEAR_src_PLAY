#include "CB_get_phrase.h"
#include "key_holder.h"

int CB_get_phrase_blk1:: get_phrase( u8 * buf, int maxlen )
{
	int len = phrase.nbytes_used;
	if( len > maxlen ) {
		FAIL("pem_passwd_cb_obj: len %d > maxlen %d",
			len, maxlen );
		return 0;
	}
	if( len == maxlen ) {
		WARN("len exactly fills maxlen - no trailing nul");
	} else {
		buf[len] = 0;
	}
	memcpy( buf, phrase.buff, len );
	INFO("%s returning phrase with %d bytes", debug_get_obj_name(), len );
#if 0
#warning bad debug choice
 if(1) {
	buffer2 buf;
	buf.hex_dump_into_8859( phrase );
	WARN("%s", (STR0) buf );
 }
#endif
	return len;
}

bool CB_get_phrase_blk1:: is_null_phrase()
{
	// if both encryption method and phrase length - it isnt null
	if( enc && phrase.nbytes_used ) return false; // not null
	if( !enc && !phrase.nbytes_used ) return true; // both null
	if( !enc ) {
		WARN("phrase set but encoding is NULL, ok for decryption");
		return false; // phrase is not null
	}
	INFO("phrase is NULL but encoding isnt NULL");
	// OK you can set an empty phrase
	// but not have to switch to a CB_get_phrase_NONE 
	// nor forget the preferred encryption method
	// just call the right things at the right time, as if NONE
	return true;
}



CB_get_phrase_key_holder::
CB_get_phrase_key_holder( key_holder & _key )
: CB_get_phrase_base( _key.get_cipher_type() )
, key( _key )
{
}

int CB_get_phrase_key_holder:: get_phrase( u8 * buf, int maxlen )
{
	int len = key.blk_key.nbytes_used;
	if( len > maxlen ) {
		FAIL("pem_passwd_cb_obj: len %d > maxlen %d",
			len, maxlen );
		return 0;
	}
	if( len == maxlen ) {
		WARN("len exactly fills maxlen - no trailing nul");
	} else {
		buf[len] = 0;
	}
	memcpy( buf, key.blk_key.buff, len );
	INFO("%s returning munged phrase with %d bytes", debug_get_obj_name(), len );
	return len;
}

bool CB_get_phrase_key_holder:: is_null_phrase()
{
	return key.blk_key.nbytes_used == 0;
}

