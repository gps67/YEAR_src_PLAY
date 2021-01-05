#include "ht1_iterator_base.h" // GHashTable

#include <stdio.h> // FILE *
#include "str2.h"
#include "table.h"

// #define KEY_T str1
// #define VAL_T row_base_t

void ht_iterator_base::run(G_for_this_3 func)
{
 	GHFunc printer = (GHFunc) func ;
 	g_hash_table_foreach (ht->ht, printer, this);
}
