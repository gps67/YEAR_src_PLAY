
#include "dgb.h"
#include "buffer2.h"

#include "SITE_X509_VALS_base.h"
using namespace CA1;

SITE_X509_VALS_base:: ~SITE_X509_VALS_base()
{
}

SITE_X509_VALS_base:: SITE_X509_VALS_base( SITE_X509_layout * _layout )
: _VALS( new X509_VALS_base() )
, VALS( *_VALS )
{
	// layout_tag == NULL means not yet set !
	// layout_tag = new SITE_X509_tag( );
	if( _layout) {
		if(!set_SITE_layout(_layout)) {
			FAIL_FAILED();
			throw "NULL FROM CTOR";
		}
	}
}

//	SITE_X509_VALS_base:: SITE_X509_VALS_base()
//	: _VALS( new X509_VALS_base() )
//	, VALS( *_VALS )
//	{
//		// layout_tag == NULL means not yet set !
//		// layout_tag = new SITE_X509_tag( );
//	}

bool SITE_X509_VALS_base:: set_SITE_layout(SITE_X509_layout * _layout)
{
	layout_scheme = _layout;
	if(!layout_scheme) return FAIL("NULL layout_scheme");
	layout_tag = layout_scheme->new_SITE_X509_tag(is_C_UNSET); // DUMMY
	if(!layout_tag) return FAIL("NULL layout_tag");
//	layout_tag = NULL;
	PASS("init layout_tag");
	return true;
}

bool SITE_X509_VALS_base:: check_C_not_CA()
{
	int depth1 = layout_tag->CA_depth();
	int depth2 = VALS.CA_depth.basic_constraint_pathLen;
	if(depth1!=depth2) {
		return FAIL_2("expected depth1 = depth2 got %d <> %d", depth1, depth2 );
	}
	if(depth1) {
		return FAIL_1("expected depth 0 got %d", depth1 );
	}
	return true;
}

bool SITE_X509_VALS_base:: check_CA_not_C()
{
	int depth1 = layout_tag->CA_depth();
	int depth2 = VALS.CA_depth.basic_constraint_pathLen;
	if(depth1!=depth2) {
		return FAIL_2("expected depth1 = depth2 got %d <> %d", depth1, depth2 );
	}
	if(!depth1) {
		return FAIL("expected nonzero depth" );
	}
	return true;
}


/*!
	Your SITE calls this every time, as part of initialisation,
	it clears everything, sets some decent defaults then calls
	part1_subject to set the subject (virtual function).

	After this you can set the expire time to more than a week.
*/
bool SITE_X509_VALS_base:: set_HERE_part0(SITE_X509_tag_enum tag)
{
	VALS.clear();
	if(!layout_scheme) return FAIL("NULL layout_scheme");

	/*
		THE layout_tag is of SITE specific class HERE !!
	*/
	layout_tag = layout_scheme->new_SITE_X509_tag( tag );
	if(!layout_tag)  return FAIL("NULL layout_tag from CTOR");

	if(!set_CA_depth_for_layout()) return FAIL_FAILED();

	// the subject prefix is set early
	// so that the precedence layers have an EARLY preset value to override
	if(!set_HERE_part1_subject()) return FAIL_FAILED();

	if(!set_HERE_part1_time_range()) return FAIL_FAILED();
	return true;
}

bool SITE_X509_VALS_base:: set_CA_depth_for_layout()
{
	int depth = layout_tag->CA_depth();
	if(!VALS.CA_depth.set_CA_pathlen( depth )) return FAIL_FAILED();
	return true;
}


///////////////////////////////////////////////////////////

/*!
	You define most of the subject prefix here (virtual)

	This is default background, that you then put CN (and OU) on top of,
	but it is useful to have something.

	This should not be defined in the base class,
	but if you are really lazy, and linking something ELSE, ...
	
	Why isnt this part of the layout?
	Seems that both classes need to be adjusted to MYSITE,
	but they are different (as in TYPE/VALUE)
*/
bool SITE_X509_VALS_base:: set_HERE_part1_subject()
{
	/*
		This is site specific for layout_tag (which includes scheme)
		So are all the functions here ...
	*/
	VALS.subject.CN	= "-UNSET-"; // calling this default LATE does this!
	switch(layout_tag->tag) {
	  case is_CA_ZERO:
	  case is_CA_ONE:
		VALS.subject.C	= "GB";		
		VALS.subject.ST	= "Berkshire";
		VALS.subject.O	= "Facilities Management Comp";
		VALS.subject.OU	= "WeRunYOurEmail";
	  break;
	  default:
		VALS.subject.C	= "GB";		
		VALS.subject.ST	= "Berks";
		VALS.subject.O	= "Information Cascade Ltd";
		VALS.subject.OU	= "DESIGN then DEVELOPMENT";
	}
	return true;
}

/*!
	default values for how long a certificate should last for.

	You can also look at the time functions (in libs_apps),
	and do things like 'slide_forwards_to_friday' or
	midnight, or month end, etc.

	base/time/tm_parts.h
	step_to_first_of_next_month_local()

	If there is a delay between REQ and ISS the absolute times
	are used! (actually the relative times are used, and these
	absolute times are adjusted down against time(NULL), hey-ho)

	I.E. you can exactly set the UST start and end times,
	you just have to write the logic that knows when that is,
	and the logic that sets it in the VALS fields (easy);
*/
bool SITE_X509_VALS_base:: set_HERE_part1_time_range()
{
	/*
		This is up for discussion, but its virtual, end of.

		If you use a PKI plain, you need to obtain the chain.
		That may mean the chain is remembered on the client.

		On the server there is a question of access to old certs,
		which have been replaced, but not expired, not CRL'd.
		IE early pre-emptive replacement every 6 months,
		when their fruit remains for 2 years.

		Things like printers dont need an aggressive update rate.
	*/

	int days = 2;
	switch(layout_tag->tag) {
	 case is_CA_ZERO:	days = 365 * 10.2;	break;
	 case is_CA_ONE:	days = 365 * 1.2;	break;
	 case is_CA_ZONE:	days = 365 * 1.1;	break;
	 case is_CA_server:	days = 365 * 1.0;	break;
	 case is_CA_user:	days = 365 * 1.0;	break;
	 case is_CA_pc:		days = 365 * 1.0;	break;
	 case is_CA_item:	days = 365 * 1.0;	break;
	 case is_C_server:	days = 365 * 1.0;	break;
	 case is_C_user:	days = 365 * 1.0;	break;
	 case is_C_pc:		days = 365 * 1.0;	break;
	 case is_C_item:	days = 365 * 1.0;	break;
	 default: 		FAIL("TAG not recognised");
				days = 7;	 	break;
	}
	VALS.iss_misc.ISS_time_range.set_now_and_ndays( days );
	return PASS_1("days = %d", days );
	return true;
}

// virtual
bool SITE_X509_VALS_base:: create_client_key_pair() // in layout_tag holder
{
}

