#include "SITE_CA_task.h" // base class
#include "dgb.h"
#include "dgb_fail.h"
#include "buffer2.h"
#include "EVP_P_KEY.h"
#include "X509_cert.h" // tag keys
#include "X509_VALS_base.h" 
#include "X509_REQ_hold.h" 
using namespace CA1;


CA_task:: ~CA_task()
{
}

/*!
	TODO: factor out fixed class new MYSITE_loader_files()
*/
CA_task:: CA_task(
	SITE_X509_layout * scheme,
	SITE_X509_tag_enum C_tag
)
{
	clear();
	// OK its a double alloc - aargh
	layout_tag = new SITE_X509_tag( scheme, C_tag );
        if(!scheme) THROW_dgb_fail("NULL layout_scheme");

	/*
		The C_tag can actually be a CA,
		but it is the child of a CA
		(except for, but including, self signed is child of CA self)
	*/

	SITE_X509_tag_enum CA_tag = scheme->get_parent_CA_tag(C_tag);

	CB_get_phrase_base * CBP = NULL;

	CA_site_loaded = new MYSITE_loaded(CA_tag);
	CA_site_loader = new MYSITE_loader_files(CA_site_loaded, scheme, CBP );

	C_site_loaded = new MYSITE_loaded(C_tag);
	C_site_loader = new MYSITE_loader_files(C_site_loaded, scheme, CBP );

	if(!VALS_set_HERE_part0(C_tag)) THROW_dgb_fail("CTOR");
}

bool CA_task:: clear()
{
	layout_tag = NULL;

//        issuer_part_name.clear();
//        issuer_cert_filename.clear();
//        issuer_key_filename.clear();

//        client_part_name.clear();
//        client_cert_filename.clear();
//        client_key_filename.clear();

	VALS = NULL; // or new value ... ?
//	new SITE_X509_VALS_base();

	VALS = new X509_VALS_base();
	REQ = new X509_REQ_hold();

	CA_site_loaded = NULL;
	CA_site_loader = NULL;

	C_site_loaded = NULL;
	C_site_loader = NULL;

	return true;
}

bool CA_task:: VALS_set_CA_depth_for_layout()
{
        int depth = layout_tag->CA_depth();
        if(!VALS->CA_depth.set_CA_pathlen( depth )) return FAIL_FAILED();
        return true;
}

bool CA_task:: VALS_check_C_not_CA()
{
        int depth1 = layout_tag->CA_depth();
        int depth2 = VALS->CA_depth.basic_constraint_pathLen;
        if(depth1!=depth2) {
                return FAIL("expected depth1 = depth2 got %d <> %d", depth1, depth2 );
        }
        if(depth1) {
                return FAIL("expected depth 0 got %d", depth1 );
        }
        return true;
}

bool CA_task:: VALS_check_CA_not_C()
{
        int depth1 = layout_tag->CA_depth();
        int depth2 = VALS->CA_depth.basic_constraint_pathLen;
        if(depth1!=depth2) {
                return FAIL("expected depth1 = depth2 got %d <> %d", depth1, depth2 );
        }
        if(!depth1) {
                return FAIL("expected nonzero depth" );
        }
        return true;
}

bool CA_task:: VALS_set_HERE_part0(SITE_X509_tag_enum _tag)
{
        VALS->clear();
	layout_tag = layout_tag->layout->new_SITE_X509_tag( _tag );
	if(!layout_tag)  return FAIL("NULL layout_tag from CTOR");
        if(!VALS_set_CA_depth_for_layout()) return FAIL_FAILED();

        /*
                THE layout_tag is of SITE specific class HERE !!
        */


        if(!VALS_set_HERE_part1_nbits_RSA()) return FAIL_FAILED();

        // the subject prefix is set early
        // so that the precedence layers have an EARLY preset value to override
        if(!VALS_set_HERE_part1_subject()) return FAIL_FAILED();

        if(!VALS_set_HERE_part1_time_range()) return FAIL_FAILED();
        return true;
}

bool CA_task:: VALS_set_HERE_part1_nbits_RSA()
{
	// http://csrc.nist.gov/publications/nistpubs/800-57/sp800-57-Part1-revised2_Mar08-2007.pdf
	// SP800-57 says 1024 bit RSA is 80 bit strength
	// and that expires at end of 2010
	//    x8   x2 - scale factor is cubic of bitlength ? maybe
	//  1024   80
	//  2048  112
	//  3072  128
	//  7680  192
	// 15360  256
	//
	// int nbits = 1024;
	//
	int nbits = 2048;
        switch(layout_tag->tag) {
          case is_CA_ZERO:
		nbits = 2048; // new basic standard ? matches 112
		nbits = 15360; // bored
		nbits = 7680; // 25 sec
		nbits = 3072; // 1. sec
	  break;
          case is_CA_ONE:
	  default:
	  break;
	}
	
	VALS->iss_misc.RSA_bits = nbits;
	return PASS("Setting RSA_bits = %d", nbits );
	return true;
}

