
#include <openssl/err.h>
#include <openssl/ssl.h>
#include "SSL_global_base.h"
#include "dgb_fail.h"
#include <openssl/rand.h>
#include "CB_get_phrase.h"
#include "X509_cert.h"
#include "EVP_P_KEY.h"
#include "str_base64.h"
#include "fd_hold.h" // CHECK_can_write_file(name)
#include "err_int.h" // get_SSL_error() - should be here maybe ?

#ifndef WIN32
extern "C" int getpid();
extern "C" int getppid();
#endif

/*
	the server has these non-secret DH parameters
	which can be unchanged for years
	you create them with openssl
	If missing, the program makes them for you (but does not chown/chmod)
*/
#define filename_DH_512 "keys/dh512.pem"
#define filename_DH_1024 "keys/dh1024.pem"

/*
	the client does not have a lot of entropy to play with
	so some randoms are placed into files,
	and requested from the server.

	It might be possible to feed values to it,
	but at least we default to each client being unique

	TODO remember to write the file(s) every hour / day
	TODO remember to call RAND_add_int( t )
	TODO build extra entropy gathering into base + base2
*/
#define filename_prng_seed "keys/prng_seed.dat"

// RUMOUR: pad data to 32 bytes, add 20 HMAC, add 5 SSL-hdr
// 32 * 48 = 1536, +25 > 1548
// 32 * 47 = 1504, +25 1529, +19==1548
// 32 * 46 = 1472, +25 1497, +51==1548
// 32 * 32 = 1024

// RUMOUR: mobiles have IP MTU 1500 making 1472 the payload
// 32*45 = 1440 +25 +7gap +28mob-proto?
//
// thats one per minute of the day


// FAIL(...) FAIL(...) will call this to check-for, output and delete
// SSL errors. Probably _THIS_ thread only

/*!
 WAS:
	Print out and forget the error messages from SSL ...
 NOW:
	Integrate with err_int and FAIL is a new subtle way
	(Ie I dont understand it either - convoluted ... maybe )

	The app and or libr _SHOULD_ know that a subsystem worked or failed,
	and call the correct get_OS_error(), or get_SSL_error(),
	probably as part of FAIL_SSL("My message");
	THEN return false, to caller, who returns FAIL_FAILED() (false)
	back up, to someone who calls DIALOG_ERROR_BOX("...")
	along with it calling FAIL_DONE(), or something.

	Currently, the lib func calls FAIL("my message") which
	looks everywhere (or not if it already has something).
	That calls do_ERRNO(), which calls the vector (base2/base1)
	which is this function.

	So get the SSL error into a format that is a bit like errno.
	Maybe PRINT and forget the extra details, just get the core fact back.

*/
void C_SSL_ERR()
{
	err_int_t & err_int = BASE1::get_err_int_for_this_thread();
	err_int.get_SSL_error();
	return;


	int err;
	while((err=ERR_get_error()))
	{
		char buf[1024];
		ERR_error_string_n(err,buf,sizeof buf);
		FAIL( "# %d # %s", (int) err, (const char *) buf );
		// BUT ERROR IS LOST - squirrel it away somewhere!
	}

	// NB ERR_get_error() -is-not- SSL_get_error( ctx, ret ) // OK
}


X509_STORE * SSL_global_base::get_cert_store() {
	return SSL_CTX_get_cert_store(ssl_ctx);
}

bool SSL_global_base::initialise_SSL_subsystem()
{
	errno_zero();
	static bool done_once = false;
	if( done_once ) {
		// (simply INIT early, from solo thread)
		// TODO stay spinning whilst other thread initialises module
		// then return with no extra action
		return true; // but dont call at same time!
	}
	done_once = true;

	SSL_load_error_strings();
	OpenSSL_add_ssl_algorithms();
	SSL_library_init(); // where was this

	// lib_base1 FAIL checks for SSL errors, but isnt linked with SSL!
	// lib_base2 registers a callback for that
	//
	set_dgb_ssl_err( C_SSL_ERR );

	// This is a minimalistic initialisation of RAND
	// each app can add _regular_ data, eg time now
	// maybe even CRCs of messages, but the initial randomness matters
	// for the generation of the master secret and other
	// Then write out the random file every once in a while (hourly)
	// and hope its half good for next time
	// eg get some random things from server
	//
	RAND_on_program_load_0(); // minimalistic something for all apps
	RAND_on_program_load_1(); // preferably something per app that you want

	return true;
}


