#ifndef cb_passphrase_base_H
#define cb_passphrase_base_H

#include "obj_ref.h"
#include "buffer2.h"
#include "SSL_global.h"

/*!
	please enter the passphrase for what exactly - show x509?

	passphrase for x509.subject
	passphrase for RSA_privateKey[ "login as me" ];
	passphrase for RSA_privateKey[ "login as admin" ];
	passphrase for RSA_privateKey[ "login as laptop not person" ];
	passphrase for RSA_privateKey[ "to open up keyring" ];
	passphrase for RSA_privateKey[ "keyring[1]key[2]" ];

	ie the keyring mechanism might return individual keys ?
	or all on this keyring share the same phrase (key)
	but several rings have been setup somehow (each one key)
*/
class cb_passphrase_base : public obj_ref0
{
public:
	//
	// can be set close to invocation
	// or permanantly "Enter the passphase"
	// should be UFT8
	// should be SAME as display
	//
	buffer2 prompt1;

	cb_passphrase_base()
	{
	}

	~cb_passphrase_base()
	{
	}

	//
	// set by each cb invocation
	//
	char * out_buff;	// ssl provides a buffer of size ...
	int out_buff_size;	// 
	bool encrypting;	// so ask for the pass twice
	int nbytes_written;	// 

	virtual bool get_passphrase(); // ASCII or binary ?

	bool get_passphrasedata_TTY(); // ASCII or binary ?

	bool install( SSL_CTX * ssl_ctx );
	bool install( SSL_global_base & globl ) {
		return install( globl.ssl_ctx );
	}

	
};

#endif
