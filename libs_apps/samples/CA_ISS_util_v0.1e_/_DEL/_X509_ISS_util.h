#ifndef X509_ISS_util_H
#define X509_ISS_util_H

#include "X509_REQ_hold.h"
#include "X509_VALS_base.h"
#include "MYSITE_CA_task.h" // class SITE_X509_tag/_type is_CA_ZERO
#include "str1.h"


class X509_cert;
#include "buffer2.h"
namespace CA1 {

/*!
	a CA utility to hold everything whilst ISSUING a new X509

	This uses a predefined layout of CA/C, eg CA_ZONE issuing C_LAPTOP
	(see SITE_X509_tag and _layout), which helps us load the correct
	CA_cert_key.pem file and set the correct parameters,
	in a site-independent way, with site-dependent variations.

	NOTE that I am not doing the right thing with REQ/ISS,
	because I am creating the RSA on the CA server, not the client.
	This helps me be certain that the client is not creating NAFF secrets.

	DITTO for the various fields of the REQ, which 'should' be set by the
	client, but are set by the server, to be sure that they contain
	proper decent values (to fix this you must VALIDATE what you are given).
	
	An added advantage, is that a design fix can be implemented on the
	server, without finding and upgrading every single client.
*/
class X509_ISS_util : public obj_ref0 // : public CA_task // : public X509_util_CERT
{
 public:
	obj_hold<CA_task>		task;

	// base class for now
	obj_hold<SITE_X509_tag>		layout_tag;
//	obj_hold<SITE_X509_layout>	layout_scheme; // is in layout_tag

	X509_VALS_iss_misc & vals_iss_misc;

	/*
		vals_iss_misc.CERT_serial
		vals_iss_misc.RSA_bits

		otherwise only used for REQ
		but maybe move everything to ISS side, for ISS2 class ?
	*/

	// ISSUER = CA parent of CLIENT child
	// CLIENT = ISSUED = what we generate


	~X509_ISS_util();

	X509_ISS_util(
		SITE_X509_tag * layout, // = NULL
		CA_task * _task
	);

	/*!
		Prepare a ISS without a KEY phrase (yet)
	*/
	X509_ISS_util(
		X509_VALS_iss_misc & vals_iss_misc,
		SITE_X509_tag * layout, // = NULL
		EVP_P_KEY * client_key_priv_if_available, // = NULL
		CA_task * _task
	);

	virtual bool ISS_make_file_names();	// similar when SQL keyname not filename

	virtual bool ISS_obtain_issuer_cb_phrase();
	virtual bool ISS_obtain_issuer_certificate_and_key();

#define X509_REQ_util CA_task
	bool ISS_mk_cert( X509_REQ_util * REQ );

	bool ISS_new_cert_version_2();
	bool ISS_copy_subject_name(X509_REQ_hold * REQ);
	bool ISS_copy_subject_alt_name(X509_REQ_hold * REQ); // but is extension
	bool ISS_copy_client_key_pub(
		X509_REQ_hold * REQ,
		obj_hold<EVP_P_KEY> & client_key_pub
	);
	bool ISS_copy_client_key_public( X509_REQ_util * REQ );
	bool ISS_copy_all_extensions(X509_REQ_hold * REQ);
	bool ISS_copy_issuer_name();

	bool ISS_set_serial( int ser );
	bool ISS_set_time_range( X509_VALS_time_range & time_range );

	bool ISS_sign_with_CA_priv();

 /////////////////// EXISTS BUT DOES NOT EXIST ///////// NOT REFERED TO

	bool ISS_write_to_files();
	bool ISS_write_to_files( const char * cert_filename, const char *key_filename );
	bool ISS_test1();

};

}; // namespace
#endif

