#ifndef enum_var_H
#define enum_var_H

#include "enum_cls.h"
#include "enum_val.h"
#include "P_ref_base.h"

/*!
	An P_Enum VAR is a P_refr _holder of an enum_val.

	VALUES never change, but VARIABLES do.
*/
struct P_Enum : public P_ref<enum_val>
{

	P_Enum( bool )
	{
		// I'm avoiding default constructor - since one SHOULD be used
		// could make it a protected function
	}

	P_Enum( enum_cls * cls )
	{
		if( !cls ) throw "NULL cls coding error";
		claim_this( cls->get_val_unset() );
	}

	P_Enum( enum_val * val )
	{
		if( !val ) throw "NULL val coding error";
		// lets hope its an enum_val type with enum_cls!
		claim_this( val );
	}

	enum_cls * get_enum_cls() const
	{
		if( !ref ) return NULL; 
		return self()->get_enum_cls();
	}

	/*!
		change this P_Enum to value extracted from ... enum, int, string, ...
	*/
	bool set_from_any( PyObject * v ) 
	{
		// this vars e
		enum_cls * cls = get_enum_cls();
		if( !cls ) { return false; }

		enum_val * val = cls->find_by_any( v );
		if( val ) {
			claim_this( val );
			return true;
		} else {
			PY_Static::TypeError( "ENUM::set_from_any( name|val|pair )" );
			return false;
		}
	}

	bool set_from_int( int vi )
	{
		enum_cls * cls = get_enum_cls();
		if( !cls ) { return false; }
		enum_val * val = cls->find_by_val( vi );
		if( !val ) return false;
		claim_this( val );
		return true;
	}

	operator str0() const
	{
		if(!self()) throw "NULL value in P_Enum";
		return (str0) *self();
	}

	operator int() const
	{
		if(!self()) throw "NULL value in P_Enum";
		return (int) *self();
	}

};

#endif

