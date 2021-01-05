#ifndef obj_inst_flag_bits_H
#define obj_inst_flag_bits_H

/*
	There are some bits in each instance for various uses

	Some are already reserved for system use, maybe GC,
	others can be reserved at a site by some sort of mixin
	then used by other classes (is_a_string_sort_of_thing)

	The allocation of these things is manual, and error prone,
	If worried, write a program that detects conflicts, etc
	Other mixins can run the same gauntlet

	inline functions might TEST_is_something(), SET_is_something()

	There is the potential for a type system with NN primary groups
	and then secondary and tertiary groups.
*/

#ifdef EXPERI_MENTAL_INST_FLAGS

	enum obj_inst_flag_bits {
		BIT_is_readonly,	//<! shared strings // is_shared
		BIT_is_marked_GC_1,	//<! unused
		BIT_is_marked_GC_2,	//<! unused
	// text
		BIT_is_ASCII,		//<! 
		BIT_is_UTF8,		//<! but might also be ASCII
		BIT_is_8859,		//<! but might also be ASCII
		BIT_is_8859_in_UTF8	//<! this is a special case ?
		BIT_uses_CSET_MARKED,	//<! eg BIG5 has virtual get_cset()
		BIT_uses_CSET_ENCODING,	//<! eg BIG5 has virtual get_encoding()
	};

#endif


#endif
