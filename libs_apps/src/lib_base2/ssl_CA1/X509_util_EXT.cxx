#include "dgb.h"
#include "dgb_fail.h"
#include "buffer2.h"
#include "EVP_P_KEY.h"

#include "X509_VALS_base.h"
#include "X509_util_EXT.h"
#include "X509_cert.h" // X509_util_EXT_iss
using namespace CA1;

X509_util_EXT_base:: X509_util_EXT_base()
: extn_held( NULL)
{
}

X509_util_EXT_base:: ~X509_util_EXT_base()
{
	clear_extn_held();
}

bool X509_util_EXT_base:: store_clear_extn_held()
{
	if(!store_extn_held()) return FAIL_FAILED();
	if(!clear_extn_held()) return FAIL_FAILED();
	return true;
}

bool X509_util_EXT_base:: clear_extn_held()
{
	if(extn_held) { // not been cleared
		X509_EXTENSION_free( extn_held );
	}
	extn_held = NULL;
	return true;
}

bool X509_util_EXT_base:: drop_extn_held() // because it HAS been used
{
	extn_held = NULL;
	return true;
}

bool X509_util_EXT_base:: mk_ext_cfg_ctx_nid_val(
	LHASH * cfg,
	X509V3_CTX * ctx,
	int nid,
	const char * value
)
{
	clear_extn_held();

	extn_held = X509V3_EXT_conf_nid(
		cfg,   // LHASH * conf
		ctx,   // X509V3_CTX * ctx
		nid,
		(char *) value
	);
	if(!extn_held){
		// TODO get string of nid
		return FAIL(
			"nid==%s, valid=='%s'",
			OBJ_nid2ln(nid),
			value
		);
	}
	return true;
}

///////////////////


X509_util_EXT_list:: X509_util_EXT_list() {
	extlist = sk_X509_EXTENSION_new_null();
	if(!extlist) THROW_dgb_fail("sk_X509_EXTENSION_new_null()");

	// we cant add an _actual_ ext to extlist for the hash
	// because the _actual_ public key is needed, for the value
	// so instead, keep a flag, for when we can
	// and remember to check the flag!
	//
	// _hash TRUE is the only choice worth having, (write code)
	// but it should still be a choice
	// 
	// hmmm currently we do add it, but when the cert has its pub
	// maybe use the bool flag somewhere
	//
	// TODO - run a leak bench,
	// does X509_REQ_hold::add_extensions( *this ) use up extlist
	// should i then clear extlist, ready for the authority key
	// or anything that the ISS wants to add

 //	ADD_EXT_subject_key_identifier_hash = false; // later not now!
}

X509_util_EXT_list:: ~X509_util_EXT_list() {
#warning double delete error
	extlist = NULL;
#ifdef WIN32
#warning WIN32 unimplemented or syntax adjust
#else
        if( extlist)
                sk_X509_EXTENSION_pop_free(extlist, X509_EXTENSION_free);
        extlist = NULL;
#endif
}


bool X509_util_EXT_base:: add_ext_nid_val( int nid, const char * value )
{
	/*
		BOOK 10-6 pg290 uses ctx, attached to CA and to CERT
		where the cert has had X509_set_pubkey(cert,pkey)

		THIS is ok, except when we want subjectKeyIdentifier = hash
		which needs the pubkey to add the 160 bit hash (or 60+4)
	*/
	LHASH * conf = NULL;
	X509V3_CTX * ctx = NULL;
	if(! add_ext_cfg_ctx_nid_val( conf, ctx, nid, value )) return FAIL_FAILED();
	return true;
}


bool X509_util_EXT_base:: add_ext_cfg_ctx_nid_val(
	LHASH * cfg,
	X509V3_CTX * ctx,
	int nid,
	const char * value
)
{
	/*
		BOOK 10-6 pg290 uses ctx, attached to CA and to CERT
		where the cert has had X509_set_pubkey(cert,pkey)

		THIS is ok, except when we want subjectKeyIdentifier = hash
		which needs the pubkey to add the 160 bit hash (or 60+4)
	*/
	if(! mk_ext_cfg_ctx_nid_val( cfg, ctx, nid, value )) return FAIL_FAILED();
	if(!store_clear_extn_held()) return FAIL_FAILED();

	drop_extn_held(); // used
	return true;
}

