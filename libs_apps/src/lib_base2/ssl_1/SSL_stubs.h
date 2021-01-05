#ifndef SSL_stubs_H
#define SSL_stubs_H

extern "C" {
	// stubs
	// these can easily be edited when openssl changes
	struct ssl_ctx_st;
	struct ssl_st;
	struct x509_store_st;
	struct x509_store_ctx_st;
	struct x509_lookup_st;
	struct x509_st;
	struct evp_pkey_st;
	struct evp_cipher_st;
	struct evp_md_st; // adding this g ?

	typedef struct ssl_ctx_st SSL_CTX;
	typedef struct ssl_st SSL;
	typedef struct x509_store_ctx_st X509_STORE_CTX;
	typedef struct x509_store_st X509_STORE;
	typedef struct x509_lookup_st X509_LOOKUP;
	typedef struct x509_st X509;
	typedef struct evp_pkey_st EVP_PKEY;
	typedef struct evp_cipher_st EVP_CIPHER;
	typedef struct evp_md_st EVP_MD;
	typedef struct evp_md_ctx_st EVP_MD_CTX; // where evp_md_st_ctx news
};

class fd_sel_ssl; // 
class CB_get_phrase_base; // 
class X509_cert;
class EVP_P_KEY;
class SSL_global_util;

#endif

