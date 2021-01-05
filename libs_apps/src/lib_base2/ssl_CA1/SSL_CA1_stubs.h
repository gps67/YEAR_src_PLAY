#ifndef SSL_CA1_stubs_H
#define SSL_CA1_stubs_H

// inspired by standard example EX10-5
// RTFM 3.1.3 Certificate Extension

// #include "SSL_stubs.h"

namespace CA1 {

	// thangs that go into X509's (but are not yet, so only in str1 format)
	class X509_VALS_base;
	class X509_VALS_CA_depth;
	class X509_VALS_key_usage;
	class X509_VALS_subject;
	class X509_VALS_subjectAltName;
	class X509_VALS_time_range;

	// things that help with X509 building (todo=analysing)
	class X509_util_EXT_list;
	class X509_util_EXT_iss;
	class X509_util_NAME;
 //	class X509_util_CERT; // was good, but so thin and it turned sideways

	// these are site specific base
	class SITE_X509_layout;
	class SITE_X509_tag;
	class SITE_X509_VALS_base;
	class SITE_CA_task;
	class CA_task;

	class MYSITE_loaded; // rename SITE_certs_loaded
	class MYSITE_loader; // rename SITE_certs_loader

	class X509_REQ_hold;
};

#endif

