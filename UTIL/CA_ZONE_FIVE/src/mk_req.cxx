

/*
	LGPL exception - the rest of this package is LGPL,
	but this file (samples/2010/CA_ISS_util/mk_req.cxx and MYSITE*)
	is an example demo file,
	so feel free to copy paste customise in your app
	without too much fuss, OK
*/

/*
	TODO rework
	move file_name_part to VALS (?) or scheme? or _task? or loader ?
	maybe split CA/C into two loaders, one scheme

	maybe dir "CA/FM_CA/company_name/vpn_96/C_PC_amd_10"
	maybe dir "CA/FM_CA/company/vpn_96/C_PC_amd_10"
	maybe dir "CA/CA_ZERO_FM_CA/"
	maybe dir "CA/CA_ONE_company_name/"
	maybe dir "CA/CA_vpn_96/"
	maybe dir "CA/C_PC_amd_10*"

	maybe a seperate ./.priv/C_PC_amd_10.{priv_key,priv_key_phrase}

	ABBR "amd_10"
	NAME "C_PC_amd_10"
	CA C_VPN_

	Regardmess of all filenames being unique in a flat DB
	we need to provide CA_ZONE and above

	we need to know CA_ZERO == "CA_ZERO_2022"
	we need to know CA_ONE  == "CA_ONE_BH1"
	we need to know CA_ZONE == "CA_ZONE_vpn_96"
	we need to know C_PC -uses- CA_ZONE "vpn_96"

		load "vpn_96" 
		find it's parent CA
	
	full tree uses subsirs per CA
	scan C_provided ...


	for now FLAT with CN == "C_PC_amd_10" CA == "CA_vpn_96"
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

	buffer2 C_PC_ABBR;
	C_PC_ABBR.print("C_PC_%s", (STR0) ABBR );
//	task->set_CN( (STR0) C_PC_ABBR );

	// SAVE NAME ${NAME} // ALIAS EA NAME // simplistic_LEX_flag("TOKEN")
	// as it comes into SCOPE send signals FIRST_USE_CREATE_VAL "cident99"
	// PICK CN = "C_PC_ABBR" // ABBR == "%s" // leaves STACK TOP HUNGRY
	// %s // %d // %s_%s // NAME_idx // { NAME %s } // 

	SITE_X509_tag_enum ISS_tag = is_C_pc;
	obj_hold<MYSITE_X509_layout> mysite_scheme = new MYSITE_X509_layout("VPN2022", (STR0) C_PC_ABBR);
	obj_hold<MYSITE_CA_task> task = new MYSITE_CA_task(  mysite_scheme, ISS_tag, (STR0) C_PC_ABBR );

	/*
		VALS <== -code- tag

		that fails - which ZONE is ITEM in 

		OBTAIN CA_ZONE	ZONE
		CREATE  C_ITEM  USER

		OBTAIN CA_ZONE	ZONE_FIVE
		CREATE CA_LIST	USERS 

		OBTAIN CA_LIST	USER
		CREATE  C_ITEM  USER_0xFFFF

	*/

	// set CN { C_VPN_PC_amd_10 }
	// set ISS_tag is_C_pc
	// set IDX POOL_ITEM lookup(CN) // CN NAME ABBR group_name SUBLEX //
	// SUBLEX bind by ID 

	if(!task->MYSITE_SET_DEMO_VALUE_FOR( ISS_tag, (STR0) C_PC_ABBR )) return FAIL_FAILED();

	/*
		VALS => REQ => ISS => files
	*/
	if(!task->MYSITE_mk_cert_from_VALS()) return FAIL_FAILED();
	return PASS("X509");
}

bool DEMO_mk_cert_group()
{
	// CA_ZONE("vpn_96")
	// REQUIRE //
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

	// FILTER // PCID = "PC" // SUBLEX // IDX found 
	// FILTER // %s_%s // C_PC_%s // SYNTAX bounces %s // %s // SCRIPTS

	// mk_LAPTOP
	if(!DEMO_mk_VPN_PC("HUB_GW")) return FAIL_FAILED();
	if(!DEMO_mk_VPN_PC("DWS")) return FAIL_FAILED();
	if(!DEMO_mk_VPN_PC("AMS_10")) return FAIL_FAILED();
	if(!DEMO_mk_VPN_PC("GPS_E6320")) return FAIL_FAILED();
	return PASS("OK");
	if(!DEMO_mk_VPN_PC("MBS_BOX1")) return FAIL_FAILED();
	if(!DEMO_mk_VPN_GW("PC_HUB_GW/*_PLUS*/")) return FAIL_FAILED();
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

#if 0
	SWITCH ${USER} { // CMNT
	}; // CMNT

	{ OBJ_LIST }
		"HUB_GW"
		"DWS"
		"AMS_10"
		"GPS_E6320"
		"MBS"

		"PC_%s"
		"OK"
		"PC_HUB_GW/*_PLUS*/" // PSG and SUBLEX in CXX_SCRIPT
#endif

	return PASS("OK");
} // FUNC mk_cert_group // regen_all // cancel old ones ? // 
// DEMO_mk_cert_group // regen_all // cancel old ones ? // 
#if 1

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
#endif

	const char * str_CA_ZONE = "CA_ZONE_";

	obj_hold<MYSITE_X509_layout> mysite_scheme
	= new MYSITE_X509_layout( "layout", str_CA_ZONE );
	obj_hold<MYSITE_CA_task> task = new MYSITE_CA_task(
		mysite_scheme,
		ISS_tag,
		str_CA_ZONE	// TODO CA scheme tag NAME // API //
	);

// OLD	if(!task->MYSITE_SET_DEMO_VALUE_FOR( ISS_tag )) return FAIL_FAILED();
// in struct DECL ZONE // 	INFO("HERE OK");

#if 0 
{
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
#endif
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
	obj_hold<MYSITE_X509_layout> mysite_scheme
	= new MYSITE_X509_layout("layout", "CNCN1a" );
	obj_hold<MYSITE_CA_task> task = new MYSITE_CA_task(  mysite_scheme, ISS_tag, "CNCN" );
	
	/*
		VALS <== -code- tag
	*/
	if(!task->MYSITE_SET_DEMO_VALUE_FOR( ISS_tag, "CNCN1" )) return FAIL_FAILED();
	
	/*
		VALS => REQ => ISS => files
	*/
	if(!task->MYSITE_mk_cert_from_VALS()) return FAIL_FAILED();
	
	return PASS("DONE");
}
		//////////////////////////////////////////////

bool DEMO_mk_cert_tag_NAME(
	SITE_X509_tag_enum ISS_tag,
	STR0 NAME
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
	STR0 layout = "layout";
	obj_hold<MYSITE_X509_layout> mysite_scheme = new MYSITE_X509_layout(layout, NAME );
	obj_hold<MYSITE_CA_task> task = new MYSITE_CA_task(  mysite_scheme, ISS_tag, NAME );
	
	/*
		VALS <== -code- tag
	*/
	if(!task->MYSITE_SET_DEMO_VALUE_FOR( ISS_tag, "NAME_CN" )) return FAIL_FAILED();
	
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
		} else
		if(arg == "C_user") {
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
 if(1)	dgb_fork_stderr_to_tcl_text(); // only when gdb is in use ?

	if(bool_main(argc, argv )) return 0;
	WARN("want to get errno ...");
	return 1;
}
