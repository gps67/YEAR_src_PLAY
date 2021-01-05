#ifndef rtts_H
#define rtts_H

#include "obj_ref.h"

/*!
	Run Time Type Spec
*/
namespace RTTS {

class spec_0;
class spec_t;
class spec_func_arg;

/*!
	Puts all RTTS (NON-OBJ) structs into a doxygen subtree
*/
struct GRP_rtts 
{
};

/*!
	Puts RTTS (OBJ ref) objects into a doxygen subtree
*/
struct GRP_rtts_obj : public obj_ref
{
};

/*!
	Most types are named (anon could be named "(BASE) *" with anon flag)

	Most names have SEVERAL forms! (eg mangled function)

		:: NAMESPACE
		NAMESPACE::NAMESPACE
		NAMESPACE::CLASS
		NAMESPACE::VAR
		NAMESPACE::FUNC
		NAMESPACE::CLASS::FUNC mangled()

	for now, there is only "name" which is the local name
	maybe have long_name (fully qualified link_name)
	name_suffix (distinguishing RHS part of mangled function name)

	All names much appear in a single HT, but may be added to more?
	EG var and func in var_list func_list and also in class_items

	Why not simply make this derived from str2? Or HT_key
*/
struct obj_named : public GRP_rtts_obj
{
	obj_hold<str2> name;

	obj_named( str2 * _name )
	: name( _name )
	{
	}

	~obj_named()
	{
	}

};

/*!
	A value is the untyped source text of a constant, eg '"Hello\nWorld"'

	This is a cop-out and enum will lead the way to improve it
*/
struct spec_value_text : public obj_named
{
	str1 text;
};

/*!
	A named member field of a struct
*/
struct spec_field : public obj_named
{
	//! The type of the field
	obj_hold<spec> field_type;

	//! The field name
	obj_hold<str2> name;

	//! The offset of the field in its immediate parent
	int offs;

	obj_hold<spec_value_text> init_value; // Constructor will init NULL or ...
};

/*!
	An arg of a function, and maybe its initial value.

	Note that type spec includes any CONST & modifier
*/
struct spec_func_arg : public obj_named
{
	obj_hold<spec> arg_type;
	obj_hold<str2> name;
	obj_hold<spec_value_text> init_value;
};

/*!
	A function prototype (possibly an anonymous type).
*/
struct spec_func_proto : public obj_named
{
	obj_hold<spec> ret_type;
	obj_hold<str2> name;
	obj_list<spec_func_arg> args;
	bool is_const;
	bool is_inline;
	bool is_static;
};

/*!
	A function.

	Note that RTTS is about the SPEC not the implementation text,
	but its tricky to design one AND leave spece for the other.
*/
struct spec_func : public obj_named
{
	obj_hold<spec_func_proto> proto;
	obj_hold<obj_ref> block;
};



/*!
	Each type, struct, class, union has a SPEC

	Also need an "anonymous" type for pointers, mfns, arrays
*/
struct spec : public obj_named
{
	spec()
	{
	}

	virtual ~spec()
	{
	}
};

/*!
*/
struct spec_pointer : public spec
{
	obj_hold<spec> target;
};

/*!
*/
struct spec_array : public spec
{
	obj_hold<spec> item;
	str1 array_text;
	int array_size; // -1 means no text or unknown as an int
};

/*!
*/
struct spec_enum_group : public spec
{
};

/*!
*/
struct spec_enum_item : public spec
{
};

/*!
	This doesnt use list of item ?
*/
struct spec_struct : public obj_named
{
	obj_list<spec_field> list_field;
	obj_list<spec_func> list_func;


	spec_struct( spec * _parent, str0 _name )
	: spec( spec * _parent, str0 _name )
	{
	}

	~spec_struct()
	{
	}

};

/*!
	
*/
struct spec_union : public spec
{
	obj_list<spec_field> list_field;
	obj_list<spec_func> list_func;


	spec_struct( spec * _parent, str0 _name )
	: spec( spec * _parent, str0 _name )
	{
	}

	~spec_struct()
	{
	}

};

}; // namespace
using namespace RTTS;
#endif
