This is the old hash

	It works mostly for KEY=str
	All others use KEY=(void*ptr)

	That needs to be reworked (possibly using sort_spec)
	to allow KEY=(obj_ref*others) (other than unique pointers)

	To do that, need to obtain
		Func_Hash
		Func_Equals
		poss_Compare < = >
	Those can be static C funcs, or virtual member funcs?
	Maybe introduce a HT<TYPE1,TYPE2> handler (like sort)

	((Looking at sort, it seems muddled))