bool CA_task:: VALS_set_HERE_part1_subject()
{
        /*
                This is site specific for layout_tag (which includes scheme)
                So are all the functions here ...
        */
        VALS->subject.CN = "-UNSET-"; // calling this default LATE does this!
        switch(layout_tag->tag) {
          case is_CA_ZERO:
          case is_CA_ONE:
                VALS->subject.C  = "GB";
                VALS->subject.ST = "Berkshire";
                VALS->subject.O  = "Information Cascade";
                VALS->subject.OU = "WeRunYourZone";
          break;
          default:
                VALS->subject.C  = "GB";
                VALS->subject.ST = "Berks";
                VALS->subject.O  = "Information Cascade";
                VALS->subject.OU = "DESIGN and DEVELOPMENT ZONE";
        }
        return true;
}

bool CA_task:: set_CN( STR0 CN ) {
                VALS->subject.CN = CN;
                INFO("VALS->subject.CN = %s", VALS->subject.CN );
		return true;
}

bool CA_task:: VALS_set_HERE_part1_time_range()
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

         case is_CA_ZERO:       days = 365 * 10.2;      break;
         case is_CA_ONE:        days = 365 * 1.2;       break;
         case is_CA_ZONE:       days = 365 * 1.1;       break;
         case is_CA_server:     days = 365 * 1.0;       break;
         case is_CA_user:       days = 365 * 1.0;       break;
         case is_CA_pc:         days = 365 * 1.0;       break;
         case is_CA_item:       days = 365 * 1.0;       break;
         case is_C_server:      days = 365 * 1.0;       break;
         case is_C_user:        days = 365 * 1.0;       break;
         case is_C_pc:          days = 365 * 1.0;       break;
         case is_C_item:        days = 365 * 1.0;       break;

         default:               FAIL("TAG not recognised");
                                days = 7;               break;
        }
        VALS->iss_misc.ISS_time_range.set_now_and_ndays( days );
        return PASS("days = %d", days );
        return true;
}



	/////////////////////////////////////////////////////////
	//		TAG + LAYOUT
	/////////////////////////////////////////////////////////






/*
	CTOR create a new TAG, one of LAYOUT

	As a special case, allow _tag to be UNSET, and set later.
	Often these TAGs are readonly, the role of this CA is ...
	and the CA only lives for the time of this issue,
	but the CA might issue one-of-a-few things, and also
	it is very convenient to set the LAYOUT, then later set the TAG.

	In some sense, this object is a wrapper on the int-enum,
	with some functions to get info about tag from layout.
	
	(used by layout_holders CTOR, when layout is known but tag not yet)
*/
SITE_X509_tag:: SITE_X509_tag(
		SITE_X509_layout * scheme,
		SITE_X509_tag_enum _tag
)
: tag(_tag)
{
	clear();
 //	gdb_invoke();
	if(!scheme) {
		throw "NULL scheme";
		return;
	}
	layout = scheme;
	if( _tag == is_C_UNSET ) {
		WARN("tag == UNSET in layout == %s",
			scheme->debug_get_obj_name() );
		// all CTOR to remain UNSET
	} else {
		set_tag( _tag );
	}
}

bool SITE_X509_tag:: clear()
{
	layout = NULL;
	tag = is_C_UNSET;
	return true;
}

/*!
	Create a new layout, with names for each role/position/point.

	Derived classes can rename these, ev C_user as "C_person", or "login".
	These strings are used in filenames, and in practice, this class
	will be shared by all programs in CA community (but each program
	will have a different role, and different access to secrets,
	but at least the all agree on the name "CA_ROOT"
*/
SITE_X509_layout:: SITE_X509_layout( const char * dbgname )
{

	if(!dbgname ) {
		dbgname = "SITE_X509_layout";
	}
	debug_set_obj_name( dbgname );

	str_CA_ZERO	= "CA_ZERO";
	str_CA_ONE	= "CA_ONE";
	str_CA_ZONE	= "CA_ZONE"; // nb not the "CA_ZONE_five" %s view

	// these are not used by me
	str_CA_server	= "CA_server";
	str_CA_user	= "CA_user";
	str_CA_pc	= "CA_pc";
	str_CA_item	= "CA_item";

	str_C_server	= "C_server"; // nb not "C_server_boxeleven"
	str_C_user 	= "C_user";	// C_user_1016 is an additional step
	str_C_pc	= "C_WorkStation";
	str_C_item	= "C_item";
}

/*!
	LAYOUT produces the TAG, it _COULD_ do something complex,
	but it defaults to a base class that (always) asks the layout.

	Your class could also have additional parameters,
	such as the difference between CA_ZONE_five and CA_ZONE_six
*/
// virtual
SITE_X509_tag * SITE_X509_layout:: new_SITE_X509_tag( SITE_X509_tag_enum _tag )
{
	return new SITE_X509_tag(this,_tag);
}


SITE_X509_tag:: ~SITE_X509_tag()
{
	clear();
}

