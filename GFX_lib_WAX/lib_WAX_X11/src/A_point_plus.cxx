#include "A_point_plus.h"
#include "buffer1.h"
// #include "str0.h"

namespace WAX {

STR0
A_Point ::
print( buffer1 & buf )
{
	buf.print("(%d,%d)", x, y );
	return (STR0) buf;
}

}; // namespace
