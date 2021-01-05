/*
	XACT is unused, its the hardcoded alternative to col_spec/row_inst
*/

namespace XACT {

class ANode;

/*!
	Each table is a tree with leaves and nodes

	Each node/leaf has a key, desc, etc

	js_obj is the JavaScript peer of this object
	It could also be a Python Object pointer
	(rename script_obj)
*/
class Node_or_Leaf : public obj_ref
{
	ANode *			grp;
	int			sort_pos;
	void *			js_obj;
 public:
	ref_hold<str2>		key;
	ref_hold<str2>		desc;
	virtual bool is_leaf() const = 0;
};

/*!
	ANode is an interior tree node - with no money value

	It can carry logical info such as: typical VAT rate,
	is_income, is_outside_of_tax_1,
*/
class ANode : public Node_or_Leaf
{
 public:
	obj_list<Node_or_Leaf> list;
	bool is_leaf() const { return false; }
};

/*!
	ALeaf is a tree-leaf, but you need a subclass to implement
	the type of leaf
*/
class ALeaf : public Node_or_Leaf
{
 public:
	bool is_leaf() const { return true; }
};

/*!
*/
class Era : public ALeaf
{
 public:
	date_glib		date_0; // normal edges - not absolute
	date_glib		date_2; // first day of next era
};

/*!
*/
class Acct : public ALeaf
{
 public:
	amnt_t			init_bal;
};

/*!
*/
class Cat : public ALeaf
{
 public:
	amnt_t			init_bal;
};

/*!
*/
class Vat : public ALeaf
{
 public:
	per_cent_t		percent;
};

//////////////////

/*!
*/
class Trans : public ALeaf
{
//	ref_hold<str2>		desc;
 public:
	date_glib		date_tax;
	date_glib		date_pay;
	ref_hold<Cat>		cat;
	ref_hold<Acct>		acct;
	amnt_t			amnt;
	ref_hold<Vat>		vat;
//
//	ref_hold<str2>		cheque;
//	ref_hold<str2>		ref_paper;
};

/*!
*/
class Xfer : public ALeaf
{
//	ref_hold<str2>		desc;
 public:
	date_glib		date_tax;
	date_glib		date_pay;
	ref_hold<Cat>		cat;
	ref_hold<Acct>		acct1;
	ref_hold<Acct>		acct2;
	amnt_t			amnt;
//NO	ref_hold<Vat>		vat;
//
//	ref_hold<str2>		cheque;
//	ref_hold<str2>		ref_paper;
};

/*!
*/
class Pay1 : public ALeaf
{
//	ref_hold<str2>		desc;
 public:
	date_glib		date_tax;
	ref_hold<Cat>		cat;
	ref_hold<Acct>		acct_EE; // directors benefit
	ref_hold<Acct>		acct_NI;
	ref_hold<Acct>		acct_PAYE;
	amnt_t			amnt_gross;
	amnt_t			amnt_net_EE;
	amnt_t			amnt_net_ER;
	amnt_t			amnt_NI_ER;
	amnt_t			amnt_NI_EE;
	amnt_t			amnt_PAYE;
};


}; // namespace

