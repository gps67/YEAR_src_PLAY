
#include "tty_stty.h"
#include "dgb.h"

using namespace BASE1;

extern bool stty_test1();

int main()
{
	if( stty_test1() )
		return 0;
	return 1;
}

