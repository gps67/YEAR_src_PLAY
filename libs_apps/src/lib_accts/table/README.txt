	libs_apps/src/lib_accts/table/

	col_spec is a bit wierd, 
	try renaming 

		col_spec	"field_selector"
		row_inst	"object_struct"

		table		"fixed_array_that_defines_a_type"
				includes spec of each row
				includes list of most rows (allocator?)

	Currently each row is a list of columns, but it could
	be a struct and not an aray. Each column has an index
	(not an offset). Each column has a type col_spec_XXX.

	get/set seems to focus on STRINGS, but there was an attempt
	to define a set of basic types (date, amount, percentage, string, ...)
	If it went down the struct/object path, there would be
	expressions built from the input expressions. The interpreter
	could do much that a compiler does, and check the arg-types.