SITE_X509_layout:: ~SITE_X509_layout()
{
}

/*!
	Most tag vars are readonly, set once, and that is here and now.
	You might also have changable tags.

	Note that "UNSET" is not a valid option.
	It is a code error to leave it UNSET from the CTOR,
	but script/data errors cannot set it to UNSET.
	If you need this, create a set_tag_UNSET() function.
*/
bool SITE_X509_tag:: set_tag( SITE_X509_tag_enum _tag )
{
	buffer2 buf;
	if(!layout->print_part_name( buf, _tag )) {buf.set("FAILNAMEUNSET");}
	INFO("%s", (STR0) buf );
	if( _tag == is_C_UNSET ) {
		_tag = is_CA_item;
		return FAIL("CANNOT HAVE tag UNSET");
	}
	tag = _tag;
	return true;
}

/*!
	The number of possible CA's below this tag-item
*/
int SITE_X509_tag:: CA_depth()
{
	return layout->CA_depth( tag );
}

/*!
	The number of possible CA's below that tag-item

	You can change this (virtual) for your site.
	It is easy to remove a layer (as no new enums needs to be defined).
	It is more tricky to add a layer (eg grouping),
	as the enums need to be defined, and some recompiling done.

	My plan is to stay with the universally useful minimum,
	but then add some enums (and sample classes) for convoluted sites,
	but I think this actually quite good (one size fits all),
	and the most common request will be to flatten it,
	to reduce the number of RSA computations per connection,
	but surely an openSSL engine that caches CA-signed-CA instead?

	If you really wanted, you could call CA_depth( get_parent_CA_tag(tag) )
	buy maybe that should be a UNIT test parallel computation / display.
*/
int SITE_X509_layout:: CA_depth(  SITE_X509_tag_enum tag )
{
 switch( tag ) {

	case is_CA_ZERO:	return 3;
	case is_CA_ONE:		return 2;
	case is_CA_ZONE:	return 1;

	case is_CA_server:
	case is_CA_user:
	case is_CA_pc:	
	case is_CA_item:

		FAIL("tag not supported");
		return 1;

/*
	case is_CA_server:	return 1;
	case is_CA_user:	return 1;
	case is_CA_pc:		return 1;
	case is_CA_item:	return 1;
*/

	case is_C_server:	return 0;
	case is_C_user:		return 0;
	case is_C_pc:		return 0;
	case is_C_item:		return 0;

	case is_C_N:
	case is_C_UNKNOWN:
	case is_C_UNSET:
		FAIL("tag not supported");
		return 0;
	default:
		FAIL("tag not supported");
		return 0;
 }
}

bool SITE_X509_layout:: is_sensible_tag(  SITE_X509_tag_enum tag )
{
 switch( tag ) {

	case is_CA_ZERO:
	case is_CA_ONE:
	case is_CA_ZONE:

 // actually you might exclude these
 // the real poin is it isnt is_C_UNSET
 //
	case is_CA_server:
	case is_CA_user:
	case is_CA_pc:	
	case is_CA_item:

	case is_C_server:
	case is_C_user:	
	case is_C_pc:
	case is_C_item:

		return true;

	case is_C_N:
	case is_C_UNKNOWN:
	case is_C_UNSET:

		return FAIL("tag not supported");
	default:
		return FAIL("tag has strange value - memory corrupted?");
 }
}

/*!
	Is that tag a CA or a C ?

	WARNING: the maximum depth of CA's is returned,
	which may be more than you typically use,
	but it should always be zero for C_cert_anything
*/
// virtual 
bool SITE_X509_layout:: is_CA( SITE_X509_tag_enum _tag )
{
	return CA_depth(_tag) > 0 ;
}

/*!
	Is that tag a CA or a C ? BEWARE of parallel code changes
*/
bool SITE_X509_tag:: is_CA()
{
	return CA_depth() > 0 ;
	return layout->is_CA( tag );
}

SITE_X509_tag_enum SITE_X509_tag:: get_parent_CA_tag()
{
	return layout->get_parent_CA_tag( tag );
}

/*!
	What your site considers to be the parent CA of a C might vary

	This is not completely isolated, as your site might need to define
	new tags, but is mostly so

	EG splice CA_TWO above CA_ZONE

	EG each ZONE has multiple CA's, some have more depth

	EG each CA_ONE might treat different CA_ZONE's differently,
	with different CA pathlen depth freedoms!
*/
SITE_X509_tag_enum SITE_X509_layout:: get_parent_CA_tag(SITE_X509_tag_enum tag )
{
 switch( tag ) {

	case is_CA_ZERO:	return is_CA_ZERO; // self sign
	case is_CA_ONE:		return is_CA_ZERO;
	case is_CA_ZONE:	return is_CA_ONE;

	case is_CA_server:
	case is_CA_user:
	case is_CA_pc:	
	case is_CA_item:

		FAIL("tag not supported");
		return is_C_UNSET;

#if 1
	/*
		here you can see how MYSITE uses one CA_ZONE for everything
		but you might want to have a CA_user for a C_user
	*/
	case is_C_server:	return is_CA_ZONE;
	case is_C_user:		return is_CA_ZONE;
	case is_C_pc:		return is_CA_ZONE;
	case is_C_item:		return is_CA_ZONE;
#else
	case is_C_server:	return is_CA_server;
	case is_C_user:		return is_CA_user;
	case is_C_pc:		return is_CA_pc;
	case is_C_item:		return is_CA_item;
#endif

	/*
		As soon as you add a safe fixed list, you add the extra enums,
		which is not safe and correct! Scripts might come here
	*/
	case is_C_N:
	case is_C_UNKNOWN:
	case is_C_UNSET:
		FAIL("tag not supported");
		return is_C_UNSET;
	default:
		FAIL("tag not supported");
		return is_C_UNSET;
 }
}

