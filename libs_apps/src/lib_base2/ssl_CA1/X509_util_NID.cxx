

#include "X509_util_NID.h"
#include <openssl/x509.h>
#include <openssl/x509v3.h>

#include "str0.h"
#include "dgb.h"
using namespace CA1;

bool X509_util_NID:: test_one()
{
	for( int nid = 0; nid < 10; nid ++ ) {
		INFO("%d %20s %s",
		nid,
		shortName_of_NID( nid ),
		 longName_of_NID( nid )
		);
	}
	return true;
}

bool X509_util_NID:: longName_to_NID( const char * longName, int & nid )
{
	if((nid = OBJ_txt2nid((STR0) longName)) == NID_undef)
	{
		return FAIL("longName '%s'", (STR0) longName);
	}
	return true;
}

/*!
	convert "CN" to NID_commonName (or not)
*/
// static
bool X509_util_NID:: shortName_to_NID( const char * shortName, int & nid )
{
	if((nid = OBJ_sn2nid((STR0) shortName)) == NID_undef)
	{
		return FAIL("shortName '%s'", (STR0) shortName);
	}
	return true;

}

// static
const char * X509_util_NID:: shortName_of_NID( int nid )
{
	return OBJ_nid2sn( nid );
}

// static
const char * X509_util_NID:: longName_of_NID( int nid )
{
	return OBJ_nid2ln( nid );
}


