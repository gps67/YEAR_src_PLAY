
#include "u32_RGBA.h"
#include "dgb.h"

//static
bool
::WAX::
u32_RGBA_t::
test_byte_order()
{
	u32_RGBA_t word;
	u32 * plain_word = (u32 *) & word;
	word.set( 0,0,0); // A == 0xff
	word.set( 0x11,0x22,0x33); // A == 0xff
	INFO("RGBA A=0xFF 0x%8X AAGGBBRR", * plain_word ); // 0xFF000000
	// on x86 lohi order of bytes in word is 0xAAGGBBAA
	// on x86 lohi order of bytes in mem is RR GG BB AA
	return true;
}
