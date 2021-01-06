
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

struct sql_link_MySQL;

/*
	move these demo values to cfg_CA_1_defaults.h
*/

using namespace VARS;

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

	V_section_decl	GRP_CA_ZERO;
	V_var_decl	var_CA_ZERO_C;  // GB
	V_var_decl	var_CA_ZERO_ST; // Berks
	V_var_decl	var_CA_ZERO_O;  // MYSITE VLAN PLUS
	V_var_decl	var_CA_ZERO_OU; // Dept that uses CA_ZONE
	V_var_decl	var_CA_ZERO_CN; // CA_ZONE_TWO

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