SSL_global_base::SSL_global_base()
{
	errno_zero();
	ssl_ctx = NULL;
	have_cert = false;
 	x509_lookup = NULL;
	priv_is_configured = false;

	// the virtual part is a bit pointless
	// when called within a CTOR
	// because the VTABLE is munged to be this class
	// TODO Maybe rewrite to allow multiple calls
	// TODO gather random info from time when GUI buttons clicked
	// build into the GUI system! Also write often so that it is kept
	// but then again, what about a READONLY application ?
	// Put as base64 into the registry? (system without egd)
	//
	initialise_SSL_subsystem();

	// Move to V3/TLS not V2 ... except ...
	WARN("Using TLS_method() - make variable?");
	ssl_ctx = SSL_CTX_new( TLS_method() );
	if(!ssl_ctx)
	{
		FAIL("SSL_CTX_new(TLS_method()");
		// return errno;
	}

	//
	// http://www.openssl.org/docs/ssl/SSL_CTX_set_options.html
	//
	//  defaults

	// When OpenSSL want to decrypt an X509's private key
	// It will call this back, to get the password from ... gui ...
	// Or not get a password, which then fails. Ditto ENCRYPTION
	//
	// NB this->ctx MUST be set 
	// TODO: rewrite as a function that uses THIS as a callback parameter
	//
	// this is for GUI programs too lazy to set their own
	// and too fragile to look in the other window
	//
	//
	set_default_passwd_cb_NONE();
	//
	// Each connection will have to set its own MGMT ??
	// SSL_CTX_set_verify( ssl_ctx, SSL_VERIFY_PEER, NULL );

}

SSL_global_base::~SSL_global_base()
{
	if( ssl_ctx ) SSL_CTX_free( ssl_ctx );
	ssl_ctx = NULL;
}

// SSL_global_base:: 
bool SSL_global_base:: check_configured_fn()
{
	if(priv_is_configured) return true;
	priv_is_configured = configure_ssl_global_parameters();
	if(!priv_is_configured) FAIL("configure_ssl_global_parameters()");
	return priv_is_configured;
	/*
		this needs to be split to allow SSL without any CERTS
	*/
}

bool SSL_global_base:: class_is_usually_client()
{
	FAIL("You should COPY and replace this default function");
	return true; 
}

bool SSL_global_base:: set_ssl_verification()
{
	WARN("You should COPY and replace this default function");

	FAIL("I dont like using files, but you are using them");
	if(!SSL_CTX_set_default_verify_paths( ssl_ctx ))
		return FAIL("SSL_CTX_set_default_verify_paths( ssl_ctx )");
	return true;
	
}

bool SSL_global_base:: configure_ssl_global_parameters()
{
	if(priv_is_configured) {
		FAIL("Already configured, but doing it again anyway");
	}
	if(class_is_usually_client()){
		if(! set_my_client_SSL_OP_options() ) return FAIL_FAILED();
	} else {
		if(! set_my_server_SSL_OP_options() ) return FAIL_FAILED();
	}
	if(! set_dh_parameters() ) return FAIL_FAILED();
//	ERRM ... now want to manually call this not just at right time
//	ERRM ... but also in right mood, as correct C_self
//	if(! set_ssl_verification() ) return FAIL_FAILED();

	return true;
}

bool SSL_global_base::CTX_set_mode( long mode)
{
	long modenow = SSL_CTX_set_mode( ssl_ctx, mode);
	INFO( "adding mode flag %x to get %x", mode, modenow );
	return true;
}

/*!
	I think this means - allow OpenSSL to pick write boundries
	which will truncate at 16K. The alternative is to request
	exactly what you want (own buffer split into << 1548 chunks,
	leaving room for headers ?) 
*/
bool SSL_global_base::ENABLE_PARTIAL_WRITE()
{
	STEP( "NOW");
	return CTX_set_mode( SSL_MODE_ENABLE_PARTIAL_WRITE );
}

/*!
	OpenSSL is strange - fussy about buffers being static
*/
bool SSL_global_base::ACCEPT_MOVING_WRITE_BUFFER()
{
	STEP( "NOW");
	return CTX_set_mode( SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER );
}

/*!
	Edit the source, if you dont like this ...

SSL_OP_CIPHER_SERVER_PREFERENCE

	When choosing a cipher, use the server's preferences instead
	of the client preferences. When not set, the SSL server will
	always follow the clients preferences. When set, the SSLv3/TLSv1
	server will choose following its own preferences. Because of the
	different protocol, for SSLv2 the server will send its list of
	preferences to the client and the client chooses.

*/
bool SSL_global_base::set_my_server_SSL_OP_options()
{
	long options = SSL_OP_CIPHER_SERVER_PREFERENCE;
 //	options |= SSL_OP_ALL;
 	options |= SSL_OP_NO_SSLv2;
 if(0)	if(!class_is_usually_server())
		options |= SSL_OP_SINGLE_DH_USE;
	options |= SSL_OP_SINGLE_DH_USE;	// both ends !
	long all_options __attribute__((unused))
	= SSL_CTX_set_options( ssl_ctx, options);
 // or // SSL_set_options(SSL_CTX *ctx, long options);
	return TRUE; // if it compiles it doesnt fail
}

