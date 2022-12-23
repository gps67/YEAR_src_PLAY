#include "SITE_X509_layout.h" // base class
#include "SITE_CA_task.h" 
#include "dgb.h"
#include "buffer2.h"
#include "EVP_P_KEY.h"
#include "X509_cert.h" // tag keys
// #include "SITE_X509_VALS_base.h" 
#include "X509_VALS_base.h" 
#include "X509_REQ_hold.h" 
#include "SSL_global.h" 

using namespace CA1;

	// MYSITE_loaded::		// a tree of certs from HERE to ROOT
	// MYSITE_loader::		// virtual IO
	// MYSITE_loader_files::	// from files (not SQL)

	////////////////////////////////////////////////////////////////

MYSITE_loaded::
~MYSITE_loaded()
{
}

MYSITE_loaded::
MYSITE_loaded( SITE_X509_tag_enum _self_tag )
{
	self_tag = _self_tag;
}


X509_cert & MYSITE_loaded:: get_cert_var( SITE_X509_tag_enum tag )
{
 switch( tag ) {

	case is_CA_ZERO:	return CA_ZERO;
	case is_CA_ONE:		return CA_ONE;
	case is_CA_ZONE:	return CA_ZONE;

	case is_CA_server:	return CA_server;
	case is_CA_user:	return CA_user;
	case is_CA_pc:		return CA_pc;
	case is_CA_item:	return CA_item;

	case is_C_server:	return C_server;
	case is_C_user:		return C_user;
	case is_C_pc:		return C_pc;
	case is_C_item:		return C_item;

	case is_C_N:
	case is_C_UNKNOWN:
	case is_C_UNSET:
			FAIL("BAD tag");
			return C_fake;
 }
	FAIL("BAD tag worse!");
	return C_fake;
}

X509_cert & MYSITE_loaded:: get_cert_var_self()
{
	return get_cert_var( self_tag );
}

bool MYSITE_loaded:: set_self_cb_phrase( CB_get_phrase_base * cb_phrase )
{
	if(!cb_phrase) cb_phrase = new CB_get_phrase_NONE();
	if(self_key) {
		// you CAN change the password being used
		if( self_key->cb_pass ) WARN("changing exising cb_phrase");
		else WARN("KEY DIDNT HAVE AN EXISTING cb_phrase to change");
		// set new callback (with retained data) for write out
		self_key -> cb_pass = cb_phrase;
	} else {
		self_key = new EVP_P_KEY( cb_phrase );
		if(!self_key) return FAIL_FAILED();
	}
	return true;
}

	////////////////////////////////////////////////////////////////

MYSITE_loader::
MYSITE_loader(
	MYSITE_loaded * _loaded,
	SITE_X509_layout * _scheme,
	CB_get_phrase_base * cb_phrase
)
: loaded(_loaded)
, scheme(_scheme)
{
	if(!scheme) THROW_dgb_fail("NULL scheme");
	if(cb_phrase) {
		if(!loaded) FAIL("NULL loaded - cannot set cb_phrase");
		if(!loaded->set_self_cb_phrase( cb_phrase )) FAIL_FAILED();
	}
}

/*
	actually doing this makes me realise that I cant remember how this works

	The CERT holds its own cb_phrase ... somehow ?

	The CERT can/not be written public only ?

	The KEY is/not written with the CERT ?

	etc

	ALSO how do self_tag / self_key get their value?
*/



// virtual
bool MYSITE_loader:: load_cert( SITE_X509_tag_enum tag )
{
	WARN("SUBCLASS SHOULD PROVIDE");
	gdb_invoke(); // subclass should provide
	return FAIL("UNWRITTEN");
}
// virtual
bool MYSITE_loader:: load_self_cert_and_key( SITE_X509_tag_enum tag, CB_get_phrase_base * cb_phrase )
{
	return FAIL("UNWRITTEN");
}
// virtual
bool MYSITE_loader:: save_cert( SITE_X509_tag_enum tag )
{
	return FAIL("UNWRITTEN");
}

bool MYSITE_loader:: save_key_second_as_now_encrypted()
{
	return FAIL("UNWRITTEN");
}

