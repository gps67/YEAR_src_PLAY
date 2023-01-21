
#include "var_tree.h"

// #include "obj_ref.h"
// #include "obj_list.h"

#include "key_holder.h" // holds prepared key = fn( phrase, purpose etc)
#include "key_file.h" // 
#include "g_rrr.h" // 

using namespace VARS;


V_file_decl:: V_file_decl(
	const char * _crypto_purpose_string,
	const char * _title 
) {
	late_init( _crypto_purpose_string, _title );
}

bool V_file_decl:: late_init(
	const char * _crypto_purpose_string,
	const char * _title 
)
{
	crypto_purpose_string = _crypto_purpose_string;
	title = _title;
	must_be_encrypted = false;
// NOT OBJ //	debug_set_obj_name( _crypto_purpose_string );
	return true;
}

//

bool V_file_decl:: load_from_buffer( blk1 & blk )
{
	Key_file kf(*G_rrr::get_global_G_rrr());
	if(! kf.load_from_buf( blk )) return FAIL_FAILED();
	int nsec = sections.N();
	for(int si = 0 ; si < nsec; si ++ ) {
		V_section_decl & sec = *sections[si];
		int nvar = sec.vars.N();
		for( int v=0; v<nvar; v++ ) {
			V_var_decl & var = *sec.vars[v];
			// returns FALSE when default used!
			kf.get_value(
				(STR0) sec.name,
				(STR0) var.name_sto,
				       var.current_value,
				(STR0) var.default_value,
					var.TEST_is_multi_line()
			);
		}
	}
	return true;
}

bool V_file_decl:: save_into_buffer( blk1 & blk )
{
	Key_file kf(*G_rrr::get_global_G_rrr());
	/*
		errm - Key_file id gtk_1b - is that base1? glib not gtk?
		errm - I dont remember stubbing out absent modules

		This was mostly ok until multiline values messed up the look
		PEM texts must be dumped as one lone line,
		optionally with glib-key-file multi-string-annotations
	*/
	kf.set_comment_for_file( title );
	int nsec = sections.N();
	for(int si = 0 ; si < nsec; si ++ ) {
		V_section_decl & sec = *sections[si];
		if( sec.do_not_save ) {
			INFO("SKIPPING SECTION '%s'", (STR0) sec.name );
			continue;
		}
		kf.set_comment_for_group(
			(STR0) sec.name,
			(STR0) sec.desc
		);
		int nvar = sec.vars.N();
		for( int v=0; v<nvar; v++ ) {
			V_var_decl & var = *sec.vars[v];
			if( var.TEST_do_not_save()) {
				INFO("%s Skipping", (STR0) var.name_sto );
				continue;
			}
			INFO("%s.%s = ... %s",
				(STR0) sec.name,
				(STR0) var.name_sto,
				(STR0) var.desc_sto
			);
			kf.set_value(
				(STR0) sec.name,
				(STR0) var.name_sto,
				       var.current_value,
					var.TEST_is_multi_line()
			);
			kf.set_comment_for_item(
				(STR0) sec.name,
				(STR0) var.name_sto,
				(STR0) var.desc_sto
			);
		}
	}
	return kf.save_into_buf( blk );
}

			/////////////////////////////

V_section_decl:: V_section_decl()
#warning there is a GCC bug where this is not init zero by new - insitu X?
: file( NULL )
, do_not_save( false )
{
	// gdb_invoke(1);
	WARN("NULL CTOR needs late_init()");
	debug_set_obj_name( "UNSET" );
	if(do_not_save) {
		WARN("**** DO NOT SAVE IS NOT FALSE ****"); // init zero
	gdb_invoke();
	}
}

V_section_decl:: V_section_decl(
	V_file_decl & _file,
	const char * _name ,
	const char * _desc
)
{
	late_init( _file, _name, _desc );
}

bool V_section_decl:: late_init_no_sto(
	V_file_decl & _file,
	const char * _name,
	const char * _desc
)
{
	do_not_save = true;
	return late_init( _file, _name, _desc );
}

