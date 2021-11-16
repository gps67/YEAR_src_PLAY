#ifndef SPIN_ELEM_INFO_H
#define SPIN_ELEM_INFO_H

#include "str1.h"
// #include "buffer/str1.h"
#include "app_types.h"
#include "SPIN_type_Check.h"
#include "PY_Object_base.h"
#include "enum_cls.h"
#include "P_Enum.h"
#include "P_weak_link.h"

struct SPIN_dtd_tables;	// dictionary plus
struct PY_weak_link;

#define ON_Element_Start	0x1
#define ON_Element_End		0x2
#define ON_data			0x4 // unused
#define ON_sdata		0x8 // unused + no meaning ?
#define IS_first_use		0x10 // force DTD to set some flags

struct fmt_bits
{
	bool own_line;
	bool gap_line;
	bool tab_here;
	str1 tab_str;

	fmt_bits()
	: own_line( false )
	, gap_line( false )
	, tab_here( false )
	, tab_str()
	{
	}
};

/*
	
*/
typedef enum Enum_Element_Type
{
	EL_empty,	// <BR>
	EL_pre,		// <PRE>
	EL_inline,	// <B class="blinking">textmostly</B>
	EL_phrase,	// element_breaks_word but not rectangular
	EL_block 	// rectangular block, <LI> <OL> <H1> 
};

struct P_Enum_ET : public P_Enum
{
	enum_cls * get_cls();
	P_Enum_ET( PyObject * any );	
	P_Enum_ET( Enum_Element_Type et );
};


struct SPIN_elem_info : public PY_Object_base
{
	P_weak_link<SPIN_dtd_tables> info_pool_weak;
	// PY_weak_link * info_pool_weak; // dtd where this elem came from
	P_ref_base name;

	P_Enum_ET el_type;
	P_ref_base ctype;	/* rcdata */

	bool element_breaks_word;
	int on_flags;

	bool own_line_before;
	bool own_line_after;
	bool gap_before;
	bool gap_after;

	P_ref_base on_element_start;	/* callable v = EH.ElementStart_H1 */
	P_ref_base on_element_end;	/* callable v = EH.ElementEnd_H1 */

	bool IS_empty()	{ return (int)el_type == EL_empty; }
	bool IS_pre()	{ return (int)el_type == EL_pre; }
	bool IS_inline() { return ((int)el_type == EL_inline) || ((int)el_type == EL_phrase); }
	bool IS_phrase() { return (int)el_type == EL_phrase; }
	bool IS_block() { return (int)el_type == EL_block; }

	void set_defaults_for_et( Enum_Element_Type et );
	void set_defaults_for_et( PyObject * etname );

	SPIN_elem_info(
		SPIN_dtd_tables * _info_pool,
		PyObject * name_,
		PyObject * et,
		PyObject * content_type,
		bool element_breaks_word_
	);
	~SPIN_elem_info();

	int get_on_flag( int mask ) { return on_flags & mask; }
	void set_on_flag( int mask ) { on_flags |= mask; }
	void clr_on_flag( int mask ) { on_flags &= ~mask; }

	SPIN_dtd_tables * get_dtd_tables()
	{
		SPIN_dtd_tables * pool;
		pool =  info_pool_weak.link_target();
		if( !pool )
		{
			 PyErr_SetString(PyExc_AttributeError,"elem_info:info_pool_weak is NULL" );
		}
		return pool;
	}
};

/* EXPORTED */

extern SPIN_elem_info * as_SPIN_elem_info( PyObject * elem );

SPIN_elem_info * new_SPIN_elem_info(
	SPIN_dtd_tables * _info_pool,
	PyObject *name,
	PyObject * etype,
	PyObject *ctype,
	bool word_break
);

int spin_elem_info_is_pre( PyObject *self );

#endif