bool X509_util_EXT_base:: OK_add_ext_nid_val( int nid, const char * value )
{
	if(!value) return true;
	if(!*value) return true;
	
	return add_ext_nid_val( nid, value );
}

bool X509_util_EXT_base:: OK_add_ext_nid_pfx_val( int nid, const char * pfx, const char * value )
{
	if(!value) return true;
	if(!*value) return true;
	buffer2 pfx_val;
	pfx_val.print("%s:%s", pfx, value );
	return add_ext_nid_val( nid, (STR0) pfx_val );
}

bool X509_util_EXT_base:: OK_add_ext_DNS( const char * val )
{
	return OK_add_ext_nid_pfx_val( NID_subject_alt_name, "DNS", val );
}


bool X509_util_EXT_base:: OK_add_ext_IP( const char * val )
{
	return OK_add_ext_nid_pfx_val( NID_subject_alt_name, "IP", val );
}


bool X509_util_EXT_base:: OK_add_ext_URI( const char * val )
{
	return OK_add_ext_nid_pfx_val( NID_subject_alt_name, "URI", val );
}


bool X509_util_EXT_base:: OK_add_ext_EMAIL( const char * val )
{
	return OK_add_ext_nid_pfx_val( NID_subject_alt_name, "email", val );
}

bool X509_util_EXT_base:: OK_add_ext_subjectAltName( X509_VALS_subjectAltName & vals )
{
	if(!OK_add_ext_DNS(vals.DNS_1))
		return FAIL_FAILED();

	if(!OK_add_ext_DNS(vals.DNS_2))
		return FAIL_FAILED();

	if(!OK_add_ext_IP(vals.IP_1))
		return FAIL_FAILED();

	if(!OK_add_ext_IP(vals.IP_2))
		return FAIL_FAILED();

	if(!OK_add_ext_URI(vals.URI_1))
		return FAIL_FAILED();

	if(!OK_add_ext_URI(vals.URI_2))
		return FAIL_FAILED();

	if(!OK_add_ext_EMAIL(vals.EMAIL_1))
		return FAIL_FAILED();

	if(!OK_add_ext_EMAIL(vals.EMAIL_2))
		return FAIL_FAILED();

	return true;
}

bool X509_util_EXT_base:: add_ext_key_usage( X509_VALS_key_usage & vals )
{
	buffer2 ku;
	if(vals.critical) {
		if(ku) ku.put_byte(',');
		ku.print( "critical" ); // thats not a key_usage, just key_usage_is_critical
	}
	if(vals.keyCertSign) { // CA
		if(ku) ku.put_byte(',');
		ku.print( "keyCertSign" );
	}
	if(vals.cRLSign) { // CA
		if(ku) ku.put_byte(',');
		ku.print( "cRLSign" );
	}
	if(vals.digitalSignature) {
		if(ku) ku.put_byte(',');
		ku.print( "digitalSignature" );
	}
	if(vals.keyEncipherment) {
		if(ku) ku.put_byte(',');
		ku.print( "keyEncipherment" );
	}
	if(ku) {
		if(!add_ext_nid_val( NID_key_usage, (STR0) ku )) return FAIL_FAILED();
		// PASS((STR0) ku);;
	}
	return PASS("DONE");;
	return true;
}


bool X509_util_EXT_base:: add_ext_CA_pathlen( bool ca, int pathlen )
{
	if( pathlen < 1 ) {
	 if(ca) {
		ca = false;
		WARN("Changing CA to false - because pathlen was %d", pathlen);
	 }
	}
	if( pathlen > 5 ) {
		WARN("pathlen %d is a bit high", pathlen);
	}
	buffer2 bcon;
	if(!ca) {
		// return true;
		// standard OMIT CA:FALSE	
		// MANY say EMIT CA:FALSE	
		bcon.print("critical,CA:FALSE");
	} else {
		bcon.print("critical,CA:TRUE,pathlen:%d", pathlen);
	}
	return add_ext_nid_val( NID_basic_constraints, (STR0) bcon );
}