bool V_section_decl:: late_init(
	V_file_decl & _file,
	const char * _name,
	const char * _desc
)
{
	file = & _file;
	name = _name;
	desc = _desc;
	_file.sections.append( this );
	debug_set_obj_name( _name );
	// if(!_name) gdb_invoke(1);
	return true;
}


			/////////////////////////////

void V_var_decl:: init0()
{
	// flags.clear_all();
	// NO // traits.clear_all();
	section = NULL;
	vartype = NULL;
}
void V_var_decl:: clear()
{
	init0();
	name_sto.clear();
	name_gui.clear();
	desc_sto.clear();
	desc_gui.clear();
	default_value.clear();
	current_value.clear();
}

V_var_decl:: V_var_decl()
{
	init0(); // already done
}

bool V_var_decl:: late_init_no_sto(
	const char * _vartype_name,
	const char * _name_gui,
	const char * _default_value,
	const char * _desc_gui
) {

	vartype = & get_V_vartype( _vartype_name );
	name_gui = _name_gui;
	set_default_value( _default_value ); // a TYPE thing ?
	desc_gui = _desc_gui;

	return true;
}

bool V_var_decl:: late_init(
	V_section_decl & _section,
	const char * _vartype_name,
	const char * _name_sto,
	const char * _name_gui,
	const char * _default_value,
	const char * _desc_sto,
	const char * _desc_gui
) {

	if( !_name_gui ) _name_gui  = _name_sto;
	if( !_desc_gui ) _desc_gui  = _desc_sto;


	name_sto = _name_sto;
	desc_sto = _desc_sto;

	bool ok = late_init_no_sto(
		_vartype_name,
		_name_gui,
		_default_value,
		_desc_gui
	);

	debug_set_obj_name( _name_sto );
	// if(!_name_sto ) gdb_invoke(true);

	section = & _section;
	if(section) _section.vars.append( this );
	return ok;
}


// virtual
bool V_var_decl:: validate(str0 str) {
	return true;
}

// virtual
bool V_var_decl:: validate() {
	return true;
}

void V_var_decl:: set_default_value( const char * str )
{
	default_value = str;
	current_value = default_value;
}

// virtual
bool V_var_decl:: set_from_text(str0 text)
{
	if(validate(text)) {
		current_value = text;
		return true;
	} else {
		return FAIL("validation"); // which should maybe be silent?
		return FAIL_FAILED();
		return true;
	}
}

// virtual
str0 V_var_decl:: get_as_text() {
	return current_value;
}

V_var_decl:: operator STR0()
{
	return (STR0) current_value;
}

/////////////////////////////

V_vartype_t:: V_vartype_t(
	V_vartype_t * _parent_type,
	const char * _name,
	const char * _desc
)
: name( _name )
, desc( _desc )
, parent_type( _parent_type )
{
	is_string = true;
	is_enum = false;
	is_trivial_subtype_of_parent_type = true;
	if(_parent_type) {
		traits = _parent_type -> traits;
	} else {
		FAIL("%s _parent_type is NULL", (STR0) name); // only root
	}
}

///////////////

V_vartype_t * VARS::get_V_vartype_string_parameter()
{
	static obj_hold<V_vartype_t> sto = NULL;
	if(!sto) {
		sto = & get_V_vartype( "STRING" );
	}
	return sto;
}

