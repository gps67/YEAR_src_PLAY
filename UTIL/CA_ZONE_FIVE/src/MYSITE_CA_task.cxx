//////////////////////////////

#include "dgb.h"
#include "buffer2.h"

#include "MYSITE_CA_task.h"
#include "X509_VALS_base.h"
using namespace CA1;

#include "EVP_P_KEY.h"
#include "X509_REQ_hold.h"


MYSITE_CA_task::
MYSITE_CA_task(
	SITE_X509_layout * _layout,
	SITE_X509_tag_enum _tag,
	STR0 CN
)
: CA_task(_layout, _tag)
{
	VALS->subject.CN = CN;
}

MYSITE_CA_task::
~MYSITE_CA_task()
{
}

/*!
	You define most of the subject prefix here (virtual)
	
	Why isnt this part of the layout?
	Seems that both classes need to be adjusted to MYSITE,
	but they are different (as in TYPE/VALUE)
*/
bool MYSITE_CA_task:: VALS_set_HERE_part1_subject()
{
	/*
		This is site specific for layout_tag (which includes scheme)
		So are all the functions here ...
	*/
//	VALS->subject.CN	= "-UNSET-";
 switch(layout_tag->tag) {
  case is_CA_ZERO:
  case is_CA_ONE:
	VALS->subject.C		= "GB";		
	VALS->subject.ST	= "Berkshire";
	VALS->subject.O		= "Layout MYSITE Information Cascade";
	VALS->subject.OU	= "WeDesignYourCA";
//	VALS->subject.CN	= "-UNSET-";
//	VALS .tokenised_sprintf( "fmt", argc, argv )
//	VALS.TOKENS.VALS // SUBLEX // "TOKEN" // CODE_RUNNER // ARGS // TXTS //
//	TOKEN sprintf_filter TOKEN_POOL u8_idx IDX
//	filter Name SPELLING PSG_SUBLEX_EXPR _DATA_ ARGS
//	SUBST {Name} TOKEN_Name 
//	EXPR argc, argv // CODE_POINT HERE = { ... } //
//	SUBST {Name} Module EVAL_VIEW_EXPR_POOL CSR_t CODE_POINT SUBST_RULE
//	PSG SUBST RULE // CODE_POINT POOL_CONST_EXPR _KNOW_API
//
//	PSG_SUBLEX MATCH ACTION = PATCH // Filter Mechansim PATCH
//	PSG_SUBLEX MATCH ACTION = MATCH // Filter Mechansim PATCH
//	PSG_SUBLEX MATCH ACTION = CTXT.var_name.expr // {Name} // TOKEN_t //
//	PSG_SUBLEX "TOKEN_t TOKEN = "{EXPR}" // PARSED _INTO_ //
//
// TIERED CODE
//
//	DECL_VAR SCOPE Name CT_DATA RT_DATA MMAP_PAGE_FILE HEAP; // =default
//	"Item_t * ea_item = EXPR; // ..."
//	EDIT finds VAR_POOL_TOKEN EXPR
//	
//	

  break;
  default:
	VALS->subject.C		= "GB";		
	VALS->subject.ST	= "Berks";
	VALS->subject.O		= "MYSITE VLAN PLUS";
	VALS->subject.OU	= "DEVELOPMENT";
//	VALS->subject.CN	= "-UNSET-";
 }
	return true;
}

bool MYSITE_CA_task:: VALS_set_HERE_part1_time_range()
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

		Here is a complete list of is_ _C_item
		// SUBLEX SPLICE is_C_item // collapse __ to _
		// NAME_MANGLER_FILTER Language_To_FileName
		// PSG with _added_PSG TOKEN .ATTR // this list of EXPR TEXT
		// TEXT = SCRIPT
		// ENUM = LIST

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
	VALS->iss_misc.ISS_time_range.set_now_and_ndays( days );
	return PASS("days = %d", days );
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
	if(!VALS_set_HERE_part0( sometag )) return FAIL_FAILED();

	SOME then have manually placed checks
        if(!VALS_check_C_not_CA()) return FAIL_FAILED();
*/

// DEBUG PREFIX - see which CA's are being used, with abbreviated CN=
// #define PFX_CA "PFXCA_"
// #define PFX_CA "CA91_"
#define PFX_CA "CA_"
bool MYSITE_CA_task:: set_HERE_CA_ZERO()
{
	if(!VALS_set_HERE_part0(is_CA_ZERO)) return FAIL_FAILED();
	VALS->subject.CN = PFX_CA "ZERO";
	VALS->key_usage.keyCertSign = true;
	if(!VALS_check_CA_not_C()) return FAIL_FAILED();
	return true;
}

