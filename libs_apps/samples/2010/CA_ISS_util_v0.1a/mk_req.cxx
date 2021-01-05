

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

	if(1)	if(!DEMO_mk_cert(is_CA_ZERO)) return FAIL_FAILED();
	if(1)	if(!DEMO_mk_cert(is_CA_ONE)) return FAIL_FAILED();
	if(1)	if(!DEMO_mk_cert(is_CA_ZONE)) return FAIL_FAILED();
	if(1)	if(!DEMO_mk_cert(is_C_pc)) return FAIL_FAILED();
	if(1)	if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
	if(1)	if(!DEMO_mk_cert(is_C_server)) return FAIL_FAILED();
	if(1)	if(!DEMO_mk_cert(is_C_item)) return FAIL_FAILED();

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
