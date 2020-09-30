

/*
	LGPL exception - the rest of this package is LGPL,
	but this file (samples/2010/CA_ISS_util/mk_req.cxx and MYSITE*)
	is an example demo file,
	so feel free to copy paste customise in your app
	without too much fuss, OK
*/


#include "dgb.h"		// FAIL, WARN etc
#include "SSL_global.h"		// everyone must init rand etc
#include "CB_get_phrase.h"	// new CB_get_phrase_NONE()
#include "EVP_P_KEY.h"		// test_write_read_PrivateKey();

#include "MYSITE_X509_layout.h"
#include "MYSITE_CA_task.h"	// MYSITE is completely user defined demo

// you dont need this, unless you load REQ from buffer, from LAN, or file
// #include "X509_REQ_hold.h"	// REQ->write_req_to_file()

// the default C_progress_genkey(...) is silent,
// you can overrule the shared lib version, see link order cmd line
// a_kept should not be globally visible though (rename and export)

#if 0
int a_kept = -1;
void C_progress_genkey( int a, int b, void * cb_arg )
{
	if( a_kept != a ) {
		if(a_kept != -1 ) printf("\n" );
		a_kept = a;
		printf("C_progress_genkey( %d, ", a );
	}
	printf("%d",b); // basically just counts to unknown varying ceiling
	printf(".");
	fflush(0);
}
#endif

using namespace CA1;


bool DEMO_mk_VPN_GW( str0 ABBR ) // ABBR = "VPN_97"
{
	buffer1 NAME;
	NAME.print("GW_%s", ABBR );
	return FAIL("TODO");
}

bool DEMO_mk_VPN_PC( str0 ABBR ) // ABBR = "GPS_D630"
{
	buffer1 NAME;
	NAME.print("PC_%s", ABBR );
	SITE_X509_tag_enum ISS_tag = is_C_pc;
	obj_hold<MYSITE_X509_layout> mysite_scheme = new MYSITE_X509_layout();
	obj_hold<MYSITE_CA_task> task = new MYSITE_CA_task(  mysite_scheme, ISS_tag );

	/*
		VALS <== -code- tag

		that fails - which ZONE is ITEM in 
	if(!task->MYSITE_SET_DEMO_VALUE_FOR( ISS_tag )) return FAIL_FAILED();

		OBTAIN CA_ZONE	ZONE
		CREATE  C_ITEM  USER

		OBTAIN CA_ZONE	ZONE_FIVE
		CREATE CA_LIST	USERS 

		OBTAIN CA_LIST	USER
		CREATE  C_ITEM  USER_0xFFFF

	*/

	/*
		VALS => REQ => ISS => files
	*/
	if(!task->MYSITE_mk_cert_from_VALS()) return FAIL_FAILED();
	return PASS("X509");
}

bool DEMO_mk_cert_group()
{

	// ISSUER_CERT = loaded from somewhere (filename?)
	// ISSUED_CERT = VALS + ROLE in layout + FILENAME
	// supposedly online signing REQ from 

	// mk_LAPTOP
	if(!DEMO_mk_VPN_PC("DWS_")) return FAIL_FAILED();
	return true;
	if(!DEMO_mk_VPN_PC("AMS_10")) return FAIL_FAILED();
	if(!DEMO_mk_VPN_PC("GPS_E6320")) return FAIL_FAILED();
	if(!DEMO_mk_VPN_PC("MBS_")) return FAIL_FAILED();
	if(!DEMO_mk_VPN_GW("HUB_GW")) return FAIL_FAILED();

	return PASS("OK");

	SITE_X509_tag_enum ISS_tag = is_CA_ZONE;

	/*
		if(1)	if(!DEMO_mk_cert(is_CA_ONE)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_CA_ZONE)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_pc)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_server)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_item)) return FAIL_FAILED();
		You have two virtual classes, layout and task
		You also have this parent _DEMO_ caller
		Change all 3 for your site,

		the current basis, is that you set VALS
		then bounce it through REQ and ISS
		then save the result

		UPDATE: it was/is flawed
		there are multiple C_pc in CA_ZONE
		we need to KNOW the CA_ZONE name
		we prefer to have the actual CA_ data name.vern
		we need different filenames for users

		_task is still much the same
		_layout needs to be thrown out
		_filename == dir_ZONE/CN # all in that dir
		_filename == dir_ROOT/CN # for a while 
	*/
	obj_hold<MYSITE_X509_layout> mysite_scheme = new MYSITE_X509_layout();
	obj_hold<MYSITE_CA_task> task = new MYSITE_CA_task(  mysite_scheme, ISS_tag );

	/*
		VALS <== -code- tag

		that fails - which ZONE is ITEM in 
	*/
	if(!task->MYSITE_SET_DEMO_VALUE_FOR( ISS_tag )) return FAIL_FAILED();

	/*
		VALS => REQ => ISS => files
	*/
	if(!task->MYSITE_mk_cert_from_VALS()) return FAIL_FAILED();

	return PASS("DONE");
}
		//////////////////////////////////////////////

