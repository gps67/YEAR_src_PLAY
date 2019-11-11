#ifndef EXPRS_H
#define EXPRS_H

#include "obj_ref.h"
#include "buffer2.h"
#include <stdarg.h>

// extern "C" { }
// I created this somewhere, mesh of mish mash
extern const char * str_of_token( int token );


/*
	The C++ tree of classes for E1 expr types

	There may be some need to FLEX delete stray bad paths.
	(On { PSG_scan }  most likely)

	For the sake of a blocked up BISON, no namespace used for EXPR.
	EXPR base class is a universal Tree_Node_Leaf (EXPR * PTR)
	EXPR base class is already virtual, so seems OK 

	Convert Tree to EA_in_ROM IDX of struct_in_ROM (no VTBL in ROM)
	Earlier would be better, maybe.

		Tree_Node_Leaf_EXPR * PTR;

		clutch += EA
		clutch += FRAME // ROM SEGMENT
	
	LISP could be widened to have Tuples
	A few seem ta have strong semantic gravities
	The same few, like addition fields tho! eg 
	
		PAIR = FRAME_TYPE + LHS + RHS + CMNT // +NAME +NAME_from_SPEC()

		LEX = VALUE( "LEX_INTEGER", "-99" ); // + flavour hints 0xFF
		LEX = VALUE( "LEX_IDENT", "cident99" ); // + flavour hints 

		EXPR = SEQ "(" opt_gap EXPR opt_gap ")" // RW_opt_gap 

		TOKEN = { id_FRAME id_ITEM }

			get_FRAME_of_FRAMED_ITEM = done, easy, possible, notnow

				First call is available, // at different level
				and it gives the Layer/STO_segment + handler

				This layer muxes in stuff,
				that may be addressible
				that may be access blocked
				that may be access RO // but ROM builder
				that may be access locks // or logic

				u32_hilo_EA_expr

					SEGMENT = DECODER.segment_one
					FRAME = NULL // but can obtained
					ITEM = struct_in_ROM * PTR

		TOKEN = EA_in_SPEC of item from enum_group=possibly_not_in_EA
		TOKEN = PSG.ROM.token
*/

#include "str1.h"
#if 0
// still having problems giving NameSpace TypeName via FLEX 
namespace EXPRS {
 struct EXPR;
};
using namespace EXPRS;
#endif

/*
	import + REUSE .y token names in EXPRS

	eg PSG_Token PUNCT_CARET_as_EA_in_ROM_of_OBJECT

	u32_hilo EA_token = EA_in_STO_of_OBJECT = OBJ.EA_in_STO
		that is a foreign id placed in OBJ
		guarantee init_NULL init_M1 init_ZERO init_SOON
		if EA == ZERO || EA == M1
		 EA = CALL find_or_recompute OBJ.EA_in_STO
		if EA == ZERO || EA == M1
		 WARN null EA ; return FAIL_FAILED
		 // fail can grab warn if thats all we got
	so
	 rejig my use of EXPR to BASE
*/

