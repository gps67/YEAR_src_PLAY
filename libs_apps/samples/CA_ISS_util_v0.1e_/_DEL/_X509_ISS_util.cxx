#include "X509_REQ_hold.h"
#include "X509_ISS_util.h"

#include "X509_VALS_base.h"

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

// was not in REQ!

#include "X509_cert.h"
using namespace CA1;


X509_ISS_util::
X509_ISS_util(
	// X509_VALS_base & _vals,
	X509_VALS_iss_misc & _vals,
	SITE_X509_tag * _layout,
	EVP_P_KEY * client_key_priv_if_available,
	CA_task * _task
)
: vals_iss_misc( _vals)
// , client_cert( NULL )
//issuer_cb_phrase( NULL )
// , issuer_key( NULL )
// , issuer_cert( NULL )
, layout_tag( _layout )
, task(_task)
{
	TRACE("HERE");
	if(!task)
                THROW_bad_mad("NULL task");
	if(!layout_tag)
                THROW_bad_mad("NULL layout_tag");

	task -> client_cert = new X509_cert();
	if(!task -> client_cert) {
		THROW_bad_mad("new X509_cert()");
	}

//	client_key_pub = new EVP_P_KEY();
//	if(!client_key_pub) {
//		THROW_bad_mad("new EVP_P_KEY()");
//	}

	// for me, at the moment, this is always true
	// but it should be possible to operate without it
	// by saving the parts at the different ends of the connection
	//
	if(client_key_priv_if_available)
		task -> client_key_priv = client_key_priv_if_available;

	// not NULL, not used
//	issuer_cert_filename = "unset_issuer_cert.pem";
//	issuer_key_filename = "unset_issuer_key.pem";
}

X509_ISS_util::
~X509_ISS_util()
{
}

bool X509_ISS_util:: ISS_make_file_names()
{
	return task->make_file_names();
}

bool X509_ISS_util:: ISS_obtain_issuer_cb_phrase()
{
	return PASS("DONE already");;
	return true;
}

bool X509_ISS_util:: ISS_obtain_issuer_certificate_and_key()
{
	return task -> obtain_issuer_certificate_and_key();
}



bool X509_ISS_util::
ISS_test1()
{
	return true;
}
