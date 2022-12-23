#ifndef SITE_X509_layout_H
#define SITE_X509_layout_H

#include "obj_ref.h"
#include "str1.h"
class buffer2;
#include "buffer2.h"

#include "X509_cert.h"

#include "SSL_stubs.h"
#include "SSL_CA1_stubs.h"

namespace CA1 {

/*!

	The layout of X509 certificates in a community of X509 tagged things.

	It is designed for a departmental server, surrounded by local users,
	eg your departmental email server, and its sibling SMB filestore.

	The short list of sibling servers, in the local computer room,
	can be re-morphed into a single (mainframe) box, or split
	into bits, but all of that LAN is a domain core, with one CA_ZONE
	certificate (and key) which it uses to register its own list of users.

	A domain LAN might be "Coventry" or "Manchester" for a company,
	but they could still have different designs, such as "UK staff",
	however, my emphasis, is on local users for local machines,
	with ONE recognised CA, needed as part of the authentication.

	I also expect the authentication, to lookup each X509 in a database,
	which is slightly different from the public key infrastructure,
	but this is outside of this layout. It is based on the fact
	that X509's are LOCALLY issued, within a finite community,
	not issued by unknown CA's in a global PKI.

	The basic concept, is that the FM company issues CA_ZONE to (you)
	and then you do everything beneath that. The root CA's belong to the
	FM (facilities management service provider), but the CA_ZONE
	belongs to your company (O= OU= CN=)

	This is a one-size fits all, with some adjustments available.
	I can already think of a different scheme, such as C_JOB_ROLE,
	C_EMPLOYEE, which are work-time identity (and hats) based,
	and C_DOCUMENT_TIME_STAMPER_SERVICE, which is machine-work-identity.

	This approach is more what I am used to: one central server, with
	lots of login's, connecting from PC_workstions. However,
	I still think this is highly generic, and will fit a lot of sites.

	NOTE: distinguishing identity - when seen as a subdir

		CA_ZERO/CA_ONE/CA_ZONE_Manchester/CA_Servers/C_BOX5
		CA_ZERO/CA_ONE/CA_ZONE_Manchester/CA_Laptops_onsite/C_PC4321
		
		This shows three things -
		* a dir tree that I am not yet using
		* a name-within-group (Manchester)
		* a name-within-group (PC4321)

		The layout class, when supplemented with extra info,
		will be able to fill out values for the group _AND_ item.

		layout will produce various strings, from those name-parts,
		including filenames, CN=commonname, DNS=name.zone.dom, etc

		This will be very site specific.
		This is in parallel to the IO (get_cb_phrase)

	NOTE that everything is within namespace CA1, so you can create CA2;
	
	-- LAYOUT --

	Certificates exist in a tree, the parent is the CA that issued it,
	plus maybe some GROUP attributes, within the certificate data.

	Leaves are one of { USER, WORKSTATION, SERVER, ITEM, .. add more here }.
	So when you connect with your email server, you should check that it
	has the correct C_SERVER certificate, at least the same as last time.

	When creating certificates, of leaves, or of CA_nodes,
	a lot of parameters can be deduced (looked up),
	and the layout tags, and corresponding C++ class code help.

	This CA, is probably being run by one process, with one role,
	in a specific directory, which helps setup the parameters.

	This CA is being asked to create a C_ or CA_ in that tree, eg
	"is_C_user" is a tag that would create a C_user. It would
	probably need additional parameters, such as the username,
	and maybe the domain (if that isnt obviously "HERE").

	The parent of "is_C_user" is the tag "is_CA_user", so this CA
	would try to load the secret CA key from a file "CA_user_key.pem"
	and public checking would look for "CA_user_cert.pem" (and CRL).

	If you want to use the filename CA_LOGIN_cert.pem, that spelling
	can be set, but the same internal enum-tag used.

	The (rather annoying) prefix "is_" helps C keep distinct names.

	The parent of "CA_user" is "CA_zone" which was allocated
	by the wider authorities, to issue all certificates in the ZONE.

	The root certificate is "CA_ZERO" (self-signed), but it is stored
	offline, to keep it safe. When a new CA_ZONE is provisioned, it
	comes from "CA_ONE".

	Note that a service (eg email) which can provision its own users,
	will have both a C_SERVER and CA_USER

	-- EDIT ME SOME TIME --

	All of the other fields can be deduced from the
	local CA's config, which is the parent "is_CA_user", on this box,
	in this domain, with parameters in file "parameters/CA_user.ini"


	i.e. CA_ZERO/CA_ONE/CA_ZONE/C_item ... or other

	enum SITE_X509_tag_enum is one item of a layout of CA_ and C_'s

	-- EDIT ME SOME TIME --

	In addition, they set a CA_pathlen of zero or one or ...

	Then you want names-of-files, or names-of-SQL-keys,
	where you find the relevent cert/key, etc

	include "SITE_X509_layout.h" // base class

	-- CLASS TREE --

	You define your own class, based on this,
	and then the virtual functions return your names, your rules,
	your layout.

	Call your class MYSITEHERE_x509_layout : public SITE_X509_layout {};

	-- TODO --

	This goes back into the class setup certs and keys for client2/serv3/X

	-- POLICY --

	After you have cutomised this class, every application in your
	suite should share the same code. This applies your POLICY
	everywhere.  EG C_SERVER is a leaf certificate, and has X509v3
	Basic Constraints: critical CA:FALSE, or not if that annoys you.
	Likewise for the CA depths.

	For me, I dont actually use CA_SERVER, I jump straight to CA_ZONE.
	This reduces the RSA computations by a bit, but maybe next year,
	I shall want to be certain that different item types used their
	own CA's This demonstrates how you have enum tags from other sites,
	and customise them yo your site. Maybe someone will persuade me
	of a universal list, that fits more sites, but can still be subsetted,
	providing both enum_tags and layout classes.

	-- TODO --

	Allow SITE strings to ovverride the builtins,
	eg CA_Things instead of CA_Items
	eg C_WorkStation instead of C_pc

	-- TODO --

	Use in validation, set CN=xxx, check for CN==xxx
*/
enum SITE_X509_tag_enum
{
	is_C_UNKNOWN = -2,
	is_C_UNSET = -1,

