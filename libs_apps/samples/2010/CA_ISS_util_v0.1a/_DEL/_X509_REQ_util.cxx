#include "X509_REQ_util.h"




X509_REQ_util:: ~X509_REQ_util()
{
}

/*!
	Prepare a REQ without a KEY phrase (yet)
*/
X509_REQ_util:: X509_REQ_util()
:UTIL()
{
	UTIL = new X509_util_CERT();
	init();
}

/*!
	UNUSED
*/
bool X509_REQ_util:: init()
{
	if(REQ) {
		FAIL("do not reuse this class twice - not tested");
		// continue anyway
	}
	//
	// create a new req object
	//
	REQ = new X509_REQ_hold();

	return true;
}

/*!
	AFTER you have setup client_key, call this, to get started
*/
bool X509_REQ_util:: set_client_key_public(EVP_P_KEY * client_key ) // ## WITH implied 'hash'
{
	if(!REQ ) return FAIL("NULL REQ");
	if( !client_key ) return FAIL("NULL client_key");
	//
	// private key must already be loaded or created
	// NB in theory client_key can exist and be empty (so dont do that)
	//

	//
	// pre-apply the client_key PUBLIC key to the cert
	// the actual signing is later though
	//
	if(!REQ->set_pubkey( client_key )) return FAIL_FAILED();

	//
	// add the SHA1 of the public key to the extentions

	if(!UTIL->extensions.add_ext_subject_key_identifier_hash( REQ->req )) return FAIL_FAILED();
	
	PASS("HERE");
	return true;
}

bool X509_REQ_util:: set_client_key_pair(EVP_P_KEY * key)
{
	if(!set_client_key_public(key)) return FAIL_FAILED();
	return true;
}

bool X509_REQ_util:: gen_client_key_pair( CB_get_phrase_base * cb_phrase, int nbits )
{
#warning MOVED TO SITE_X509_layout::

	obj_hold<EVP_P_KEY> key = new EVP_P_KEY( cb_phrase );
	if(!key) return FAIL("NULL new key");
	if(!key->gen_RSA_nbits(nbits)) return FAIL_FAILED();
	if(!set_client_key_pair( key )) return FAIL_FAILED();
	return true;
}

///////////////////////////////////////

/*!
	After adding all the bits to the subject name variable,
	add the subject name to the certificate.
*/
bool X509_REQ_util:: set_subject_name() {
	return REQ->set_subject_name( UTIL->subject );
}

bool X509_REQ_util:: add_extensions()
{
	if(! REQ->add_extensions( UTIL->extensions )) {
		return FAIL_FAILED();
	}
	return true;
}

bool X509_REQ_util:: sign_REQ_with_client_key()
{
	if( !client_key ) return FAIL("NULL client_key");
	return REQ->sign_REQ_with_client_key( client_key );
}

///////////////////////////////////////
#include "X509_VALS_base.h"



/*!
	The str1 vals have been prepared, now apply them ...

	TWO STEPS: build the UTIL area, and set into the REQ,
*/
bool X509_REQ_util:: apply_VALS( X509_VALS_base & vals )
{
	// move the str1/bool vals to the openSSL stucts
	// but without referring to the X509 REQ object
	// so that it can also be an X509 ISS object ;-)
//	if(!UTIL->SET_values_base( vals ))return FAIL_FAILED();


	// copy the str1 values into X509 NAME entries
	if(!UTIL.subject.set( vals.subject )) return FAIL_FAILED();

	if(!extensions.add_ext_CA_depth(vals.CA_depth)) return FAIL_FAILED();

	/*
		extensions.add_ext_subject_key_identifier_hash( cert )

		The RSA id is taken from the cert, into the extensions
		to be put back on the cert, called by ... elsewhere
		see mk_cert
	*/
	// eg mail:lists@information-cascade.co.uk
	if(!UTIL.extensions.OK_add_ext_subjectAltName( vals.subjectAltName ))
		return FAIL_FAILED();

	// key may be used for SIGN, SERVER, ...
	if(!UTIL.extensions.add_ext_key_usage( vals.key_usage )) return FAIL_FAILED();


	// above builds subject, now apply it
	// assign the X509 NAME struct to the X509 REQ
	if(!set_subject_name()) return FAIL_FAILED();

	// the above build a list of extensions (probably fails if NULL)
	if(!add_extensions()) return FAIL_FAILED();

	return true;
}

bool X509_REQ_util:: apply_VALS( X509_VALS_base * vals )
{
	if(!vals) return FAIL("NULL vals");
	return apply_VALS( *vals );
}


////////////////////////////////////////////////////////////
