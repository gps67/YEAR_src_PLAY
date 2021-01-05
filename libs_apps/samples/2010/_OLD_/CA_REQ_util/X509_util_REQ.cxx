// #include "X509_util_base.h"
#include "X509_util_REQ.h"
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

X509_util_base::
X509_util_base()
: subject() // , subj( NULL )
, extensions()
{
}

X509_util_base::
~X509_util_base()
{
}


/*!
	Copy the VALS down to the prep-zones, ready to add to cert
*/
bool X509_util_base:: SET_values_base( X509_VALS_base & vals )
{

	// copy the str1 values into X509 NAME entries
	if(!subject.set( vals.subject )) return FAIL_FAILED();

	if(!extensions.add_ext_CA_depth(vals.CA_depth)) return FAIL_FAILED();

	// no original data to take it from
	// maybe move to the CA_depth part ??
#warning NOT HERE NEITHER subject_key_identifier_hash
if(0)	if(!extensions.add_ext_subject_key_identifier_hash()) return FAIL_FAILED();

	// eg mail:lists@information-cascade.co.uk
	if(!extensions.OK_add_ext_subjectAltName( vals.subjectAltName ))
		return FAIL_FAILED();

	// key may be used for SIGN, SERVER, ...
	if(!extensions.add_ext_key_usage( vals.key_usage )) return FAIL_FAILED();

	return true;
}

////////////////////////


X509_util_REQ:: ~X509_util_REQ()
{
	if(req) X509_REQ_free(req);
	req = NULL;
}

/*!
	Prepare a REQ without a KEY phrase (yet)
*/
X509_util_REQ:: X509_util_REQ()
: X509_util_base()
, req(NULL)
{
	client_rsa = new EVP_P_KEY();
	init();
}

/*!
	Prepare a REQ with a KEY phrase
*/
X509_util_REQ:: X509_util_REQ(
	 CB_get_phrase_base * _cb_pass
)
: X509_util_base()
, req(NULL)
{
	client_rsa = new EVP_P_KEY( _cb_pass );
	init();
}

/*!
	UNUSED
*/
bool X509_util_REQ:: init()
{
	if(req) {
		FAIL("do not reuse this class twice - not tested");
		// continue anyway
	}
	//
	// create a new req object
	//
	req = X509_REQ_new();
	if(!req ) return FAIL("X509_REQ_new()");

	return true;
}

/*!
	AFTER you have setup client_rsa, call this, to get started
*/
bool X509_util_REQ:: init_req_with_pub_key()
{
	if(!req ) {
		FAIL("CODE_ERROR - init() should have set this");
		if(!init()) return FAIL_FAILED();
	}

	//
	// private key must already be loaded
	// NB client_rsa can exist and be empty (so dont do that)
	//
	if(!client_rsa) return FAIL("NULL client_rsa");

	//
	// pre-apply the client_rsa PUBLIC key to the cert
	// the actual signing is later though
	//
	if(!X509_REQ_set_pubkey(req, client_rsa->pkey))
		return FAIL("X509_REQ_set_pubkey()");

	return true;
}

/*!
	convert "commonName" to NID_commonName

	man OBJ_nid2obj
	openssl-1.0.0/doc/crypto/OBJ_nid2obj.pod
	openssl/objects.h
	openssl-1.0.0/crypto/asn1/a_strnid.c
	openssl-1.0.0/crypto/objects/obj_dat.h

"CN",	"commonName",			NID_commonName,
"C",	"countryName",			NID_countryName,
"L",	"localityName",			NID_localityName,
"ST",	"stateOrProvinceName",		NID_stateOrProvinceName,
"O",	"organizationName",		NID_organizationName,
"OU",	"organizationalUnitName",	NID_organizationalUnitName,

	and then other ASN enum-names for other things,
	including other parts of the X509 and KEY and ...


*/
// static
bool X509_util_REQ:: longName_to_NID( const char * longName, int & nid )
{
	if((nid = OBJ_txt2nid((STR0) longName)) == NID_undef)
	{
		return FAIL_1("longName '%s'", (STR0) longName);
	}
	return true;
}

/*!
	convert "CN" to NID_commonName (or not)
*/
// static
bool X509_util_REQ:: shortName_to_NID( const char * shortName, int & nid )
{
	if((nid = OBJ_sn2nid((STR0) shortName)) == NID_undef)
	{
		return FAIL_1("shortName '%s'", (STR0) shortName);
	}
	return true;

}

// static
const char * X509_util_REQ:: shortName_of_NID( int nid )
{
	return OBJ_nid2sn( nid );
}

// static
const char * X509_util_REQ:: longName_of_NID( int nid )
{
	return OBJ_nid2ln( nid );
}

bool X509_util_REQ:: add_subject_NID_value( int nid, const char * value )
{
	return subject.OK_add_entry_nid( nid, value );
}