bool MYSITE_loader:: load_self_cert_and_key()
{
	if(!loaded) return FAIL("NULL loaded");
	CB_get_phrase_base * cb_phrase = NULL;
	if(loaded->self_key) cb_phrase = loaded->self_key->cb_pass;
	// self_tag was set in CTOR! so it does exist
	return load_self_cert_and_key( loaded->self_tag, cb_phrase );
}

/*!
	be careful that self_tag is actually set ... !
*/
bool MYSITE_loader:: load_self_cert_and_key( CB_get_phrase_base * cb_phrase )
{
	// self_tag was set in CTOR! so it does exist
	// cb_phrase might be another question
	if(!loaded) return FAIL("NULL loaded");
	return load_self_cert_and_key( loaded->self_tag, cb_phrase );
}


bool MYSITE_loader:: save_self_cert_as_txt()
{
	if(!loaded) return FAIL("NULL loaded");
	return save_cert_as_txt( loaded->self_tag );
}

bool
MYSITE_loader::
save_cert_as_txt( SITE_X509_tag_enum tag )
{
	return FAIL("UNWRITTEN");
}

/*!
	the cb_phrase is in the key

	the self_key and self_tag must be set

	this must be implemented by the deriving class
*/
bool MYSITE_loader:: save_self_cert_and_key()
{
	return FAIL("UNWRITTEN subclass function");
}

X509_cert & MYSITE_loader:: get_self_cert()
{
	return loaded->get_cert_var_self();
}

EVP_P_KEY * MYSITE_loader:: get_self_key()
{
	return loaded->self_key;
}

bool MYSITE_loader:: save_chain_above_self()
{
	if(!loaded) return FAIL("NULL loaded");
	if(!scheme) return FAIL("NULL scheme");
	bool ok = true;
	SITE_X509_tag_enum tag = loaded->self_tag;
	while( tag != is_CA_ZERO ) {
		tag = scheme->get_parent_CA_tag( tag );
		if(!save_cert( tag )) {
			buffer2 tagname;
			scheme->print_part_name( tagname, tag );
			WARN("error processing tag '%s'", (STR0) tagname );
			ok=false;;
		}
	}
	if(ok)
		return PASS("OK");
	else
		return FAIL_FAILED();
}

bool MYSITE_loader:: load_chain_above_self()
{
	if(!loaded) return FAIL("NULL loaded");
	if(!scheme) return FAIL("NULL scheme");
	bool ok = true;
	SITE_X509_tag_enum tag = loaded->self_tag;
	while( tag != is_CA_ZERO ) {
		tag = scheme->get_parent_CA_tag( tag );
		if(!load_cert( tag )) {
			buffer2 tagname;
			scheme->print_part_name( tagname, tag );
			WARN("error processing tag '%s'", (STR0) tagname );
			ok=false;;
		}
	}
	if(ok)
		return PASS("OK");
	else
		return FAIL_FAILED();
}

bool MYSITE_loader:: save_chain_and_self()
{
	if(!save_self_cert_and_key()) return FAIL_FAILED();
	if(!save_chain_above_self()) return FAIL_FAILED();
	return PASS("OK");
}

bool MYSITE_loader:: load_chain_and_self()
{
	if(!load_self_cert_and_key()) return FAIL_FAILED();
	if(!load_chain_above_self()) return FAIL_FAILED();
	return PASS("OK");
}

bool MYSITE_loader:: save_root_and_self()
{
	if(!save_self_cert_and_key()) return FAIL_FAILED();
	if(!save_root()) return FAIL_FAILED();
	return PASS("OK");
}

bool MYSITE_loader:: save_root()
{
	return save_cert( is_CA_ZERO );
}

bool MYSITE_loader:: load_root_and_self()
{
	if(!load_self_cert_and_key()) return FAIL_FAILED();
	if(!load_root()) return FAIL_FAILED();
	return PASS("OK");
}

bool MYSITE_loader:: load_root()
{
	return load_cert( is_CA_ZERO );
}

bool MYSITE_loader:: check_key_was_encrypted() // ERROR_BOX if not
{
	if(!loaded) return FAIL("NULL loaded");
	if(!loaded->self_key) return FAIL("NULL self_key");
	if(!loaded->self_key->KEY_was_crypted) {
		// side effect - save a movable file
		save_key_second_as_now_encrypted();
		return FAIL("NOT encrypted");
	}
	return PASS("YES");
}

