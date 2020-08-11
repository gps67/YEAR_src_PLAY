#ifndef SITE_X509_VALS_H
#define SITE_X509_VALS_H

struct buffer2;
#include "SSL_CA1_stubs.h"
#include "SITE_X509_VALS_base.h"

namespace CA1 {

/*!
	You call your class MYSITE_X509_VALS : public SITE_X509_VALS_base
	and do much the same as this code 
*/
class MYSITE_X509_VALS : public SITE_X509_VALS_base
{
 public:
	MYSITE_X509_VALS( SITE_X509_layout * _layout );
	~MYSITE_X509_VALS();
 virtual
	bool set_HERE_part1_subject();
 virtual
	bool set_HERE_part1_time_range();

	bool set_HERE_CA_ZERO();
	bool set_HERE_CA_ONE();
	bool set_HERE_CA_ZONE( const char * CN_ZONE_NAME );

	bool set_HERE_C_server( // server name and some flag // DNS:
		const char * CN_server,
		const char * DNS_FQDN_1 = NULL,
		const char * DNS_NAME_2 = NULL
	);
	bool set_HERE_C_user( // user name and some flag // EMAIL:
		const char * CN_user,
		const char * EMAIL
	);
	bool set_HERE_C_pc( // user name and some flag // EMAIL:
		const char * CN_PC
	);
	bool set_HERE_C_item( // item name and some flag // RID
		const char * CN_ITEM
	);

	bool set_DEMO_VALUE_FOR( SITE_X509_tag_enum ISS_tag );
} ;

}; // namespace
#endif
