#ifndef var_tree_H
#define var_tree_H VAR_FILE_REALLY_OK_H

#include "obj_ref.h"
#include "str1.h"
#include "obj_list.h"
#include "int_bitmap.h" // already in obj

// STUBS

	class key_holder;	// holds prepared key = fn( phrase, purpose etc)

// TODO:	trim leading and trailing blanks for set_from_text(buff)

namespace VARS {

 class V_file_decl;
 class V_section_decl;
 class V_var_decl;

 class V_vartype_t;	// NOT UDEF with SPEC + INST // HARD CODED MOSTLY

// class V_file_inst;
// class V_section_inst;
// class V_var_inst;

 struct V_type_traits_t {
	
	int_bitmap flags; // CTOR default should always be called = 0
	void clear_all() { flags.clear_all(); }
	
	enum {
		V_enc_B64,
		V_enc_e1,
		V_enc_e2,
		
		V_is_multi_line,
		
		V_do_not_save,
		V_is_pass,	// hidden in GUI entry
		V_OK
	};
	
	bool TEST_enc_B64()	{ return flags.bit_test( V_enc_B64 ); }
	bool SET_enc_B64()	{ return flags.bit_set( V_enc_B64 ); }
	bool TEST_do_not_save()	{ return flags.bit_test( V_do_not_save ); }
	bool SET_do_not_save()	{ return flags.bit_set( V_do_not_save ); }
	bool TEST_is_pass()	{ return flags.bit_test( V_is_pass ); }
	bool SET_is_pass()	{ return flags.bit_set( V_is_pass ); }
	bool SET_is_multi_line() { return flags.bit_set( V_is_multi_line ); }
	bool TEST_is_multi_line() { return flags.bit_test( V_is_multi_line ); }
	
 };

 /*!
	A VARS::V_vartype_t describes a var type
	
	This may be useful, to guarantee better defaults.
	EG PEM things already have BASE64 applied to them
	EG bool is_password_type - so dont dump too often
	EG bool is_binary_blob +- is_a_bitmap_sort_of_thing
 */
 class V_vartype_t : public obj_ref
 {
  public:
	str1 name;	// TYPE name
	str1 desc;	// TYPE description ...
	V_vartype_t * parent_type;
	
	V_type_traits_t traits;

	bool is_string; // true they all are ...
	bool is_enum; //
	bool is_trivial_subtype_of_parent_type;
	
	V_vartype_t(
		V_vartype_t * _parent_type,
		const char * _name,
		const char * _desc
	);
 };
	
 // everything is a string
	
 V_vartype_t * get_V_vartype_string_parameter();
 V_vartype_t & get_V_vartype( const char * type_name_or_expr );


 class V_file_decl // IS a struct NOT an OBJ
 {
  public:

	str1 crypto_purpose_string;
	str1 title;
	bool must_be_encrypted;

	obj_list<V_section_decl> sections;

	V_file_decl() {}

	V_file_decl(
		const char * _crypto_purpose_string,
		const char * _title 
	);
	bool late_init(
		const char * _crypto_purpose_string,
		const char * _title 
	);

	bool load_from_buffer(
		blk1 & blk
	);
	bool save_into_buffer(
		blk1 & blk
	);
 };

 class V_section_decl : public obj_ref
 {
  public:
	V_file_decl * file;
	str1 name;
	str1 desc;
	obj_list<V_var_decl> vars;
	bool do_not_save;

	V_section_decl();

	V_section_decl(
		V_file_decl & _file,
		const char * _name ,
		const char * _desc
	);

	bool late_init(
		V_file_decl & _file,
		const char * _name ,
		const char * _desc
	);

	bool late_init_no_sto(
		V_file_decl & _file,
		const char * _name ,
		const char * _desc = ""
	);

/*
	V_var_decl * new_var_decl(
		V_vartype_t * _vartype,
		const char * _name,
		const char * _default_value,
		const char * _desc
	);
*/
 };

 class V_var_decl : public obj_ref
 {
  public:
	V_section_decl * section;
	V_vartype_t * vartype; // slightly overstated - future room
	str1 name_sto;
	str1 name_gui; // in primary language
	str1 desc_sto;
	str1 desc_gui;
	str1 default_value;
	str1 current_value;	// errm - thats an INST not a DECL ...


//	bool TEST_enc_B64()	{ return traits.TEST_enc_B64(); }
//	bool SET_enc_B64()	{ return traits.SET_enc_B64(); }
 //	bool TEST_do_not_save()	{ return traits.TEST_do_not_save(); }
 //	bool SET_do_not_save()	{ return traits.SET_do_not_save(); }
 //	bool TEST_is_pass()	{ return traits.TEST_is_pass(); }
 //	bool SET_is_pass()	{ return traits.SET_is_pass(); }
 //	bool SET_is_multi_line() { return traits.SET_is_multi_line(); }
 //	bool TEST_is_multi_line() { return traits.TEST_is_multi_line(); }

	bool TEST_do_not_save()	{ return vartype->traits.TEST_do_not_save(); }
	bool TEST_is_pass()	{ return vartype->traits.TEST_is_pass(); }
	bool TEST_is_multi_line() { return vartype->traits.TEST_is_multi_line(); }
	bool SET_do_not_save()	{ return vartype->traits.SET_do_not_save(); }
	bool SET_is_pass()	{ return vartype->traits.SET_is_pass(); }
	bool SET_is_multi_line() { return vartype->traits.SET_is_multi_line(); }

	V_var_decl();

	void init0();
	void clear();

	void set_default_value( const char * str );

#if 0
	V_var_decl(
		V_section_decl & _section,
		V_vartype_t & _vartype,
		const char * _name,
		const char * _default_value,
		const char * _desc
	);

	V_var_decl(
		V_section_decl & _section,
		const char * _vartype_name,
		const char * _name,
		const char * _default_value,
		const char * _desc
	);
#endif

	bool late_init(
		V_section_decl & _section,
		const char * _vartype_name,
		const char * _name_sto,
		const char * _name_gui,
		const char * _default_value,
		const char * _desc_sto,
		const char * _desc_gui
	);

	bool late_init_no_sto(
		const char * _vartype_name,
		const char * _name_gui,
		const char * _default_value,
		const char * _desc_gui
	);

	virtual bool validate(str0 str);
	virtual bool validate();
	virtual bool set_from_text(str0 text);
	virtual str0 get_as_text();

	operator STR0();

 };



	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

#if 0
 class V_file_inst : public obj_ref
 {
  public:
	obj_hold<V_file_spec> spec;
 };
 class V_section_inst : public obj_ref
 {
  public:
	obj_hold<V_section_spec> spec;
 };
 class V_var_inst : public obj_ref
 {
  public:
	obj_hold<V_var_spec> spec;
 };
#endif


};
#endif
