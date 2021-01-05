
#include "ht1_iterator_test.h" // GHashTable

#include <stdio.h> // FILE *

void  ht_iterator_test::print1(
	KEY_T * _key,
	VAL_T * _val,
	ht_iterator_test * iterator
)
{
	str2 * desc = _val->desc1;
	fprintf( iterator->file, "KEY = %s VAL.DESC = %s\n",
		_key->get(),
		desc->get()
	);
}

