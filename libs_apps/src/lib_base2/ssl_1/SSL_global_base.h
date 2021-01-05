#ifndef SSL_global_base_H
#define SSL_global_base_H

#include "blk1.h"
#include "obj_ref.h"
#include "SSL_stubs.h"
#include "buffer2.h"
#include "evp_md_calculator.h"

/*!
	one connection (but without the fd connection)

	I dont have time to do this today
	The need, comes from X509_STORE_CTX
	which returns a pointer to an SSL
	which _CAN_ be asked to hold an object such as self (fd_sel_ssl*)
*/
class SSL_one : public obj_ref
{
 public:
	SSL * ssl;
};

/*!
	You can have several ssl_ctx, but why

	You must set options and cipher list BEFORE creating a new SSL

	Maybe that means a class that knows that it will be a server or client
*/
class SSL_global_base : public obj_ref
{
 public:
	SSL_CTX * ssl_ctx;

	// now opaque field
	X509_STORE * get_cert_store();
	// { return SSL_CTX_get_cert_store(ssl_ctx); }

	// put inside for namespace reasons
	static int get_idx_of_SSL_self();
	static bool set_SSL_self( SSL * ssl, obj_ref * self );
	static obj_ref * get_SSL_self( SSL * ssl );
	static SSL * get_ssl_of_X509_STORE_CTX( X509_STORE_CTX * ctx );
	static obj_ref * get_SSL_self_from_X509_STORE_CTX( X509_STORE_CTX * ctx );
	static fd_sel_ssl * get_fd_self_from_X509_STORE_CTX( X509_STORE_CTX * ctx );
	////

	str1 seed_file_name_kept;

	bool have_cert;

	SSL_global_base();

	~SSL_global_base();

	virtual bool initialise_SSL_subsystem();
	virtual void RAND_on_program_load_0(); // leave this
	virtual void RAND_on_program_load_1(); // write this
	virtual const char * seed_file_name( const char * filename = NULL );
	virtual bool seed_file_write( const char * filename = NULL );
	virtual bool seed_file_read( const char * filename = NULL );
	// ifdef FEED_entropy_gatherer //
	// collect entropy from network
	// munge again and pass to /dev/random_gatherer
	// no need, thats /dev/random's problem
	virtual bool RAND_from_local_noise( p0p2 blk_of_noise );
	// subclass adds pointer to roller over seed_file
	// which xors the blk_of_noise over the next part of seed file
	// probably after buffering 1K and folding it down to 256 with 64 random
	// plus insert random jumps around
	// then crypting with a random key which is erased (lo grade)
	// then rolling with self to make it harder to predict
	// you can add the link to send it to EGD but only after masking
	// BEWARE of tracing DLL calls that carry simple crc's of passwords
	// or blocks of memory that contain data, even free() gives it away
	// ALSO w_entry_pass -> set_text_ZAP();

	// LIBS_APPS GUI can help (with base2 ssl is probably laoded)
	// RAND_row() adds ticks to entropy
	// C_event_clicked() calls RAND_now() and also event*
	// C_event_configure() and others

 private:
	/*!
		a new SSL for this connection
	*/
	SSL * SSL_new(obj_ref * self);
	bool priv_is_configured;
 public:
	SSL * SSL_new_client(obj_ref * self);
	SSL * SSL_new_server(obj_ref * self);

	virtual bool class_is_usually_client();
	bool class_is_usually_server() { return !class_is_usually_client(); }

	virtual bool set_my_client_SSL_OP_options();
	virtual bool set_my_server_SSL_OP_options();
	virtual bool set_dh_parameters();
	virtual bool set_ssl_verification();
	virtual bool configure_ssl_global_parameters();
	bool check_configured_fn();

	bool set_cipher_list( const char * list = NULL );

	bool DEL_use_key_cert_files(
		const char * key_filename,
		const char * cert_filename
	);

	bool set_own_cert( X509_cert & cert );
	bool set_own_key( EVP_P_KEY * key );
	bool set_own_cert_key( X509_cert & cert, EVP_P_KEY * key );
	bool set_own_certificate_chain(
		X509_cert & cert,
		EVP_P_KEY & pass,
		CB_get_phrase_base & obj_pass,
		X509_cert & CA_a, // issuer or own cert
		X509_cert & CA_b, // CA_a was created by ...
		X509_cert & CA_c, // NULL means OK
		X509_cert & CA_d // NULL means OK easier than a list ?
	);

	bool add_CA_to_own_chain( X509 * CA_step ); 
	bool add_CA_to_own_chain( X509_cert & CA_step ); // use_up / TAKE

	bool load_remote_client_CA_list( const char * filename );
	bool add_remote_client_CA_name( X509 * ca_cert );
	bool add_remote_client_CA_name( X509_cert & ca_cert ); // use_same PEEK

	// fail if passphrase needed
	void set_default_passwd_cb_NONE();
	void set_default_passwd_cb(CB_get_phrase_base & uobj);

	bool load_verify_locations( const char * CA_file, const char * CA_dir );

// TODO // X509_STORE add CERT to trusted CA list - but not from file
// TODO // surround verify stack with extra ? - or set vars - or laters

 	X509_LOOKUP *x509_lookup;
	bool add_trusted_CA( X509_cert & ca );

	bool CTX_set_mode( long mode);
	bool ENABLE_PARTIAL_WRITE();
	bool ACCEPT_MOVING_WRITE_BUFFER();
};


// THESE ARE OUTSIDE OF ANY CLASS

// void RAND_bytes( const void * buf, int nbytes );
void RAND_add_nn_bytes( int nn, const void * buf );
void RAND_add_blk( blk1 & blk );
void RAND_add_int( int t );
void RAND_add_int_plus( int t ); // plus stack address, time, ... etc
void RAND_add_str( const char * s );

bool RAND_get_nn_bytes( int nn, u8 * buffer );
bool RAND_get_nn_into_blk( int nn, blk1 & blk );
bool RAND_get_nn_bytes_of_random( int nn, blk1 & blk );
bool RAND_get_32_base64( buffer2 & buf );
bool call_RAND_load_file( const char * filename, int k_max );

// http://www.openssl.org/docs/crypto/X509_STORE_CTX_new.html
// SSL_global_base * self_from_ctx( 

/*
	Shredder must have at least one go at leaking md5s
	eg fold into circ_buffer xor with intentional jumps
	eg crypt using rolling seed_file

	void RAND.write_RAND_bytes( const void * buf, int nbytes );
	void RAND.write_RAND_blk( blk1 & blk );
	void RAND.write_RAND_int( int t );

	void pre_RAND_bytes( const void * buf, int nbytes );
	void pre_RAND_blk( blk1 & blk );
	void pre_RAND_int( int t );
*/
#endif