bool X509_util_EXT_base:: add_ext_CA_depth( X509_VALS_CA_depth & vals )
{
	if(vals.basicConstraint_CA == true3 ) {
	 if(!add_ext_CA_pathlen( true, vals.basic_constraint_pathLen ) )
			return FAIL_FAILED();
	} else {
		if(!add_ext_CA_pathlen( false, 0 ) ) return FAIL_FAILED();
	}
	return true;
}

/*!
	You can chose how the certificate identifies itself.

	The usual way is 160 bit SHA-1 of public key,
	http://www.ietf.org/rfc/rfc3280.txt

	You may ONLY call this when the extension has already been
	associated with a public key! (Maybe it is expanded immediately)

		I THINK THIS MAY CHANGE THE CODE LAYOUT A BIT

*/
bool X509_util_EXT_base:: add_ext_subject_key_identifier_hash( X509_REQ * cert_req ) // 4+60 bit sha-1 or 160 bit sha-1
{
	if(!cert_req) return FAIL("NULL cert_req");

//	return FAIL("hash");
	LHASH * conf = NULL;
	X509 * cert_issuer_NULL = NULL;
	X509 * cert_client_NULL = NULL;
	X509_CRL * cert_CRL_NULL = NULL;
	int no_flags = 0;
	X509V3_CTX  ctx ;
	X509V3_set_ctx(
		&ctx,
		cert_issuer_NULL,
		cert_client_NULL,
		cert_req,
		cert_CRL_NULL,
		no_flags
	);
	
	if(!add_ext_cfg_ctx_nid_val(
		conf, &ctx,
		NID_subject_key_identifier,
		"hash"
	)) return FAIL_FAILED();

	return PASS("hash");
	return true;
}



	//////////////////////////////////////////////////////////////////

bool X509_util_EXT_list:: store_extn_held()
{
#ifdef WIN32
#warning WIN32 unimplemented or syntax adjust
#else
	if(!sk_X509_EXTENSION_push(extlist, extn_held))
		return FAIL("sk_X509_EXTENSION_push()");
#endif
	return drop_extn_held(); // used
	return true;
}

	//////////////////////////////////////////////////////////////////

/*!
	The hash of the issuers key

	This is to be done by the ISS phase, not by the REQ.
	(There is a strange possible approach, but no).

	We cant add the extension to the list of extensions,
	(or we could), because that was for the REQ, so add it to the CERT.

*/
bool X509_util_EXT_iss:: add_ext_authority_key_identifier(
		X509 * cert_issuer,
		const char * opts
) // 4+60 bit sha-1 or 160 bit sha-1
{
	// NOTE REQ must have subject has in it, particularly self sign seq
	if(!cert_issuer) return FAIL("NULL cert_issuer");
	// first one wins // see v3_akey.c:111 
	if(!opts) opts = "keyid";
	if(!opts) opts = "keyid,issuer:always";
	if(!opts) opts = "keyid:always"; // unable to get issuer keyid!
	if(!opts) opts = "hash"; // not valid


	LHASH * conf = NULL;
//	X509 * cert_issuer_NULL = NULL;
	X509 * cert_client_NULL = NULL;
	X509_REQ * cert_req_NULL = NULL;
	X509_CRL * cert_CRL_NULL = NULL;
	int no_flags = 0;
	X509V3_CTX  ctx ;
	X509V3_set_ctx(
		&ctx,
		cert_issuer,
		cert_client_NULL,
		cert_req_NULL,
		cert_CRL_NULL,
		no_flags
	);
	
	if(!add_ext_cfg_ctx_nid_val(
		conf, &ctx,
		NID_authority_key_identifier,
		opts
	)) return FAIL_FAILED();

	return PASS("NID_authority_key_identifier");
	return true;
}

bool X509_util_EXT_iss:: store_extn_held()
{
	if(!client_cert) return FAIL("NULL client_cert");
	if(!client_cert->cert) return FAIL("NULL client_cert->cert");
	if(!X509_add_ext( client_cert->cert, extn_held, -1 ))
		return FAIL("X509_add_ext( cert, extn_held, -1 );");
	drop_extn_held(); // used
	return PASS("OK");
}
