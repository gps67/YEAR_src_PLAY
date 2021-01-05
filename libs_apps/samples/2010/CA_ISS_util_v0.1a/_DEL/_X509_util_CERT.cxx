
#include "X509_util_CERT.h"

// expand STUBS from header
#include "X509_VALS_base.h"

#include "dgb.h"

using namespace CA1;

X509_util_CERT_base::
X509_util_CERT_base()
: subject() // , subj( NULL )
, extensions()
{
}

X509_util_CERT_base::
~X509_util_CERT_base()
{
}


/*!
	Copy the VALS down to the prep-zones, ready to add to cert
*/
bool X509_util_CERT_base:: SET_values_base( X509_VALS_base & vals )
{

	// copy the str1 values into X509 NAME entries
	if(!subject.set( vals.subject )) return FAIL_FAILED();

	if(!extensions.add_ext_CA_depth(vals.CA_depth)) return FAIL_FAILED();

	/*
		extensions.add_ext_subject_key_identifier_hash( cert )

		The RSA id is taken from the cert, into the extensions
		to be put back on the cert, called by ... elsewhere
		see mk_cert
	*/
	// eg mail:lists@information-cascade.co.uk
	if(!extensions.OK_add_ext_subjectAltName( vals.subjectAltName ))
		return FAIL_FAILED();

	// key may be used for SIGN, SERVER, ...
	if(!extensions.add_ext_key_usage( vals.key_usage )) return FAIL_FAILED();

	return true;
}


////////////////////
X509_util_CERT::
X509_util_CERT()
: X509_util_CERT_base()
{
}

X509_util_CERT::
~X509_util_CERT()
{
}

