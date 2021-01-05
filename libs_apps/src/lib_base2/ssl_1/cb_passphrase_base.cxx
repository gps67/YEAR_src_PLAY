
#include "cb_passphrase_base.h"
#include "dgb.h"
#include <openssl/ssl.h>


int C_pem_passwd_cb(char *buf, int size, int rwflag, void *userdata)
{
	cb_passphrase_base * self = (cb_passphrase_base *) userdata;
	if(!self) {
		FAIL("NULL self");
		return 0;
	}

	self->out_buff = buf;
	self->out_buff_size = size;
	self->encrypting = rwflag; // 0=read 1=write

	self->nbytes_written = 0;	// 

	// do some defaults

	if( !self->prompt1 ) {
		self->prompt1.print("Enter the passphrase");
	}

	if( self->get_passphrase() ) {
		return self->nbytes_written;
	} else {
		// FAIL already called ?
		WARN("self->get_passphrase() returned FALSE");
		return 0;
	}
}

//	cb_passphrase_base:: 
bool cb_passphrase_base:: install( SSL_CTX * ssl_ctx )
{
	if(!ssl_ctx) return FAIL("NULL ssl_ctx");
	SSL_CTX_set_default_passwd_cb( ssl_ctx, C_pem_passwd_cb );
	SSL_CTX_set_default_passwd_cb_userdata( ssl_ctx, this );
	return true;
}

