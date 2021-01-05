#include "X509_VALS_base.h"
#include <time.h>
#include "dgb.h"

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

	// these are part of the ISSUES cert not the REQUEST
	ISS_version = 2;		// V2 or ...
	ISS_serialNumber = 1;	// or 20 byte thing?

	ISS_time_start = 0 ;
	ISS_time_end = 0 ;

	return true;
}

bool X509_VALS_base:: set_CA_pathlen( int pathlen )
{
	if( pathlen > 0 ) {
		CA_depth.set_CA_pathlen( pathlen );
		
	} else {
	}
}

bool X509_VALS_base:: set_time_start_now_end_days(int days)
{
	ISS_time_start = time(NULL);
	int time_step = days * 24 * 60 * 60;
	ISS_time_end = ISS_time_start + time_step;
}