/*!
	Only define the overrides you want for your site

	This has been replaced by SITE_X509_tag - so delete this code?
*/
const char * SITE_X509_layout:: print_part_filename_base1a(
	SITE_X509_tag_enum _tag
)
{
	return NULL; //	return "C_LAPTOP"; // but laters
}

/*!
	The builting standard strings for all SITEs, all normal names

	The returned string is expected to survive until the next idle-loop,
	or at least some back up the stack for a short distance.
	Two will be checked aut at once.

	This is the name used in filenames.

*/
const char * SITE_X509_layout:: print_part_filename_base1b(
	SITE_X509_tag_enum _tag
)
{

 switch( _tag ) {
	case is_CA_ZERO:	if(str_CA_ZERO) return str_CA_ZERO;
	case is_CA_ONE:		if(str_CA_ONE)  return str_CA_ONE;
	case is_CA_ZONE:	if(str_CA_ZONE) return str_CA_ZONE;

	case is_CA_server:	if(str_CA_server) return str_CA_server;
	case is_CA_user:	if(str_CA_user)	return str_CA_user;
	case is_CA_pc:		if(str_CA_pc)	return str_CA_pc;
	case is_CA_item:	if(str_CA_item)	return str_CA_item;

	case is_C_server:	if(str_C_server) return str_C_server;
	case is_C_user:		if(str_C_user)	return str_C_user;
	case is_C_pc:		if(str_C_pc)	return str_C_pc;
	case is_C_item:		if(str_C_item)	return str_C_item;

	default: break;
 }
 switch( _tag ) {

	case is_CA_ZERO:	return "CA_ZERO";
	case is_CA_ONE:		return "CA_ONE";
	case is_CA_ZONE:	return "CA_ZONE";

	case is_C_server:	return "C_server";
	case is_C_user:		return "C_user";
	case is_C_pc:		return "C_pc";
	case is_C_item:		return "C_item";

	default:
		FAIL("tag not supported");
		return NULL;
 }
}

/*!
	this is the fallback for unknowns and mysterious ones.

	One day I shall write emy 'enum' code generator,
	and it will do things like this.
*/
const char * SITE_X509_layout:: print_part_filename_base1c(
	SITE_X509_tag_enum _tag
)
{
 switch( _tag ) {

	case is_CA_server:	return "CA_server";
	case is_CA_user:	return "CA_user";
	case is_CA_pc:		return "CA_pc";
	case is_CA_item:	return "CA_item";

	case is_C_N:		return "CA_N";
	case is_C_UNKNOWN:	return "CA_UNKNOWN";
	case is_C_UNSET:	return "CA_UNSET";

	default:
		FAIL("tag not supported");
		return "CA_UNKNOWN";
 }
}

bool SITE_X509_tag:: print_part_name( buffer2 & buf ) {
	return layout->print_part_name( buf, tag );
}

bool SITE_X509_layout:: print_part_name( //	ie "CA_ZERO"
	buffer2 & buf,
	SITE_X509_tag_enum _tag 
)
{
	const char * namepart;
	namepart = print_part_filename_base1a( _tag );
	if(!namepart) {
	//	actually 1a is _SUPPOSED to return NULL !
	//	WARN("NULL from print_part_filename_base1a( tag %d )", _tag);
		namepart = print_part_filename_base1b( _tag );
	}
	if(!namepart) {
		WARN("NULL from print_part_filename_base1b( tag %d )", _tag);
		namepart = print_part_filename_base1c( _tag );
	}
	if(!namepart) {
		WARN("NULL from print_part_filename_base1c( tag %d )", _tag);
		buf.print("XXX");
		return FAIL("tag %d", _tag);
	}
	return buf.print("%s", (STR0) namepart );
	
}

/*
	get "CA_ZONE", "C_Item"

	DELETE THIS - move to _layout pure
*/
bool SITE_X509_tag:: get_part_name_parent_child(
	buffer2 & buf_issuer,
	buffer2 & buf_issued
)
{
	return layout->get_part_name_parent_child( buf_issuer, buf_issued, tag );
}

