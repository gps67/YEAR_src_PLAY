#ifndef T_field_H
#define T_field_H

// typedef enum T_field
enum T_field
{
	T_amnt,
	T_date,
	T_date_rel,
	T_percent,
	T_str_key,		// str used as key
	T_str_desc,		// str may contain spaces

	T_link_grp,	// dotdot in same table
	T_link_row,	// row is in another table

	T_expr		// something that eventually returns T_plainer
};

/*
	types from neko:

		NULL
		INT
		FLOAT
		BOOL
		ARRAY
		FUNCTION
		STRING
		OBJECT
		ABSTRACT	of kind ...

	python:

		PyObject - they all are

		- Mutable
		- Immutable
		- XTN_with_feature
		- EXTN_with_pointer_to_that;

	These here are the types found in a spread sheet
	(very simple accounting types)
	Maybe merge in others, and appreciate alias mappings.
	
*/

#endif
