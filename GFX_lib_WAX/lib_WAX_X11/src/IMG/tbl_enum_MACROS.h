#ifndef tbl_enum_MACROS_H
#define tbl_enum_MACROS_H


 #define _X2_N_ARY( ary ) ( sizeof(ary) / sizeof(ary[0]) ) // ints.h //

 #define ENUM_TBL( pfx, tbl_name, tbl_desc, rows_name, rows_data... ) \
 	\
	 enum_row_t rows_name[] = rows_data ; \
	\
	 enum_tbl_t  tbl_name( #tbl_name, tbl_desc,  pfx,  N_ARY(rows_name), rows_name );

 #define ITEM3(ENU,alias,desc) { #ENU, alias, desc, ENU },
 #define ITEM2(ENU,alias) ITEM3(ENU,alias,NULL)
 #define ITEM1(ENU) ITEM3(ENU, NULL, NULL)


#endif

