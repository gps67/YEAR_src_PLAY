
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include "ints.h"
#include "dgb.h"
#include "dgb_fail.h"

#include "X509_util_NAME.h"
#include "X509_VALS_base.h"
using namespace CA1;

X509_util_NAME:: ~X509_util_NAME()
{
	// LURK should I delete subj
	// TODO make it a thing where adding to an X509 takes reference copy
}

X509_util_NAME:: X509_util_NAME()
{
	subj = X509_NAME_new();
	if(!subj ) {
		FAIL("Failed to create X509_NAME object");
		THROW_dgb_fail("CTOR ENOMEM");
	}
}

bool X509_util_NAME:: add_entry_nid_ascii( int nid, const char * value )
{
//	http://www.openssl.org/docs/crypto/X509_NAME_add_entry_by_txt.html

	int t = X509_NAME_add_entry_by_NID(
		subj,
		nid,
		MBSTRING_ASC,	// type 
		(u8 *) value,	// bytes
		-1,		// len -1 means strlen
		-1,		// loc -1 means append
		0		// set 0 means normal, not merge adjacent
	);

	if(!t) {
		INFO("nid %d == %s value = %s", nid, OBJ_nid2ln(nid), value );
		return FAIL("X509_NAME_add_entry_by_NID()");
	}

	// TRACE nid to name etc
	return true;
}

bool X509_util_NAME:: add_entry_nid_utf8( int nid, const char * utf8 )
{
	int t = X509_NAME_add_entry_by_NID(
		subj,
		nid,
		MBSTRING_UTF8,	// type 
		(u8 *) utf8,	// bytes
		-1,		// len -1 means strlen
		-1,		// loc -1 means append
		0		// set 0 means normal, not merge adjacent
	);

	if(!t) {
		INFO("nid %d == %s value = %s", nid, OBJ_nid2ln(nid), utf8 );
		return FAIL("X509_NAME_add_entry_by_NID()");
	}

	// TRACE nid to name etc
	return true;
}

/*
	if value is not empty, add it as NID

	OK is returned, even if nothing was done (that is OK)

	defaults to utf8 (easilty switched, with extensive consequence)
*/
bool X509_util_NAME:: OK_add_entry_nid( int nid, const char * val )
{
	if(!val) return true;
	if(!*val) return true;
	return add_entry_nid_utf8( nid, val );
}

bool X509_util_NAME:: set( X509_VALS_subject & vals )
{

 if(!OK_add_entry_nid( NID_countryName,      vals.C )) return FAIL_FAILED();
 if(!OK_add_entry_nid( NID_localityName,     vals.L )) return FAIL_FAILED();
 if(!OK_add_entry_nid( NID_stateOrProvinceName, vals.ST )) return FAIL_FAILED();
 if(!OK_add_entry_nid( NID_organizationName,  vals.O )) return FAIL_FAILED();
 if(!OK_add_entry_nid( NID_organizationalUnitName, vals.OU )) return FAIL_FAILED();
 if(!OK_add_entry_nid( NID_commonName,        vals.CN )) return FAIL_FAILED();
	return true;
}