bool SSL_global_base::set_my_client_SSL_OP_options()
{
	return set_my_server_SSL_OP_options();
}

bool SSL_global_base:: set_cipher_list( const char * list )
{
	if(!list) {
		STEP("list == NULL - using preset" );
		list = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH";
	}
	INFO("%s", list );
	if(!SSL_CTX_set_cipher_list( ssl_ctx, list )){
		return FAIL("SSL_CTX_set_cipher_list() failed");
	}
	return true;
}

DH * load_dh_file( const char * filename, int keylen )
{
	DH * ret = NULL;
	BIO * bio = BIO_new_file( filename, "r" );
	INFO("(%s,%d)", filename, keylen);
	if(!bio) {
		FAIL("server is missing DH parameters file %s", filename );
		buffer2 cmd;
		cmd.print("openssl dhparam -check -text -5 %d -out %s", keylen, filename );
		WARN("Running %s", (STR0) cmd );
		system( (STR0) cmd );
		bio = BIO_new_file( filename, "r" );
		if(!bio) {
			FAIL("STILL ABSENT %s", filename );
			return NULL;
		}
	}
	ret = PEM_read_bio_DHparams(bio,NULL,NULL,NULL);
	if( !ret ) {
		FAIL("ERROR reading file '%s'", filename );
	}
	BIO_free( bio );
	return ret;
}

DH * CB_tmp_dh_callback( SSL * ssl, int es_export, int keylen )
{
	DH * ret = NULL;
	static DH * dh_512 = NULL;
	static DH * dh_1024 = NULL;
	INFO("callback(ssl,is_export,keylen=%d)", keylen);
	switch(keylen) {
	 case 512:
		if(!dh_512) {
			dh_512 = load_dh_file( filename_DH_512, keylen );
		}
		ret = dh_512;
	 break;
	 case 1024:
	 default:
		if(!dh_1024) {
			dh_1024 = load_dh_file( filename_DH_1024, keylen );
		}
		ret = dh_1024;
	 break;
	}
	if(!ret) FAIL("returning null DH");
	return ret;
}

// virtual
bool SSL_global_base::set_dh_parameters()
{
	STEP("setting callback");
	SSL_CTX_set_tmp_dh_callback( ssl_ctx, CB_tmp_dh_callback );
	//	return FAIL("SSL_CTX_set_tmp_dh_callback() failed");
	return true;
}

// TODO // move this to another class(es)

// static 
int SSL_global_base::get_idx_of_SSL_self()
{
	static int idx = -1;
	if( idx<0 ) { // 0 is the first user defined idx
	// http://www.openssl.org/docs/ssl/SSL_CTX_set_verify.html
	// http://www.openssl.org/docs/crypto/CRYPTO_set_ex_data.html
	// http://www.openssl.org/docs/ssl/SSL_get_ex_data_X509_STORE_CTX_idx.html
	// http://www.openssl.org/docs/crypto/RSA_get_ex_new_index.html
		idx = SSL_get_ex_new_index(
			0,	// MY LONG,
			NULL,	// MY DATA POINTER (name of the idx)
			NULL,	// new_func
			NULL,	// dup_func
			NULL	// free_func
		);
		if(idx<0) {
			FAIL("SSL_get_ex_new_index() ran out of something");
			THROW_dgb_fail("SSL_get_ex_new_index() must not fail");
		}
		INFO("idx = %d", idx );
	}
	return idx;
}

bool SSL_global_base::set_SSL_self( SSL * ssl, obj_ref * self )
{
	int idx = get_idx_of_SSL_self();
	if(idx<0) { return FAIL("HERE"); }
	INFO("setting SSL data at idx = %d", idx ); // log once per fd
	int t = SSL_set_ex_data( ssl, idx, self );
	if(t==1) {
		return true;
	}
	return FAIL("SSL_set_ex_data() failed");
}

obj_ref * SSL_global_base::get_SSL_self( SSL * ssl )
{
	int idx = get_idx_of_SSL_self();
	if(idx<0) { FAIL("HERE"); return NULL; } // 
	if(!ssl) { FAIL("NULL ssl"); return NULL; } // 
	obj_ref * self = (obj_ref*) SSL_get_ex_data(ssl, idx);
	if(!self) {
		FAIL("CODE ERROR didnt set self");
		// THROW_dgb_fail("get_SSL_self() must not fail"); // can
	}
	return self;
}

