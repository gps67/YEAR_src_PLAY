/*
	XACT is UNUSED - its a 'hardcoded' alternative to col_spec (sketch)

	There would still be scope for a virtual field pointer:
	a VTBL object that can point to even simple ints in a container struct,
	but theres no need as the code would do most things, and if the
	input CSV changes, the code would too.

	Its likely that XACT:TRANS happens, but XACT:SPEC doesnt, but ...

	This file is unlikely to be used, its the meta-info about each
	object/field, possible expr to item.
*/

namespace XACT
{

 class Row_Spec;

/*!
	VAL SPEC

	This could have
		T_struct	with Row_Spec =  Spec[]
		T_array		of basic type Spec
*/

  typedef enum {
	T_ERROR_UNSET,
	T_amnt,
	T_date,
	T_date_rel,
	T_rate,
	T_str_key,
	T_str_desc,
	T_link
  } Type_Tag;

 class Val_Type_Spec : public GRP_lib_accts
 {
  public:
	Type_Tag	type_tag;

	Val_Type_Spec( _type_tag )
	: type_tag( _type_tag )
	{
	}

	str0 name()
	{
		return name_from_tag( type_tag );
	}

	bool set_by_name( str0 name )
	{
		type_tag = tag_from_name( name );
		return( type_tag != T_ERROR_UNSET );
	}

	str0 name_from_tag( Type_Tag _type_tag )
	{
	 switch( _type_tag ) 
	 {
	  case T_amnt:		return "T_amnt";
	  case T_date:		return "T_date";
	  case T_date_rel:	return "T_date_rel";
	  case T_rate:		return "T_rate";
	  case T_str_key:	return "T_str_key";
	  case T_str_desc:	return "T_str_desc";
	  case T_link:		return "T_link";
	  default:
		 g_print("ERROR");
		 return "T_ERROR_UNSET";
	 }
	}

	static Type_Tag tag_from_name( str0 name )
	{
	 if( name == "T_amnt" )		return T_amnt;
	 if( name == "T_date" )		return T_date;
	 if( name == "T_date_rel" )	return T_date_rel;
	 if( name == "T_rate" )		return T_rate;
	 if( name == "T_str_key" )	return T_str_key;
	 if( name == "T_str_desc" )	return T_str_desc;
	 if( name == "T_link" )		return T_link;
	 g_print("ERROR");
	 return T_ERROR_UNSET;
	}
 };

/*!
	FIELD SPEC
*/

 class Field_Spec : public obj_ref
 {
  public:
	Val_Type_Spec	type_spec;
	obj_hold<str2>	name;
	obj_hold<str2>	desc;
	obj_hold<Row_Spec> row_spec; // check row is valid
	int		field_offs;
	obj_hold<table>	table; // this pins the spec down too much ?

	Field_Spec( 
		Type_Tag	_type_tag,
		str0		_name,
		str0		_desc,
		Row_Spec *	_row_spec,
		int		_field_offs
	)
	: type_spec( _type_tag )
	, name( _name )
	, desc( _desc )
	, row_spec( _row_spec )
	, field_offs( _field_offs )
	{
	}

	void * GET_EA_IN( void * _struc )
	{
		char * P = (char *) _struc;
		return P + field_offs;
		// gcc extn - a cast IS (not) an lvalue
	}
 }

 class FieldSpec_Amnt : public Field_Spec
 {
	FieldSpec_Amnt(
		str0	_name,
		str0	_desc,
		Row_Spec *	_row_spec,
		int	_field_offs
	)
	: Field_Spec( T_amnt, _name, _desc, _row_spec, _field_offs )
	{
	}

	amnt_t * get_EA_in( void * _struc )
	{
		return (amnt_t*) GET_EA_IN( _struct );
	}
 };

/*!
	FIELD VALUES
*/

 class fld_base		// completely owned by row // with VTBL
 {
  public:
	virtual Field_Spec * get_fld_spec();
 };

 class fld_amnt : public fld_base
 {
  public:
	amnt_t	amnt;

	virtual Field_Spec * get_fld_spec()
	{
		static obj_hold<Field_Spec> spec; // init NULL
		if( !spec )
		{
			spec = new Field_Spec
		}
		return spec;
	}
 };

/*!
	A fld_link_XXX is a pointer to a row in another table

	all fld_link_XXX are grouped, each implements its own
	specific link class, but pattern similarly

	A linked row is a pointer held in a field. That field has a 
	name, desc, ...  (and possibly set/get funcs)
*/

 class fld_link_base : public fld_base
 {
  public:
	// obj_hold<row_vat>	link; // each subclass provides this
 };

 class fld_link_vat : public fld_link_base
 {
  public:
	obj_hold<row_vat>	link;
 };

/*!
	ROW SPEC

	Each row_inst has a (shared) row_spec, which holds a list of
	fld_specs. Note that this information is known to the C++ compiler,
	and is also registered by the running program.

	The program can use compiler named fields, the introspector
	uses the registered ones (in a dyn list).

	This duplication can be confusing, and appears wasteful,
	(like duplicated sections in a cobol program), but its
	a manual implementation of a meta-object-preprocessor,
	and opens the door to attaching consistency, and maybe
	a script interface.

	An alternative was to add the spec fields to the C++ class,
	but there is no apparent instance of the C++ class (itself
	not its value instances). That COULD use a virtual function
	that returns the spec ...

*/
 class Row_Inst_base : public obj_ref
 {
	obj_hold<Row_Spec_base> _spec;
  public:
 };

 class Row_Spec_base : public obj_ref
 {
  public:
	dyn_array<Field_Spec>	field_specs;
 };

 class Row_Inst_trans : public Row_Inst_base
 {
  public:
	Field_Inst_link_vat		vat;
	Field_Inst_link_cat		cat;
	Field_Inst_amnt			amnt;
	Field_Inst_link_acct		acct;

	Row_Spec_trans * get_spec()
	{
		return (Row_Spec_trans *) _spec;
	}
 };

 class Row_Spec_trans : public Row_Spec_base
 {
  public:
	Field_Spec_link_vat		vat;
	Field_Spec_link_cat		cat;
	Field_Spec_amnt			amnt;
	Field_Spec_link_acct		acct;
	Row_Spec_trans()
	: vat( "vat", "The VAT Rate", this, offsetof(Row_Inst_trans::vat ) )
	{
		field_specs.append( &vat );
	}
	void declare_names()
	{
		field_specs.add( "vat", "The VAT RATE"
	}
 };

/*!
	ROW INSTANCE
*/

 class row_inst_base : public obj_ref
 {
  public:
 };

 class row_inst_trans : public row_inst_base
 {
  public:
	fld_link_vat		vat;
	fld_link_cat		cat;
	fld_amnt		amnt;
	fld_link_acct		acct;
 };

}; // namespace

