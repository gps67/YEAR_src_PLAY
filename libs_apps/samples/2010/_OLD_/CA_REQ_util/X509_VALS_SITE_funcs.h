#ifndef X509_VALS_SITE_funcs_H
#define X509_VALS_SITE_funcs_H

#include "X509_VALS_base.h"

class X509_VALS_SITE_funcs : public X509_VALS_base 
{
 public:
	~X509_VALS_SITE_funcs();	// is obj_ref based
	X509_VALS_SITE_funcs();

	virtual bool set_cert_values();

	bool set_CA_depth_for_CA_ZERO();
	bool set_CA_depth_for_CA_ONE();
	bool set_CA_depth_for_CA_ZONE();
	bool set_C_not_CA();

	// these funcs belong HERE not any other SITE
	// at worst they need a repeating of all the values
	// but really the values come from a config file
	// and from calling the one above, then changing things

	bool set_HERE_part1();
	bool set_HERE_CA_ZERO();
	bool set_HERE_CA_ONE();
	bool set_HERE_CA_ZONE(); // box5 name required
	bool set_HERE_C_server(); // server name and some flag // DNS:
	bool set_HERE_C_user(); // user name and some flag // EMAIL:
	bool set_HERE_C_pc(); // user name and some flag // EMAIL:
	bool set_HERE_C_item(); // item name and some flag // RID

} ;

#endif
