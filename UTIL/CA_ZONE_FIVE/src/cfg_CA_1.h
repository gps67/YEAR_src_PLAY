
#ifndef cfg_CA_H
#define cfg_CA_H

// OK the phrase and login forms are being a bit confused here
// this is to allow a split later

// TOWRDS generic login .profile 
// TOWARDS Tokenised Script DATA // login script setup_CTXT

// TOWARDS VENDABLE login DATA access_permit
/*
	laptop holds sealed application
	open helpers are available, to fast track login
	ie initial connection DATA, 
	or MENU of login_HATS

	usual case
		login once each day
		checkin the secure loop (it can veto)
		get new connection via hotdesk LAN
		be happy
	
	other cases
		forgot my password
		its a friends laptop but i am me
		requesting permit from login hub
		requesting permit from permit
		registering new pc
	
	Sealed App has some local data files
		ZONES mount open like an FS
		login_permit_keystore // users full login enables access
		BLOB_ZONE CryptedBlob
		BLOB_ZONE ECMA_Byte_Array_wrapped
	
	CodeBook Scripts
		USER_LOGIN
		PASS_PHRASE_to_RSA_priv_one
		PASS_PHRASE_to_RSA_priv_two // binary obtained from PERMIT
		PASS_PHRASE_is_PRNG_key_blob // 

		USER_PROFILE
		.permits["STR0"]
		.key_ring["keys"].ITEM["STR0"]

		Negociate with LOGIN_SERVER
		Generate a new key for this session
		Made of 2 halves
		Revealed after commit
			L->-R BLOB128 = crypt K2=K2 clear=BLOB128_clear
			L-<-R BLOB128 = crypt K2=K2 clear=BLOB128_clear
			L->-R BLOB128 = here_is K2=K2.data
			L-<-R BLOB128 = here_is K2=K2.data

		Join 2 Halves
			BLOB256 = MIX (method) BLOB128 + BLOB128

		Use that as the PERMIT.enabler to open key_ring
		Decide to keep all key_ring_zone_three open
		Decide to temp all key_ring_zone_zero { close soon }

		IMPORT SCRIPTS that team play the key ring access rules

		Apply BLOB as RSA_priv key over BLOB that reveals BLOB
		Apply BLOB as CBK_priv with CVK_SEED

		POPUP on operators desk
			activate standard operator scripts for FORM
			match actions + PERMITS with ROM.RULES.POPUPS
				ROM = AUTH_SERVER code book of scripts

		AGREE
			shared_var
				"STR0" -> linked_item
		AGREE
			loaded_env
				"CTXT" -> loaded u3_u5_SUITE
				preloaded from ROM
				.session.scripts and their demands OK
		LOCAL
			active_env
				"thread_vars" -> loaded u3u5_suite_
			selectables
				"u8_POOL"
				"u16_ITEM_IDX"

			SUITE[ u3_suite ]
			LAYER[ u5_LIBR ]

			LIBR = SUITE.LIBR[ u5 ]

			DECODER: {
				// everyone is ising "thread_vars"
				// it is a DOME that imports loads of related
				// it appears as an addressable item
				// within its own workspace
				// but even if it didn;t we have
				// SUITE thread += SUITE_thread_vars

				/ *
					CT tell DECODER that
						THIS = LIBR
						LIBR.type = virtual from T=LIBR

						that obtains a nearby VAR
						eg GS is immediately available
						ie u3u5_LIBR
						   u8 == GS_VAR_POOL_ITEM[u8]
						   i16_hilo == expr data
						   i16_hilo == IDX(STR0("varname"))
					// DECODE varname == ITEM.obj.varname

					// ALIAS varname rowname

				* /
			}

	usual case
		this is a MACRO being invoked
		take ZERO time to login = done, have PERMIT, CT_PERMIT
		GEN obtains MACRO
		GEN does clever stuff
		GEN invokes MACRO inline

			DATA_view.GETTER.get_u32_u32_at_OFFS( ... ) {
				u32_u32_clutch & retvar 

				SZ8 = 8 == sizeof( u32_u32_pair_in_blob }
				OFFS = u16 * SZ8

				retvar = GETTER.get_u64_is_u32_u32_pair_in_reg

			DATA_view.GETTER.get_u64_is_u32_u32_pair_in_reg

				retvat = GETTER.get_u64_hilo_in_reg

			DATA_view.GETTER.get_u64_hilo_in_reg

				retvat = GETTER.get_u64_hilo // ByteArray




*/

/*
	This might be a managed app // tool for
	Many apps use a database
	We want a variety of data store connectors
	Web Forms that match these vars ... DDRY

	.profile.blob.sqlite.var_pool.SQL {
		DATA CONNECTOR TO
			mysql
		LOGIN SCRIPT DATA
			server_ip4_addr
			server_ip4_port
			server_login_user1
			server_login_user1_pass_GETTER
			server_database_name

*/