 // upper level CA's

	is_CA_ZERO,	// [0] // OFFLINE // external FM creates CA1 every month
	is_CA_ONE,	// [1] // NATION or CORPORATION
	is_CA_ZONE,	// [2] // REGION or CITY or SERVER room or ADMIN

 // I dont use these, but you might - and base class funcs should tol
 // MAYBE create these as NAMED_SERVICE
 // "LAPTOP_" is subscription of C_pc_vpn_user
 // "VPN_" is subscription of C_pc_vpn_user to C_SERV_VPN
 // %s_%s for all clients AND %s_%s for one+ server per group
 
	is_CA_server, // a server is a service, or _SECOND_ twist
	is_CA_user,	
	is_CA_pc,	
	is_CA_item,
 //	is_CA_vpn, // openvpn node on VLAN_5 on ZONE_FIVE	
 //	-each- // needs a DB and scripts

 // CA_ZONE("BOX5") issues all these for its domain/community/area

	is_C_server, // a server is a service
	is_C_user,	
	is_C_pc,	
	is_C_item,

 // not a valid option

	is_C_N // excludes UNKNOWN UNSET and itself
};

/*
	The keys will be stored as PEM strings (usually),
	in files called "keys/CA_ZERO_cert.pem"
	or coded varnames like "STOR::pem_cert_CA_ZERO"

	Each site will store the keys somewhere ...
	or maybe simply fake and subclass
*/
enum SITE_sto_enum {
	sto_UNSET,
	sto_FILES,
	sto_SQL,
	sto_GLIB_KEYFILE,
	sto_C_VARNAME
};


/*!
	This _tag class wraps an enum-tag, and is automatically created by
	the layout, which defines it completely (using virtual functions).

	Each tagged layout.item, knows its own parent (by asking layout)

	This class should be more or less, left as-is

	An intelligent _layout can create an intelligent _tag,
	but the default is to bounce that intelligence back to the _layout

	Each layout_scheme and _tag, has a _HERE_ concept,
	which is not really mentioned, but would be loaded from a file
	of parameters. Each different CA would be on a machine of its own,
	in a directory of its own, with a parameters file of its own.

	Those parameters, might as well be handled by the _scheme_layout
	(not this _tag), but they are important.

	Many sites, will have an additional parameter (MYSITE_X509_layout)
	to create a CA for "CA_ZONE_five" (which is of genre CA_ZONE,
	and loads the "five" parameter from "args/CA_ZONE/${ZONE_NAME:-five}"

	MYSITE_X509_layout * mylayout = new MYSITE_X509_layout();
	SITE_X509_tag * tag = // possible subtype /
		mylayout->make_tag( "is_CA_ZONE", "five");
	
*/


class SITE_X509_tag : public obj_ref0
{
	bool set_tag( SITE_X509_tag_enum _tag );
 public:

