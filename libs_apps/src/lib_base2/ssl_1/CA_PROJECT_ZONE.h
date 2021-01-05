#ifndef CA_PROJECT_ZONE_H
#define CA_PROJECT_ZONE_H

#include "str1.h"
#include "blk1.h"
#include "X509_cert.h"
#include "EVP_P_KEY.h"


/*
	CA_ZONE is the CA for the ZONE community, huddled around BOX5.
	BOX5 is an email server, a fileserver, an office system, etc.

		BOX5 = a linux box, mainframe, or huddle of boxes

	An office infrastructure might have an email box, dns, smb, ...,
	and all the users in that community, register on that one box.
	It might be a single server-box, or a rack of collaborating things,
	but it is a BOX = Community = Domain. The idea, that that a
	community can be provisioned, and given a CA_ZONE.

		CA_ZONE = the certificate authority for the BOX5 community

	In a corporate WAN, BOX5 might be the email-office-server
	for the accounting department in Swansea, with some highly
	secure SMB shares on it, or one of its close peers.

	I previously wanted separate CA_BOX5_USERS from _PCS from _SERVERS
	but that got a bit confusing. Here there is one CA_ZONE at the heart.

		CA_ZONE == the only local CA

	You can still have C_ZONE_service3, to identify the dial-up service,
	even if it is hosted on the same box or a different box.
	In that case, it is implicit that "here == C_ZONE_server == server3".

	Note that this design uses a central (CA_ZONE) pool of user-ids,
	which is NOT ALLOCATED by the dial-up-service, but IS USED by it.
	This is a design limitation, to make it make sense.

	I always want to categorise C_ZONE_USER_4321 from C_ZONE_PC_4123,
	so that a certificate can only be used for one purpose, but this
	will have to be marked in the C_cert data fields, or the list of
	issues certs in the database(s). All of these C_certs
	are produced by CA_ZONE (rename CA_COMMUNITY?)

		TODO: notes on fields within cert are nearby, really!

		SEE: X509 CN="C_BOX5_USER4321"
		SEE: X509 CN="C_USER4321" // issued by CA_ZONE

	Above the one-CA-does-it-all, is the provisioning antecedence.
	The very root is CA_ZERO, which is stored offline, but has a ten
	year lifespan. It signs CA_ONE, which is actually online,
	and can dynamically provision CA_ZONE, which then is providing
	you with your new C_BOX5_USER_4321, which is carried in this struct.

		CA_PROJECT_b == CA_ONE_b 

	This struct does not contain ALL of these fields, you are supposed
	to know if you are a USER or a PC (or possibly BOTH!),
	but this layout might help you stay on top of it.

	There is another complexity (which goes into the field values),
	where the FacilitiesManagementCompany (FMC),
	provides just about everything for the NationalCorporate.

	Note that SSL servers, present their certificate first.
	There are options to request one, options to subsequently
	switch to another, and SIGN_with_ approaches, but the easy way,
	is to present one and stick with it.

		C_BOX5_service3 (or _server) 
		C_ZONE5_service2

		The client might need to have records for each service,
		

	SSL clients can think about it a bit more, but they probably
	decided who they were calling, and for what (why). They might
	have an X509 they use for SSL, or one for SIGN (etc).
	Maybe the X509 is just a vehicle that carries the RSA/pub

		C_ZONE_rsa525221

	You can have your own project layout, this one is called "ZONE",
	your might be known as "COMMUNITY_ZONE_11", good luck.

*/
int CA_PROJECT_ZONE_note;

/*!
	CA_PROJECT_ZONE carries some certificates, from here to there,
	with names that almost make sense. You probably only have one (or zero)
	of the private keys, but if you are using the itemtype name, it helps.
*/
class CA_PROJECT_ZONE
{
 public:
	~CA_PROJECT_ZONE();
	CA_PROJECT_ZONE();

	X509_cert	CA_ZERO;	// ten years offline master
	X509_cert	CA_ONE;		// one year online ROOT (but not root?)

	X509_cert	CA_ZONE;	// A provisioned thing

	X509_cert	C_ZONE_server;	// BOX5 itself, or a close friend
	X509_cert	C_ZONE_user;	// box5 registers user4321
	X509_cert	C_ZONE_pc;	// box5 registers pc4321
	X509_cert	C_ZONE_item;	// box5 registers item4321

	EVP_P_KEY	PRIV_KEY_C_ZONE_server;
	EVP_P_KEY	PRIV_KEY_C_ZONE_user;
	EVP_P_KEY	PRIV_KEY_C_ZONE_pc;
	EVP_P_KEY	PRIV_KEY_C_ZONE_item;
};

	/*
		TODO make CA_ONE a self signed ROOT
		which is ALSO signed by CA_ZERO

		That will require a separate check,
		or even a small list of CA_ZERO peers,
		that vouch for each-other,
		but can be dropped from the chain, (the automatic chain)
		because CA_ONE is at the root.

		TODO make CA_ONE the CA_ROOT,
		but then sign other things with something verisigned,
		eg www.THISISME.dom or www.SERVICE.thisisus.dom
	*/


#endif