bool DEMO_mk_cert(
	SITE_X509_tag_enum ISS_tag
)
{
	/*
		You have two virtual classes, layout and task
		You also have this parent _DEMO_ caller
		Change all 3 for your site,

		the current basis, is that you set VALS
		then bounce it through REQ and ISS
		then save the result
	*/
	obj_hold<MYSITE_X509_layout> mysite_scheme = new MYSITE_X509_layout();
	obj_hold<MYSITE_CA_task> task = new MYSITE_CA_task(  mysite_scheme, ISS_tag );

	/*
		VALS <== -code- tag
	*/
	if(!task->MYSITE_SET_DEMO_VALUE_FOR( ISS_tag )) return FAIL_FAILED();

	/*
		VALS => REQ => ISS => files
	*/
	if(!task->MYSITE_mk_cert_from_VALS()) return FAIL_FAILED();

	return PASS("DONE");
}
		//////////////////////////////////////////////

int
bool_main(int argc, char *argv[])
{


	//
	// Before anything seed the prng, and init the library
	// we dont use the instance, just the PRNG to generate RSA
	//
	obj_hold<SSL_global_server_eg>  _ssl_serv = new SSL_global_server_eg();

	const char * progname = *argv++;
	argc--;
	char * argv_default[] = { (char *)"ALL", NULL }; // const
	if(argc==0) {
		argv = argv_default;
		argc= 1;
	}
	while(argc>0) {
		str0 arg = *argv++;
		argc--;
		if(arg == "CA_ROOT") {
			if(!DEMO_mk_cert(is_CA_ZERO)) return FAIL_FAILED();
			continue;
		} else
		if(arg == "CA_ONE") {
			if(!DEMO_mk_cert(is_CA_ONE)) return FAIL_FAILED();
		} else
		if(arg == "CA_ZONE") {
			if(!DEMO_mk_cert(is_CA_ZONE)) return FAIL_FAILED();
		} else
		if(arg == "C_pc") {
			if(!DEMO_mk_cert(is_C_pc)) return FAIL_FAILED();
			if(!DEMO_mk_cert(is_C_pc)) return FAIL_FAILED();
			if(!DEMO_mk_cert(is_C_pc)) return FAIL_FAILED();
			if(!DEMO_mk_cert(is_C_pc)) return FAIL_FAILED();
		} else
		if(arg == "C_user") {
			if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
			if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
			if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
			if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
			if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
			if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
		} else
		if(arg == "C_server") {
			if(!DEMO_mk_cert(is_C_server)) return FAIL_FAILED();
		} else
		if(arg == "C_item") {
			if(!DEMO_mk_cert(is_C_item)) return FAIL_FAILED();
			if(!DEMO_mk_cert(is_C_item)) return FAIL_FAILED();
			if(!DEMO_mk_cert(is_C_item)) return FAIL_FAILED();
			if(!DEMO_mk_cert(is_C_item)) return FAIL_FAILED();
		} else
		if(arg == "ALL") {
			if(!DEMO_mk_cert_group()) return FAIL_FAILED();

#if 0
		if(1)	if(!DEMO_mk_cert(is_CA_ONE)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_CA_ZONE)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_pc)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_pc)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_pc)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_server)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_server)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_server)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_item)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_item)) return FAIL_FAILED();
		if(1)	if(!DEMO_mk_cert(is_C_item)) return FAIL_FAILED();
#endif

		} else {
			return FAIL("unrecognised arg '%s'", (STR0) arg );
		}
	}

	return PASS("----------------- ok -----------------------");
	// DTOR adds some ref_zero noise
}

int
main(int argc, char *argv[])
{
	if(bool_main(argc, argv )) return 0;
	WARN("want to get errno ...");
	return 1;
}
