#ifndef MYSITE_X509_layout_H
#define MYSITE_X509_layout_H
/*
	refactoring work in progress ....
*/

/*
	LGPL exception - the rest of this package is LGPL,
	but this file (samples/2010/CA_ISS_util/mk_req.cxx and MYSITE*)
	is an example demo file,
	so feel free to copy paste customise in your app
	without too much fuss, OK
*/


#include "dgb.h"		// FAIL, WARN etc
#include "SSL_global.h"		// everyone must init rand etc
#include "CB_get_phrase.h"	// new CB_get_phrase_NONE()
#include "EVP_P_KEY.h"		// test_write_read_PrivateKey();

#include "SSL_stubs.h"
#include "SSL_CA1_stubs.h"
#include "SITE_X509_layout.h"

using namespace CA1;

/*
	All of the customisation, of the VALS that go into the REQ,
	is done in MYSITE_CA_task and MYSITE_layout, but you could easily
	need to do some of that here, maybe check a field, or something.

	#include "X509_VALS_base.h"	// string values
*/
// #include "X509_VALS_base.h"


/*
	SITE_CA_task has virtual hooks that are called at the right time,
	or you can assign values _BEFORE_ the hooks find them missing
	You base your class on SITE_CA_task (ditto for _layout)

		#include "SITE_CA_task.h"

	MYSITE_CA_task is those overrides, and also functions that do DEMO,
	or for you LIVE arrangements, invoking things as you want them.
	IE put your functions here.

		#include "MYSITE_CA_task.h"
	
*/
// #include "SITE_CA_task.h"
#include "MYSITE_CA_task.h"	// MYSITE is completely user defined demo


// you dont need this, unless you load REQ from buffer, from LAN, or file
#include "X509_REQ_hold.h"	// REQ->write_req_to_file()

// the default C_progress_genkey(...) is silent,
// you can overrule the shared lib version, see link order cmd line
// a_kept should not be globally visible though (rename and export)

#if 0
int a_kept = -1;
void C_progress_genkey( int a, int b, void * cb_arg )
{
	if( a_kept != a ) {
		if(a_kept != -1 ) printf("\n" );
		a_kept = a;
		printf("C_progress_genkey( %d, ", a );
	}
	printf("%d",b); // basically just counts to unknown varying ceiling
	printf(".");
	fflush(0);
}
#endif


/*!
	The layout is the SCHEME for all locally used enum tags.
	This demonstrates the renaming of PC/WORKSTATION/LAPTOP,
	along with OTHER regional variations.

	This is not specific to the C_pc,
	it is also used by CA_ZERO

	USAGE: the layout must be correct for every point in the layout,
	but will only be used for two of the them. CA and issued item.

	The CA_task class (optionally) uses _layout to load CA_key
	from the right file ("keys/CA_ZERO_key.pem") with phrase=X
	That obviously _also_ requires this to be the correct machine, etc,
	but when it is, it works.

	Any _TEMP_ filenames are extra (but would go here ...)

	SQL names, not filenames, are an extra step of abstraction,
	but could use "keys/CA_ONE_key.pem" as a primary key,
	but when this is an item in peer group, you will need its key.

	TODO: SITE_layout provides ITEM_subsel eg "ZONE5" in CA_ZONE_ZONE5
	probably remarkably similar to CN= "USER1016" but less human
*/
class MYSITE_X509_layout : public SITE_X509_layout
{
 public:
        MYSITE_X509_layout();

	// might not be called - eg already set
	bool obtain_issuer_cb_phrase( CA_task * task );
	bool obtain_client_cb_phrase( CA_task * task );

	// added by me for MYSITE
	bool obtain_CB_for_tag(
		obj_hold<CB_get_phrase_base> &cb_phrase,
		SITE_X509_tag_enum ISS_tag
	);

};

// }; // NS
#endif

