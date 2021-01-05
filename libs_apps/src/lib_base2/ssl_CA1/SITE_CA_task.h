#ifndef SITE_CA_task_H
#define SITE_CA_task_H

#include "obj_ref.h"
#include "buffer2.h"
#include "SSL_stubs.h"
#include "SSL_CA1_stubs.h"

#include "X509_util_NAME.h"
#include "X509_util_EXT.h"
#include "SITE_X509_layout.h"

namespace CA1 {

/*!
	A SITE_CA_task is an instance of a CA creating a client X509
	(which might also be a CA), in a site specific manner,
	and using a REQ/ISS algorithm.

	Other sites might split the REQ/ISS halves,
	and there is a join line where that can be done.

	The VALS_xxx prefix, is for the values that go _INTO_ the REQ
	(my preferred form of REQ).

	You have some choice (confusion) about the subclasses

		MYSITE_CA_task
		MYSITE_layout

	Which one should specify that RSA keys are 1024 bits,
	except for CA_ZERO which is 2048. You decide ...

	There is an additional multiplier, load parameters from file,
	which can very per-host, per dir, per partname, per parent selector
	(there is no parent selector, other than parent_CA_tag, but could be)

	The idea was that a layout, would be fairly fixed, for a site,
	then different places within the site, would use different parameters,
	and values that go into VALS, loaded from ./settings/keys/CA_item/.ini
*/
class CA_task : public obj_ref0
{
 public:

	obj_hold<SITE_X509_tag>		layout_tag;

	obj_hold<X509_VALS_base>	VALS;
	obj_hold<X509_REQ_hold>		REQ;
//	obj_hold<X509_util_CERT>	UTIL;
	X509_util_NAME			UTIL_subject;
	X509_util_EXT_list /*_req*/	UTIL_extensions;

	// these two can actually be the same thing
	// but only the CA can have the self_key loaded
	// the other would need a different self_
	// splitting them simply means copying them over

	obj_hold<MYSITE_loaded>		CA_site_loaded;
	obj_hold<MYSITE_loader>		CA_site_loader;
	obj_hold<MYSITE_loaded>		C_site_loaded;
	obj_hold<MYSITE_loader>		C_site_loader;

	/*
		NOW I want sparse-of-four load+save
	*/

	obj_hold<EVP_P_KEY>		client_key_pub;
	obj_hold<EVP_P_KEY>		client_key_priv;

	CA_task(
		SITE_X509_layout * scheme,
		SITE_X509_tag_enum _tag
	);
	~CA_task();
	bool clear();

	bool VALS_set_CA_depth_for_layout();
	bool VALS_check_C_not_CA();
	bool VALS_check_CA_not_C();

	bool VALS_set_HERE_part0(SITE_X509_tag_enum _tag);
 virtual
        bool VALS_set_HERE_part1_nbits_RSA();
 virtual
        bool VALS_set_HERE_part1_subject();
 virtual
        bool VALS_set_HERE_part1_time_range();

        bool REQ_set_client_key_priv( EVP_P_KEY * key );
        bool REQ_create_client_key_priv();
 virtual
	bool obtain_client_cb_phrase(); // SITE_


//	The CA_task gets the _layout to do these things (called at right time)
//	prefix with SITE_ ???


	virtual bool obtain_issuer_certificate_and_key(); // ISS
	virtual bool obtain_issuer_cb_phrase();

	virtual bool make_file_names();
	virtual bool ISS_write_client_cert_and_key(); // _to_files!

	bool clear_issuer_cert_and_key();
	bool clear_client_cert_and_key();

	bool REQ_apply_VALS();
	bool REQ_apply_VALS( X509_VALS_base & vals );
	bool REQ_sign_with_client_key_priv();

	//////////////////////////

//	virtual bool ISS_make_file_names();	// similar when SQL keyname not filename

//	virtual bool ISS_obtain_issuer_cb_phrase();
//	virtual bool ISS_obtain_issuer_certificate_and_key();

	bool REQ_mk_cert_from_VALS();
	bool ISS_mk_cert(); // from REQ and a few VALS and issuer* etc

	bool ISS_new_cert_version_2();
	bool ISS_copy_REQ_subject_name();
	bool ISS_copy_REQ_subject_alt_name(); // but is extension
	bool ISS_copy_REQ_client_key_public(); // from REQ
	bool ISS_get_client_key_public_FROM_REQ();
	bool ISS_put_client_key_public_INTO_CERT();
	bool ISS_copy_REQ_all_extensions(); // from REQ
	bool ISS_copy_CA_issuer_name(); // from issuer_cert
	bool ISS_copy_CA_issuer_hash(); // from issuer_cert

	bool ISS_copy_VALS_serial();
	bool ISS_copy_VALS_time_range();
	bool ISS_put_serial( int ser );
	bool ISS_put_time_range( X509_VALS_time_range & time_range );

	bool ISS_sign_with_CA_priv();

 /////////////////// EXISTS BUT DOES NOT EXIST ///////// NOT REFERED TO

//	bool ISS_write_to_files( const char * cert_filename, const char *key_filename );

};
}; // NS
#endif

