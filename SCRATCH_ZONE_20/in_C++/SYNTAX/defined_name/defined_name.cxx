#include <stdio.h>

#define ONE 1
#define TWO 2

// #item // text passed into macro // unexpanded // as "string"
//  item // text passed into macro //   expanded // as code_text

#define show( item ) printf("'%s' -->  %d\n", #item, item )

int main()
{
	show( ONE );
	show( TWO );
	show( 1+2 );
	show( ONE + TWO + 100 );
	
	return 0;
}
