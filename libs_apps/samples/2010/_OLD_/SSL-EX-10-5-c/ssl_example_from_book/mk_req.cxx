#include <stdio.h>
#include <stdlib.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>
#include <openssl/pem.h>

#include "dgb.h"
#include "EVP_P_KEY.h"
#include "SSL_global.h"
#include "CB_get_phrase.h"
#include "BIO_mem.h"
#include "util_buf.h"

// inspired by standard example EX10-5

#define CLIENT_KEY_CIPHER EVP_des_ede3_cbc()
#define CLIENT_KEY_SECRET "secret"
// #define CLIENT_KEY_SECRET NULL
#define CLIENT_KEY_FILE "client_key.pem"
#define CLIENT_REQ_FILE "client_req.pem"
#define CLIENT_subjectAltName "DNS:vlan.information-cascade.co.uk"
#define CLIENT_subjectAltName_DNS2 "hublan.information-cascade.co.uk"
#define CLIENT_DNS_1 "vlan.information-cascade.co.uk"
#define CLIENT_DNS_2 "hublan.information-cascade.co.uk"
#define CLIENT_commonName "HSID_1234"
#define N_entries (sizeof(entries)/sizeof(entries[0]))

struct entry
{
  const char *key;
  const char *value;
};

struct entry entries[] = {
  {"countryName",		"UK"},
  {"stateOrProvinceName",	"Berks"},
  {"localityName",		"Reading"},
  {"organizationName",		"Information Cascade"},
  {"organizationalUnitName",	"Software Devel"},
//  {"XXcommonName",		CLIENT_commonName },
};

/*
	ALSO V3 extension:
		char *name =	"subjectAltName";
		char *value =	CLIENT_subjectAltName;
*/

/*
	I want ...
		HANDSET_ID = "HSID_1234" ??

	I do not have (unlike a web server which does)
		#define CLIENT_subjectAltName "DNS:vlan.kyzo.co.uk"
*/
/*
	RTFM 3.1.3 Certificate Extension
*/

int a_kept = -1;
void C_progress_genkey( int a, int b, void * cb_arg )
{
	if( a_kept != a ) {
		 a_kept = a;
		printf("\nC_progress_genkey( %d, ", a );
	}
	printf("%d",b); // basically just counts to unknown varying ceiling
	printf(".");
	fflush(0);
}

class REQ_util : public obj_ref0
{
 public:
	X509_REQ *req;
	obj_hold<EVP_P_KEY> client_rsa;

	// temp vars for building with 
	X509_NAME *subj;
	STACK_OF (X509_EXTENSION) * extlist;

	~REQ_util()
	{
		if(req) X509_REQ_free (req);
		req = NULL;

#warning double delete error
extlist = NULL;
		if( extlist)
			sk_X509_EXTENSION_pop_free (extlist, X509_EXTENSION_free);
		extlist = NULL;

	}
	REQ_util(
		 CB_get_phrase_base * _cb_pass
	)
	: req(NULL)
	, subj( NULL )
	{
		client_rsa = new EVP_P_KEY( _cb_pass );
	}
	bool init()
	{
		return true;
	}

	bool init_req_with_pub_key()
	{
		// private key must already be loaded
		if(!(req = X509_REQ_new ())) return FAIL("X509_REQ_new()");
		//
		if(!client_rsa) return FAIL("null client_rsa");
		if(!X509_REQ_set_pubkey (req, client_rsa->pkey))
			return FAIL("X509_REQ_set_pubkey()");

		//
		// not that subj is completely detatched from REQ
		// until it is added
		//
		if(!(subj = X509_NAME_new ()))
			return FAIL ("Failed to create X509_NAME object");

		//
		// extlist is also freestanding
		//
		extlist = sk_X509_EXTENSION_new_null();
		if(!extlist) return FAIL("sk_X509_EXTENSION_new_null()");

		return true;
	}