bool MYSITE_CA_task:: set_HERE_CA_ONE()
{
	if(!VALS_set_HERE_part0(is_CA_ONE)) return FAIL_FAILED();
	VALS->subject.CN = PFX_CA "ONE";
	VALS->key_usage.keyCertSign = true;
	if(!VALS_check_CA_not_C()) return FAIL_FAILED();
	return true;
}

bool MYSITE_CA_task:: set_HERE_CA_ZONE( const char * CN_ZONE_NAME )
{
	if(!VALS_set_HERE_part0(is_CA_ZONE)) return FAIL_FAILED();
	VALS->subject.CN = CN_ZONE_NAME; // "CA_ZONE_BOX5"
	buffer2 OU_str;
// MATCH DETECT KEYWORDS PHRASES
// FILTER SOURCE PAIR = { LHS RHS } // GEN VARNAME = %s
//	OU_str.print("%s %s", (STR0) VALS->subject.OU, (STR0) CN_ZONE_NAME );
	OU_str.print("%s %s", (STR0) "Dept that uses", (STR0) CN_ZONE_NAME );
	VALS->subject.OU = OU_str; // "Users of CA_ZONE_BOX5"
	VALS->key_usage.keyCertSign = true;
	if(!VALS_check_CA_not_C()) return FAIL_FAILED();
	return true;
}

bool MYSITE_CA_task:: set_HERE_C_server(
	const char * CN_server,
	const char * DNS_NAME_1, // FQDN 
	const char * DNS_NAME_2 // local VLAN
)
{
	if(!VALS_set_HERE_part0(is_C_server)) return FAIL_FAILED();
	if(!VALS_check_C_not_CA()) return FAIL_FAILED();
	//
	// what fields hold what - is site specific - you replace this class
	//
	VALS->subject.CN = CN_server;	// "BOX5"
	VALS->subjectAltName.DNS_1 = DNS_NAME_1;
	// eg "box5.information-cascade.co.uk";
	VALS->subjectAltName.DNS_2 = DNS_NAME_2; // "box5.vlan2.local";
	return true;
}

/*!
	You will certainly want to change the layout of USER certificates,
	and here is a part of were you do it, with a simple example.

	TODO ADD PARAMETER SUBLEX_in_filename "{CN}" "db_x509/{O}/{OU}/..."
	STR0 fmt = "db_X509/${O}/${OU}/C_user_${CN}_cert.pem"; // filename
	// REQUIRE CN = for_use_in_filename(CN); // UPGRADE // DIP
	// or do that from outside %s = ${CN} = CN
	// provide world where %s varname works
*/
bool MYSITE_CA_task:: set_HERE_C_user(
	const char * CN_user,	// "USER_231";
	const char * EMAIL	// "lists@information-cascade.co.uk";
)
{
	if(!VALS_set_HERE_part0(is_C_user)) return FAIL_FAILED();
	if(!VALS_check_C_not_CA()) return FAIL_FAILED();
	//
	VALS->subject.CN = CN_user;
	VALS->subjectAltName.EMAIL_1 = EMAIL;
	VALS->key_usage.digitalSignature = true;
	return true;
}

bool MYSITE_CA_task:: set_HERE_C_pc(
	const char * CN_PC
)
{
	if(!VALS_set_HERE_part0(is_C_pc)) return FAIL_FAILED();
	if(!VALS_check_C_not_CA()) return FAIL_FAILED();
	//
	VALS->subject.CN = CN_PC;

	buffer2 url;
	url.print("http://www.%s/home/%s/",
		"information-cascade.co.uk",	// hardcoded but OK
		(STR0) CN_PC
	);
	VALS->subjectAltName.URI_1 = url;
	return true;
}

bool MYSITE_CA_task:: set_HERE_C_item(
	const char * CN_ITEM
)
{
	if(!VALS_set_HERE_part0(is_C_item)) return FAIL_FAILED();
	if(!VALS_check_C_not_CA()) return FAIL_FAILED();
	//
	VALS->subject.CN = CN_ITEM;
	return true;
}


//////////////////////////////