V_vartype_t & VARS::get_V_vartype( const char * type_name_or_expr )
{

	// a quick HOME_BREW DIALECT FORM_FIELD VOCAB HERE
	// VAR == FIELD // EDIT // SCRIPTS //
	// VAR "{ SET VAR VALUE }" // VALUE == "%s" // CODE to GEN DIALECT
	// VALUE "{ STR0 name }" // CODE that creates SELF in SCRIPT // EA //
	// KEY "{ STR0 }" // " KEY %s" // DECL VAR VALUE ${1:-DEFAULT} //
	// ${1:-"{ `ARGV` }"} // TODO test this and use this // PSG transformation rule
	// "$1" // above is an EXPR based on "{ this ITEM = VAR_VAL ]"
	// "typ_NAME" // idx_ITEM = lookup "NAME" // DETAILS += CTXT SESS
	// TYPE_t NAME // CMNT // PSG CODE DECL VAR SESS STO EXPR // %s
	// FIELD_NAME_t FIELD; // PSG DECL NAME auto declares NAME_t and PHRASE
	// FIELD == "FIELD_NAME" // MATCH %s // ANYSTR %s //
	// FIELD in a FORM // DIALECT = words NOUNS SPEC TEXT
	// FIELD has EXPLAIN // AUTO_GEN EXPLAIN_t & EXPLAIN ;
	// VALUE has EXPLAIN // GUESS // PALLETTE // BEHAVIOUR_CODE // EA_
	// TRANSLATE needs ENUM_VALUES as CODE_asif_VAR CODE_VAR
	// CODE_VAR // UDEF GETTER // API //
	// DIALECT NOUN // PASS password for this service SESS // ITEM // ...
	// ... EA OBJ SPEC TABLES STRING_POOL ITEM_IDX_AS_STR0_cident //
	// cident is often filtered to cident_LOCAL PICK == "HERE"
	// cident_BASH_VARNAME_in_USAGE 
	// USAGE when cident used in SCRIPT and TABLE[KEY=="{ cident }"]
	// cident FIELD_NAME %s // OPTION "{ cident FILE_NAME }" //
	// OPTION "{ %s %s }" // CMNT // AUTO_PSG GEN code from MENTION _t
	// OPTION "{ cident %s }" // AUTO_PSG DECODE STEP PARSE STEP
	// OPTION "{ cident FILE_NAME }" // VAR_NAME == "FIELD_NAME" //
	// SCRIPT "{ SCRIPT = { SCRIPT } }" // PRE_WRAPPED
	//
	// COMPILE code in CMNTS // strict MINI_MACHINE //


	static obj_hold<V_vartype_t> typ_ROOT = NULL;
	static obj_hold<V_vartype_t> typ_STRING = NULL;
	static obj_hold<V_vartype_t> typ_PASSWORD = NULL;
	static obj_hold<V_vartype_t> typ_PEM_KEY = NULL;

	/*
		confused - rework needed

		"PASSWORD" should cause asterisks in form fields

		move the bit fields to here
		dont use bits on FIELD_inst only on FIELD_TYPE
	*/

	if(!typ_ROOT) {

	 typ_ROOT = new V_vartype_t(
		NULL,	// ROOT can have either NULL or SELF-ROOT as parent
		"ROOT",
		"ROOT is an abstract class"
	 );

	 typ_STRING = new V_vartype_t(
		typ_ROOT, // parent vartype should be specified to be tidy
		"STRING",
		"Everything is a string"
	 );

	 typ_PASSWORD = new V_vartype_t(
		typ_STRING, // parent vartype should be specified to be tidy
		"PASSWORD",
		"a ******** string"
	 );
	 typ_PASSWORD->traits.SET_is_pass();

	 typ_PEM_KEY = new V_vartype_t(
		typ_STRING, // parent vartype should be specified to be tidy
		"PEM_KEY",
		"a PEM KEY as a string"
	 );
	 typ_PEM_KEY->traits.SET_is_multi_line();


/*
	obj_hold is static but what it points to isnt

	 typ_ROOT->ref_static();
	 typ_STRING->ref_static();
	 typ_PASSWORD->ref_static();
	 typ_PEM_KEY->ref_static();
*/
	}
	// Thats the static initialisation

	
	if(!type_name_or_expr) {

		return * typ_STRING;

	// rewrite so that the STORE is always there
		gdb_invoke(true);
		// type_name_or_expr "STRING";
	}

	str0 name = type_name_or_expr;

	if( !name ) return *typ_STRING;
	if( name == "" ) return *typ_STRING;
	if( name == "STRING" ) return *typ_STRING;
	if( name == "ROOT" ) return *typ_ROOT;
	if( name == "PASSWORD" ) return *typ_PASSWORD;
	if( name == "PEM_KEY" ) return *typ_PEM_KEY;

	FAIL("unknown '%s'", type_name_or_expr );

	return * typ_STRING;
}



///////////////
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


};
#endif


