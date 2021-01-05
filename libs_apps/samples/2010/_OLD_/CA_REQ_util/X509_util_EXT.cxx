#include "X509_util_EXT.h"
#include "dgb.h"
#include "bad_mad.h"
#include "buffer2.h"
#include "X509_VALS_base.h"


X509_util_EXT:: X509_util_EXT() {
	extlist = sk_X509_EXTENSION_new_null();
	if(!extlist) THROW_bad_mad("sk_X509_EXTENSION_new_null()");

	// we cant add an _actual_ ext to extlist for the hash
	// because the _actual_ public key is needed, for the value
	// so instead, keep a flag, for when we can
	// and remember to check the flag!
	//
	// _hash TRUE is the only choice worth having, (write code)
	// but it should still be a choice

	ADD_EXT_subject_key_identifier_hash = false; // later not now!
}

X509_util_EXT:: ~X509_util_EXT() {
#warning double delete error
	extlist = NULL;
        if( extlist)
                sk_X509_EXTENSION_pop_free(extlist, X509_EXTENSION_free);
        extlist = NULL;
}

bool X509_util_EXT:: add_ext_nid_val( int nid, const char * value )
{
	X509_EXTENSION * ext = X509V3_EXT_conf_nid(
		NULL,   // LHASH * conf
		NULL,   // X509V3_CTX * ctx
		nid,
		(char *) value
	);
	if(!ext){
		// TODO get string of nid
		return FAIL_2(
			"nid==%s, valid=='%s'",
			OBJ_nid2ln(nid),
			value
		);
	}
	if(!sk_X509_EXTENSION_push(extlist, ext))
		return FAIL("sk_X509_EXTENSION_push()");
	return true;
}

bool X509_util_EXT:: OK_add_ext_nid_val( int nid, const char * value )
{
	if(!value) return true;
	if(!*value) return true;
	
	return add_ext_nid_val( nid, value );
}

bool X509_util_EXT:: OK_add_ext_nid_pfx_val( int nid, const char * pfx, const char * value )
{
	if(!value) return true;
	if(!*value) return true;
	buffer2 pfx_val;
	pfx_val.print("%s:%s", pfx, value );
	return add_ext_nid_val( nid, (STR0) pfx_val );
}

bool X509_util_EXT:: OK_add_ext_DNS( const char * val )
{
	return OK_add_ext_nid_pfx_val( NID_subject_alt_name, "DNS", val );
}


bool X509_util_EXT:: OK_add_ext_IP( const char * val )
{
	return OK_add_ext_nid_pfx_val( NID_subject_alt_name, "IP", val );
}


bool X509_util_EXT:: OK_add_ext_URI( const char * val )
{
	return OK_add_ext_nid_pfx_val( NID_subject_alt_name, "URI", val );
}


bool X509_util_EXT:: OK_add_ext_EMAIL( const char * val )
{
	return OK_add_ext_nid_pfx_val( NID_subject_alt_name, "email", val );
}

bool X509_util_EXT:: OK_add_ext_subjectAltName( X509_VALS_subjectAltName & vals )
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

bool X509_util_EXT:: add_ext_key_usage( X509_VALS_key_usage & vals )
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
	}
	return true;
}


bool X509_util_EXT:: add_ext_CA_pathlen( bool ca, int pathlen )
{
	if( pathlen < 1 ) {
	 if(ca) {
		ca = false;
		WARN_1("Changing CA to false - because pathlen was %d", pathlen);
	 }
	}
	if( pathlen > 5 ) {
		WARN_1("pathlen %d is a bit high", pathlen);
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

bool X509_util_EXT:: add_ext_CA_depth( X509_VALS_CA_depth & vals )
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
bool X509_util_EXT:: add_ext_subject_key_identifier_hash() // 4+60 bit sha-1 or 160 bit sha-1
{
	if(!add_ext_nid_val( NID_subject_key_identifier, "hash" )) {
		FAIL("You meed to set a public key first");
		WARN("ignoring error"); // must call FAIL to eat SSL_error
	}
	return true;
}