/*!
	set the CLIENT certs

	loader knows parent age (when loaded doesnt)
*/
bool MYSITE_loader:: set_own_certificate_chain( SSL_global_util * _SSL_Global)
{
	if(!loaded) return FAIL("NULL loaded");	
	if(!loaded) return FAIL("NULL scheme");	
	if(!_SSL_Global) return FAIL("NULL _SSL_Global");	
	SSL_global_util & SSL_Global = *_SSL_Global;

	// eg not loaded yet
	if(!loaded->self_key) return FAIL("NULL self_key");

	// http://www.openssl.org/docs/ssl/SSL_CTX_use_certificate.html
	// only sets cb_ for an instant ... then sets back to ???
	if(!SSL_Global.set_own_cert_key(
		loaded->get_cert_var_self(),
		loaded->self_key
	))	return FAIL_FAILED();

	SITE_X509_tag_enum tag = loaded->self_tag;
	while( tag != is_CA_ZERO ) {
		tag = scheme->get_parent_CA_tag( tag );
		if(!SSL_Global.add_CA_to_own_chain(
			loaded->get_cert_var( tag )
		)) return FAIL_FAILED();
	}

//	// instead of load_verify_locations, do this ...
//	// that only does the sent to remote and request remote to ..
//	if(!add_CA_to_own_chain( CA_a )) return FAIL_FAILED();
//	if(!add_CA_to_own_chain( CA_b )) return FAIL_FAILED();
//	if(!add_CA_to_own_chain( CA_c )) return FAIL_FAILED();
//	if(!add_CA_to_own_chain( CA_d )) return FAIL_FAILED();
	STEP("DONE Setting own ID");
	return true;
	// now call set client_CA_list (from list from outdated PEM)
	// now call set verify peer
	return true;
}

bool MYSITE_loader:: add_trusted_CA( SSL_global_util * _SSL_Global, SITE_X509_tag_enum tag )
{
	if(!loaded) return FAIL("NULL loaded");
	X509_cert & CA_cert = loaded->get_cert_var( tag );
	if(!_SSL_Global->add_trusted_CA(  CA_cert )) return FAIL_FAILED();
	return true;
	return PASS("CN = %s", (STR0)  CA_cert.get_str_CN()); // base done
}

/*!
	when the client calls, these are the ROOT CA's that it might use

	The actual CA is used
*/
bool MYSITE_loader:: set_trusted_CA_list( SSL_global_util * _SSL_Global)
{
	if(!_SSL_Global) return FAIL("NULL _SSL_Global");
	if(!loaded) return FAIL("NULL loaded");
	if(!scheme) return FAIL("NULL scheme");
	if(!scheme->is_sensible_tag( loaded->self_tag )) return FAIL_FAILED();

	if(!add_trusted_CA( _SSL_Global, is_CA_ZERO )) return FAIL_FAILED();
	if(0) return PASS("DONE - early");

	// TEST:
	// with trusted = { ZERO }
	// we only need to trust the ROOT CA

	// GUESS:
	// with trusted = { ZERO ZONE server } // and exact match
	// not sure it we 'ca' trust a non-root (self signed) one
	// even though it would save some time if we did

	// TEST:
	// with trusted = { ZERO ZONE } // and ZONE different RSA
	// change {ZONE server} on gw
	// login file
	// did seem to take longer?
	// maybe the RSA checks are done deeper?
	// or maybe I need to actually time it properly	
	//

	if(!add_trusted_CA( _SSL_Global, is_CA_ONE )) return FAIL_FAILED();
	if(0) return PASS("DONE - early");

	switch( loaded->self_tag ) {
	 case is_C_pc:
	 case is_C_server:
		if(!add_trusted_CA( _SSL_Global, is_CA_ZONE )) return FAIL_FAILED();
		break;
	default:
		WARN("TAG is xxx");
	}
	return PASS("DONE");
}

	////////////////////////////////////////////////////////////////

