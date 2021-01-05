#include "X509_cert.h"
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include "dgb.h"
#include "buffer2.h"
#include "CB_get_phrase_base.h"
#include "util_buf.h" // blk_read_entire_file
#include "openssl/objects.h" // 
#include "openssl/x509v3.h" // 

#include "EVP_P_KEY.h" // CA_set_pubkey

	// TODO - look at BIO_mem.h (see EVP_P_KEY)

	/*!
		construct this holder from nothing
	*/
	X509_cert::X509_cert()
	: cert( NULL )
	, keep( false )
	{
	}

	/*!
		construct this holder from a pre-allocated reference
	*/
	X509_cert::X509_cert( X509 * _cert )
	: cert( _cert )
	, keep( false )
	{
	}

	/*!
		destructor - there is NO ssleay reference count
		LURK LEAK refcount to be kept when ?
	*/
	X509_cert::~X509_cert()
	{
		if( cert ) {
		//	show_some_info();
			if( keep ) {
				INFO("X509_free( %s ) %x", get_str_CN(), cert );
				X509_free( cert );
				keep = false;
			} else {
				INFO("X509_free( %s ) %x NOT CALLED", get_str_CN(), cert );
			}
			cert = NULL;
		}
	}
	
	X509_cert:: operator X509 *() {
	if(0)	WARN("WHO OWNS THIS COPY OF THE CERT %x %s", this, get_str_CN());
		return get_so_dont_delete();
		// return cert;
	}

	/*!
		set this holder from a pre-allocated reference

		seems that some functions sort of expect this to never happen
		and passing these values ... cause later use of them
		and if that function has quit ...
	*/
	void X509_cert::set( X509 * _cert, bool _keep )
	{
		INFO("X509_cert::set(%x,keep)", _cert );
		if( _cert == cert ) {
			return;
		}
		if( cert && keep) X509_free( cert );
		cert = _cert;
		keep = _keep;
	}

	void X509_cert::set_to_keep( X509 * _cert )
	{
		set( _cert, true );
	}

	void X509_cert::set_peek( X509 * _cert )
	{
		set( _cert, false );
	}

	X509 * X509_cert::get_and_remove() // get, used, set NULL
	{
		if(!keep) {
			WARN("keep was already false (%s)", get_str_CN());
			show_subject();
		}
		keep = false;
		X509 * C = cert;
		cert = NULL;
		return C;
	}

	X509 * X509_cert::get_so_dont_delete() // do not delete, but can reuse
	{
	//	if(!keep) if(cert) WARN("keep was already false (%s)", get_str_CN());
		keep = false;
		return cert;
	}

	X509 * X509_cert::get_peek()
	{
		return cert;
	}

	/*!
		The CA wants to set the pubkey without knowing the private key

		It probably got it from the REQ
	*/
	bool X509_cert:: CA_set_pubkey( EVP_P_KEY * key )
	{
		if(!X509_set_pubkey( cert, key->pkey )) {
			return FAIL("X509_set_pubkey( cert, key->pkey ))");
		}
		return true;
	}

	/*!
		The CA wants to set the pubkey without knowing the private key

		It probably got it from the REQ
	*/
	bool X509_cert:: CA_sign_with_issuers_key( EVP_P_KEY * key )
	{
		if(!key) return FAIL("NULL key");
		const EVP_MD * digest = key->get_correct_digest_method_for_X509_sign();
		if(!digest) return FAIL_FAILED();

		if(!X509_sign( cert, key->pkey, digest )) {
			return FAIL("Error signing cert");
		}
		return true;
	}

	bool X509_cert::show_subject()
	{
		if( !cert ) return FAIL("## CERT NULL ##\n");

		char * str_kept =
		  X509_NAME_oneline (X509_get_subject_name(cert),0,0);

		const char * str = str_kept;
		if(!str) str = "''"; // highlight TOKEN = emptystring

		// NULL or empty STRING // {} // '' // "" // NULL // PTR
		// need to properly allocate ROM str //
		// use str_with_default = str
		// printing it like this, leads to PARSE_LEX_emptyString
		// DIALECT empty_string_in_SCRIPT_style
		// DIALECT SUBLEX words are cident99 _NAME

		INFO("Subject: %s", str );
		OPENSSL_free (str_kept); // if(str_kept) // call OK with NULL
		return true;
	}

	bool X509_cert::show_issuer()
	{
		if( !cert ) return FAIL("## CERT NULL ##\n");
		char * str;
		str = X509_NAME_oneline (X509_get_issuer_name (cert),0,0);
		if(!str) str=(char*)"''";
		INFO("Issuer:  %s", str );
		OPENSSL_free (str);
		return true;
	}

	
	/*!
		debug, show which cert is being used
	*/
	bool X509_cert::show_some_info()
	{
		if( !cert ) return FAIL("## CERT NULL ##\n");


		show_issuer();
		show_subject();

		if(0) {
			buffer2 temp;
			PEM_out(temp);
			INFO("PEM = '%s'", (STR0) temp );
		}
		return true;
	}

	/*!
		debug, show which cert is being used
	*/
	bool X509_cert::show_more_info()
	{
		char * str;

		if( !cert ) return FAIL("## CERT NULL ##\n");

		str = X509_NAME_oneline (X509_get_subject_name(cert),0,0);
		if(!str) str=(char*)"''";
		INFO("Subject: %s", str );
		OPENSSL_free (str);

		str = X509_NAME_oneline (X509_get_issuer_name (cert),0,0);
		if(!str) str=(char*)"''";
		INFO("Issuer:  %s", str );
		OPENSSL_free (str);

		if(1) {
			buffer2 temp;
			PEM_out(temp);
			INFO("PEM = '%s'", (STR0) temp );
		}

// gdb_invoke(true);
// http://www.openssl.org/docs/crypto/OBJ_nid2obj.html
// EX5-8
// crypto/objects/objects.h
//

		int N_ext = X509_get_ext_count(cert);
		INFO("N_ext %d", N_ext );
		for(int i=0; i<N_ext; i++ ) {
			WARN("X509_get_ext - decoder needed");
#define TODO_99
#warning CODE_TODO_99 x509 v3 ext
#ifndef TODO_99
			// none of these need deleting ...
			X509_EXTENSION * ext = X509_get_ext( cert, i );
			ASN1_OBJECT * asn1 = X509_EXTENSION_get_object(ext);
			int nid = OBJ_obj2nid( asn1 );
			const char * extstr = OBJ_nid2sn( nid );
			INFO("ext[%d] %d '%s'", i, nid, extstr );
// # INFO  # show_more_info() # N_ext 2
// # INFO  # show_more_info() # ext[0] 'basicConstraints'
// # INFO  # show_more_info() # ext[1] 'extendedKeyUsage'
// "subjectAltName"
// issuerAltName
// extendedKeyUsage.clientAuth // except  value 'TLS Web Server Authentication'
// extendedKeyUsage.serverAuth // ...
// basicConstraints.CA FALSE
// keyUsage
// privateKeyUsagePeriod
// ... critical
// NOT pathLenConstraint
// += critcal_flag


			const X509V3_EXT_METHOD * meth = X509V3_EXT_get( ext );
			if(!meth) {
				FAIL("NULL meth");
				continue; // or break
			}

			const unsigned char * ext_value_data = ext->value->data;
			void * ext_strg = NULL;
			if(meth->it) 
			 ext_strg = ASN1_item_d2i(
				NULL,
				&ext_value_data,
				ext->value->length,
				ASN1_ITEM_ptr(meth->it)
			 );
			else
			 ext_strg = meth->d2i(
				NULL,
				&ext_value_data,
				ext->value->length
			 );
/*
	include/openssl/safestack.h

		#define STACK_OF(type) struct stack_st_##type

		#define sk_CONF_VALUE_num(st) SKM_sk_num(CONF_VALUE, (st))
		#define SKM_sk_num(type, st) \
			sk_num(CHECKED_STACK_OF(type, st))

		... talks about (void *) problems ...
		#define CHECKED_STACK_OF(type, p) \
			    ((_STACK*) (1 ? p : (STACK_OF(type)*)0))





		typedef struct stack_st
		{
			int num;
			char **data;
			int sorted;
				
			int num_alloc;
			int (*comp)(const void *, const void *);
		} _STACK;  /+ Use STACK_OF(...) instead +/



*/
#ifdef WIN32
#warning THIS FUNCTIONALITY IS MISSING FROM WIN32 BECAUSE OF SYNTAX
	// actually I think it is a GCC version thing
#else
			// no casting error on UNIX!
			STACK_OF(CONF_VALUE) * val = NULL;
#define BACKPORT 1
#ifndef BACKPORT
			if( meth->i2v) val = meth->i2v( meth, ext_strg, NULL );
#else
#warning "clean up this BACKPORT to openssl 1.0 from 0.9.8"
			v3_ext_method* meth3 = (v3_ext_method*) meth;
			if( meth->i2v) val = meth->i2v( meth3, ext_strg, NULL );

#endif
#define val2 val

			if(val2) // wasnt needed ?
			for (int j = 0;  j < sk_CONF_VALUE_num(val2);  j++ )
			{
				CONF_VALUE * nval = sk_CONF_VALUE_value(val2, j);
				INFO("name '%s' value '%s'",
					nval->name,
					nval->value
				);
			}
#endif
#endif // todo_99

		}

		return true;
	}

	const char * X509_cert::get_str_CN()
	{
		static buffer1 buf;
		buf.clear();
		return get_str_CN(buf);
	}

	const char * X509_cert::get_str_CN(buffer1 & buf)
	{
		buf.trailing_nul();
		buf.get_space(60);

		if(!cert) {
			buf.print("(NULL-CN-FROM-NULL-cert)");
			return "(NULL CERT)";
		}

		X509_NAME * subj = X509_get_subject_name(cert);
		if(!subj) {
			show_some_info();
			FAIL("X509_get_subject_name() failed");
			buf.print("(NULL-CN-FROM-NULL-subj)");
			return "(****)";
		}
		char * P = buf.gap_addr_signed_char();
		int t = X509_NAME_get_text_by_NID(
			subj,
			NID_commonName,
			P,
			buf.space_avail()
		);
		if( t < 0 ) {
			FAIL("X509_NAME_get_text_by_NID() failed");
			show_some_info();
			return "";
		}
		buf.nbytes_used += t; // redo when CIRC ?
		// NUL was placed but not counted
		// can get len needed if wanted
		return P; // it didnt move!
	}

	/*!
		dump X509 in DER format
	*/
	bool X509_cert::DER_out( buffer2 & buf )
	{
		if( !cert ) {
			return FAIL("## CERT NULL ##\n");
		}
		unsigned char * out = NULL; // openssl mallocs for us
		int len = i2d_X509( cert, &out );
		if( len < 0 ) {
			FAIL("i2d_X509() shouldnt fail");
			return false;
		}
		if(buf.put_nn_bytes( len, out )) {
			free( out );
			buf.trailing_nul();
			if(1) {
			 int off = buf.nbytes_used - len;
			 u8 * p = & buf.buff[off];
			 INFO("DER='...unreadable binary...'", p );
			}
		} else {
			free( out );
			return FAIL("i2d_X509() worked but put_nn_bytes() failed");
		}
		return true;
	}

	// not circular buffer, not parse from, maybe add nbytes_parsed
	bool X509_cert::DER_in( buffer2 & buf ) // no longer a circ
	{
		int len = buf.nbytes_used;
		// int len = buf.circ_data_avail();
		const unsigned char *in = uSTR0_from_STR0( buf ); // cast adds NUL
		// errm // STR0 is (const char *) // illegal cast to ...
		const unsigned char *in_old = in;
		X509 *px = cert;
		WARN("X509 LEAK?"); // or cert2 == this->cert ?? //
		X509 * cert2 __attribute__ ((unused)) = d2i_X509(&px, &in, len);
		if(!cert) {
			FAIL("d2i_X509(px,in,len) failed to parse a DER X509");
			return false;
		}
		int len_used __attribute__ ((unused)) = in - in_old; // nbytes_parsed
		// buf.circ_used( len_used );
		buffer1 b;
		INFO("parsed OK %s", get_str_CN(b));
		return true;
	}

