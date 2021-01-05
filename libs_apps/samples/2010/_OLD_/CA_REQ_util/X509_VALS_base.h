#ifndef X509_VALS_base_H
#define X509_VALS_base_H


/*
	These parts are (more-or-less) final type

		struct X509_VALS_subject;
		struct X509_VALS_subjectAltName;
		struct X509_VALS_key_usage;
		struct X509_VALS_CA_depth;

	They are added into something that should have derived

		class X509_VALS_base : public obj_ref0

	You then create your own site-specific, project specific class,
	and various copies for the various CA_{_ZERO,_ONE,_ZONE, ... }
*/

/*!
*/
enum bool3
{
	false3 = 0,
	true3 = 1,
	dunno3 = -1
};

#include "str1.h"

struct X509_VALS_subject
{
	str1 C;		// =GB
	str1 ST;	// =Bucks
	str1 L;		// =Reading
	str1 O;		// =Customer2
	str1 OU;	// =FMC-SERVICE
	str1 CN;	// =C_BOX5_server

	X509_VALS_subject();
	~X509_VALS_subject();
	bool clear();
};

/////////////////////////////////////////

struct X509_VALS_subjectAltName
{
	str1 DNS_1;
	str1 DNS_2;

	str1 IP_1;
	str1 IP_2;

	str1 URI_1;
	str1 URI_2;

	str1 EMAIL_1;
	str1 EMAIL_2;

	bool clear();
	X509_VALS_subjectAltName();
};

/////////////////////////////////////////

/*!
	X509_VALS for key_usage, ie may be used as a client/server/sign/etc
*/
struct X509_VALS_key_usage
{
	bool critical;
	bool digitalSignature;
	bool keyEncipherment;

	bool keyCertSign;
	bool cRLSign;

	X509_VALS_key_usage();
	bool clear();
};

struct X509_VALS_CA_depth
{
	bool3 basicConstraint_CA;
	int basic_constraint_pathLen; // -1 means unset

	X509_VALS_CA_depth();
	bool clear();

	bool set_basicConstraint_CA_false() { return set_CA_pathlen( 0 ); }
	bool set_basicConstraint_CA_true_pathlen_1() { return set_CA_pathlen( 1 ); }
	bool set_basicConstraint_CA_true_pathlen_2() { return set_CA_pathlen( 2 ); }
	bool set_basicConstraint_CA_true_pathlen_3() { return set_CA_pathlen( 3 ); }

	bool set_CA_pathlen( int pathlen );
	bool set_C_not_CA() { return set_CA_pathlen(0); }
};

/*!
	A sheet of X509 CERT values, used to create an X509 REQ or so

	You will also need an RSA, [phrase], or return a new RSA
	NO NO NO - you only need the PUBLIC key ... of the client
	(but again, do we trust our client to create a good one)
*/
class X509_VALS_base : public obj_ref0
{
 public:
	X509_VALS_subject		subject;
	X509_VALS_subjectAltName	subjectAltName;
	X509_VALS_CA_depth		CA_depth;
	X509_VALS_key_usage		key_usage;

	bool set_CA_pathlen( int pathlen );

	// following need to be moved around a bit ...

	//	X509_PUBKEY * pubkey;

	int ISS_version;		// V2 or ...
	int ISS_serialNumber;	// or 20 byte thing?

	time_t ISS_time_start;
	time_t ISS_time_end;

	// functions

	~X509_VALS_base();
	X509_VALS_base();
	bool clear();

	bool set_time_start_now_end_days(int days);

	// sign method is SHA1 or DSS1 or ...
	//

	/*
		See CA_layout_BOX5
		See CA_PROJECT_BOX5
		See CA_PROJECT_ZONE

		CA_ZONE is the CA of the local domain
		CA_ZONE is everything, except its own parent (FM co)
		CA_ZONE does NOT produce CA children

		C_ZONE_server
		C_ZONE_service3

		C_ZONE_user1234
		C_ZONE_pc1234
		C_ZONE_item9876


		now build a tree of classes and instances
		where the parent picks up its defaults from the source
		and the derived modifies one field (CN)

	------------------------
	-- or put another way --
	------------------------

		CA_ZERO - offline CA with a 10 year lifespan
		CA_ONE - the number one ROOT CA (except zero is the root ca)
		CA_BOXES - the CA for machines
		C_BOX57 - the machine in question

		CA_ZERO - offline CA with a 10 year lifespan
		CA_ONE - the number one ROOT CA (except zero is the root ca)
		CA_GROUP57 - ITEMS in group 57 (ITEM=person, ITEM=PC, ITEM=printer)
		C_ITEM_1234 - the ITEM in GROUP57 

		Since ITEMS belong to a GROUP,
		and a group is probably a machine (BOX57)
		there will be a pair of certificates, and private keys

			C_BOX57 - the machine
			CA_GROUP57 - people whose login is controlled by BOX57

		each user will be (or PC, or whatever ITEM of GROUP is)

			C_ITEM_1234

	*/
};

#endif