/*
	get "CA_ZONE", "C_Item"
*/
bool SITE_X509_layout:: get_part_name_parent_child(
	buffer2 & buf_issuer,
	buffer2 & buf_issued,
	SITE_X509_tag_enum _tag 
)
{
	if(buf_issuer) WARN("issuer expect '' got '%s'", (STR0) buf_issuer );
	if(buf_issued) WARN("issued expect '' got '%s'", (STR0) buf_issued );
 if(1) {
	buf_issuer.clear();
	buf_issued.clear();
 }

	bool ok = true;
	SITE_X509_tag_enum tag2 = get_parent_CA_tag(_tag);

	if(!print_part_name( buf_issuer, tag2 )) ok = false;
	if(!print_part_name( buf_issued, _tag )) ok = false;

	if(ok) return true; else return FAIL_FAILED();
}


// virtual
bool CA_task:: make_file_names()
{
	return PASS("--UNUSED-VECTOR--"); // useful one though ? mkdir ?
}



/*!
	The tag has precedence over the layout (it is produced by the layout)
*/
// virtual
bool CA_task:: obtain_issuer_certificate_and_key()
{
#warning adjust
#warning how do we know the CA_ZERO and CA_ONE has been loaded ??
//	if(issuer_cert && issuer_key) return PASS("ALREADY DONE");

        if(C_site_loader->loaded->self_tag == is_CA_ZERO) {
                INFO("SELF SIGN ...");
		/*
			CA_ZERO uses the client_key as the issuer key
			The client_key must already be loaded ...
			and it MUST be the _PRIV part (if you split REQ...)
		*/
#if 0
                CA_site_loader->loaded->CA_ZERO  = C_site_loader->loaded->CA_ZERO;
#else
                CA_site_loader->loaded->CA_ZERO.set_peek( C_site_loader->loaded->CA_ZERO );
#endif
                CA_site_loader->loaded->self_key = C_site_loader->loaded->self_key;
//                issuer_key  = client_key_priv;
//              issuer_cert  = client_cert;
		// FAIL(" issuer_cert = client_cert;");
                if(!client_key_priv) return FAIL("NULL client_key_priv - when self signing");
                return true;
        }

	// the issuer key is
	// where is get_parent of tag ...
	// it is _CLOSE_ to the MYSITE_loaded CTOR
	if(!CA_site_loader->load_self_cert_and_key()) return FAIL_FAILED();
	return PASS("NEWLY DONE");
}

/////////////////

// virtual
bool CA_task:: obtain_issuer_cb_phrase() // this is the _CLIENT_
{
	return PASS("********** no no no **********");
	return CA_site_loader -> obtain_self_cb_phrase();
}

bool MYSITE_loader:: obtain_self_cb_phrase()
{
	return PASS("****************** no no no ****************");
	return FAIL("TODO match up");
};

/*!
	obtain the SECRET phrase from elsewhere, for use later

	??? leave as was if not NULL ???
*/
bool MYSITE_loader:: obtain_self_cb_phrase( obj_hold<CB_get_phrase_base> & cb_phrase )
{
	cb_phrase = new CB_get_phrase_NONE();
	return PASS("****************** no no no ****************");
	return FAIL("TODO match up");
};


bool CA_task:: ISS_write_client_cert_and_key() // as .pem and also as .pem.txt
{
	if(! C_site_loader -> save_self_cert_and_key()) return FAIL_FAILED();
	if(! C_site_loader -> save_self_cert_as_txt()) return FAIL_FAILED();
	return true;
}

// virtual 
bool CA_task:: obtain_client_cb_phrase()
{
	return C_site_loader -> obtain_self_cb_phrase();
}


bool CA_task:: REQ_create_client_key_priv() // and phrase
{
	if( C_site_loader -> get_self_key() ) return PASS("Already DONE");
//	if(client_key_priv) return PASS("Already DONE");

	obj_hold<CB_get_phrase_base>	client_cb_phrase;
	if(!C_site_loader -> obtain_self_cb_phrase(client_cb_phrase)) return FAIL_FAILED();
	if(!obtain_client_cb_phrase()) return FAIL_FAILED();
	if(!client_cb_phrase) return FAIL("NULL client_cb_phrase");

	if(!VALS) return FAIL("NULL VALS");
	int nbits = VALS->iss_misc.RSA_bits;

	obj_hold<EVP_P_KEY> key = new EVP_P_KEY( client_cb_phrase );
	if(!key) return FAIL("NULL new key");
	if(!key->gen_RSA_nbits(nbits)) return FAIL_FAILED();
	if(!REQ_set_client_key_priv( key )) return FAIL_FAILED();

	return PASS("DONE nbits = %d", nbits);
}

/*!
	If you have a pre-existing private key, call this
	NB client_cb_phrase should be set, but it has ALREADY been put into key
	SO not actually needed !
*/
bool CA_task:: REQ_set_client_key_priv( EVP_P_KEY * key )
{
	C_site_loader -> loaded -> self_key = key;

	client_key_priv = key;
	client_key_pub = key;

	if(!REQ->set_pubkey( client_key_pub )) return FAIL_FAILED();
	return PASS("DONE");
}