	// X500_NAME ... specific ...
	bool longName_to_NID( const char * longName, int & nid )
	{
		if((nid = OBJ_txt2nid((STR0) longName)) == NID_undef)
		{
			return FAIL_1 ("longName '%s'", (STR0) longName);
		}
		return true;

	}

	bool add_subject_longName_value( const char * longName, const char * value )
	{
		int nid = NID_undef;
		X509_NAME_ENTRY *ent = NULL;

		if(!longName_to_NID( longName, nid ))
			return FAIL_FAILED();

		if (!(ent = X509_NAME_ENTRY_create_by_NID(
			NULL,
			nid,
			MBSTRING_ASC,
			(u8 *) value,
			-1
		)))
			return FAIL ("X509_NAME_ENTRY_create_by_NID()");

		if (!X509_NAME_add_entry (subj, ent, -1, 0))
			return FAIL("X509_NAME_add_entry()");

	//	TRACE("DONE");
		TRACE_2 ("DONE %s, %s", longName, value );
		return true;
	}

	bool add_CN( const char * cn ) {
		// could use compiled NID, or not
		return add_subject_longName_value( "commonName", cn );
	}

	bool set_subject_name() {
		return set_subject_name( subj );
	}
	bool set_subject_name( X509_NAME * _subj )
	{
		if(!X509_REQ_set_subject_name(req, _subj))
			return FAIL("Error adding subject to request");
		TRACE("DONE");
		return true;
	}

	bool add_ext( const char * name, const char * value )
	{
		X509_EXTENSION *ext;
		if (!(ext = X509V3_EXT_conf(
			NULL,
			NULL,
			(char *) name,
			(char *) value
		)))
		      return FAIL_2 ("%s, %s", name, value );
		if(!sk_X509_EXTENSION_push (extlist, ext))
			return FAIL("sk_X509_EXTENSION_push()");
		TRACE_2 ("DONE %s, %s", name, value );
		return true;
	}

	bool add_subjectAltName_ext( const char * pfx, const char * val )
	{
		buffer2 buf;
		buf.print("%s:%s", pfx, val ); // eg "DNS:www.here.dom"
		X509_EXTENSION *ext;
		if (!(ext = X509V3_EXT_conf(
			NULL,
			NULL,
			(char *) "subjectAltName",
			(char *) (STR0) buf
		)))
		      return FAIL_2 ("%s, %s", pfx, val );
		if(!sk_X509_EXTENSION_push (extlist, ext))
			return FAIL("sk_X509_EXTENSION_push()");
		TRACE_2 ("DONE %s, %s", pfx, val );
		return true;
	}

	bool add_ext_DNS( const char * dns )
	{
		buffer2 buf;
		buf.print("DNS:%s", dns );
		return add_subjectAltName_ext( "DNS", dns );
		return add_ext("subjectAltName",buf);
	}

	bool add_ext_CA( bool ca, int pathlen )
	{
		if( !ca ) {
			return add_subjectAltName_ext( "CA", "false" );
		}
		if( pathlen < 1 ) {
			add_subjectAltName_ext( "CA", "false" );
			return FAIL_1("pathlen = %d", pathlen);
		}
		buffer2 dec;
		dec.print("%d", pathlen);
		if(!add_subjectAltName_ext( "CA", "true" )) return FAIL_FAILED();
		return add_subjectAltName_ext( "pathlen", dec );
	}

	bool add_extensions()
	{
		if (!X509_REQ_add_extensions (req, extlist))
			return FAIL ("Error adding subjectAltName to the request");
		TRACE("DONE");
		return true;
	}

	bool client_sign()
	{
		// pick the correct digest
		EVP_MD *digest = NULL;
		if(client_rsa->is_DSA()) {
			digest = (EVP_MD*) EVP_dss1 ();
		} else 
		if(client_rsa->is_RSA()) {
			digest = (EVP_MD*) EVP_sha1 ();
		} else {
			return FAIL("client_rsa is neither RSA nort DSA - dont know what digest to use");
		}
		if(!X509_REQ_sign( req, client_rsa->pkey, digest))
			return FAIL ("Error signing request");
		TRACE("DONE");
		return true;
	}

