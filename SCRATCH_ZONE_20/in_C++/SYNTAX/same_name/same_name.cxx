#include "stdio.h"

int main()
{
	struct name1 {
		int x;
		const char * str;
		void print() {
			printf("%d %s\n", x, str );
		}
	};
	name1 name1; // looks like this is permitted // no _t needed
	name1.x = 99;
	name1.str = "hello";
	name1.print();
}