SSL * SSL_global_base::get_ssl_of_X509_STORE_CTX( X509_STORE_CTX * ctx )
{
	if(!ctx) {
		FAIL("NULL ctx");
		return NULL;
	}
	int idx = SSL_get_ex_data_X509_STORE_CTX_idx();
	if( idx<0 ) {
		FAIL("BAD idx");
		return NULL;
	}
	SSL * ssl = (SSL*) X509_STORE_CTX_get_ex_data( ctx, idx );
	if(!ssl)
		FAIL("NULL ssl");
	return ssl;
}

obj_ref * SSL_global_base::get_SSL_self_from_X509_STORE_CTX( X509_STORE_CTX * ctx )
{
	SSL * ssl = get_ssl_of_X509_STORE_CTX( ctx );
	obj_ref * obj = get_SSL_self( ssl );
	return obj;
}

fd_sel_ssl * SSL_global_base::get_fd_self_from_X509_STORE_CTX( X509_STORE_CTX * ctx )
{
	return (fd_sel_ssl*) get_SSL_self_from_X509_STORE_CTX( ctx );
}


/*!
	a new SSL for a connection "self"

	NB the returned SSL* will soon be owned by an fd_sel_ssl
	which will also set SELF on it (with/out a ref count!)
	-- actually we set SELF now --
*/
SSL * SSL_global_base::SSL_new(obj_ref * self)
{
	// last chance to set SSL_Global options before forking instance
	if(!check_configured_fn()) return (SSL*) NULL_dgb_fail("not good enough to proceed");

	// make this a PRIVATE function, only called by ...
	// FAIL("COMMON USE - neither client nor server YET set");
	// an SSL connection structure
	SSL * ssl = ::SSL_new( ssl_ctx );
	if(!ssl)
	{
		throw "SSL_new(ssl_ctx);"; // dgb_fail ...
	}

	// nb 'self' is inevitably fd_sel_ssl, a connections with an SSL
	// http://www.openssl.org/docs/ssl/SSL_CTX_set_verify.html#
	// it needs this
	//
	set_SSL_self( ssl, self );

	return ssl;
}

/*!
	a new SSL for a new connection
*/
SSL * SSL_global_base::SSL_new_client(obj_ref * self)
{
	// an SSL connection structure
	SSL * ssl = SSL_new(self);
	if(!ssl)
	{
		FAIL("HERE");
		return NULL;
	}
	STEP("SSL_set_connect_state() -- Setting as client");
	SSL_set_connect_state( ssl );
	return ssl;
}

/*!
	a new SSL for this incoming connection
*/
SSL * SSL_global_base::SSL_new_server(obj_ref * self)
{
	// an SSL connection structure
	SSL * ssl = SSL_new(self);
	if(!ssl)
	{
		FAIL("HERE");
		return NULL;
	}
	STEP("SSL_set_accept_state() -- Setting as server");
	SSL_set_accept_state( ssl );
	return ssl;
}

/*!
	Do not call this - instead call set_own_certificate_chain(... args ...)
*/
bool SSL_global_base::DEL_use_key_cert_files(
	const char * key_filename,
	const char * cert_filename
)
{
	if(have_cert) {
		WARN("already have cert");
		have_cert = false;
	}
	// actually cert must be loaded before key
	// because key is checked against it?
	int isok1; // 0 = error 1 = OK

	INFO("cert file %s", cert_filename );
	isok1 = SSL_CTX_use_certificate_file( ssl_ctx, cert_filename, SSL_FILETYPE_PEM );
	if( !isok1  )
	{
		FAIL("SSL_CTX_use_certificate_file %s", cert_filename );
		return false;
	}

	INFO("key file %s", key_filename );
	isok1 = SSL_CTX_use_RSAPrivateKey_file( ssl_ctx, key_filename, SSL_FILETYPE_PEM );
	if( !isok1 )
	{
		FAIL("SSL_CTX_use_RSAPrivateKey_file %s", key_filename );
		return false;
	}

// !!!	X509 * cert  = SSL_CTX_get_certificate(ssl_ctx);
	INFO ("SSL_CTX_check_private_key()");
	bool ok = SSL_CTX_check_private_key(ssl_ctx);
	if(!ok) return FAIL("SSL_CTX_check_private_key()");
	have_cert = true;;
	return true;
}

static int C_default_passwd_cb_NONE( char *buf, int num, int w, void *arg )
{
	WARN("returning -1");
	return -1;
}

void SSL_global_base::set_default_passwd_cb_NONE()
{
	SSL_CTX_set_default_passwd_cb_userdata(ssl_ctx, NULL );
	SSL_CTX_set_default_passwd_cb(ssl_ctx, C_default_passwd_cb_NONE);
}

