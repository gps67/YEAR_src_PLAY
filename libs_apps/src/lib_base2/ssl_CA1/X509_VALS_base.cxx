#include <time.h>
#include "dgb.h"

#include "X509_VALS_base.h"
using namespace CA1;
//	X509_VALS__subject::

X509_VALS_subject:: X509_VALS_subject()
{
}

X509_VALS_subject:: ~X509_VALS_subject()
{
}

bool X509_VALS_subject:: clear()
{
	C.clear();
	ST.clear();
	L.clear();
	O.clear();
	OU.clear();
	CN.clear();
	return true;
}

	/////////////////////////////////////////

bool
X509_VALS_subjectAltName::
clear()
{
	DNS_1.clear();
	DNS_2.clear();

	IP_1.clear();
	IP_2.clear();

	URI_1.clear();
	URI_2.clear();

	EMAIL_1.clear();
	EMAIL_2.clear();
	return true;
}

X509_VALS_subjectAltName::
X509_VALS_subjectAltName()
{
	// clear();
}

	/////////////////////////////////////////

X509_VALS_key_usage::
X509_VALS_key_usage()
{
	clear();
}

bool
X509_VALS_key_usage::
clear()
{
	critical = false; // it is critical, or not
	digitalSignature = false;
	keyEncipherment = false;
	keyCertSign = false; // CA
	cRLSign = false; // CA
	return true;
}


	/////////////////////////////////////////

X509_VALS_CA_depth::
X509_VALS_CA_depth()
{
}

bool X509_VALS_CA_depth:: clear()
{
	basicConstraint_CA = dunno3;
	basic_constraint_pathLen = -1; // -1 means unset
	return true;
}

bool X509_VALS_CA_depth:: set_CA_pathlen( int pathlen )
{
	if( pathlen > 0 ) {
		basicConstraint_CA = true3;
		basic_constraint_pathLen = pathlen;
	} else {
		basicConstraint_CA = false3;
		basic_constraint_pathLen = 0;
	}
	return true;
}

	/////////////////////////////////////////

X509_VALS_time_range:: X509_VALS_time_range()
{
	clear();
}

bool X509_VALS_time_range:: clear()
{
	 return set_now_and_ndays(7);
}

bool X509_VALS_time_range:: set_now_and_ndays(int n)
{
	if(!set_now()) return FAIL_FAILED();
	if(!add_days(n)) return FAIL_FAILED();
	return true;
}

bool X509_VALS_time_range:: set_now()
{
	ISS_time_start = time(NULL);
	return true;
}

bool X509_VALS_time_range:: add_days(int days)
{
	int time_step = days * 24 * 60 * 60;
	ISS_time_end = ISS_time_start + time_step;
	return true;
}

bool X509_VALS_time_range:: set_t0_t2(time_t t0, time_t t2)
{
	if(t2<=t0) {
		return FAIL("NOT t0 < t2");
	}
	ISS_time_start = t0;
	ISS_time_end = t2;
	return true;
}


X509_VALS_iss_misc:: X509_VALS_iss_misc()
{
	clear();
}

/*
	TODO: actually use these fields ...
*/
bool X509_VALS_iss_misc:: clear()
{
	RSA_bits = 1024;
	CERT_serial =0;
	ISS_version =0;		// V2 or ... fixed!
	ISS_serialNumber =0;	// or 20 byte thing?

	// these are part of the ISSUES cert not the REQUEST
	ADD_EXT_subject_key_identifier_hash=true;
	ADD_EXT_authority_key_identifier=true;
	ADD_EXT_authority_key_identifier_opts=NULL;
	ISS_version = 2;		// V2 or ...
	ISS_serialNumber = 10;	// or 20 byte thing?
	CERT_serial = 57;

 	ISS_time_range.clear();

	return true;
}

	/////////////////////////////////////////

X509_VALS_base:: ~X509_VALS_base()
{
}

X509_VALS_base:: X509_VALS_base()
{
	clear();
}

bool X509_VALS_base:: clear()
{
	subject.clear();

	CA_depth.clear();

	key_usage.clear();

	subjectAltName.clear();

	iss_misc.clear();
 //	iss_misc.iss_time_range.clear();

	return true;
}

bool X509_VALS_base:: set_CA_pathlen( int pathlen )
{
	if( pathlen > 0 ) {
		return CA_depth.set_CA_pathlen( pathlen );
	} else {
		return CA_depth.set_C_not_CA();
	}
}

