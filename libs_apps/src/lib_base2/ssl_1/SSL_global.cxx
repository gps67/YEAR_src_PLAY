
#include <openssl/err.h>
#include <openssl/ssl.h>
#include "SSL_global.h"
#include "dgb_fail.h"
#include <openssl/rand.h>
#include "CB_get_phrase.h"
#include "X509_cert.h"
#include "EVP_P_KEY.h"

SSL_global_server_eg::SSL_global_server_eg()
{
	// AUTO_CTOR
}

SSL_global_server_eg::~SSL_global_server_eg()
{
	// AUTO_DTOR
}

bool SSL_global_server_eg:: class_is_usually_client()
{
	// is_server // so false is_client
	return false;
}

bool SSL_global_server_eg:: set_ssl_verification()
{
	// load KEYS from files // OPTION not from files
	const char *filename_C_server_cert = "keys/C_server_cert.pem";
	const char *filename_C_server_key =  "keys/C_server_key.pem";

	const char *filename_CA_parent_1 = "keys/CA_of_server_1.pem";
	const char *filename_CA_parent_2 = "keys/CA_of_server_2.pem";
	const char *filename_CA_parent_3 = "keys/CA_of_server_3.pem";
	const char *filename_CA_parent_4 = "keys/CA_of_server_4.pem";
//	filename_CA_parent_3 = NULL;
	filename_CA_parent_4 = NULL;

	const char *filename_CA_acceptable_list = "keys/CA_acceptable_list.pem";

	// in my jumbled test rig
	// the client and server have links from the same dir
	// these are the acceptable CA s that we want the client to send
	// 1a 1b 1c - do not have to be within date, just correct subjects
	// and then I am not sure if this works, yet, or not
	const char *filename_CA_client_1a = "keys/CA_of_client_1.pem";
	const char *filename_CA_client_1b = "keys/CA_of_group2_1.pem";
	const char *filename_CA_client_1c = "keys/CA_of_group3_1.pem";

	X509_cert C_self;
	X509_cert CA_parent_1;
	X509_cert CA_parent_2;
	X509_cert CA_parent_3;
	X509_cert CA_parent_4;
	X509_cert CA_NULL;

	// TODO some leaking like this around
	// TODO ...
	obj_hold<CB_get_phrase_blk1> _cb_pass = new CB_get_phrase_blk1(EVP_des_ede3_cbc());
	CB_get_phrase_blk1 & cb_pass = *_cb_pass;
	cb_pass.set_phrase( "MyPassPhrase" ); 
	cb_pass.set_phrase( "secret" ); 


	obj_hold<EVP_P_KEY> PKEY = new EVP_P_KEY( cb_pass );
	if(!PKEY->read_from_file( filename_C_server_key )) {
		FAIL_FAILED();
	}

 if(!C_self     .PEM_from_file( filename_C_server_cert )) return FAIL_FAILED();
 if(!CA_parent_1.PEM_from_file( filename_CA_parent_1 )) WARN("tried");
 if(!CA_parent_2.PEM_from_file( filename_CA_parent_2 )) WARN("tried");
 if(!CA_parent_3.PEM_from_file( filename_CA_parent_3 )) WARN("tried");
 if(!CA_parent_4.PEM_from_file( filename_CA_parent_4 )) WARN("tried");

	/*
		setup my own certificate chain, to pass to peer

		I only have one parent, the _1 _2 _3 means great-grand-parent
	*/
	if(!set_own_certificate_chain(
		C_self, *PKEY, cb_pass,
		CA_parent_1,
		CA_parent_2,
		CA_parent_3,
		CA_parent_4
	))
		return FAIL_FAILED();

	/*
		The CA's that I trust when seen in a PEER

			I think that this both requests these,
			and validates against them,
			as two separate actions ?


			Or maybe its not the request list ?
			And my client is only sending one anyway

		I am not 100% certain that this works as expected.
		It seems that the ROOT must be trusted.
		And that the PEER must supply the chain.
		And that the PEERs ROOT can optionally be omitted
		(as it apprears in the issued by, signature on the tail)

		If my own parent is for servers, and not for clients,
		it should NOT be added to the trusted list

		If my own parent is the ONLY CA around here, it must be!
	*/
//	if(!add_trusted_CA( CA_parent_1 )) return FAIL_FAILED();
	if(!add_trusted_CA( CA_parent_2 )) return FAIL_FAILED();
	if(!add_trusted_CA( CA_parent_3 )) return FAIL_FAILED();
//	if(!add_trusted_CA( CA_parent_4 )) return FAIL_FAILED();

if(0)
{

 if(0)
 {
	if(!load_remote_client_CA_list( filename_CA_acceptable_list ))
	 WARN("xxxx");

 } else {

 //	const char *filename_CA_client_1a = "keys/CA_of_client_1.pem";
	X509_cert CA_client_1a;
	X509_cert CA_client_1b;
	X509_cert CA_client_1c;
	if(!CA_client_1a.PEM_from_file( filename_CA_client_1a )) WARN("tried");
//	if(!CA_client_1b.PEM_from_file( filename_CA_client_1b )) WARN("tried");
//	if(!CA_client_1c.PEM_from_file( filename_CA_client_1c )) WARN("tried");
	if(!add_remote_client_CA_name( CA_client_1a )) WARN("xxx");
 //	if(!add_remote_client_CA_name( CA_client_1b )) WARN("xxx");
 //	if(!add_remote_client_CA_name( CA_client_1c )) WARN("xxx");
 }
}

	return true;
}