/*!
#warning no reference counting on uobj OK
*/
void SSL_global_base::set_default_passwd_cb(CB_get_phrase_base & uobj)
{
   SSL_CTX_set_default_passwd_cb(ssl_ctx, CB_get_phrase_base::C_get_passwd_fn );
   SSL_CTX_set_default_passwd_cb_userdata(ssl_ctx, & uobj);
}

/*!
	used when checking the remote PEER's certificates + CA
*/
bool SSL_global_base::load_verify_locations( const char * CA_file, const char * CA_dir )
{
        if(SSL_CTX_load_verify_locations( ssl_ctx, CA_file, CA_dir )) {
		INFO("OK CA_file = %s CA_dir = %s", CA_file, CA_dir );
		return true;
	} else {
		return FAIL("OK CA_file = %s CA_dir = %s", CA_file, CA_dir );
	}
}

/*!
	you call add_certificate(me_x509) - set own certificate (other fn)
	you call add_CA_to_own_chain(my_CA_x509)
	you call add_CA_to_own_chain(CA_of_my_CA_x509)

	NB then no need to call load verify locations - for ME
*/
bool SSL_global_base::add_CA_to_own_chain( X509 * CA_step )
{
	if(!CA_step) return true; // not an error
	if(!SSL_CTX_add_extra_chain_cert( ssl_ctx, CA_step ) ) {
		return FAIL("SSL_CTX_add_extra_chain_cert failed");
	}
	return true;
	// return PASS("SSL_CTX_add_extra_chain_cert() = OK");
}

/*!
	what if it fails ???
*/
bool SSL_global_base:: add_CA_to_own_chain( X509_cert & CA_step )
{
	if(!CA_step) return true; // not an error
	if(!CA_step.cert) return true; // not an error
	INFO("STEP CN = %s", CA_step.get_str_CN());
	return add_CA_to_own_chain( CA_step.cert );
}

bool SSL_global_base:: set_own_cert( X509_cert & cert )
{
	if(have_cert) {
		WARN("already have cert");
		have_cert = false;
	}

	// http://www.openssl.org/docs/ssl/SSL_CTX_use_certificate.html
	//
	INFO("!SSL_CTX_use_certificate() SELF CN = %s", cert.get_str_CN());
	if(!SSL_CTX_use_certificate( ssl_ctx, cert )) {
		cert.show_some_info();
		return FAIL("SSL_CTX_use_certificate()");
	}
	have_cert = true;;
	return true;
}

bool SSL_global_base:: set_own_key( EVP_P_KEY * key )
{
	set_default_passwd_cb(*key->cb_pass); // if it wants to be NULL ..
	if(!SSL_CTX_use_PrivateKey( ssl_ctx, key->pkey ))
	{
		return FAIL("SSL_CTX_use_RSAPrivateKey()");
	}
#warning maybe this should be blk1 with an empty key - this ASKS on TTY!
	set_default_passwd_cb_NONE(); // was either used or not
	return true;
}

bool SSL_global_base:: set_own_cert_key( X509_cert & cert, EVP_P_KEY * key )
{
	if(!set_own_cert( cert )) return FAIL_FAILED();
	if(!set_own_key( key )) return FAIL_FAILED();
	return true;
}

bool SSL_global_base:: set_own_certificate_chain(
	X509_cert & cert,
	EVP_P_KEY & pass,
	CB_get_phrase_base & obj_pass,
	X509_cert & CA_a, // issuer or own cert
	X509_cert & CA_b, // CA_a was created by ...
	X509_cert & CA_c, // NULL means OK
	X509_cert & CA_d  // NULL means OK easier than a list ?
)
{
	return FAIL("DEPRECATED");
	if(!set_own_cert_key( cert, & pass )) return FAIL_FAILED();

	// instead of load_verify_locations, do this ...
	// that only does the sent to remote and request remote to ..
	if(!add_CA_to_own_chain( CA_a )) return FAIL_FAILED();
	if(!add_CA_to_own_chain( CA_b )) return FAIL_FAILED();
	if(!add_CA_to_own_chain( CA_c )) return FAIL_FAILED();
	if(!add_CA_to_own_chain( CA_d )) return FAIL_FAILED();
	STEP("DONE Setting own ID");
	return true;
	// now call set client_CA_list (from list from outdated PEM)
	// now call set verify peer
}

