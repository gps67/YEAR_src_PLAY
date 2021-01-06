#ifndef MYSITE_CA_task_H
#define MYSITE_CA_task_H

struct buffer2;
#include "SSL_CA1_stubs.h"
#include "SITE_X509_layout.h"
#include "SITE_CA_task.h"

namespace CA1 {

/*!
	You call your class MYSITE_CA_tas : public MYSITE_CA_tas
	and do much the same as this code 
*/
class MYSITE_CA_task : public CA_task
{
 public:
	MYSITE_CA_task( SITE_X509_layout * _layout, SITE_X509_tag_enum tag );
	~MYSITE_CA_task();
 virtual
	bool VALS_set_HERE_part1_subject();
 virtual
	bool VALS_set_HERE_part1_time_range();

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

	bool MYSITE_SET_DEMO_VALUE_FOR( SITE_X509_tag_enum ISS_tag );

	bool MYSITE_mk_cert_from_VALS();
} ;

}; // namespace
#endif

