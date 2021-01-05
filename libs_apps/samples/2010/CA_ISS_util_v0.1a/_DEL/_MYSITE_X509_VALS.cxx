//////////////////////////////

#include "dgb.h"
#include "buffer2.h"

#include "MYSITE_X509_VALS.h"
using namespace CA1;


MYSITE_X509_VALS::
MYSITE_X509_VALS( SITE_X509_layout * _layout )
: SITE_X509_VALS_base(_layout)
{
}

MYSITE_X509_VALS::
~MYSITE_X509_VALS()
{
}

/*!
	You define most of the subject prefix here (virtual)
	
	Why isnt this part of the layout?
	Seems that both classes need to be adjusted to MYSITE,
	but they are different (as in TYPE/VALUE)
*/
bool MYSITE_X509_VALS:: set_HERE_part1_subject()
{
	/*
		This is site specific for layout_tag (which includes scheme)
		So are all the functions here ...
	*/
	VALS.subject.CN	= "-UNSET-";
 switch(layout_tag->tag) {
  case is_CA_ZERO:
  case is_CA_ONE:
	VALS.subject.C	= "GB";		
	VALS.subject.ST	= "Berkshire";
	VALS.subject.O	= "Facilities Management Comp";
	VALS.subject.OU	= "RNDCA";
//	VALS.subject.CN	= "-UNSET-";
  break;
  default:
	VALS.subject.C	= "GB";		
	VALS.subject.ST	= "Berks";
	VALS.subject.O	= "Information Cascade Ltd";
	VALS.subject.OU	= "DEVELOPMENT";
//	VALS.subject.CN	= "-UNSET-";
 }
	return true;
}

bool MYSITE_X509_VALS:: set_HERE_part1_time_range()
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
	 case is_CA_ONE:	days = 365 * 3.2;	break;
	 case is_CA_ZONE:	days = 365 * 1.6;	break;
	 case is_CA_server:	days = 365 * 1.2;	break;
	 case is_CA_user:	days = 365 * 1.2;	break;
	 case is_CA_pc:		days = 365 * 1.2;	break;
	 case is_CA_item:	days = 365 * 1.2;	break;
	 case is_C_server:	days = 365 * 1.2;	break;
	 case is_C_user:	days = 365 * 1.2;	break;
	 case is_C_pc:		days = 365 * 1.2;	break;
	 case is_C_item:	days = 365 * 1.2;	break;
	 default: 		days = 7;	 	break;
	}
	VALS.iss_misc.ISS_time_range.set_now_and_ndays( days );
	return PASS_1("days = %d", days );
	return true;
}

// set VALS for when HERE is CA_ZERO

/*
	These set_HERE functions,
	are supposed to be customised to the site,
	but then should handle multiple instances,
	beyond the one DEMO value.

	The functions accept parameters,
	to distinguish each instance.

	ALL of the functions start with some initialiser
	if(!set_HERE_part0( sometag )) return FAIL_FAILED();

	SOME then have manually placed checks
        if(!check_C_not_CA()) return FAIL_FAILED();
*/

bool MYSITE_X509_VALS:: set_HERE_CA_ZERO()
{
	if(!set_HERE_part0(is_CA_ZERO)) return FAIL_FAILED();
	VALS.subject.CN = "CA_ZERO";
	VALS.key_usage.keyCertSign = true;
	if(!check_CA_not_C()) return FAIL_FAILED();
	return true;
}

bool MYSITE_X509_VALS:: set_HERE_CA_ONE()
{
	if(!set_HERE_part0(is_CA_ONE)) return FAIL_FAILED();
	VALS.subject.CN = "CA_ONE";
	VALS.key_usage.keyCertSign = true;
	if(!check_CA_not_C()) return FAIL_FAILED();
	return true;
}

bool MYSITE_X509_VALS:: set_HERE_CA_ZONE( const char * CN_ZONE_NAME )
{
	if(!set_HERE_part0(is_CA_ZONE)) return FAIL_FAILED();
	VALS.subject.CN = CN_ZONE_NAME; // "CA_ZONE_BOX5"
	VALS.key_usage.keyCertSign = true;
	if(!check_CA_not_C()) return FAIL_FAILED();
	return true;
}

bool MYSITE_X509_VALS:: set_HERE_C_server(
	const char * CN_server,
	const char * DNS_NAME_1, // FQDN 
	const char * DNS_NAME_2 // local VLAN
)
{
	if(!set_HERE_part0(is_C_server)) return FAIL_FAILED();
	if(!check_C_not_CA()) return FAIL_FAILED();
	//
	// what fields hold what - is site specific - you replace this class
	//
	VALS.subject.CN = CN_server;	// "BOX5"
	VALS.subjectAltName.DNS_1 = DNS_NAME_1;
	// eg "box5.information-cascade.co.uk";
	VALS.subjectAltName.DNS_2 = DNS_NAME_2; // "box5.vlan2.local";
	return true;
}

/*!
	You will certainly want to change the layout of USER certificates,
	and here is a part of were you do it, with a simple example.
*/
bool MYSITE_X509_VALS:: set_HERE_C_user(
	const char * CN_user,	// "USER_231";
	const char * EMAIL	// "lists@information-cascade.co.uk";
)
{
	if(!set_HERE_part0(is_C_user)) return FAIL_FAILED();
	if(!check_C_not_CA()) return FAIL_FAILED();
	//
	VALS.subject.CN = CN_user;
	VALS.subjectAltName.EMAIL_1 = EMAIL;
	VALS.key_usage.digitalSignature = true;
	return true;
}

bool MYSITE_X509_VALS:: set_HERE_C_pc(
	const char * CN_PC
)
{
	if(!set_HERE_part0(is_C_pc)) return FAIL_FAILED();
	if(!check_C_not_CA()) return FAIL_FAILED();
	//
	VALS.subject.CN = CN_PC;

	buffer2 url;
	url.print("http://www.%s/home/%s/",
		"information-cascade.co.uk",	// hardcoded but OK
		(STR0) CN_PC
	);
	VALS.subjectAltName.URI_1 = url;
	return true;
}

bool MYSITE_X509_VALS:: set_HERE_C_item(
	const char * CN_ITEM
)
{
	if(!set_HERE_part0(is_C_item)) return FAIL_FAILED();
	if(!check_C_not_CA()) return FAIL_FAILED();
	//
	VALS.subject.CN = CN_ITEM;
	return true;
}


//////////////////////////////

/*
	You will actually modify and use set_HERE_...
	but as an axample of the parameters to pass to those,
	here is a parameterless DEMO thing
*/
bool MYSITE_X509_VALS:: set_DEMO_VALUE_FOR( SITE_X509_tag_enum ISS_tag )
{
	bool ok = true;
	switch(ISS_tag) {
	 case is_CA_ZERO:  ok = set_HERE_CA_ZERO(); break;
	 case is_CA_ONE:   ok = set_HERE_CA_ONE(); break;
	 case is_CA_ZONE:  ok = set_HERE_CA_ZONE("CA_ZONE_BOX5"); break;
	 case is_C_server: ok = set_HERE_C_server("BOX5"); break;
	 case is_C_pc:     ok = set_HERE_C_pc("LAPTOP_1010"); break;
	 case is_C_user:   ok =  set_HERE_C_user(
					"USER1016",
					"lists@information-cascade.co.uk"
				);
				break;
	 case is_C_item: ok =  set_HERE_C_item("ITEM9871"); break;
	 default:
		return FAIL("ISS_tag not recognised");
	}
}