bool absorb_SSL_error_reason( int e_r )
{
// bool absorb_SSL_error( X509_R_CERT_ALREADY_IN_HASH_TABLE )
 //	http://www.openssl.org/docs/crypto/ERR_get_error.html
 //	http://www.openssl.org/docs/crypto/ERR_error_string.html
 //	http://www.openssl.org/docs/crypto/ERR_GET_LIB.html
 //	http://www.openssl.org/docs/crypto/err.html

	unsigned long e3 = ERR_peek_error();
 //	int e_lib = ERR_GET_LIB(e3);
 //	int e_func = ERR_GET_FUNC(e3);
 	int e_reason = ERR_GET_REASON(e3);

	if( e_r == e_reason ) {
	//	const char * s_lib   =  ERR_lib_error_string(e3);
	//	const char * s_func   = ERR_func_error_string(e3);
		const char * s_reason = ERR_reason_error_string(e3);
		e3 = ERR_get_error();
		// NB you must remove error before calling INFO
		// because it snoops around and find it!
		INFO("OK: %s", s_reason );
		return true;
	} else
		return false;
}

bool SSL_global_base::add_trusted_CA( X509_cert & ca )
{
	if(!ca) {
		WARN("NULL ca->cert");
		return true; // OK enough
	}
	buffer1 buf;
	INFO("%s", ca.get_str_CN(buf));
	WARN("testme");
	if(!X509_STORE_add_cert( get_cert_store(), ca ))
		return FAIL("X509_STORE_add_cert() failed");
	return true;

#if 0
	// opaque changed this
	// maybe above 1 liner added - makes following pointless
	// x509_lookup: file or dir of hashed files (not mem)
	// ref-count on ca ?
	//

	if(!x509_lookup) {
		// method: "Load file into cache"
		x509_lookup = X509_STORE_add_lookup(
			get_cert_store(),
			X509_LOOKUP_file() // TYPE file but ... good enough
		);
	}
//	if(!x509_lookup->method) (return FAIL("NULL x509_lookup->method");
//	method == x509_lookup_file
//	if(!x509_lookup->method->ctrl) (return FAIL("NULL x509_lookup->method->ctrl");
// by_file.c :94 by_file_ctrl

	if(!X509_STORE_add_cert(
		x509_lookup->store_ctx,
		ca.get_so_dont_delete()	// x509_lu.c 411 will delete the X509 * cert
	)) {
		// it is not so bad if:
		// cert already in hash table
		if( absorb_SSL_error_reason( X509_R_CERT_ALREADY_IN_HASH_TABLE ) ) {
			return true;
		}
		return FAIL("X509_STORE_add_cert() failed");
	}
#endif
	return true;

	// see also int X509_STORE_add_crl(X509_STORE *ctx, X509_CRL *x)

}


bool SSL_global_base::load_remote_client_CA_list( const char * filename )
{
	STACK_OF(X509_NAME) *cert_names = NULL;
	cert_names = SSL_load_client_CA_file(filename);
	if(!cert_names) {
		return FAIL("did not load %s", filename );
	}
	SSL_CTX_set_client_CA_list( ssl_ctx, cert_names );
	/* SSL_CTX_set_client_CA_list does not have a return value, so ... */
	if (SSL_CTX_get_client_CA_list(ssl_ctx) == NULL) {
		return FAIL("NULL list from SSL_CTX_get_client_CA_list()");
	}
	return true;
}

bool SSL_global_base::add_remote_client_CA_name( X509 * ca_cert )
{
	// ssl/ssl_cert.c 653 b 679
	if(!SSL_CTX_add_client_CA( ssl_ctx, ca_cert ))
	{
		return FAIL("SSL_CTX_add_client_CA() failed");
	}
	return true;
}

bool SSL_global_base::add_remote_client_CA_name( X509_cert & ca_cert )
{
	return add_remote_client_CA_name( ca_cert.get_peek() );
	
}

///// RANDOM STUFF

// #define filename_prng_seed "prng_seed.dat"

// virtual
bool SSL_global_base::seed_file_write( const char * filename )
{
	bool ok = true;
	filename = seed_file_name( filename );
	INFO( "# RAND_write_file( '%s' ) - 1024 bytes is default, valgrind complains", filename );
	ok = ok && RAND_write_file( filename ); // write PRNG to disk for later
	if(ok)  {
		dgb_sleep_less(); // I am not mad
	} else {
		FAIL( "# RAND_write_file( '%s' )", filename );
		// ERR_print_errors_fp(stderr);
	}
	return ok;
}