#include "str1.h"
#include "key_file.h"
#include "var_tree.h"
#include "var_tree2.h"

#include "X509_VALS_base.h"

struct sql_link_MySQL;

/*
	move these demo values to cfg_CA_1_defaults.h
*/

using namespace VARS;
using namespace CA1;

namespace CFG_CA {
struct kf_cfg_CA_0 : public obj_ref0 // move this to V_file as base
{
	V_file_decl V_file;

	// NOT virtual
	~kf_cfg_CA_0();
	kf_cfg_CA_0();

	bool load_from_buffer( blk1 & buf ) {
		return V_file.load_from_buffer( buf );
	}
	bool save_into_buffer( blk1 & buf ) {
		return V_file.save_into_buffer( buf );
	}

};

struct V_var_group_decl_CA_VALS {
	V_var_decl	var_CA_name;  // "VPN_97" // CA_ZONE("VPN_97") //
	V_section_decl	GRP_CA;
	V_var_decl	var_CA_C;  // GB
	V_var_decl	var_CA_ST; // Berks
	V_var_decl	var_CA_O;  // MYSITE VLAN PLUS
	V_var_decl	var_CA_OU; // Dept that uses CA_ZONE
	V_var_decl	var_CA_CN; // CA_ZONE_TWO
	bool late_init(
		V_file_decl & V_file,	// the container, uplinks
		const char * GRP_NAME,	// "CA_ZERO" 
		const char * subname,	// "VPN_97" 
		X509_VALS_subject & VALS
	);

	// ELF // ITEM // CA_name links to a CSR in MINI_WORLD
	// CA_NAME // name // "VPN_97" // STR0 ANYSTR //
	// residual MATCH fade // repeat relearn via ROM instant recalc
	// FADE STATE { FIND FOUND API SESS_ASPECT_STATE } { SETTING FADER }
	// FADER_STATE %2.1f PERCENT_FADER
	// ELF.lookup( NAME ) // VPN_97 // so well known it is now a var NAME
	// CODE_POINT adjacent comment notices MATCH // VPN_97 = "%s_%s" VPN %s
	// NAME %s NAME // NAME = %s%s%s LHS MID RHS // PRE_COMPILE_CT_STR0
	// NAME %s NAME // NAME = VPN_%s LHS MID RHS // PRE_COMPILE_CT_STR0
	// LHS { "VPN" } 
	// MID { "_" }
	// EXPR { "VPN_%s" RHS } // RHS is a PARAMETER seeking to be itself
	// MATCH "STR0_t ANYSTR" // PREDICT use of STR0_t for STR_t for TOKEN
	// CA_ZONE("VPN_97") // AS_IF MATCH // var = setting CSR == TOKEN //
	// CSR_ALIAS // any MATCH will do // NEARBY_CODE gets activated
	// CSR_FLOW // DECODE("CSR") // API("CSR") // API("STEP") //
	// CSR_KNOW // VPN_97 == "VPN_44" //
	// PRE_MATCH TOKEISE TOKEN == lookup("VPN_97") // 
	// PRE_MATCH TOKEISE TOKEN == lookup("name") // 
	// PRE_MATCH TOKEISE TOKEN == lookup("CA_subname") // VAR_POOL("VPN_%s")
	// UNPROVIDED %s args // well we have to check, so MATCH AUTO_ARG_LOAD
	// UNPROVIDED %s args // AUTO_VAR { %s VAR_NAME } //
	// KNOW "VAR_NAME" VAR_NAME "CA_ZONE_vpn_hub2" // hub2 == "VPN_91_RSA"
	// own VPN over MUX over SSL with RSA_PERMIT // proof of access
	// proof of access to RSA.key_priv // API for phase_protected_files
	// proof of previous SIGN TEXT_SIGN_META // META from within
	// usage CLAIM use of KEY over TEXT proved agreement YES
	// usgae TEXT = { CLAIM TEXT } // CLAIM = CODE_POINT_CLARIFIES
	// usage PROOF += CLARIFICATION_tests_CLAIM // DIALECT += TRUST_CLAIM
	// usage CA_ZONE _VPN_97 allocated X509 TOPKENISED_AS_X509
	// usgae X509_t VPN_PC_%s // detect hunger for "ams_10" // SUBLEX festure
	// SUBLEX // MATCH "AMS_10" as ALIAS "ams_10" at XPOS in FILE
	// 
	// usage "CA_ZERO_%s"
	// usage "CA_ONE_%s"
	// usage "CA_ZONE_%s"
	// usage "CA_VPN_%s"
	// usage "CA_EMAIL_%s"
	//
	// usage DO_NOT_CARRY_PASSWORD_use_PROXY
	// usage DO_NOT_CARRY_PASSWORD_use_NONE_or_DONE // you can check SIGN
	// usage SIGN TEXT // TREE of signed sub-secion features EDITS MARKS
	// eg DISAGREE default and pick OPTION
	// ie SENSE_OPTIONS from SUITE of similar meaning options
	// ROM contains code that API makes available // API // secure_asm //


