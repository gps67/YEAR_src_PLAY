#ifndef ht1_iterator_base_h
#define ht1_iterator_base_h


#include "ht1.h" // GHashTable

#include <stdio.h> // FILE *
#include "str2.h"
#include "table.h"

struct ht_iterator_base;

typedef void  (* G_for_this_3) (
	KEY_T * _key,
	VAL_T * _val,
	ht_iterator_base * iterator
);

/*!
	Iterate over a hash table
*/
struct ht_iterator_base : public GRP_lib_base
{

// this is your data, eg stdout to print to, or SQL to save to
// sub-classes add more
//
//	void * data1;

// this is an easy place to find these values

 //	KEY_T * key;
 //	VAL_T * val;

	ht1 * ht;


//- static
//-	void G_for_this(
//-		KEY_T * key,
//-		VAL_T * val,
//-		ht_iterator_base * iterator
//-	);

	ht_iterator_base( ht1 * _ht ) {
		ht = _ht;
	}

	void run(G_for_this_3 func);
};

#endif