// virtual 
const char * SSL_global_base:: seed_file_name( const char * filename )
{
	bool changed = false;
	if( filename ) {
		if( seed_file_name_kept == filename ) {
			changed = false;
			// already happy
		} else {
			seed_file_name_kept = filename;
			changed = true;
			if(!CHECK_can_write_file( seed_file_name_kept )) {
				FAIL("CANNOT WRITE SEED FILE");
				// but keep name anyway!
			}
		}
	} else if(!seed_file_name_kept) {
		// try builtin defaults
		seed_file_name_kept =  "keys/prng_seed.dat";
		if(!CHECK_can_write_file( seed_file_name_kept )) {
			seed_file_name_kept =  "prng_seed.dat";
			if(!CHECK_can_write_file( seed_file_name_kept )) {
				FAIL("CANNOT WRITE SEED FILE");
				// but stay to keep trying! + failing!
			}
		}
		changed = true;
	} else {
		changed = false;
		// already happy
	}
	if( changed ) {
		call_RAND_load_file( seed_file_name_kept, -1 );
	}
	return seed_file_name_kept;
}

// virtual
bool SSL_global_base::seed_file_read( const char * filename )
{
	errno_zero();
	bool ok = true;
	filename = seed_file_name( filename );
	ok = ok && call_RAND_load_file( filename, 99 ); // load all / NEVER A FIFO

	return ok;
}

// virtual
bool SSL_global_base::RAND_from_local_noise( p0p2 blk_of_noise )
{
	// add data to entropy
	// TODO but first apply some watering down
	// so that it is not predictable
	// deducable, what data was,
	// or broadcasting KEYS + MD's over tracable function calls
	//
	// But then again, if you have usurped the DLL, or FN call 
	// you already have the KEYS at the most direct point
	//
	int len = blk_of_noise.blk_len(); 
	RAND_add_nn_bytes( len, blk_of_noise.p0 );
	return false;
}

// virtual 
void SSL_global_base::RAND_on_program_load_0()
{
	// gdb_invoke(true);
	seed_file_read();
	// put your app specific files here
	call_RAND_load_file("app_init_file.ini", 32 );
	call_RAND_load_file("keys/cfg_two.dat", 32 );
	call_RAND_load_file("cfg_two.dat", 32 );
	errno_zero();

	// contains 20 bytes entropy + 16 bytes allocated random


#ifdef WIN32
	RAND_screen();
#else
//	THIS TAKES AGES - so dont do it !
//	Maybe test for an absent seed_file and then do this
//	call_RAND_load_file( "/dev/random", 256 ); // real entropy is hard to find
	call_RAND_load_file( "/dev/urandom", 2048 ); // OS PRNG is quicker
#endif
	RAND_add_int_plus( 99 );
	seed_file_write();
	RAND_add_int_plus( 101 );
	dgb_sleep_less(); // I am not mad
}

// virtual 
void SSL_global_base::RAND_on_program_load_1()
{
	STEP("Opportunity for UDEF randomness");
}

////////////////////////////////////

// WOW THIS GETS NOT PUTS! // void RAND_bytes( const void * buf, int nbytes )

void RAND_add_nn_bytes( int nbytes, const void * buf )
{
	if(!nbytes) return;
	// add a few bytes of randomness (counting as one and quarter)
	// call often with stack, time, pid,
	// even how many times it is called adds something
	
	RAND_add( buf, nbytes, (double) (nbytes+5)/4.5 );
	INFO("nbytes %d", nbytes );
}

void RAND_add_blk( blk1 & blk )
{
	RAND_add_nn_bytes( blk.nbytes_used, blk.buff );
}

void RAND_add_int( int t )
{
	// add a few bytes of randomness (counting as two)
	// call often with stack, time, pid,
	// even how many times it is called adds something
	
	RAND_add_nn_bytes( sizeof(t), &t );
}
void RAND_add_str( const char * s )
{
	if(!s) {
		RAND_add_int(0);
		return;
	}
	int l = strlen(s);
	RAND_add_nn_bytes( l, s );
}
void RAND_add_int_plus( int t ) // plus stack address, time, ... etc
{

#ifdef WIN32
	// every WIN32 event, you can call:
	// int  RAND_event(UINT iMsg, WPARAM wParam, LPARAM lParam);
	// combined with GetTickCount() for a better client key
	// REPORTED TO BE SLOW ? // RAND_screen();
	struct rand_data_t
	{
		int theint;
		pid_t pid;
		time_t time;
		DWORD ticks;
		void * ptr_to_a_stack_local;
	} rand_data;
	rand_data.theint = t;
	rand_data.time = time(NULL);
	rand_data.ticks = GetTickCount();
	rand_data.ptr_to_a_stack_local = & rand_data;
 //	rand_data.pid = ****
#else
	struct rand_data_t
	{
		int theint;
		pid_t pid;
		pid_t ppid;
		time_t time;
		void * ptr_to_a_stack_local;
	} rand_data;
	rand_data.theint = t;
	rand_data.pid = getpid();
	rand_data.ppid = getppid();
	rand_data.time = time(NULL);
	rand_data.ptr_to_a_stack_local = & rand_data;
#endif
	RAND_add_nn_bytes( sizeof( rand_data ), &rand_data );
}