	// API // += // GEN ASM and run in a SAFE environment // SQUARED //
	// API // += // RUN ASM WASM in a CHAOS environment // SQUARED //
	// API // += // RUN ASM WASM in a SANDBOX environment // known //
	// API // += // RUN JS in a SANDBOX environment // known //
	// API // += // RUN CXX in a SANDBOX environment // known // ROM //

	// API += APPLY RSA_key_priv to TASK // via PROXY // permit app //
	// API += PERMIT app to use RSA_key_priv from fd = OPEN PERMITS
	// API simulates REMOTE API_vars // CACHE // fd = OPEN // LOTS //
	// late_features + OPEN += what ; OPEN += PERMITS //  I_HAVE_RSA //
	// PERMITS += PERMIT // REQUIRES RSA_key_priv SIGNED_BY_RSA
	// SIGN = { TEXT SHA1 } // { SHA1 /* 32_bytes_of_256_bits or_other */ }
	// SIGN = { TEXT SHA1 } // SHA1 == "BITS_256_in_4_WORDS" // COPY_KNOW
	// WRAP applied RSA outside SANDBOX with logic from ROM

};

/*!
	config for the gateway (server)

	NB some details might be available from /home/config/global.cfg
	but they must be transferred to here.

	This has the local GUI possible, that has the remote WEB possible.

	If necessary, it is the settings from the provisioning
*/
struct kf_cfg_CA_1 : public kf_cfg_CA_0
{ 

	bool save_file( const char * filename );
	bool load_file( const char * filename );

	str1 file_name;

	bool save_file_() {return save_file( (STR0)file_name ); }
	bool load_file_() {return load_file( (STR0)file_name ); }

	V_var_decl	var_random_str;

	void gen_random_str();
	void use_random_str();

	V_var_group_decl_CA_VALS V_grp_CA_ZERO;
	V_var_group_decl_CA_VALS V_grp_CA_ONE;
	V_var_group_decl_CA_VALS V_grp_CA_ZONE;

#if 0
	V_section_decl	GRP_CA_ZERO;
	V_var_decl	var_CA_ZERO_C;  // GB
	V_var_decl	var_CA_ZERO_ST; // Berks
	V_var_decl	var_CA_ZERO_O;  // MYSITE VLAN PLUS
	V_var_decl	var_CA_ZERO_OU; // Dept that uses CA_ZONE
	V_var_decl	var_CA_ZERO_CN; // CA_ZONE_TWO
#endif

	V_section_decl	GRP_DEMO_CONNECT;

	V_var_decl	var_db_pref; // mysql || pgsql || pdb || _

	V_var_decl	var_mysql_host;
	V_var_decl	var_mysql_port;
	V_var_decl	var_mysql_db;
	V_var_decl	var_mysql_user;
	V_var_decl	var_mysql_pass;
	V_var_decl	var_mysql_rootuser;
	V_var_decl	var_mysql_rootpass;

	V_var_decl	var_pgsql_host;
	V_var_decl	var_pgsql_port;
	V_var_decl	var_pgsql_db;
	V_var_decl	var_pgsql_user;
	V_var_decl	var_pgsql_pass;
	V_var_decl	var_pgsql_rootuser;
	V_var_decl	var_pgsql_rootpass;

	bool connect_mysql( sql_link_MySQL & link );

#if 0
	V_var_decl	var_C_SERVER_cert;
	V_var_decl	var_C_SERVER_key;
	V_var_decl	var_C_SERVER_key_phrase_part;

	// really want _old _new _etc (not to mention CRL)
	// currently active in this server
	V_var_decl	var_CA_ZONE_cert; // it is my CA of C_DEMO_cert
	V_var_decl	var_CA_ONE_cert; // the online one
	V_var_decl	var_CA_ZERO_cert; // the untouchable offline one
#endif

	V_section_decl GRP_DEMO_DEMO;

	kf_cfg_CA_1(); // INTENSIVE - CTORS all vars
	~kf_cfg_CA_1(); // 

	bool late_init();
	bool set_DEMO_values();

	// QUICK CALLERS

	static const char * get_filename_default_hardcoded(); // cfg_CA_1.kf

}; // struct
}; // NameSpace
#endif