#if 0
	bool X509_cert::DER_in( buffer3 & buf ) // no longer a circ
	{
		int len = buf.circ_data_avail();
		int len = buf.circ_data_avail();
		const unsigned char *in = buf.circ_addr_wr();
		const unsigned char *in_old = in;
		X509 *px = cert;
		X509 * cert2 __attribute__ ((unused)) = d2i_X509(&px, &in, len);
		if(!cert) {
			FAIL("d2i_X509(px,in,len) failed to parse a DER X509");
			return false;
		}
		int len_used = in - in_old;
		buf.circ_used( len_used );
		buffer1 b;
		INFO("parsed OK %s", get_str_CN(b));
		return true;
	}
#endif


/*!
	dump X509 in PEM format
*/
bool X509_cert::PEM_out( buffer2 & buf )
{
	if( !cert ) {
		return FAIL("## CERT NULL ##\n");
	}

	/*
		create a memory based stream bio 
	*/
	BIO * bio_mem = BIO_new(BIO_s_mem());
	// NO // BIO_set_close(bio_mem, BIO_NOCLOSE);
	// NO /* So BIO_free() leaves BUF_MEM alone */

	/*
		now put some data into the bio_mem
		using pretty much any openssl _bio_ routine
	*/
	int ret = PEM_write_bio_X509( bio_mem, cert );
	if( ret < 0 ) {
		BIO_free(bio_mem);
		buffer1 buf;
		return FAIL("PEM_write_bio(bio,cert{%s}) returned %d",
			get_str_CN(buf), ret );
	}

	/*
		now memcpy the bio_mem contents into the buffer
		the copy overhead is not that much, (also the new.delete)
		and it is a convenience that does this ...

		It _DOES_ have to be a BIO_s_mem
	*/

	BUF_MEM *bptr;
	BIO_get_mem_ptr(bio_mem, &bptr);
	buf.put_nn_bytes( bptr->length, bptr->data );

	BIO_free(bio_mem);
	return true;
}

