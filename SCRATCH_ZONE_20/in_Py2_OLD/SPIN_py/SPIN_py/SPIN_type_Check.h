#ifndef SPIN_type_CHECK_H
#define SPIN_type_CHECK_H

// THIS IS // #include "SPIN_type_Check.h"

struct SPIN_dtd_tables;
struct SPIN_sdata_pool;
struct SPIN_elem_info;

extern PyTypeObject SPIN_dtd_tables_Type;
extern PyTypeObject SPIN_elem_info_Type;
extern PyTypeObject SPIN_elem_obj_Type;
extern PyTypeObject SPIN_attr_obj_Type;
extern PyTypeObject SPIN_sdata_Type;
extern PyTypeObject SPIN_tree_print_Type;
extern PyTypeObject SP_peg_kit_Type;
extern PyTypeObject SPIN_top_Type;
extern PyTypeObject SPIN_write_buffer_Type;
extern PyTypeObject SPIN_string_Type;

#define SPIN_dtd_tables_Check(v)	((v)->ob_type == &SPIN_dtd_tables_Type)
#define SPIN_elem_info_Check(v)	((v)->ob_type == &SPIN_elem_info_Type)
#define SPIN_elem_obj_Check(v)	((v)->ob_type == &SPIN_elem_obj_Type)
#define SPIN_attr_obj_Check(v)	((v)->ob_type == &SPIN_attr_obj_Type)
#define SPIN_sdata_Check(v)	((v)->ob_type == &SPIN_sdata_Type)
// #define IS_SPIN_tree_print(v) ((v)->ob_type == &SPIN_tree_print_Type)

#define IS_SPIN_tree_print(v) PY_Static::Expect_Type( v, &SPIN_tree_print_Type)
#define SPIN_write_buffer_Check(v)	((v)->ob_type == &SPIN_write_buffer_Type)

#define Expect_SPIN_dtd_tables(v) PY_Static::Expect_Type( (PyObject* )v, &SPIN_dtd_tables_Type ) 

#endif
