#ifndef X509_REQ_hold_H
#define X509_REQ_hold_H

#include "obj_ref.h"
#include <openssl/x509.h>
#include <openssl/x509v3.h>

#include "SSL_stubs.h"
#include "SSL_CA1_stubs.h"

// #include "X509_util_NAME.h"
// #include "X509_util_EXT.h"


class str1;
// class EVP_P_KEY;
// class CB_get_phrase_base;

namespace CA1 {


class X509_REQ_hold : public obj_ref0
{
 public:
	X509_REQ *req;

	~X509_REQ_hold();
	X509_REQ_hold();

	bool write_to_buffer( blk1 & buf );
	bool write_req_to_file( const char * filename );
	bool set_pubkey( EVP_P_KEY * client_rsa );
	bool get_pubkey( EVP_P_KEY * client_rsa );
	bool sign_REQ_with_client_key( EVP_P_KEY * client_rsa );
	bool add_extensions(X509_util_EXT_list & UTIL);
	bool set_subject_name( X509_util_NAME & _subj );
	bool set_subject_name( X509_NAME * _subj );

};

}; // namespace
#endif