/*!
	All of the VALS except ISS_* are applied to the REQ binary fields.

	If you want to set the VALS, do so _BEFORE_ calling this,
	and set them into the task->VALS->*
*/
bool CA_task:: REQ_apply_VALS() // this.VALS appears global // a fast parameter
{
	if(!VALS) return FAIL("NULL VALS");
	return REQ_apply_VALS( *VALS );
}

bool CA_task:: REQ_apply_VALS( X509_VALS_base & vals )
{
	/*
		 copy the str1 values into X509 NAME entries
			CN = "USER1234"
	*/
	if(!UTIL_subject.set( vals.subject )) return FAIL_FAILED();

	/*
		add this setting to the list of extentions
			X509v3 Basic Constraints: critical
			CA:false
		or
			X509v3 Basic Constraints: critical
			CA:TRUE, pathlen:2

	*/
	if(!UTIL_extensions.add_ext_CA_depth( vals.CA_depth ))
		return FAIL_FAILED();


	/*
		add this setting to the list of extentions
			X509v3 Subject Alternative Name:
				mail:lists@information-cascade.co.uk
		the subjectAltName is an extension
	*/
	if(!UTIL_extensions.OK_add_ext_subjectAltName( vals.subjectAltName ))
		return FAIL_FAILED();

	/*
		Inorder to add this hash of the client key,
		the REQ must _PREVIOUSLY_ have the client_key_pub set

	X509v3 Subject Key Identifier: 
		55:05:71:89:11:F7:39:1A:89:79:A6:3A:4C:36:25:51:AC:0D:7F:CC

	:	extensions.add_ext_subject_key_identifier_hash( reqcert )

		The RSA id is taken from the cert, into the extensions
		to be put back on the cert, called by ... elsewhere
		see mk_cert
	*/
/////	if(!UTIL_extensions.add_ext_subject_key_identifier_hash( client_key_pub )) return FAIL_FAILED();

	if(!UTIL_extensions.add_ext_subject_key_identifier_hash( REQ->req )) return FAIL_FAILED();

	/*
		EXTENSION that says key may be used for SIGN, SERVER, ...

			X509v3 Key Usage:
				Certificate Sign

	*/
	if(!UTIL_extensions.add_ext_key_usage( vals.key_usage )) return FAIL_FAILED();

	/*
		-- now put those sub-structures into the REQ certificate --
	*/

	/*
		REQ.NAME = SUBJECT
	*/
	if(!REQ->set_subject_name(UTIL_subject)) return FAIL_FAILED();

	/*
		REQ.EXTENSIONS = extensions
	*/
	if(!REQ->add_extensions(UTIL_extensions)) return FAIL_FAILED();

// TODO add authorityKeyIdentifier to ISS (when issuer PUB key is available)
// TODO repeat this code, and list of one extension then ...
// TODO go back to X509_util_CERT base class for REQ and ISS ?????

	return true;
}

bool CA_task:: REQ_sign_with_client_key_priv()
{
	if( !client_key_priv ) return FAIL("NULL client_key");
	return REQ->sign_REQ_with_client_key( client_key_priv );
}

bool CA_task:: ISS_copy_VALS_serial()
{
        int serial = VALS->iss_misc.CERT_serial ++;
        if(!ISS_put_serial(serial)) return FAIL_FAILED();
	return true;
}

bool CA_task:: ISS_copy_VALS_time_range()
{
	return ISS_put_time_range( VALS->iss_misc.ISS_time_range );
}

/*!
	this is a DEMO, an alternative is LOAD REQ CERT FROM FILE/buffer
	when client_pub will be around, but _priv not
*/
bool CA_task:: REQ_mk_cert_from_VALS()
{
	/*
		we either create a new RSA, or use an existing PAIR
		_OR_ use the public half of a pair for the ISS
		_++_ use the private half for the REQ (must?)

		Creating the RSA and setting its cb_phrase
		protects it immediately, and later!
		(it retains the cb_phrase so write_PrivateKey() has it)
	*/

	if(!REQ_create_client_key_priv()) return FAIL_FAILED();

	/*
		copy the basic string VALS into the REQ cert via utils
	*/
	if(!REQ_apply_VALS()) return FAIL_FAILED();


	/*
		The client_key was used above, it is used again,
		but this time the PRIVATE side signs (its own public+info)
	*/
	if(!REQ_sign_with_client_key_priv()) return FAIL_FAILED();

	return PASS("DONE");
}

/////////////////////

