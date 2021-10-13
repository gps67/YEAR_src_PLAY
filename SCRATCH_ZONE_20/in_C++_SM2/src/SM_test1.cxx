#include "ints.h"
#include "str0.h"
#include "dgb.h"	// FAIL

int
bool_main(int argc, char *argv[])
{

	//
	// TODO argv_builder
	//
	const char * progname = *argv++;
	argc--;
	char * argv_default[] = { (char *)"ALL", NULL }; // const
	if(argc==0) {
		argv = argv_default;
		argc= 1;
	}

	//
	// run ARGV
	//
	while(argc>0) {
		str0 arg = *argv++;
		argc--;
		if(arg == "test_1") {
//			if(!DEMO_mk_cert(is_CA_ZERO)) return FAIL_FAILED();
			continue;
		} else
		if(arg == "test_1") {
//			if(!DEMO_mk_cert(is_CA_ONE)) return FAIL_FAILED();
		} else
		if(arg == "test_1") {
//			if(!DEMO_mk_cert(is_CA_ZONE)) return FAIL_FAILED();
		} else
		if(arg == "test_1") {
//			if(!DEMO_mk_cert(is_C_pc)) return FAIL_FAILED();
//			if(!DEMO_mk_cert(is_C_pc)) return FAIL_FAILED();
//			if(!DEMO_mk_cert(is_C_pc)) return FAIL_FAILED();
//			if(!DEMO_mk_cert(is_C_pc)) return FAIL_FAILED();
		} else
		if(arg == "test_1") {
//			if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
//			if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
//			if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
//			if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
//			if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
//			if(!DEMO_mk_cert(is_C_user)) return FAIL_FAILED();
		} else
		if(arg == "test_1") {
//			if(!DEMO_mk_cert(is_C_server)) return FAIL_FAILED();
		} else
		if(arg == "test_1") {
//			if(!DEMO_mk_cert(is_C_item)) return FAIL_FAILED();
//			if(!DEMO_mk_cert(is_C_item)) return FAIL_FAILED();
//			if(!DEMO_mk_cert(is_C_item)) return FAIL_FAILED();
//			if(!DEMO_mk_cert(is_C_item)) return FAIL_FAILED();
		} else
		if(arg == "ALL") {
//			if(!DEMO_mk_cert_group()) return FAIL_FAILED();

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