// -------------------

SSL_global_client_eg::SSL_global_client_eg()
{
}

SSL_global_client_eg::~SSL_global_client_eg()
{
}

bool SSL_global_client_eg:: class_is_usually_client()
{
	return true;
}

bool SSL_global_client_eg:: set_ssl_verification()
{
	const char *filename_C_client_cert = "keys/C_client_cert.pem";
	const char *filename_C_client_key =  "keys/C_client_key.pem";

	const char *filename_CA_parent_1 = "keys/CA_of_client_1.pem";
	const char *filename_CA_parent_2 = "keys/CA_of_client_2.pem";
	const char *filename_CA_parent_3 = "keys/CA_of_client_3.pem";
	const char *filename_CA_parent_4 = "keys/CA_of_client_4.pem";
//	filename_CA_parent_3 = NULL;
	filename_CA_parent_4 = NULL;

//	const char *filename_CA_acceptable_list = NULL;

	X509_cert C_self;
	X509_cert CA_parent_1;
	X509_cert CA_parent_2;
	X509_cert CA_parent_3;
	X509_cert CA_parent_4;
	X509_cert CA_NULL;

	// TODO some leaking like this around
	// TODO ...
	obj_hold<CB_get_phrase_blk1> _cb_pass = new CB_get_phrase_blk1();
	CB_get_phrase_blk1 & cb_pass = *_cb_pass;
	cb_pass.set_phrase( "MyPassPhrase" ); 
	cb_pass.set_phrase( "secret" ); 


	obj_hold<EVP_P_KEY> PKEY = new EVP_P_KEY( cb_pass );
	if(!PKEY->read_from_file( filename_C_client_key )) {
		FAIL_FAILED();
	}

 if(!C_self     .PEM_from_file( filename_C_client_cert )) return FAIL_FAILED();
 if(!CA_parent_1.PEM_from_file( filename_CA_parent_1 )) WARN("tried");
 if(!CA_parent_2.PEM_from_file( filename_CA_parent_2 )) WARN("tried");
 if(!CA_parent_3.PEM_from_file( filename_CA_parent_3 )) WARN("tried");
 if(!CA_parent_4.PEM_from_file( filename_CA_parent_4 )) WARN("tried");

	/*
		setup my own certificate chain, to pass to peer

		I only have one parent, the _1 _2 _3 means great-grand-parent
	*/
	if(!set_own_certificate_chain(
		C_self, *PKEY, cb_pass,
		CA_parent_1,
		CA_parent_2,
		CA_parent_3,
		CA_parent_4
	))
		return FAIL_FAILED();

	/*
		The CA's that I trust when seen in a PEER

		I am not 100% certain that this works as expected.
		It seems that the ROOT must be trusted.
		And that the PEER must supply the chain.
		And that the PEERs ROOT can optionally be omitted
		(as it apprears in the issued by, signature on the tail)

		If my own parent is for servers, and not for clients,
		it should NOT be added to the trusted list
	*/
	if(!add_trusted_CA( CA_parent_1 )) return FAIL_FAILED();
	if(!add_trusted_CA( CA_parent_2 )) return FAIL_FAILED();
	if(!add_trusted_CA( CA_parent_3 )) return FAIL_FAILED();
	if(!add_trusted_CA( CA_parent_4 )) return FAIL_FAILED();

	return true;
}
