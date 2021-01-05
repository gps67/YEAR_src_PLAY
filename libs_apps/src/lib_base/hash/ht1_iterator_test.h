#ifndef ht1_iterator_test_h
#define ht1_iterator_test_h

#include "ht1_iterator_base.h" // GHashTable
#include <stdio.h> // FILE *

struct ht_iterator_test : public ht_iterator_base{

// this is your data, eg stdout to print to, or SQL to save to
// sub-classes add more
//
//	void * data1;

	FILE * file;

	ht_iterator_test( ht1 * _ht, FILE * _file)
	: ht_iterator_base( _ht )
	, file( _file )
	{
	}

 static
	void  print1(
		KEY_T * _key,
		VAL_T * _val,
		ht_iterator_test * iterator
	);
};


#endif
