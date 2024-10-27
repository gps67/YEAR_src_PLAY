#include "stdio.h"

	int N() {
		return 99;
	}

	int N; // = 0 // expect INIT_ZERO_BYTE

	same_name_AVAR_FUNC.cxx:7:13:	// FILE:LINE_CPOS // BPOS //
	error:
	‘int N’ redeclared as different kind of entity


int main()
{
	struct name1 {
		int x;
		const char * str;
		void print() {
			printf("%d %s\n", x, str );
		}
	};
	name1 name1; // no _t needed // some difference found elsewhere
	name1.x = 99;
	name1.str = "hello";
	name1.print();
}
