#ifndef SITE_X509_VALS_base_H
#define SITE_X509_VALS_base_H

struct buffer2;
#include "SSL_CA1_stubs.h"
#include "X509_VALS_base.h"
#include "SITE_x509_layout.h" // base class

namespace CA1 {

/*!
	SITE_X509_VALS_base holds VALS and layout and KNOWS functions ...

	I am having a problem with this class name,
	because the VALS is not a base class of it,
	so this is a VALS holder, or VALS-FOR-THIS-THING-on-this-site
	I think it (maybe) should hold additional things such as the
	phrase or the issuer cert or ... be the REQ transaction holder

	Actually, SITE_X509_tag and this are so close,
	they might be derived ...

	This class knows that C_user must have an EMAIL (fixed)
	and a CN="USER_1234", within the domain of the issuer zone

	It also knows the file layout, on the client, on the server,
	and for requests on the issuing machine, or at least it
	goes a long way towards doing so ...

*/
class SITE_X509_VALS_base : public obj_ref0 // X509_VALS_base 
{
 public:

	~SITE_X509_VALS_base();	// is obj_ref based
//	SITE_X509_VALS_base();
	SITE_X509_VALS_base( SITE_X509_layout * _layout );

	/*
		The SITE_X509_VALS_base acts on X509_VALS_BASE
		but without it being a subclass, just a pointer
	*/
//	obj_hold<X509_VALS_base> _VALS;
	X509_VALS_base * _VALS;
	X509_VALS_base & VALS;

	/*
		The SITE_X509_VALS_base exists before we know what the tag will be
		set_HERE_part_1 uses _scheme to build _tag (pos on scheme)
		after that only use _tag
	*/
	obj_hold<SITE_X509_layout> layout_scheme; // held to make layout_tag
	obj_hold<SITE_X509_tag> layout_tag;

	/*
		This VALS setter will use layout (and its VTL fns)
	*/
	bool set_SITE_layout(SITE_X509_layout * _layout);

	/*
		your class will also be quite generic
	*/
	bool set_CA_depth_for_layout();

	/*
		with this helpful checker (debug)
	*/
	bool check_C_not_CA();
	bool check_CA_not_C();

	// these funcs belong HERE not any other SITE
	// at worst they need a repeating of all the values
	// but really the values come from a config file
	// and from calling the one above, then changing things

	bool set_HERE_part0(SITE_X509_tag_enum _tag);
	// now the layout tag is set .. no need for the parameter
 virtual
	bool set_HERE_part1_subject();
 virtual
	bool set_HERE_part1_time_range();

 virtual
	bool create_client_key_pair(); // asks layout to do it!

} ;

}; // namespace
#endif