/*!
	add_subject_longName_value( "countryName", "GB" )
*/
bool X509_util_REQ:: add_subject_longName_value( const char * longName, const char * value )
{
	int nid = NID_undef;

	// convert longName "countryName" to nid
	if(!longName_to_NID( longName, nid )) return FAIL_FAILED();

	// add it to subject
	if(!subject.OK_add_entry_nid( nid, value )) return FAIL_FAILED();
	return true;
}

/*!
	The CN is the commonName, is the full name of the person/machine

	You must call this during the setup subj phase

	but see also Alternate ... extensions
bool X509_util_REQ:: add_CN( const char * value ) {
	// could use compiled NID, or not
	return subject.add_entry_nid( NID_commonName, value );
}
*/

/*!
	After adding all the bits to the subject name variable,
	add the subject name to the certificate.
*/
bool X509_util_REQ:: set_subject_name() {
	return set_subject_name( subject.subj );
}

/*! DITTO but PRIVATE ?
*/
bool X509_util_REQ:: set_subject_name( X509_NAME * _subj )
{
	if(!X509_REQ_set_subject_name(req, _subj))
		return FAIL("Error adding subject to request");
	TRACE("DONE");
	return true;
}

bool X509_util_REQ:: add_ext_nid_val( int nid, const char * val )
{
	return extensions.add_ext_nid_val( nid, val );
}

/*!
bool X509_util_REQ:: add_ext( const char * name, const char * value )
*/

bool X509_util_REQ:: add_ext_key_usage( const char * value )
{
	return add_ext_nid_val( NID_key_usage, value );
}


bool X509_util_REQ:: add_extensions()
{
	if(!X509_REQ_add_extensions(req, extensions.extlist))
		return FAIL("Error adding extensions to the request");
	TRACE("DONE");
	return true;
}

bool X509_util_REQ:: client_sign()
{
	// pick the correct digest
	EVP_MD *digest = NULL;
	if(client_rsa->is_DSA()) {
		digest = (EVP_MD*) EVP_dss1 ();
	} else 
	if(client_rsa->is_RSA()) {
		digest = (EVP_MD*) EVP_sha1 ();
	} else {
		return FAIL("client_rsa is neither RSA nort DSA - dont know what digest to use");
	}

	if( extensions.ADD_EXT_subject_key_identifier_hash ) {
		// problem is - that extlist is based without the public key
		// and certainly it must be applied before it signs over itself!
#warning TODO CODE subject_key_identifier_hash
if(0)		if(!extensions.add_ext_nid_val( NID_subject_key_identifier, "hash"))
			return FAIL_FAILED();
		// or has the horse bolted
		// do ifter add_extensions
	} else {
		FAIL("ADD_EXT_subject_key_identifier_hash is FALSE - write more code");
	}

	if(!X509_REQ_sign( req, client_rsa->pkey, digest))
		return FAIL("Error signing request");
	TRACE("DONE");
	return true;
}

bool X509_util_REQ:: write_to_buffer( blk1 & buf )
{
	BIO_mem bio_mem;
	if(!PEM_write_bio_X509_REQ( bio_mem, req )) {
		return FAIL("PEM_write_bio_X509_REQ()");
	}
	if(!bio_mem.write_to_buffer( buf )) return FAIL_FAILED();
	TRACE("DONE");
	return true;
}

bool X509_util_REQ:: write_req_to_file( const char * filename )
{
	TRACE_1("%s", filename);
	blk1 buf;
	buf.get_space( 2000 ); // 700 +
	if(!write_to_buffer( buf )) return FAIL_FAILED();
	if(! blk_write_to_file( buf, filename )) return FAIL_FAILED();
	TRACE_1("DONE %s", filename);
	return true;
}

/*!
	I use the above, that goes via buffer,
	but there is this comparison available
*/
bool X509_util_REQ:: write_to_file1( const char * filename )
{
	TRACE_1("%s", filename);
	FILE *fp;
	if(!(fp = fopen(filename, "w")))
		return FAIL("Error writing to request file");
	if(PEM_write_X509_REQ(fp, req) != 1)
		return FAIL("Error while writing request");
	fclose(fp);
	TRACE_1("%s DONE OK", filename);
	return true;
}

///////////////////////////////////////
#include "X509_VALS_SITE_funcs.h"



/*!
	The str1 vals have been prepared, now apply them ...
*/
bool X509_util_REQ:: VALS_SITE_funcs( X509_VALS_SITE_funcs & vals )
{
	// move the str1/bool vals to the openSSL stucts
	// but without referring to the X509 REQ object
	// so that it can also be an X509 ISS object ;-)
	if(!SET_values_base( vals ))return FAIL_FAILED();

	// above builds subject, now apply it
	// assign the X509 NAME struct to the X509 REQ
	if(!set_subject_name()) return FAIL_FAILED();

	// the above build a list of extensions (probably fails if NULL)
	if(!add_extensions()) return FAIL_FAILED();

	return true;
}