/*!
	If there was a field in the REQ that you dont like,
	you should not call this function, it will be copied over.

	Maybe I could write an ISS that does not need a REQ,
	and simply build the cert from VALS, which is what I want.
	And build VALS from a carefully constructed app-specific request
*/
bool CA_task:: ISS_mk_cert()
{

	if(!REQ) return FAIL("NULL REQ");
	if(!VALS) return FAIL("NULL VALS");
//	if(!client_cert) return FAIL("NULL new X509_cert()");

	if(!make_file_names()) return FAIL_FAILED();
        if(!ISS_new_cert_version_2()) return FAIL_FAILED();

	if(!client_key_priv) {
		WARN("NO client_key_priv - this may beed debugging");
		// I have debugged with it always known
	}

	// can do this halfway down, but sooner is OK
	// or call when needed ...
        if(!obtain_issuer_certificate_and_key()) return FAIL_FAILED();

	// you must load and save the increasing serial - optimistically
	// there is no lock and rollback 
	if(!ISS_copy_VALS_serial()) return FAIL_FAILED();

	// setting up VALS is done pre-REQ, so ... if not from REQ then what?
	if(!ISS_copy_VALS_time_range()) return FAIL_FAILED();

//	// subject_alt_name is one of the REQ extensions ...
//	if(!ISS_copy_REQ_subject_alt_name(REQ->REQ)) return FAIL_FAILED();

	// copy from REQ ...
        if(!ISS_copy_REQ_subject_name()) return FAIL_FAILED();
	if(!ISS_copy_REQ_client_key_public()) return FAIL_FAILED();

        if(!ISS_copy_CA_issuer_name()) return FAIL_FAILED();
	if(!ISS_copy_REQ_all_extensions()) return FAIL_FAILED(); // from REQ
        if(!ISS_copy_CA_issuer_hash()) return FAIL_FAILED(); // direct to client_cert
	// need to create issuer hash after REQ (when issuer cert avail)
	// need to create issuer hash _AFTER_ copying subject hash (self sign)
	// ie after ISS_copy_REQ_all_extensions

	if(!ISS_sign_with_CA_priv()) return FAIL_FAILED();

#if 0
	// remote these checks ... all pass
	if(!client_cert) return FAIL("NULL 1a");
	if(!client_cert->cert) return FAIL("NULL 1b");
//	if(!client_key_pub) return FAIL("NULL 1c");
//	if(!client_key_pub->pkey) return FAIL("NULL 1d"); // NULL
#endif

	return PASS("DONE");
	return true;
}

bool CA_task:: ISS_new_cert_version_2()
{
	// this creates the holder not the cert !
	// but you cant share the cert with anyone else
	// until AFTER this is called ...
	X509_cert & client_cert_var = C_site_loader -> get_self_cert();
//	client_cert = new X509_cert();
//	if(!client_cert) return FAIL("NULL new X509_cert()");

	X509 * c = X509_new();
	if(!c) {
		THROW_dgb_fail("X509_new()");
	}
	if(!X509_set_version( c, 2L )) {
		return FAIL("X509_set_version(c,2)");
	}
//	client_cert->set_to_keep(c);
	client_cert_var.set_to_keep(c);
	return true;
}

bool CA_task:: ISS_put_serial( int ser )
{
	X509_cert * client_cert = & C_site_loader -> get_self_cert();
	ASN1_INTEGER_set(X509_get_serialNumber( client_cert->cert), ser );
	return true;
}

bool CA_task:: ISS_copy_REQ_subject_name()
{
	X509_cert * client_cert = & C_site_loader -> get_self_cert();
	if(!client_cert->cert) return FAIL("NULL client_cert->cert");

	X509_NAME * name = X509_REQ_get_subject_name( REQ->req );
	if(!name) {
		return FAIL("X509_REQ_get_subject_name(req)");
	}
	if(!X509_set_subject_name( client_cert->cert, name ) ) {
		return FAIL("X509_REQ_set_subject_name(client_cert, name)");
	}
	return PASS("DONE");
	return true;
}

bool CA_task:: ISS_copy_REQ_subject_alt_name() // extract it from extensions list
{
	X509_cert * client_cert = & C_site_loader -> get_self_cert();
	if(!client_cert->cert) return FAIL("NULL client_cert->cert");

	STACK_OF(X509_EXTENSION) * req_exts = X509_REQ_get_extensions( REQ->req );
	if(!req_exts) {
 		return FAIL("X509_REQ_get_extensions( REQ->req );");
	}

	int pos = X509v3_get_ext_by_NID(
		req_exts,
		NID_subject_alt_name,
		-1
	);
	X509_EXTENSION * subject_alt_name = X509v3_get_ext(
		req_exts,
		pos
	);

	if(!subject_alt_name) {
		FAIL("NULL subject_alt_name - a bit rare");
		return true; // but reported
	}

	if(!X509_add_ext( client_cert->cert, subject_alt_name, -1 )) {
		return FAIL("X509_add_ext( client_cert->cert, subject_alt_name, -1 );");
	}

	// delete from original list ?? as in _MOVE_
	// LURK leak
	// unused anyway - copied with the rest of them

	return PASS("DONE");
	return true;
}

/*!
	COPY pubkey from: REQ -> client_key_pub 
	APPLY pubkey client_key_pub -> client_cert
*/
bool CA_task:: ISS_copy_REQ_client_key_public()
{
	if(!ISS_get_client_key_public_FROM_REQ()) return FAIL_FAILED();
	if(!ISS_put_client_key_public_INTO_CERT()) return FAIL_FAILED();
	return PASS("DONE");
}

