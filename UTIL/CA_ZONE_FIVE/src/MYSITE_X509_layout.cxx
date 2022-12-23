/*
	LGPL exception - the rest of this package is LGPL,
	but this file (samples/2010/CA_ISS_util/mk_req.cxx and MYSITE*)
	is an example demo file,
	so feel free to copy paste customise in your app
	without too much fuss, OK
*/

#include "MYSITE_X509_layout.h"

//	#include "dgb.h"		// FAIL, WARN etc
//	#include "SSL_global.h"		// everyone must init rand etc
//	#include "CB_get_phrase.h"	// new CB_get_phrase_NONE()
//	#include "EVP_P_KEY.h"		// test_write_read_PrivateKey();

//	#include "SSL_stubs.h"
//	#include "SSL_CA1_stubs.h"

using namespace CA1;


/*!
	Plain CTOR, sets a helpful debug name, and renames "PC"
*/
MYSITE_X509_layout:: MYSITE_X509_layout(
		STR0 layout_name,
		STR0 C_NAME
) : SITE_X509_layout( layout_name )
{
	str_C_pc = C_NAME;
}

#if 0
	if(! MYSITE_obtain_phrase(
		buffer1 phrase; // APPEND
		SITE_X509_tag_enum ISS_tag
		STR0 NAME;
	)) return FAIL_FAILED();
#endif

bool MYSITE_obtain_phrase(
	buffer1 & phrase_utf, // APPEND
	SITE_X509_tag_enum ISS_tag,
	STR0 NAME
) {

	const char * phrase = NULL;
	switch(ISS_tag) {
	 case is_CA_ZERO:  phrase = "secret0"; break;
	 case is_CA_ONE:   phrase = "secret1"; break;
//	 case is_CA_ZONE:  phrase = "secret2"; break;
	 case is_CA_ZONE:  phrase = NULL; break;
	 case is_C_pc:     phrase = "secret3"; break;
	 case is_C_user:   phrase = "secret4"; break;
//	 case is_C_server: phrase = "secret5"; break;
	 case is_C_server: phrase = NULL; break;
	 case is_C_item:   phrase = "secret6"; break;
	 default:
		return FAIL("ISS_tag not recognised");
	}
	if(!phrase) { phrase = ""; }

	phrase_utf.print("%s", phrase );
	
	
	return WARN("%s",(STR0)phrase_utf);//;
	return true;
}

/*!
	all this automation - where is the secret phrase?

	NOTE also that you can have a _MUNGED_ phrase,
	that is not as easy to crack, and is also easier
	to lock yourself out, when normal tools dont work!
	Unless you can type in a phrase using a hex editor.

	This function was not virtual, and can laod the phrase from
	a file, from a variable, maybe from a GUI form,
	(now or later), maybe hardwired into the program,
	or some secret extracted from the BIOS NOR flash.
*/
bool MYSITE_X509_layout:: obtain_CB_for_tag(
	obj_hold<CB_get_phrase_base> &cb_phrase,
	SITE_X509_tag_enum ISS_tag,
	STR0 NAME
) {
	// MEGA LURK
//	str_C_pc = NAME;

	buffer1 phrase; // APPEND // empty

	if(! MYSITE_obtain_phrase(
		phrase,
		ISS_tag,
		NAME
	)) return FAIL_FAILED();

	if(!phrase) {
		// some server software cant handle cb_phrase
		// other variations would be mangled
		// ie phrase + method + purpose_str
		cb_phrase = new CB_get_phrase_NONE();
		return PASS("MYSITE sets NONE");
	}

	/*
		REWRITE obtain_phrase_for_key_rsa_priv

			we must already be an IDX_t 
			CXX SCRIPT looks up DB via CACHE even REMOTE

		REWRITE

			ITEM_KEY = { IDX = POOL.lookup( KEY ) }
			POOL == STO_ITEM EA_ITEM_from_KEY // and SESS // ()

		REWRITE

			KEY = u5_idx // in local CODE_POOL ENUM
			ITEM = C_VPN_user_laptop // ABBR //
			user == "amd"
			laptop == "10" // ABBR //
			C_VPN == "C_VPN" // CA_ZONE_VPN // CA_VPN_vpn_73 //

			DBID = load_ITEM_from_ether # maybe a DB 
			EA_ITEM is DBID_LOADED

		REWRITE

			ITEM.get_phrase_utf8();
			ITEM.get_phrase_utf8(buffer1 & phrase); // APPEND //
			// SUBLEX will optimise to APPEND // same func


	*/

	FAIL("phrase '%s'", (STR0) phrase );

	buffer2 buf;
	buf.print("%s_", (STR0) phrase );
	/* mysite_scheme-> */
	print_part_name( buf, ISS_tag );

	phrase = (STR0) buf;

	CB_get_phrase_blk1 *
	cb_phrase1 = new CB_get_phrase_blk1();
	cb_phrase1->set_enc_for_key_pem(); // EVP_des_ede3_cbc()
	cb_phrase = cb_phrase1;
	cb_phrase1->set_phrase( phrase ); // my pass phrase
	cb_phrase1->debug_set_obj_name( "CLIENT_KEY_SECRET_" );
	return PASS("DONE");
}

#if 0
/*!
	Virtual - the issuers certificate is probably loaded from
	file, but where is the phrase that protects that! Here it is.
*/
bool MYSITE_X509_layout:: obtain_issuer_cb_phrase( CA_task * task )
{
	SITE_X509_tag_enum CA_tag = task ->layout_tag -> get_parent_CA_tag();
	if(!obtain_CB_for_tag( task->issuer_cb_phrase, CA_tag))
		return FAIL_FAILED();

	return PASS("DONE in MYSITE");
}

/*!
	Virtual - the new certificate might itself be a CA,
	which must use the same phrase, (and method),
	to encrypt the new certificates _key on the way out,
	to be the same on the way in.

	You set this _layout class, for all your programs,
	on all your machines, as well as setting the same .ini
	files, and the same MYSITE_CA_task (or maybe not)
*/
bool MYSITE_X509_layout:: obtain_client_cb_phrase( CA_task * task )
{
	SITE_X509_tag_enum C_tag = task ->layout_tag -> tag; // poss CA
	if(!obtain_CB_for_tag( task->client_cb_phrase, C_tag))
		return FAIL_FAILED();

	return PASS("DONE in MYSITE");
}
#endif
