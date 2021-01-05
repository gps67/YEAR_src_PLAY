#include "X509_REQ_hold.h"
#include "X509_VALS_base.h"
#include "X509_util_NAME.h"
#include "X509_util_EXT.h"

#include "SSL_global.h"
#include "CB_get_phrase.h"
#include "dgb.h"
#include "EVP_P_KEY.h"
#include "BIO_mem.h"
#include "util_buf.h"

#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>
#include <openssl/pem.h>
using namespace CA1;

////////////////////////

X509_REQ_hold:: 
X509_REQ_hold() {
	req = X509_REQ_new();
}

X509_REQ_hold:: 
~X509_REQ_hold() {
	if(req) X509_REQ_free(req);
	req = NULL;
}

bool X509_REQ_hold:: write_to_buffer( blk1 & buf )
{
	BIO_mem bio_mem;
	if(!PEM_write_bio_X509_REQ( bio_mem, req )) {
		return FAIL("PEM_write_bio_X509_REQ()");
	}
	if(!bio_mem.write_to_buffer( buf )) return FAIL_FAILED();
	STEP("DONE");
	return true;
}

bool X509_REQ_hold:: write_req_to_file( const char * filename )
{
	INFO("%s", filename);
	blk1 buf;
	buf.get_space( 2000 ); // 700 +
	if(!write_to_buffer( buf )) return FAIL_FAILED();
	if(! blk_write_to_file( buf, filename )) return FAIL_FAILED();
	INFO("DONE %s", filename);
	return true;
}

bool X509_REQ_hold:: set_pubkey( EVP_P_KEY * client_key )
{
	if(!client_key) return FAIL("NULL client_key");
	if(!X509_REQ_set_pubkey(req, client_key->pkey))
		return FAIL("X509_REQ_set_pubkey()");
	return true;
}

bool X509_REQ_hold:: get_pubkey( EVP_P_KEY * client_key )
{
	if(!client_key) return FAIL("NULL client_key");
	EVP_PKEY * pkey = X509_REQ_get_pubkey(req); 
	if(!pkey) {
		// NULL key returns false too
		// clear any possible previous key from elsewhere
		client_key->set(NULL);
		return FAIL("X509_REQ_get_pubkey(%s)", debug_get_obj_name());
	}
	client_key->set( pkey );
	return true;
}

bool X509_REQ_hold:: sign_REQ_with_client_key( EVP_P_KEY * client_rsa )
{
	// pick the correct digest
	const EVP_MD *digest = client_rsa ->get_correct_digest_method_for_X509_sign();
	if(!digest) return FAIL_FAILED();

	if(!X509_REQ_sign( req, client_rsa->pkey, digest))
		return FAIL("Error signing request");
	STEP("DONE");
	return true;
}

bool X509_REQ_hold:: add_extensions(X509_util_EXT_list & EXTS)
{
	if(!X509_REQ_add_extensions(req, EXTS.extlist))
		return FAIL("Error adding extensions to the request");
	STEP("DONE");
	return true;
}

bool X509_REQ_hold:: set_subject_name( X509_util_NAME & NAME )
{
	return set_subject_name( NAME.subj );
}

/*! DITTO but PRIVATE ?
*/
bool X509_REQ_hold:: set_subject_name( X509_NAME * _subj )
{
	if(!X509_REQ_set_subject_name(req, _subj))
		return FAIL("Error adding subject to request");
	STEP("DONE");
	return true;
}




////////////////////////


