
#include <stdio.h>
#include <glib.h>
#include "g_rrr.h"

#include "key_file_enc.h"
#include "dgb.h"
#include "dgb.h"

#include "key_holder.h"
#include "evp_cipher.h"
#include "util_buf.h"


Key_file_enc::Key_file_enc( G_rrr & g_rrr )
:Key_file( g_rrr )
{
}

bool Key_file_enc::load_from_encrypted_file( key_holder & ciph, const char * filename )
{
	evp_cipher_base evp( ciph, de_crypt );
//	evp.init_decrypt();
	u32 max_K_size = 200;
	if(!blk_read_entire_file( evp.buf_in, filename, max_K_size )) return FAIL_FAILED();
	if(!evp.process_entire_buf()) return FAIL_FAILED();
//	if(!evp.update_from_buf()) return FAIL_FAILED();
//	if(!evp.final()) return FAIL_FAILED();
	if(!load_from_buf( evp.buf_out )) return FAIL_FAILED();
	return true;
}

bool Key_file_enc::save_into_encrypted_file( key_holder & ciph, const char * filename )
{
	evp_cipher_base evp( ciph, en_crypt );
//	evp.init_encrypt();
	if(!save_into_buf( evp.buf_in )) return FAIL_FAILED();

	if(!evp.process_entire_buf()) return FAIL_FAILED();
//	if(!evp.update_from_buf()) return FAIL_FAILED();
//	if(!evp.final()) return FAIL_FAILED();

	if(!blk_write_to_file( evp.buf_out, filename )) return FAIL_FAILED();
	return true;
}

// to avoid accidentally saving in plain text

bool Key_file_enc::load_from_file( const char * filename, bool error_if_absent )
{
	return FALSE_dgb_fail("encryption requires a KEY");
}

bool Key_file_enc::save_into_file( const char * filename ) {
	return FALSE_dgb_fail("encryption requires a KEY");
}
