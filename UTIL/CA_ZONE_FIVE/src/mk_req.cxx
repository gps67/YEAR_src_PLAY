

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

#include "cfg_CA_1.h"

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
	NAME.print("GW_%s", (STR0) ABBR );
	return FAIL("TODO");
}

bool DEMO_mk_VPN_PC( str0 ABBR ) // ABBR = "GPS_D630"
{
	buffer1 NAME;
	NAME.print("PC_%s", (STR0) ABBR );
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
	// EDIT SOURCE LIST // GEN { FOREACH PCID !DEMO_mk_VPN_PC( PCID ) }
	// EDIT SOURCE DATA // GEN LIST_of_IDENT KEYS VAR_POOL
	// VAR_POOL += LOAD_CSR_in_TREE_of_SPEC // multi bundle
	// CT RT TREE SPEC STO STR TREE_as_DB_tree
	// DB_three uses virtual_base_class, provides sample DB_api_module
	// DB_api is CACHE_api_FILTER // need safe way of installing this code
	// DB_api is ELF_SEGMENT_api // CXX_API // CALL_api stack_frame CPU
	// DB_api is FILTER // DB_api in; DB_api out; // drop _t DIALECT
	// BENCH // _t "{ DIALECT drop_t _t }" // expect TYPE_NAME as VAR_NAME

	// ISSUER_CERT = loaded from somewhere (filename?)
	// ISSUED_CERT = VALS + ROLE in layout + FILENAME
	// supposedly online signing REQ from 

	// mk_VAR // DB[key] // KEY PC_ID //

	// mk_LAPTOP
	if(!DEMO_mk_VPN_PC("PC01_HUB_GW")) return FAIL_FAILED();
	if(!DEMO_mk_VPN_PC("PC02_DWS")) return FAIL_FAILED();
	return PASS("OK");
	if(!DEMO_mk_VPN_PC("PC02_AMS_10")) return FAIL_FAILED();
	if(!DEMO_mk_VPN_PC("PC03_GPS_E6320")) return FAIL_FAILED();
	return PASS("OK");
	if(!DEMO_mk_VPN_PC("PC04_MBS_")) return FAIL_FAILED();
	if(!DEMO_mk_VPN_GW("PC05_HUB_GW/*_PLUS*/")) return FAIL_FAILED();
	// KEY NEEDS TIGHT /*CMNT*/ parse mid_lex // semi_lex_edge //
	if(!DEMO_mk_VPN_GW("%s_%s/*%s*/")) return FAIL_FAILED();
	// OBJ_ID from ("%s_%s/*%s*/") {
		// BEHAVE AS_IF fmt looking for a STACK of ARGS
		// PROVIDE { %s "PC05" }
		// PROVIDE { %s "HUB_GW" }
		// PROVIDE { %s "_PLUS_" } // CMNT_FACTORY DATA_AS_CODE_SCRIPT
		// PROVIDE { IDX %d } // LIST of PC_ID "PC%02d" { IDX = N++ }
		// ALLOC RETVAR { idx = N++ } // PC_ID = ALLOC_PC_ID_N ++ //
		// REGEN DATA from NAME // int pcid = GET %d from "PC%02d" //
	// }


	return PASS("OK");
return PASS("OK");

	// following code left to re-integrate with USAGE
	// probably fell short of parameters // maybe

	// note above expects DB_PCID "PC_ID" is a CSR in env SESS_CTXT
	// alias SESS SESS_CTXT // VAR_POOL_MERGE // add ALIAS SUBSCRIBER
	// alias CTXT SESS_CTXT // VAR & POOL.lookup("STR0") // TYPE_as_VAR_NAME
	// TYPE_as_VAR_NAME // HINT leave a hungry api var connecting as TYPE("STR0")
	// HINT MATCH ("WELL_KNOWN_TYPE_NAME") // one of "STR0" OBJ_NAME
	// INFER // TYPE_USED_AS_NAME("STR0") // CTOR NLAPI
	// NLAPI // NOUN_VERB_ADJECTIVE_SUBLEX // ALIAS EACH { NOUN VERB LEX SUBLEX }
	// LEX // NOUN // OBJ_t & OBJ = DB["NAME"] // AUTO_VAR
	// DB["MATCHES"] // CACHE as SESS.STO and DB.KEEP
	// DB["EXPR"] // find a readonly preloaded tables API // VARS //

	// DB["{ idx = N ++ }" // "{ INTENTIONAL LEX SPACING }"
	// DB["PC05"]
	// DB["GW_HUB"]
	// DB["GW_HUB_PC05"]
	// DB["PC05_GW_HUB"]
	// DB["ALIAS"] // API_PLUS PLUS_API NAME suggests ENUM_GROUP 
	// DB["ALIAS"] // API_PLUS PLUS_API NAME suggests ENUM_GROUP 
	// DB.lookup("STR0"); // TYPE_as_NAME expects STR0_t // provided BY
	// DB["KEY"]
	// DB["PSG"]
	// DB["IDX"]
	// DB["EXPR"]
	// DB["RPC"]
	//




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
	// X509 uses openssl-crype but not SSL
	// we dont use the instance, just the PRNG to generate RSA
	//
	obj_hold<SSL_global_server_eg>  _ssl_serv = new SSL_global_server_eg();

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
	// cfg_CA_ load keyfile config file "cfg_CA_file.dat"
	//
        ::CFG_CA::kf_cfg_CA_1 cfg;
        // SWITCH TO SESSION
        cfg.late_init();
        if(cfg.load_file_()) {
                INFO("loaded");
		INFO("unnecessary SAVE might update fields descrs");
                cfg.save_file_();
        } else {
		// TODO // if bad pass - dont overwrite cfg ??
                INFO("defaulted");
                cfg.set_DEMO_values();
                cfg.save_file_();
        }

	//
	// run ARGV
	//
	while(argc>0) {
		str0 arg = *argv++;
		argc--;
		if(arg == "CA_ZERO") {
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
	gdb_sigaction( argv[0] ); // sets progname_argv0 
//	check_tty_012(); // 
 if(0)	dgb_fork_stderr_to_tcl_text(); // only when gdb is in use ?

	if(bool_main(argc, argv )) return 0;
	WARN("want to get errno ...");
	return 1;
}