// #warning "gen_e1_yacc.tab.hh" ...
#include "gen_e1_yacc.tab.hh"
// #warning "gen_e1_yacc.tab.hh" DONE

 /*!
 	BISON not playing nice, with namespaces used in UNION
	I dont have time to fix it, so kluding global EXPR for PSG_EXPR
	PSG_EXPR_Tree_Node_Leaf_ITEM
	 is a subtype of PSG_EXPR
		PSG_EXPR
		 Tree_Node_Leaf_ITEM
	  that keeps EXPR simple
	 is also wrapper type data container
		Tree_Node_Leaf_ITEM
		 PSG_EXPR
	
	 maybe use a third name, eg HERE=GEN_y

	 	CTXT == GEN_into_yacc_y
	 	CTXT == GET_from_yacc_o
	 	CTXT == PARSE_input
		CTXT == Tree_BUILD

		CTXT == Tree_BUILD for GEN_into_yacc or PSG_Tree_loader
	
	ROM Loader PREBUILT

		dlopen a compiled Module
		Provide it with the ENTIRE export-MAIN-import elf tables
		# elf is fast, dont feel the need to stare at some manuals tho

		Load SCRIPT via PSG_SCRIPT_Parser
		Load SCRIPT via STO_Tokenised_SCRIPT loader // PSG=SCRIPT_Parser
		Load SCRIPTED_BIMARY_DATA via BUILTIN_ROM_of_script_specs

		To build a ROM, you need all the same datatypes, plus writers
		C has STRUCTS and we can sucessively refine input SPEC
		BYTE STREAM loses alignment - OK
		BYTE STREAM might have padded alignment luck alignment - OK

		Tuplo == ... u8 ...
		DECODER of byte_stream
		 OPCODE EA_in_ROM
		    ROM = Layer_ROM( u3_u5 )
		  u3_u5 Layer_6_SEGMENT_30 // Layer_1_segment_22
		     u8 u8 // ROM_TABLE[ u8 ]
		    u16 u16 // TABLE_ITEM[ u16 ] // N_check_maybe_required
	
	LOGIC says u3_u5 are two completely different STEPS in decoder
	SPEED decoding ROM_or_OTHER * inst = DECODE_BYTE_ZERO[[ u3_u5 ] || \
	if(!inst) {
		inst = slow_DECODE_BYTE_ZERO( u3_u5 ); //
		if(!inst) return FAIL_FAILED();
	}

	One Layer of 32 ROMS. No More.
	One Layer of 32 STOS. AND THEN MORE.

		nb ROM is mix of DATA SPEC CODE
	
	u8_ROM_u24_EXPR

		After standard mount ROM, using LIBR anyway
		Get full control over what happens to u24_EXPR

		u24 is one DECODER
		u8 u16 is one DECODER (probably coded from u32_hilo)

		EACH ROM = 256 x 64K
		 64K IDX = common
		 64K OFFS = sometimes
		 64K_click = u64 * click_factor // 1 2 4 8 16 256 1K
	
	u32_ROM_EXPR

		On a 64 bit machine,
		can easily have sparse load of [u32]

		On a UNIX box might have HOLE-FILE but might lose it
		So stay at low end of 4G
		So stay at low end of u24_16M // x4 x16
		
		On a 32 bit machine
		try to stay under 1G, or 16M or 64K !!

	baseclass PSG_EXPR
		Its usage in the PSG world
	baseclass Tree_Node_Leaf
		Its storage in the Tree STO ROM world
	benchClass ITEM
		ITEM hovvers over script, on bench

	PSG_EXPR_Tree_Node_Leaf_ITEM
	// : multi inheritance ? // not very efficient
	// primary class = first mentioned, second is THIS_PLUS_OFFS
	// OPTION field = second_class_appears here as SCU
	// MESSAGE PTR_to_BASE  => ptr_to_field // knows self
	// MESSAGE PTR_to_BASE <=  ptr_to_field // MISSING mix_in
	// with control over our own VTBL we can base-class the VTBLs
	// Then inherit both, simply fills out the VTBL with non-placeholders

	 	
 */


namespace EXPRS {
 struct PRINTER;
};

/*!
	EXPR = virtual BASE CLASS 

	obj_ref0 = ref counted object, needs attentionm obj_holder<t> or &
 	recompiled fine with // struct EXPR : public obj_ref0 {}
 */
 struct EXPR {
  virtual ~EXPR() {}
  virtual void print_to( EXPRS::PRINTER * printer) = 0;
  virtual void print_to_NULL(); // create a dummy printer, use copy out, drop
 };

namespace EXPRS {


 struct PRINTER
 { public:
  buffer2 out;
  int indent;
  virtual ~PRINTER() {
  }
  PRINTER() {
  	indent = 0;
  }

