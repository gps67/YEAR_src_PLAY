#include "X509_VALS_SITE_funcs.h"
#include "dgb.h"

X509_VALS_SITE_funcs:: ~X509_VALS_SITE_funcs()
{
}

X509_VALS_SITE_funcs:: X509_VALS_SITE_funcs()
: X509_VALS_base()
{
}

bool X509_VALS_SITE_funcs:: set_CA_depth_for_CA_ZERO()
{ return CA_depth.set_CA_pathlen( 3 ); }

bool X509_VALS_SITE_funcs:: set_CA_depth_for_CA_ONE()
{ return CA_depth.set_CA_pathlen( 2 ); }

bool X509_VALS_SITE_funcs:: set_CA_depth_for_CA_ZONE()
{ return CA_depth.set_CA_pathlen( 1 ); }

bool X509_VALS_SITE_funcs:: set_C_not_CA()
{ return CA_depth.set_CA_pathlen( 0 ); }


/*!
	The idea is to write these for your machines,

	also set some general defaults, then more specifics

		SITE SPECIFIC
*/
bool X509_VALS_SITE_funcs:: set_cert_values()
{
	clear();

// or done by each func
//	if(!set_HERE_part1()) return FAIL_FAILED();

	if(!set_HERE_C_pc()) return FAIL_FAILED();

	return true;


//	subjectAltName.DNS_1 = "vlan.information-cascade.co.uk";
//	subjectAltName.DNS_2 = "hublan.information-cascade.co.uk";
//	subjectAltName.EMAIL_2 = "lists@information-cascade.co.uk";

//	key_usage_critical = true;
//	key_usage_digitalSignature = true;
//	key_usage_keyEncipherment = true;

//	key_usage_keyCertSign = true;
//	key_usage_cRLSign = true;

//	CA_depth.set_CA_pathlen( 3 );

	ISS_version;		// V2 or ...
	ISS_serialNumber;	// or 20 byte thing?

	ISS_time_start;
	ISS_time_end;

	return true;
}


bool X509_VALS_SITE_funcs:: set_HERE_part1()
{
	clear();

	subject.C	= "GB";		
	subject.ST	= "Berks";
	subject.O	= "InformationCascade";
	subject.OU	= "RND";
	subject.CN	= "-UNSET-";
	return true;
}

bool X509_VALS_SITE_funcs:: set_HERE_CA_ZERO()
{
	if(!set_HERE_part1()) return FAIL_FAILED();
	if(!set_CA_depth_for_CA_ZERO()) return FAIL_FAILED();
	subject.CN = "CA_ZERO";
	return true;
}

bool X509_VALS_SITE_funcs:: set_HERE_CA_ONE()
{
	if(!set_HERE_part1()) return FAIL_FAILED();
	if(!set_CA_depth_for_CA_ONE()) return FAIL_FAILED();
	subject.CN = "CA_ONE";
	return true;
}

bool X509_VALS_SITE_funcs:: set_HERE_CA_ZONE()
{
	if(!set_HERE_part1()) return FAIL_FAILED();
	if(!set_CA_depth_for_CA_ZONE()) return FAIL_FAILED();
	subject.CN = "CA_BOX5";
	return true;
}

bool X509_VALS_SITE_funcs:: set_HERE_C_server()
{
	if(!set_HERE_part1()) return FAIL_FAILED();
	if(!set_C_not_CA()) return FAIL_FAILED();
	subject.CN = "BOX5";
	subjectAltName.DNS_1 = "box5.information-cascade.co.uk";
	subjectAltName.DNS_2 = "box5.vlan2.local";
	return true;
}

bool X509_VALS_SITE_funcs:: set_HERE_C_user()
{
	if(!set_HERE_part1()) return FAIL_FAILED();
	if(!set_C_not_CA()) return FAIL_FAILED();
	subject.CN = "USER_231";
	subjectAltName.EMAIL_1 = "lists@information-cascade.co.uk";
	return true;
}

bool X509_VALS_SITE_funcs:: set_HERE_C_pc()
{
	if(!set_HERE_part1()) return FAIL_FAILED();
	if(!set_C_not_CA()) return FAIL_FAILED();
	subject.CN = "PC_321";
	return true;
}

bool X509_VALS_SITE_funcs:: set_HERE_C_item()
{
	if(!set_HERE_part1()) return FAIL_FAILED();
	if(!set_C_not_CA()) return FAIL_FAILED();
	subject.CN = "ITEM_4421";
	return true;
}


