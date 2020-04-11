


// #include "FIX_GCC.h"
// #include "cpu_var_GCC.h"
#include <stdio.h>
 

   struct CTXT_t
   {
     int x;
     int y;

     CTXT_t()
     {
     	x = 5;
	y = 6;
     }

     void print( const char * msg = NULL )
     {
     	if(!msg) msg = "print()";
     	printf("  CTXT %s\n", msg );
     	printf("  CTXT x = %d\n", x );
     	printf("  CTXT y = %d\n", y );
     }

   };

bool init_cpu_var_CTXT()
{
	static CTXT_t CTXT;
	cpu_var_CTXT = & CTXT;
	return true;
}

int main()
{
	init_cpu_var_CTXT();
	cpu_var_CTXT -> print("from_main");
	return 0;
}

