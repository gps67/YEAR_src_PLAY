#ifndef X509_util_CERT_H
#define X509_util_CERT_H

#include "obj_ref.h"
// #include <openssl/x509.h>
// #include <openssl/x509v3.h>
 
#include "SSL_CA1_stubs.h"
#include "X509_util_EXT.h"
#include "X509_util_NAME.h"

namespace CA1 {


/*!
	X509_util_XXX handles the XXX[NID] layout of fragments of X509
	X509_util_CERT_base brings together X509_util_{NAME,EXT}

	Its main job is to process a prepared set of values X509_VALS_base

		see X509_VALS_base.h		- holds all vars needed
		see X509_VALS_SITE_func.h	- sets above for SITE

	Its main weakness, is a lack of public key, (hash, no signing anything).
	Normal openSSL uses a CTX that has links to the cert, key etc,
	and passes that to the add_ext functions, but since it is only
	used in a few places ... we dont

	This _base, is really a collection of branches, ready to be attached
	to the core cert, (along with use of the public and private keys),
	but these can be prepared from VALS.

	CURRENTLY this is SET from VALS, not GET VALS from ..
	which works because I am doing the REQ and ISS, both on the server.
	Thats easier to 'know' that no weak RSA was supplied,
	nor strange combination of ext values + settings.
	The down-side is that the server knows the clients private key.
	But thats OK for me, here.
*/
class X509_util_CERT_base : public obj_ref0
{
 public:
	X509_util_CERT_base();
	~X509_util_CERT_base();

	X509_util_NAME subject;
	//
	//	"CN",	NID_commonName,
	//	"C",	NID_countryName,
	//	"L",	NID_localityName,
	//	"ST",	NID_stateOrProvinceName,
	//	"O",	NID_organizationName,
	//	"OU",	NID_organizationalUnitName,

	X509_util_EXT extensions;
	//
	//	EMAIL, DNS, IP, URI, ...
	//	key_usage
	//	CA_pathlen
	//	

	// subject_key_identifier = NOT QUITE
	//
	// this requires the public key of the certificate

	bool SET_values_base( X509_VALS_base & vals );

};

///////////////////

/*!
	X509_util handles the NID layout of framgments of X509
	X509_util_CERT brings together X509_util_{NAME,EXT}

	Its main job is to process a prepared set of values X509_VALS_base

		see X509_VALS_base.h		- holds all vars needed
		see X509_VALS_SITE_func.h	- sets above for SITE

	Its main weakness, is a lack of public key, (hash, no signing anything)
*/
class X509_util_CERT : public X509_util_CERT_base
{
 public:
	X509_util_CERT();
	~X509_util_CERT();

 // _base provides ...
 //
 //	X509_util_NAME subject;
 //	X509_util_EXT extensions;
 //	bool SET_values_base( X509_VALS_base & vals );

};

}; // namespace
#endif
