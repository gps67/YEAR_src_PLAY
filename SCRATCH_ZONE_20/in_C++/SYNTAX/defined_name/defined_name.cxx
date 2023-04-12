#include <stdio.h>

#define ONE 1
#define TWO 2

// #item // text passed into macro // unexpanded // as "string"
//  item // text passed into macro //   expanded // as code_text

#define show( item ) printf("'%s' -->  %d\n", #item, item )

int main()
{

	// output:
	//
	//	'ONE' -->  1
	//	'TWO' -->  2
	//	'1 +2' -->  3	// SP was preserved
	//	'ONE + TWO + 100' -->  103

	show( ONE );
	show( TWO );
	show( 1 +2 );
	show( ONE + TWO + 100 );
	
	return 0;
}