/*!
	copy pubkey from: REQ -> client_key_pub
*/
bool CA_task:: ISS_get_client_key_public_FROM_REQ()
{
	if(client_key_pub) {
		WARN("ALREADY HAVE THIS");
		return PASS("DONE");
	}


	client_key_pub = new EVP_P_KEY();
	if(!client_key_pub) {
		THROW_dgb_fail("new EVP_P_KEY()");
	}

	// copy from REQ into client_key_pub
	if(!REQ) return FAIL("NULL REQ");
	if(!REQ->get_pubkey( client_key_pub )) return FAIL_FAILED();
	return PASS("HAVE client_key_pub");
	return PASS("DONE");
}

bool CA_task:: ISS_put_client_key_public_INTO_CERT()
{
	X509_cert * client_cert = & C_site_loader -> get_self_cert();
	if(!client_cert->CA_set_pubkey( client_key_pub )) return FAIL_FAILED();
	return PASS("USED client_key_pub");
}

bool CA_task:: ISS_copy_REQ_all_extensions()
{
	X509_cert * client_cert = & C_site_loader -> get_self_cert();
	STACK_OF(X509_EXTENSION) * req_exts = X509_REQ_get_extensions( REQ->req );
	int N = X509v3_get_ext_count(req_exts);
	for(int i=0; i<N; i++ ) {
		X509_EXTENSION * ex = X509v3_get_ext(
			req_exts,
			i
		);
		if(!X509_add_ext(client_cert->cert, ex, -1 )) {
			return FAIL("X509_add_ext(client_cert, ex, -1 );");
		}
	}
	return PASS("DONE");
	return true;
}

bool CA_task:: ISS_copy_CA_issuer_hash()
{
	X509_cert * issuer_cert  = & CA_site_loader->get_self_cert();
	X509_cert * client_cert = & C_site_loader -> get_self_cert();
    	if(!VALS->iss_misc.ADD_EXT_authority_key_identifier) {
		WARN("VALS SAYS NO");
		return true;
	}
	X509_util_EXT_iss ISS_extensions( client_cert );
	if(!ISS_extensions.add_ext_authority_key_identifier(
		issuer_cert->cert,
		(STR0) VALS->iss_misc.ADD_EXT_authority_key_identifier_opts

	)) return FAIL_FAILED();
	return PASS("DONE");
	return FAIL("TODO");
}

bool CA_task:: ISS_copy_CA_issuer_name()
{
	X509_cert * issuer_cert = & CA_site_loader->get_self_cert();;
	X509_cert * client_cert = & C_site_loader -> get_self_cert();
	if(!issuer_cert) return FAIL("NULL issuer_cert");
	if(!issuer_cert->cert) return FAIL("NULL issuer_cert->cert");
	if(issuer_cert  == client_cert) {
		// need NAME to be set before calling this
		INFO("SAME");
	}

	X509_NAME * name = X509_get_subject_name( issuer_cert->cert );
	if(!name) {
		return FAIL("X509_get_subject_name(issuer_cert->cert)");
	}
	if(!X509_set_issuer_name( client_cert->cert, name ) ) {
		return FAIL("X509_REQ_set_issuer_name(client_cert, name)");
	}
	return PASS("DONE");
	return true;
}

bool CA_task:: ISS_put_time_range( X509_VALS_time_range & time_range )
{
//	gdb_invoke();
	X509_cert * client_cert = & C_site_loader -> get_self_cert();
	int t0 = time(NULL);
	int t1 = time_range.ISS_time_start-t0;
	int t2 = time_range.ISS_time_end-t0;
	if(!X509_gmtime_adj( X509_get_notBefore(client_cert->cert), t1 )) {
		return FAIL("X509_gmtime_adj( X509_get_notBefore(client_cert), start ))");
	}

	if(!X509_gmtime_adj( X509_get_notAfter(client_cert->cert), t2 )) {
		return FAIL("X509_gmtime_adj( X509_get_notBefore(client_cert), end ))");
	}

	return PASS("DONE");
	return true;
}

bool CA_task:: ISS_sign_with_CA_priv()
{
	EVP_P_KEY * issuer_key = CA_site_loader->get_self_key();
//	X509_cert * issuer_cert = & CA_site_loader->get_self_cert();;
	X509_cert * client_cert = & C_site_loader->get_self_cert();;
// gdb_invoke();
	if(!issuer_key) return FAIL("NULL issuer_key");
	if(!client_cert) return FAIL("NULL client_cert");

	// calls X509_sign
	if(!client_cert -> CA_sign_with_issuers_key( issuer_key ))
		return FAIL_FAILED();
 /*
	if(!X509_sign( client_cert->cert, issuer_key->pkey, digest))
		return FAIL("Error signing cert");
 */
	STEP("DONE");
	return true;
}

//////////


//////////////////////////////////////////////////