bool RAND_get_nn_bytes( int nn, u8 * buffer )
{
	int t = RAND_bytes( buffer, nn );
	if( t == -1 ) {
		return FAIL("RAND_bytes() returned -1");
	}
	if( t == 1 ) { return true; }
	if( t == 0 ) {
		return FAIL("RAND_bytes() returned 0");
	}
	return FAIL("RAND_bytes() returned bad value");
	/*
		an alternative is to call RAND_pseudo_bytes()
		which returns 1 if it could find some cryptographically strong
		0 if not
	*/
}

bool RAND_get_nn_into_blk( int nn, blk1 & blk )
{
	if(!blk.get_space( nn ))
		return FAIL("NO SPACE");

	u8 * P = blk.gap_addr();
	if(! RAND_get_nn_bytes( nn, P )) return FAIL_FAILED();
	blk.nbytes_used += nn; // this is naff, even if compat with buffer3
	return true;
}

bool RAND_get_nn_bytes_of_random( int nn, blk1 & blk ) {
	return RAND_get_nn_into_blk( nn, blk );
}

/*!
	WAS == 20 bytes in 28 bytes container
	NOW == 32 bytes in 44+ byte BASE64 string # or TAB $1 LF TAB $2 LF #
	NOW == 64 bytes in 85+ byte BASE64 string # 2 lines ? #  
	OPT == 2 lines (multiline) narrower that 72 // _than_tillroll_width
	GET u64 eight_random_bytes # obviously low RD request u64_WORD
	RAND needs to be in STREAM mode with one TWIST per GET however many N
	so RAND can pre-generate it's chosen block size, eg 4096_bytes
	so RAND can supply streams of individual request for u64_bits // 8 //
	ask PRNG for some random data == H_bytes == u4 // expensive sourcce
	ask PRNG for some random data == 1 bytes == u8
	ask PRNG for some random data == 8 bytes == u64
	ask PRNG for some random data == 32 bytes == 256 bits == u64[4]
	exactly 64 bytes # or N_bytes_32
	smudge it a bit
	sha1 it to smudge it some more (hence 20 bytes)
	print it out as base64 ASCII single word (no surrounding spaces)
	which makes it 25

	This is useful to send the client some random seed.
	It does not have to decode it, just add it to its PRNG.

	It might actually be 16 bytes not 20 OK
	That depends on the use of SHA1, and its only a name, as a hint
*/
bool RAND_get_32_base64( buffer2 & RETVAR ) // caller provides BASE64 storage
{
	// start off with 20 random bytes // from an expensive source
	static const int N20 = 32; // builtin const/var N20 == 20 // edit here
	blk1 RAND_bytes; // OUTPUT BYTES[ N20 ]
	// ## get_space( N20 ) || FAIL FAILED // blk retains its own ALLOC
	// get 20 RAND bytes // into blk1 [&] RAND_bytes
	if(!RAND_get_nn_into_blk( N20, RAND_bytes )) return FAIL_FAILED();

	// SCATTER to 64 bytes of MESSAGE_DIGEST_KEY // proper checksum
	// into md_calc
	md_calculator_base md_calc;
 //	md_calc.init_whirlpool(); // 512 = 64 bytes
 	// sha1 remembered into md_calc
	if(!md_calc.init_sha1()) return FAIL_FAILED(); // 160 bits = 20 bytes
	// virtual call update_from_ RETVAR // _buf == RAND_bytes
	// runs DIGEST over N20 bytes into md_calc
	if(!md_calc.update_from_buf(RAND_bytes)) return FAIL_FAILED(); 
	if(!md_calc.final()) return FAIL_FAILED(); // into its own md

	// PRINT 64 BYTES in 4/3 85.3 bytes one long line raw or 2 lines LF SP
	// from md_calc // 64 binary bytes
	// into RETVAR // RETVAL PARAMETER // 90 base64 bytes
	str_base64 conv64;
	if(!conv64.encode( md_calc.md, RETVAR )) return FAIL_FAILED();
	
	return true;
}


// C // not in class //
bool call_RAND_load_file( const char * filename, int k_max )
{

	if(k_max < 1 ) k_max = 9999;
	int bytes = k_max * 1024;
	bool ok = true;
	ok = ok && RAND_load_file( filename, bytes ); // load all / NEVER A FIFO
	if(ok)  {
		dgb_sleep_less(); // I am not mad
		INFO("filename %s k_max %d", filename, k_max );
	} else {
		// OK // dbg_sleep_more(); // allow optimistic RAND files
		// /etc/motd ~/.profile  
		return FAIL( "# FAIL FROM RAND_load_file( '%s' )", filename );
	}
	return true;
}
