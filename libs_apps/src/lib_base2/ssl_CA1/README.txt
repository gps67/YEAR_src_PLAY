
CA1 is an experimental MESS, sorry.

	One day, CA1 will be swept away, to give CA2,
	but I have a deadline, and as a closed box, it sort of works.

	The problem is incremental design, and a mental stutter, has led
	to lots of classes, which should be dumb, attempting to know.
	Then the point of intelligent knowledge has repeated over
	several classes.

I really like _VALS_

	These are string values that are to be put into the request
	(maybe some integer values, dates, etc).

	Instead of single stepping over a REQ, look at a bunch of strings.
	Then apply thoses strings to the REQ and to the ISS.

	For some reason, there is a class that holds a _VALS_ and maybe
	knows how to set those values for this site. This class must go,
	but another must come back, to check, or carefully-set values.

	class X509_VALS_base : public obj_ref0
	{
	 public:
		X509_VALS_subject               subject;
		X509_VALS_subjectAltName        subjectAltName;
		X509_VALS_CA_depth              CA_depth;
		X509_VALS_key_usage             key_usage;
		
		X509_VALS_iss_misc              iss_misc;
	//      X509_VALS_time_range            iss_time_range;
		
		bool set_CA_pathlen( int pathlen );


I generally like X509_util = wrappers on openSSL structs

	X509_util_NAME { X509_NAME * subj; ...
	X509_util_EXT {  STACK_OF (X509_EXTENSION) * extlist; ...
NOT	X509_util_CERT { X509_util_NAME subject; X509_util_EXT extensions; ...

	They do openSSL X509 component manipulation,
	building CERT from VALS.
	The last one needs a rename, maybe

I really like: is_CA_ZONE

	To know what a certificate is going to be,
	you need to know its 'point on the tree' and its peergroup identity.

	is_CA_pc (and all its friends) is a good thing,
	and will go a long way as one size fits many,
	then again as two sizes fits most.

	The peergroup identity is the name "PC1234" of the item,
	and the identity of the group ("OUR desk based PC's in Manchester")

	TO this end the _tag has accidentally become an instance,
	when it is supposed to only be an inteligent ENUM.

I sort of like: .._layout

	This knows that the data is to be held in a file, not a DB.

	It knows that for this peergroup, 'pc' is spelled 'LAPTOP'

	It knows the filename is 'keys/CA_ZERO_key.pem'

	It knows that the subject (O/OU) settings come from .. a file

	**IT SHOULD NOT** (but does) know other stuff

I think we need: CA_ONE_REQ_ISS

	This is the action, from end to end, and the owner of all the bits.

	Within the list of parts, there can be multiple customised classes,
	(one customisation is 'load parameters from a file', another is DB),
	which only handle one aspect.

	It will be customised in N-dimensions,
	one of the main ones, being that it is a REQ and ISS on the same box.

	A sibling class, might prepare the REQ on the client (as PKI intended),
	but it would probably have to extract REQ to VALS, check, then apply.
	That would stop sneaky CA parameters getting past.

	Adding this class might help, as it will become the INSTANCE
	that is passed instead of layout/tag, and it can hold the _cert
	for both issuer and issued.

	It would smudge the boundries of things like "REQ keeps client_key"
	but ... with luck ...

Things that will go / change / rename

	SITE_X509_VALS_base.h

		obtaining VALS for a particular CA action
		should come from ....

		If VALS needs any additional intelligence,
		then maybe it can stay, or maybe VALS should be held by ...
		CA_ONE_task_action

	SITE_X509_tag

		this will STOP being an instance (which is CA_ONE_task_action)
		and go back to being a plain enum holder
	