	//! This item exists in the layout world
	obj_hold<SITE_X509_layout> layout;

	//! layout[item] is almost possible
	SITE_X509_tag_enum tag;

	/*
		GROUP SELECTOR = "this machine, this dir, those parameters"
		ITEM SELECTOR = "CN=USER_1010"

		Notice how they both sort of disappear,
		for this example. But if this is an INSTANCE selector,
		then they are there, and have things like client_key_phrase
		for issuer and issued
	*/

	~SITE_X509_tag();
	SITE_X509_tag(
		SITE_X509_layout * scheme,
		SITE_X509_tag_enum _tag = is_C_UNSET
	);

	bool clear(); // not scrub, but decr_ref things that scrub themselves

	int CA_depth();
	bool is_CA();

	SITE_X509_tag_enum get_parent_CA_tag();

	bool get_part_name_parent_child(
		buffer2 & buf_issuer,
		buffer2 & buf_issued
	);
	bool print_part_name( buffer2 & buf );

	// these might be blank, but here is a good place to hold them



};

/*!
	a TEMPLATE of the layout of the various certificates

	the INSTANCE (was _tag) has a point on that template,
	and that template on a layer
		group_names	"EUROTEL"	"HUB9"
		item_names	"HUB9"		"HSID1234"
		item_enum	"is_C_server"	"is_C_pc"

	A layout of items, knows parentage of all items at once,
	the site specific names of all items, and the filenames.

	This is a general BASE class of MYSITE, YOURSITE, variations ...
*/
class SITE_X509_layout : public obj_ref0
{
 public:
	str1 str_CA_ZERO;
	str1 str_CA_ONE;
	str1 str_CA_ZONE;

	// these are options
	str1 str_CA_server;
	str1 str_CA_user;
	str1 str_CA_pc;
	str1 str_CA_item;

	str1 str_C_server;
	str1 str_C_user;
	str1 str_C_pc;
	str1 str_C_item;

	/*
		virtual DTOR and CTOR
	*/

	~SITE_X509_layout();
	SITE_X509_layout( const char * dbg_name = NULL );

	virtual SITE_X509_tag * new_SITE_X509_tag( SITE_X509_tag_enum _tag );

	virtual int CA_depth( SITE_X509_tag_enum _tag );
	virtual bool is_CA( SITE_X509_tag_enum _tag );
	virtual SITE_X509_tag_enum get_parent_CA_tag( SITE_X509_tag_enum _tag );
	virtual bool is_sensible_tag( SITE_X509_tag_enum _tag );

	virtual bool get_part_name_parent_child(
		buffer2 & buf_issuer,
		buffer2 & buf_issued,
		SITE_X509_tag_enum _tag 
	);

	// baseclass bultin non-virtuals
 private:
  virtual
	const char * print_part_filename_base1a( SITE_X509_tag_enum _tag );
	const char * print_part_filename_base1b( SITE_X509_tag_enum _tag );
	const char * print_part_filename_base1c( SITE_X509_tag_enum _tag );
 public:
	bool print_part_name( buffer2 & buf, SITE_X509_tag_enum _tag );


};


/*!
	The _layout/_scheme knows the filenames,
	The loader uses the _scheme to tell it the file names
	The loader also STORES the things that are loaded

	The _CLASS_ _layout can be shared client and server,
	it can redefine the spelling of "PC" (eg as "DESKTOP")
*/
class MYSITE_loaded : public obj_ref0
{
 public:

	// you wont have all of these loaded, pos_in_layout and above

	X509_cert CA_ZERO;
	X509_cert CA_ONE;
	X509_cert CA_ZONE;

	X509_cert  CA_server;
	X509_cert  CA_user;
	X509_cert  CA_pc;
	X509_cert  CA_item;

	X509_cert C_server;
	X509_cert C_user;
	X509_cert C_pc;
	X509_cert C_item;

	X509_cert C_fake; // get_var is not a bool it needs something


	// only one will have a private key available (cb_phrase on the key)
	// 
	// nb 'self' means this, the CA has two of these, CA_SELF, C_CLIENT

	obj_hold<EVP_P_KEY> self_key;
	SITE_X509_tag_enum self_tag;

	X509_cert & get_cert_var( SITE_X509_tag_enum tag );
	X509_cert & get_cert_var_self();
	bool set_self_cb_phrase( CB_get_phrase_base * cb_phrase );

