/*
	refactoring work in progress ....
*/

#include "X509_VALS_SITE_funcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>
#include <openssl/pem.h>

#include "dgb.h"
#include "EVP_P_KEY.h"
#include "SSL_global.h"
#include "CB_get_phrase.h"
#include "BIO_mem.h"
#include "util_buf.h"

// inspired by standard example EX10-5

#define CLIENT_KEY_CIPHER EVP_des_ede3_cbc()
#define CLIENT_KEY_SECRET "secret"
// #define CLIENT_KEY_SECRET NULL
#define CLIENT_KEY_FILE "client_key.pem"
#define CLIENT_REQ_FILE "client_req.pem"
#define CLIENT_REQ_FILE1 "client_req1.pem"



/*
	I want ...
		HANDSET_ID = "HSID_1234" ??

	I also want a class oop for CA_ZERO = CA_ONE = etc
*/
/*
	RTFM 3.1.3 Certificate Extension
*/

int a_kept = -1;
void C_progress_genkey( int a, int b, void * cb_arg )
{
	if( a_kept != a ) {
		 a_kept = a;
		printf("\nC_progress_genkey( %d, ", a );
	}
	printf("%d",b); // basically just counts to unknown varying ceiling
	printf(".");
	fflush(0);
}

#include "X509_VALS_base.h"
#include "X509_VALS_SITE_funcs.h"
#include "X509_util_REQ.h"


int
main(int argc, char *argv[])
{


	//
	// Before anything seed the prng, and init the library
	//
	obj_hold<SSL_global_server_eg>  _ssl_serv = new SSL_global_server_eg();

/*
	Note that libs_apps is a bit touchy about static obj_ref0 objects.
	So its best to use a reference, or go back to pointers
	(albeit ref_counting_smart_pointers)

	If that is a hassle (using . instead of ->) setup a reference

	SSL_global_server_eg & ssl_serv = *_ssl_serv;
*/

	//
	// REQ_VAL are the 'client' request values (set by server, to be sure)
	//

	obj_hold<X509_VALS_SITE_funcs> REQ_VAL = new X509_VALS_SITE_funcs();
//	if(!REQ_VAL->set_cert_values()) return FAIL_FAILED();
if(0)	if(!REQ_VAL->set_HERE_C_pc()) return FAIL_FAILED();
	if(!REQ_VAL->set_HERE_C_user()) return FAIL_FAILED();

 // THESE VALUES ARE ALREADY IN set_cert_values //
 if(0) {
	REQ_VAL->subject.CN = "HSID_4321";
	REQ_VAL->subjectAltName.DNS_1 = "vlan.information-cascade.co.uk";
	REQ_VAL->subjectAltName.DNS_2 = "hublan.information-cascade.co.uk";
//	REQ_VAL->key_usage_critical = true;
//	REQ_VAL->key_usage_digitalSignature = true;
//	REQ_VAL->key_usage_keyEncipherment = true;

//	REQ_VAL->key_usage_keyCertSign = true;
//	REQ_VAL->key_usage_cRLSign = true;

	// REQ_VAL->set_basicConstraint_CA_true_pathlen_3();
	REQ_VAL->CA_depth.set_CA_pathlen(2);
 }

	
	// change any defaults

	// further down REQ_VAL will be applied to the REQ

	//
	// use a plain text passphrase for the client key on disk
	//
	const EVP_CIPHER * enc = CLIENT_KEY_CIPHER; // EVP_des_ede3_cbc()
	obj_hold<CB_get_phrase_blk1> cb_phrase = new CB_get_phrase_blk1(enc);
	cb_phrase->set_phrase( CLIENT_KEY_SECRET ); // my pass phrase
	cb_phrase->debug_set_obj_name( "CLIENT_KEY_SECRET_" );

	// create a new REQ with/out a CB phrase

	obj_hold<X509_util_REQ> REQ = new X509_util_REQ( cb_phrase );
// 	obj_hold<X509_util_REQ> REQ = new X509_util_REQ();

	// create a new RSA for the client (edit if it has one prepared)
	// the RSA will (eventually) be protected with the CB phrase

	if(!REQ->client_rsa->gen_RSA_1024()) return FAIL_FAILED();
//	if(!REQ->client_rsa->gen_RSA_2048()) return FAIL_FAILED();

	// write that RSA to a PEM file, via passphrase encryptor

	if(!REQ->client_rsa->write_PrivateKey( CLIENT_KEY_FILE ))
		return FAIL_FAILED();

	// read that RSA back in ... just for fun you know ...

	if(!REQ->client_rsa->read_PrivateKey( CLIENT_KEY_FILE ))
		return FAIL_FAILED();

	bool crypted = REQ->client_rsa->KEY_was_crypted;
	if(crypted) {
		INFO("KEY was encrypted");
	} else {
		WARN("KEY was NOT encrypted");
	}

	// must init subj before adding fields
	// could make KEY a CTOR parameter

	if(!REQ->init_req_with_pub_key()) return FAIL_FAILED();

// gdb_invoke(true);

	if(!REQ->VALS_SITE_funcs( * REQ_VAL )) return FAIL_FAILED();


	// now process the REQ

	if(!REQ->client_sign()) return FAIL_FAILED();

	if(!REQ->write_req_to_file( CLIENT_REQ_FILE )) return FAIL_FAILED();
//	if(!REQ->write_to_file1( CLIENT_REQ_FILE1 )) return FAIL_FAILED(); // TEST

/* write the completed request */

  return 0;
}
