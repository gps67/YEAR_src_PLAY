#ifndef enum_val_H
#define enum_val_H

#include "enum_cls.h"

struct enum_val : public PY_Object_base
{
	P_weak_link<enum_cls> classspec;

	P_Int    	intval;
	P_String 	strval;

	enum_val(
		enum_cls * 	classspec,
		int    		intval,
		const char * 	strval
	);

	operator str0() const
	{
		return (str0) strval.get_str();
	}

	operator int() const
	{
		return intval.get_int();
	}

	enum_cls * get_enum_cls()
	{
		enum_cls * cls = classspec.link_target();
		if(!cls)
		{
			PY_Static::TypeError( "P_Enum has NULL class" );
		}
		return cls;
	}

};

#endif