bool
MYSITE_loader_files::
save_cert_as_txt( SITE_X509_tag_enum tag )
{
	if(!loaded) return FAIL("NULL loaded");

	// input filename
	buffer2 filename_cert; // _pem
	if(!print_filename_cert_pem( filename_cert, tag )) return FAIL_FAILED();

	// output filename
	buffer2 filename_cert_txt;
	if(!print_filename_cert_txt( filename_cert_txt, tag )) return FAIL_FAILED();

	buffer2 cmd;
//	cmd.print("set -x ;")
	cmd.print("openssl x509 -noout -in '%s' -text 2>&1 | tee '%s'",
		(STR0) filename_cert,
		(STR0) filename_cert_txt
	);
	system( (STR0) cmd );
	return true;
}

MYSITE_loader_files::
MYSITE_loader_files(
	MYSITE_loaded * _loaded,
	SITE_X509_layout * _scheme,
	CB_get_phrase_base * _cb_phrase,
	const char * _pfx_dir // = "db_certs"
)
: MYSITE_loader(_loaded, _scheme, _cb_phrase )
{
	if(!scheme) THROW_dgb_fail("NULL scheme");
	pfx_dir.set(_pfx_dir); // "keys/" on client "zone5/box5" on server
	if(!scheme->is_sensible_tag( loaded->self_tag ) ) {
		FAIL("BAD self_tag");
		FAIL("BAD self_tag--------------");
		FAIL("BAD self_tag");
		gdb_invoke();
		// want loaded to do this,
		// but it requires a scheme, which loaded does not have!
	}
}

bool MYSITE_loader_files::
print_filename_cert_pem( buffer2 & buff, SITE_X509_tag_enum tag )
{
	if(!scheme) return FAIL("NULL scheme");
	if(!buff.print( "%s", (STR0) pfx_dir )) return FAIL_FAILED();
	if(!scheme->print_part_name( buff, tag )) return FAIL_FAILED();
	if(!buff.print( "%s", "_cert.pem" )) return FAIL_FAILED();
	return true;
}

bool MYSITE_loader_files::
print_filename_cert_txt( buffer2 & buff, SITE_X509_tag_enum tag )
{
	if(!scheme) return FAIL("NULL scheme");
	if(!buff.print( "%s", (STR0) pfx_dir )) return FAIL_FAILED();
	if(!scheme->print_part_name( buff, tag )) return FAIL_FAILED();
	if(!buff.print( "%s", "_cert.txt" )) return FAIL_FAILED();
	return true;
}

bool MYSITE_loader_files::
print_filename_key_pem( buffer2 & buff, SITE_X509_tag_enum tag )
{
	if(!scheme) return FAIL("NULL scheme");
	if(!buff.print( "%s", (STR0) pfx_dir )) return FAIL_FAILED();
	if(!scheme->print_part_name( buff, tag )) return FAIL_FAILED();
	if(!buff.print( "%s", "_key.pem" )) return FAIL_FAILED();
	return true;
}

bool MYSITE_loader_files::
load_cert_from_file( SITE_X509_tag_enum tag )
{
	if(!loaded) return FAIL("NULL loaded");
	buffer2 filename_cert;
	if(!print_filename_cert_pem( filename_cert, tag )) return FAIL_FAILED();

	X509_cert & C = loaded->get_cert_var( tag );
        if(!C.PEM_from_file( (STR0) filename_cert )) return FAIL_FAILED();

	return PASS("LOADED %s", (STR0) filename_cert );;
	return true;
}


/*!
	I presume that the CERT was saved with only the public key in it,
	and that the private key is in a separate file (a pair)
	and that the cb_phrase opens the key, opens the cert,

	BUT the cb_phrase is also offered to the CERT ... errm ...

	NOTE that we need the cb_phrase, to read the KEY,
	and reuse the same cb_phrase for the CERT, so its all good

	NOTE derived class (here) must set self_tag and self_key
*/
bool MYSITE_loader_files::
load_cert_and_key_from_file( SITE_X509_tag_enum tag, CB_get_phrase_base * cb_phrase )
{
	// ########## // save_self_cert_and_key_into_file();
	// ########## // load_cert_and_key_from_file( tag, cb_phrase )

	buffer2 filename_cert;
	if(!print_filename_cert_pem( filename_cert, tag )) return FAIL_FAILED();

	buffer2 filename_key;
	if(!print_filename_key_pem( filename_key, tag )) return FAIL_FAILED();

	if(!loaded) return FAIL("NULL loaded");

	loaded->self_tag = tag;
	loaded->self_key = new EVP_P_KEY( cb_phrase ); // PHRASE NEEDED

	if(!loaded->self_key -> read_PrivateKey( (STR0) filename_key )) return FAIL_FAILED();

	X509_cert & C = loaded->get_cert_var( tag );
#warning cb_phrase IS used to get PEM_from_file
        if(!C.PEM_from_file( cb_phrase, (STR0) filename_cert )) return FAIL_FAILED();

	return PASS("LOADED %s", (STR0) filename_cert );;
	return true;
}