	bool write_to_buffer( blk1 & buf )
	{
		BIO_mem bio_mem;
		if(!PEM_write_bio_X509_REQ( bio_mem, req )) {
			return FAIL("PEM_write_bio_X509_REQ()");
		}
		if(!bio_mem.write_to_buffer( buf )) return FAIL_FAILED();
		TRACE("DONE");
		return true;
	}

	bool write_req_to_file( const char * filename )
	{
		blk1 buf;
		buf.get_space( 2000 ); // 700 +
		if(!write_to_buffer( buf )) return FAIL_FAILED();
		if(! blk_write_to_file( buf, filename )) return FAIL_FAILED();
		TRACE_1("DONE %s", filename);
		return true;
	}

	bool write_to_file1( const char * filename )
	{
		FILE *fp;
		if (!(fp = fopen (CLIENT_REQ_FILE, "w")))
			return FAIL ("Error writing to request file");
		if (PEM_write_X509_REQ (fp, req) != 1)
			return FAIL ("Error while writing request");
		fclose (fp);
		return true;
	}
};

int
main (int argc, char *argv[])
{
  int i;
  EVP_MD *digest;


/*
	Before anything seed the prng, and init the library
*/
	SSL_global_server_eg ssl_serv; // needed to init subsystems

	//
	// use a plain text passphrase for the client key on disk
	//
	const EVP_CIPHER * enc = CLIENT_KEY_CIPHER;
	obj_hold<CB_get_phrase_blk1> cb_phrase = new CB_get_phrase_blk1(enc);
	cb_phrase->set_phrase( CLIENT_KEY_SECRET );
	cb_phrase->debug_set_obj_name( "CLIENT_KEY_SECRET_" );

	// create a new REQ 

	obj_hold<REQ_util> REQ = new REQ_util( cb_phrase );

	// create a new RSA for the client (unless it has one prepared)

	if(!REQ->client_rsa->gen_RSA_1024()) return FAIL_FAILED();
//	if(!REQ->client_rsa->gen_RSA_2048()) return FAIL_FAILED();

	// write that RSA to a PEM file, via passphrase encryptor

// gdb_invoke(true);

	if(!REQ->client_rsa->write_PrivateKey( CLIENT_KEY_FILE ))
		return FAIL_FAILED();


	// read that RSA back in ... just for fun you know ...

	if(!REQ->client_rsa->read_PrivateKey( CLIENT_KEY_FILE ))
		return FAIL_FAILED();

	// must init subj before adding fields
	// could make KEY a CTOR parameter

	if(!REQ->init_req_with_pub_key()) return FAIL_FAILED();

// gdb_invoke(true);

	for (i = 0; i < N_entries; i++)
	{
		if(!REQ->add_subject_longName_value( 
				entries[i].key,
				entries[i].value
		)) return FAIL_FAILED();
	}


	if(!REQ->set_subject_name()) return FAIL_FAILED();

/* add an extension for the FQDN we wish to have */
	if(!REQ->add_ext(
	    "subjectAltName",
	    CLIENT_subjectAltName
	)) return FAIL_FAILED();

	if(!REQ->add_ext_DNS( CLIENT_subjectAltName_DNS2))
		return FAIL_FAILED();

	// actually I think that the CA should do this
 if(0)
	if(!REQ->add_ext(
	    "subjectAltName",
	    "CA:true"
	)) return FAIL_FAILED();

 if(0)
	if(!REQ->add_ext_CA( true, 2 ) ) return FAIL_FAILED();

	if(!REQ->add_CN( "HSID_4321" ) ) return FAIL_FAILED();

	if(!REQ->add_extensions()) return FAIL_FAILED();

	if(!REQ->client_sign()) return FAIL_FAILED();

	if(!REQ->write_req_to_file( CLIENT_REQ_FILE )) return FAIL_FAILED();

/* write the completed request */

  return 0;
}