	MYSITE_loaded( SITE_X509_tag_enum _self_tag );
	~MYSITE_loaded();
};

/*!
	REDO:- only try to save one cert at a time
	REDO:- pass cert + pem-text as parameters
	REDO:- base class calls self to resolve 'filename' 'keyval'

	NOTE: this is save-and-load-er, and often split, save here, load there
*/
class MYSITE_loader: public obj_ref0
{
 public:
	obj_hold<MYSITE_loaded> loaded;
	// maybe 'scheme' is better than 'layout', maybe not
	obj_hold<SITE_X509_layout>	scheme;

	MYSITE_loader(
		MYSITE_loaded * _loaded,
		SITE_X509_layout * _scheme,
		CB_get_phrase_base * cb_phrase
	);

 virtual
	bool load_cert( SITE_X509_tag_enum tag );
 virtual
	bool load_self_cert_and_key( SITE_X509_tag_enum tag, CB_get_phrase_base * cb_phrase );
	bool load_self_cert_and_key( CB_get_phrase_base * cb_phrase );
	bool load_self_cert_and_key();
 virtual
	bool save_self_cert_and_key();
 virtual
	bool save_cert_as_txt( SITE_X509_tag_enum tag );
	bool save_self_cert_as_txt(); // or show_cert_as_txt()
 virtual
	bool save_cert( SITE_X509_tag_enum tag );

 virtual // giver user choice of putting encrypted key in place
	bool save_key_second_as_now_encrypted();

	X509_cert & get_self_cert();
	EVP_P_KEY * get_self_key();
	bool set_self_cert( X509_cert * );

	virtual bool obtain_self_cb_phrase();
	virtual bool obtain_self_cb_phrase( obj_hold<CB_get_phrase_base> & cb_phrase );

	bool save_chain_above_self();
	bool load_chain_above_self();
	bool save_chain_and_self();
	bool load_chain_and_self();
	bool save_root_and_self();
	bool save_root();
	bool load_root_and_self();
	bool load_root();

	bool check_key_was_encrypted(); // need top_app for ERROR_BOX if not

	bool add_trusted_CA( SSL_global_util * _SSL_Global, SITE_X509_tag_enum tag );

	// loaded does not have a scheme - so cannot figure parent-age
	// CLIENT OK
	virtual bool set_own_certificate_chain( SSL_global_util * _SSL_Global);
	virtual bool set_trusted_CA_list( SSL_global_util * _SSL_Global);

};

	////////////////////////////////////////////////

/*!
	load CA from its file, in its dir
	requires scheme to also do its half of that

		merge MYSITE_loader_files 
		merge MYSITE_loaded
		merge SITE_X509_layout * scheme

	move to load CA from it's DBID
	which needs more that "CA_ZONE" ie "CA_VPN" or "CA_vpn_96"
*/
class MYSITE_loader_files : public MYSITE_loader
{
 public:

	buffer2 pfx_dir; // "keys/" on client "zone5/box5" on server

	MYSITE_loader_files(
		MYSITE_loaded * _loaded,
		SITE_X509_layout * _scheme,
		CB_get_phrase_base * _cb_phrase,
		const char * _pfx_dir = "db_certs/"
	);

	// non-virtual custom functions - 
	bool print_filename_cert_pem( buffer2 & buff, SITE_X509_tag_enum tag );
	bool print_filename_cert_txt( buffer2 & buff, SITE_X509_tag_enum tag );
	bool print_filename_key_pem( buffer2 & buff, SITE_X509_tag_enum tag );

	// non virtual _file versions (called by virtual stubs)
	bool load_cert_from_file( SITE_X509_tag_enum tag );
	bool load_cert_and_key_from_file( SITE_X509_tag_enum tag, CB_get_phrase_base * cb_phrase );
	bool save_cert_into_file( SITE_X509_tag_enum tag );
	bool save_self_cert_and_key_into_file();

 virtual
	bool save_cert( SITE_X509_tag_enum tag );
 virtual
	bool save_self_cert_and_key();
 virtual
	bool save_cert_as_txt( SITE_X509_tag_enum tag );

 virtual
	bool load_cert( SITE_X509_tag_enum tag );
 virtual
	bool load_self_cert_and_key( SITE_X509_tag_enum tag, CB_get_phrase_base * cb_phrase );

 virtual
	bool save_key_second_as_now_encrypted();
};

}; // namespace
#endif