/*!
	Create a CERT from VALS and from the parent cert/key/phrase

	The parent CA cert.key is loaded by
*/
bool MYSITE_CA_task:: MYSITE_mk_cert_from_VALS()
{

	if(!REQ_mk_cert_from_VALS()) return FAIL_FAILED();

#define TRACE_CLIENT_KEY_FILE "trace_client_req_key.pem"
#define TRACE_CLIENT_REQ_FILE "trace_client_req_cert.pem"


	/*
		We dont need to do this - but it tests that the key is PRIV
		We do need to communicate the KEY back to the caller (+phrase)
		but that is done later with the resulting cert, after ISS.
	*/
	if(1)
	if(!client_key_priv->test_write_read_PrivateKey( TRACE_CLIENT_KEY_FILE ))
		return FAIL_FAILED();

	/*
		We now have a valid X509_REQ to XMIT to the CA
		(without the private key), and ask it to sign it.
		It presumably would load it (from NET or file)
	*/

	if(1)
	if(!REQ->write_req_to_file( TRACE_CLIENT_REQ_FILE )) return FAIL_FAILED();


	/*
		DO THE ISS WORK
	*/
	if(!ISS_mk_cert()) return FAIL_FAILED();

	if(!ISS_write_client_cert_and_key()) return FAIL_FAILED(); // TEST

  STR0 cn = "GET_CN";
  return PASS("DONE %s", cn);;
  return true;
}


//////////////////////////////

/*
	You will actually modify and use set_HERE_...
	but as an axample of the parameters to pass to those,
	here is a parameterless DEMO thing

	The idea is that all of the _HERE_ functions, are real.
	Each is parametised, and sufficient to create items,
	for your site (possibly with an added ZONE parameter,
	and a file-loaded from ./ZONE/.).

	The _DEMO_ is _NOT_ realistic, not for LIVE programs,
	but it might help debugging and things
*/
bool MYSITE_CA_task:: MYSITE_SET_DEMO_VALUE_FOR( SITE_X509_tag_enum ISS_tag, STR0 CN1 )
{
//
// Vigil Keeping by Saffie
//	https://open.spotify.com/track/0ha55LHYe49no72wmpQxst
//
	static u32 N_users = 0;
	static u32 N_items = 0;
	static u32 N_zones = 2; // P1 == P2 = NEXT == "FIVE"
	static u32 N_boxes = 3; // 
	bool ok = true;
	buffer2 NAME;

	switch(ISS_tag) {
	 case is_CA_ZERO:  ok = set_HERE_CA_ZERO(); break;
	 case is_CA_ONE:   ok = set_HERE_CA_ONE(); break;
	 case is_CA_ZONE: {
	 	buffer2 NAME;
		NAME.print( PFX_CA "ZONE_" ); // "CA_"
		u32 idx = N_zones ++;
		switch(idx) {
		 case 0: NAME += "ZERO_RESERVED"; break;
		 case 1: NAME += "ONE_RESERVED"; break;
		 case 2: NAME += "VPN"; break;
		 case 3: NAME += "THREE"; break;
		 case 4: NAME += "FOUR"; break;
		 case 5: NAME += "FIVE"; break;
		 case 6: NAME += "SIX"; break;
		 case 7: NAME += "SEVEN"; break;
		 case 8: NAME += "EIGHT"; break;
		 case 9: NAME += "NINE"; break;
		 case 10: NAME += "TEN"; break;
		 default:
		 	NAME.print("%d", idx);
		}

	 	ok = set_HERE_CA_ZONE((STR0)NAME);
	 } break;
	 case is_C_server: {
		u32 idx = N_boxes ++;
		NAME.print("BOX%d", idx );
	 	ok = set_HERE_C_server(NAME);
	 } break;
	 case is_C_pc: {
	  // static buffer2 cn;
	  // cn.clear();
	  // cn.print( "LAPTOP_%4.4d", 1010+N_items++ );
	  ok =  set_HERE_C_pc(CN1);
	 }
	 break;
	 case is_C_user: {
	 // this is inside out
	  switch( N_users ++ ) {
	   case 0:
	 		   ok =  set_HERE_C_user(
					"USER1016",
					"lists@information-cascade.co.uk"
				);
	   case 1:
	 		   ok =  set_HERE_C_user(
					"USER1025",
					"user@BOX5.VLAN_5.local"
				);
	   case 2:
	 		   ok =  set_HERE_C_user(
					"USER1035",
					"user@BOX5.VLAN_5.local"
				);
	   default:
	   		N_users--;
	  }
	 } break;
	 case is_C_item: {
	  static buffer2 cn;
	  cn.clear();
	  cn.print( "ITEMx%4.4X", N_items++ );
	  ok =  set_HERE_C_item(cn);
	 }
	 break;

	 default:
		return FAIL("ISS_tag not recognised");
	}
	if(!ok) return FAIL_FAILED();
	return ok;
}


