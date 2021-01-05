#ifndef fd_ssl_client_H
#define fd_ssl_client_H

#include "fd_sel_ssl.h"


/*!
	the start of something interesting?
*/
class fd_ssl_client : public fd_sel_ssl
{
 public:
	//! constructor - init NULL
	fd_ssl_client();
	~fd_ssl_client();

	virtual void avail_space();
	virtual void avail_data();
	void ssl_says_connected();

	int test1_Main( SSL_global_base & SSL );
	static void test1( SSL_global_base & SSL );

	virtual bool class_is_client() { return true; }
};

#endif