bool X509_cert::PEM_in( CB_get_phrase_base * cb_phrase, str0 val )
{
	buffer2 buf;
	buf.set( val );
	if(!PEM_in( cb_phrase, buf )) return FAIL_FAILED();
	return true;
}

/*!
	read PEM in from buffer, using the callback phrase obj
*/
bool X509_cert::PEM_in( CB_get_phrase_base * cb_phrase, buffer2 & buf ) // must be a circ
{
// 3	int len_max = buf.circ_data_avail();
// 3	unsigned char *in = buf.circ_addr_rd();
	int len_max = buf.nbytes_used;
	const unsigned char *in = uSTR0_from_STR0( buf );

	BIO * bio_mem = BIO_new_mem_buf( in, len_max ); // READONLY not deleted

	/*
		QUESTION - WHEN is the cb_phrase actually used?

		I know it is used when reading the KEY from a file,
		so that the PRIVATE KEY is usable.

		?MAYBE? it is only used here, when the CERT
		contains the KEY in the same file ...
	*/

	X509 * x2 = PEM_read_bio_X509(
		bio_mem,
		&cert, // NULL, // &cert,// or else use set( px )
		CB_get_phrase_base::C_get_passwd_fn,
		cb_phrase // u_obj passed back to C_get_passwd_fn
	);
	if(!x2) INFO("NULL x2");
	// set( x2 );
	// x2; // unused

	// len_used - figure out how much the bio pointer has advanced

	BUF_MEM * mem2 = NULL;
	BIO_get_mem_ptr(bio_mem, &mem2);
	// BIO is using the original memory readonly, and leaves .length
	int len_used = len_max - mem2->length;
	if(len_used < 0) {
		FAIL("len_used negative");
		len_used = len_max;
	}

// 3 // buf.circ_used( len_used );

	BIO_free( bio_mem ); // hopefully ...


	if(!cert) {
		FAIL("PEM_read_bio_X509(bio,px,cb,uobj) failed to parse a PEM X509");
		return false;
	}

	// buffer2 buf_CN;
	// INFO("parsed OK CN = '%s'", get_str_CN(buf_CN));
	show_msg_CN( "OK PEM_in()");
	// show_some_info();
	return true;
}

bool X509_cert::show_msg_CN( const char * msg )
{
	buffer2 buf_CN;
	INFO( "%s CN = '%s'", msg, get_str_CN(buf_CN));
	return true;
}
	
bool X509_cert::PEM_from_file(
	CB_get_phrase_base * cb_phrase,
	const char * filename
)
{
	buffer2 mem_in;
	u32 max_K_size = 60;
	if(!blk_read_entire_file( mem_in, filename, max_K_size ))
		return FAIL("%s", filename );
	// LUCK - buffer3 (circ_) is OK with this
	if(! PEM_in( cb_phrase, mem_in ))
		return FAIL("%s", filename );
	return true;
}

bool X509_cert::PEM_from_file( const char * filename )
{
	obj_hold<CB_get_phrase_base> cb_phrase = new CB_get_phrase_NONE();
	return PEM_from_file( cb_phrase, filename );
}

bool X509_cert::PEM_into_file( const char * filename )
{
	INFO("%s", filename);
	buffer2 buf;
	buf.get_space( 2000 ); // 700 +
	if(!PEM_out( buf )) return FAIL_FAILED();
	if(! blk_write_to_file( buf, filename )) return FAIL_FAILED();
	INFO("DONE %s", filename);
	return true;
}


