

/*!
	The X509_REQ_util class, is used to set parameters manage a X509 REQ

	The client is the future owner of the X509, and knows the RSA.

	The server issues the X509, and knows its own RSA, to sign this X509,
	but the server does not know the client RSA.

	... except here ... where the server prepares it all, sends it
	to the client, and forgets that it knew the unknowable secret.
	This is because the client might generate a request that I 
	dont like, and I would have to search every corner to check.
	Instead I prepare it.

	An alternative, would be to prepare the REQ, send it to the client
	to sign, check it comes back unchanged except for the SIGN,
	(which is just a question of RTFM, and a bit of coding,
	not a comprehensive search for what I never knew to look for),
	then sign it.

	The down side of this approach, is that the client might
	trust a MITM a bit too much.

	The MITM is also why the cb_pass is probably not used,
	and if it is, why it should be a blk1 after the key has been munged.
	After all, if my passphrase is "bananas" this week, it might be
	"..." next week.
*/
class X509_REQ_util : public obj_ref0 // X509_util_CERT
{
 public:
	obj_hold<X509_REQ_hold> REQ;

	obj_hold<X509_util_CERT> UTIL;

	//! the RSA key that client knows
	obj_hold<EVP_P_KEY> client_key;

	//
	// temp vars for building with 
	//

	~X509_REQ_util();

	/*!
		Prepare a REQ without a KEY phrase (yet)
	*/
	X509_REQ_util();

	/*!
		UNUSED
	*/
	bool init();


	/*!
		AFTER you have setup client_key, call this, to get started

		Maybe call from gen_client_key() and load_client_key()
	*/
	bool set_client_key_public(EVP_P_KEY * client_key );
	bool set_client_key_pair(EVP_P_KEY * key);
	bool gen_client_key_pair( CB_get_phrase_base * cb_phrase, int nbits );

	bool apply_VALS( X509_VALS_base & vals );
	bool apply_VALS( X509_VALS_base * vals );

	/*!
		After adding all the bits to the subject name variable,
		add the subject name to the certificate.
	*/
	bool set_subject_name() ;

	//////////////////////////////////

	bool add_extensions();

	bool sign_REQ_with_client_key();

//
//	Maybe bring some of these back
//	or rework them for SQL storage
//
//	bool write_to_file1( const char * filename );
};
