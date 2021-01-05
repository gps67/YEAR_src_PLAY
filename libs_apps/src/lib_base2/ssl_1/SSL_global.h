#ifndef SSL_global_H
#define SSL_global_H
// #include "SSL_global_base.h"
#include "SSL_global_util.h"

class SSL_global_server_eg : public SSL_global_util
{
 public:
	SSL_global_server_eg();
	~SSL_global_server_eg();
	virtual bool class_is_usually_client();
	virtual bool set_ssl_verification();
};

class SSL_global_client_eg : public SSL_global_util
{
 public:
	SSL_global_client_eg();
	~SSL_global_client_eg();
	virtual bool class_is_usually_client();
	virtual bool set_ssl_verification();
};

#endif

