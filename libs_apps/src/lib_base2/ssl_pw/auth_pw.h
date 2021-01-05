#ifndef auth_pw_H
#define auth_pw_H

#include "buffer2.h"
#include "key_holder.h"

/*
	ssl_auth_pw = here = old DES style CRC32's etc

		there is very little 'ssl_' involved,
		this is all stanard DES and what-have-you,
		but SSL is being used as the crypto provider

			auth_pass

	ssl_auth_rsa = elsewhere
	ssl_auth_sign = elsewhere
	ssl_auth_SRP = future here maybe
*/


namespace AUTH {

 class PW_UTIL_CSET {
  public:
	static bool pick_n_random_bytes(
		char * buf,
		int pickn,
		int pooln=0,
		const char * pool = NULL
	);

	static bool pick_n_random_bytes(
		buffer2 & buffer,
		int pickn,
		int pooln,
		const char * pool
	);
 };

 /*!
	PW_UTIL_UNIX [_old] is the old fashioned DES crypt hash
	tht you used to find in /etc/passwd.

	Nowadays, its MD5, or SHA, or SRP, or ... in /etc/shadow or LDAP or ...

	man 3 crypt
 */
 class PW_UTIL_UNIX {
  public:
	/*
		encourage caller to keep pass in base 64
		its good for debugging and not seeing things
	*/
	static bool compare_passenc_pass64(
		const char * crypted_true_pass,	// stored in /etc/shadow
		const char * clearpass64	// attempt in base64
	);

	static bool compare_passenc_pass(
		const char * crypted_true_pass,	// stored in /etc/shadow
		const char * clearpass		// attempt from user
	);

	static bool prepare_passenc_from_clear(
		buffer2 & crypted_password,	// return appended // clear()?
		const char * clearpass		// input pass
	);

	static bool RAND_salt2( char * salt );
	static bool dense_chars_1(const char * * ptr_str );
	static bool easy_chars_2(const char * * ptr_str );
	static bool RAND_pass8_dense( buffer2 & buff );
	static bool RAND_pass8_easy2( buffer2 & buff );
 };

 class vnc_key_holder : public key_holder
 {
  public:
	 vnc_key_holder();
	 ~vnc_key_holder();
	bool set_to_well_known_vnc_key();
	bool set_to_users_pass8( buffer2 & plain );
	bool flip_all_bytes_in_key();
 };

 class PW_UTIL_VNC {
  public:
	static bool RAND_pass8_dense_vnc( buffer2 & buff );
	static bool encrypt_vncpass64( buffer2 & crypt, const char * plain64 );
	static bool encrypt_vncpass( buffer2 & crypt, const char * plain );
	static bool decrypt_vncpass( const char * crypt, buffer2 & plain );
	static bool encrypt_vncpass_TIGHT( buffer2 & crypt, const char * plain );
	static bool hex10_encrypt_vncpass( buffer2 & hex10crypt, const char * plain );

	static bool vncpassfile_read(
		const char * filename,
		buffer2 & pw_vnc_plain
	);

	static bool vncpassfile_write_uid_gid(
		const char * filename,
		const char * pw_vnc_plain,
		// mod is 400
		int uid,
		int gid
	);

 };

};

#endif