  buffer2 & buff() { return out; }

  virtual void print_EXPR ( EXPR * expr ) 
  {
  	if(!expr) { WARN("NULL expr"); return; }
	expr -> print_to( this );
  }
  void print_EXPR_indented ( EXPR * expr ) {
  	if(!expr) { WARN("NULL expr"); return; }
  	indent ++;
	print_EXPR( expr );
  	indent --;
  }
  void print_STR ( const char * str ) {
  	out.print("%s", str );
  }

  void ind_indent()
  {
  	int(indent);
  }

  void ind(int _ind) {
	for(int i=0; i<_ind; i++ ) {
		out.print("| ");
	}
  }
 };


 // everything (mostly) in EXPRS is a subclass of EXPR
 // EXPR base is outside the namespace

 // Tuples start here, or stay in PAIR
 // soon SEQ <--> STRUCT

 struct EXPR_name : public EXPR {
	str1 name;

 	EXPR_name( const char * _name ) 
	: name( _name )
	{
	}
	void print_to( PRINTER * printer)
	{
		printer -> print_STR( name );
	}
 };

 struct EXPR_lhs_rhs : public EXPR {
	EXPR * lhs;
	EXPR * rhs;
	int op;
	str1 name;
 //	str1 CMNT; // or preparsed ...

	// const char * name;
	EXPR_lhs_rhs()
	{
		init_null();
	}
	void init_null()
	{
		lhs = NULL;
		rhs = NULL;
		op = ' ';
		name = (const char *) NULL;
	}

  // virtual
 	 void print_to( PRINTER * printer)
	 {
		buffer2 & out = printer->buff();
		if(!printer->indent)
			out.print("#### line ####\n");
		if(name) {
			printer->ind_indent();
			out.print("NAME_%s\n", (STR0)name );
			return;
		}
		if(lhs) {
			printer->print_EXPR_indented( lhs );
		}

		// print INDENT "+"
		printer->ind_indent();
		out.print("%s\n", str_of_token(op) );

		if(rhs) {
			printer->print_EXPR_indented( rhs );
		}
	}
};

/*
	these are OK in the EXPRS namespace
	It was only the base class EXPR that we have forced global
	It was only the union decl // struct EXPR * expr;

	these squish everything into a PAIR, even UNIT_Name_Token

	currently returning C++ VTBL pointer
	Maybe push all the virtuals there
	INST_variety_IDENT_VALUE_SEQ_ONE_LIST_RULE_or_INST

 **	Still need to decode from EXPR * rhs; MYTYPE * get_RHS()   **

		(MYTYPE *) rhs	// check_MYTYPE( rhs )

		(MYTYPE *) rhs	// COMES from SPEC
		(MYTYPE *) rhs	// COMES from SPEC FEATURE( EA_MYTYPE = SPEC)

	STILL want to move to u32_hilo_u32_cpu // DECODER


*/

extern EXPR * E1( const char * name );		// expr == IDEN_anystr
extern EXPR * E2( EXPR * lhs_, int op_, EXPR * rhs_ ); // expr == EXPR op EXPR
extern EXPR * mk_E_id( const char * name_ ); // expr == IDEN_anystr // REPEAT ??
extern EXPR * E1( int op_, EXPR * rhs_ );  // expr = TOKEN_unary RHS
extern EXPR * E_plus_E( EXPR * lhs,  EXPR * rhs );  // EXPR + EXPR
extern EXPR * E_times_E( EXPR * lhs,  EXPR * rhs ); // EXPR * EXPR
extern EXPR * E_power_E( EXPR * lhs,  EXPR * rhs ); // EXPR ^ EXPR

}; // namespace

// SYNTAX OK // was union field name not type
// typedef ::EXPRS::EXPR EXPR_t2;
//		using namespace EXPRS;
// SYNTAX OK // was union field name not type
// typedef EXPR EXPR_t;
#endif
