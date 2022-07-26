#ifndef tbl_enum_H
#define tbl_enum_H

 // general presumption is strings are pre-allocated in ROM by CC
 // so owner of these structs takes responsibility for them

 struct enum_row_t {
	const char * name;
	const char * alias;
	const char * desc;
	int val_int;
 };

 struct enum_tbl_t {
	str0 tbl_name;
	str0 tbl_desc;
	str0 pfx;
	int N_rows;
	enum_row_t * rows;

	enum_tbl_t(
		str0 _tbl_name,
		str0 _tbl_desc,
		str0 _pfx,
		int _N_rows,
		enum_row_t * _rows
	)
	: tbl_name(_tbl_name)
	, tbl_desc(_tbl_desc)
	, pfx(_pfx)
	, N_rows(_N_rows)
	, rows(_rows)
	{
		// table
	}

	bool dump();
	bool lookup_val( int val, int & idx );
	bool lookup_name( str0 name, int & idx );
	const char * name_of_value( int val );
 };

#endif