bool MYSITE_loader_files::
save_cert_into_file( SITE_X509_tag_enum tag )
{
	/*
		no cb_phrase is required, saving the PUBLIC side of things
	*/
	if(!loaded) return FAIL("NULL loaded");
	buffer2 filename_cert;
	if(!print_filename_cert_pem( filename_cert, tag )) return FAIL_FAILED();

	X509_cert & C = loaded->get_cert_var( tag );
        if(!C.PEM_into_file( (STR0) filename_cert )) return FAIL_FAILED();

	return PASS("LOADED %s", (STR0) filename_cert );;
	return true;
}

bool MYSITE_loader_files:: save_cert( SITE_X509_tag_enum tag )
{
	if(!loaded) return FAIL("NULL loaded");
	if( tag == loaded->self_tag ) {
			WARN("tag == loaded->self_tag - should also save key ??");
	}
	buffer2 filename_cert;
	if(!print_filename_cert_pem( filename_cert, tag )) return FAIL_FAILED();

	X509_cert & C = loaded->get_cert_var( tag );

        if(!C.PEM_into_file( (STR0) filename_cert )) return FAIL_FAILED();

	return PASS("SAVED %s", (STR0) filename_cert );;
	return true;
};

bool MYSITE_loader_files:: save_self_cert_and_key()
{
	return save_self_cert_and_key_into_file();
};

bool MYSITE_loader_files::
save_self_cert_and_key_into_file()
{
	/*
//	CB_get_phrase_base * cb_phrase = K->cb_pass;
		I do need the cb_phrase,
		it is used to store the KEY (not the public cert)
		but it must already be IN THE KEY
		as needed by the K-> funcs
	*/
	if(!loaded) return FAIL("NULL loaded");
	SITE_X509_tag_enum tag = loaded->self_tag;
	buffer2 filename_cert;
	buffer2 filename_key;
	if(!print_filename_cert_pem( filename_cert, tag )) return FAIL_FAILED();
	if(!print_filename_key_pem( filename_key, tag )) return FAIL_FAILED();

	obj_hold<EVP_P_KEY> K = loaded->self_key;
	X509_cert & C = loaded->get_cert_var( tag );

        if(!C.PEM_into_file( (STR0) filename_cert )) return FAIL_FAILED();
	if(!K->write_PrivateKey( (STR0) filename_key )) return FAIL_FAILED();

	return PASS("SAVED %s", (STR0) filename_cert );;
	return true;
}

// virtual
bool MYSITE_loader_files:: load_cert( SITE_X509_tag_enum tag )
{
	return load_cert_from_file( tag );
}
// virtual
bool MYSITE_loader_files:: load_self_cert_and_key( SITE_X509_tag_enum tag, CB_get_phrase_base * cb_phrase )
{
	return load_cert_and_key_from_file( tag, cb_phrase );
}


bool MYSITE_loader_files:: save_key_second_as_now_encrypted()
{
	EVP_P_KEY * KEY = loaded->self_key;
	if(!KEY) return FAIL("NULL KEY");
	if(!KEY->KEY_was_crypted) {
	}
		buffer2 filename_C_me_key;
		print_filename_key_pem( filename_C_me_key, is_C_pc );
		buffer2 filename_enc_buf;
		filename_enc_buf.print("%s_encrypted", (STR0) filename_C_me_key );
	if(!KEY->write_PrivateKey( (STR0) filename_enc_buf )) {
		INFO("THAT FAILED");
	}
	return PASS("DONE");
}
