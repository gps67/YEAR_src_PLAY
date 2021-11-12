#include "dgb.h"

class BASE_t
{
 public:
	int X,Y,Z;
	BASE_t()
	{
		INFO("CTOR");
		X = Y = Z = 2;
	}
};

class PLUS_t : public BASE_t
{
 public:
	int extra;

	virtual ~PLUS_t()
	{
		INFO("DTOR");
	}

	PLUS_t()
	: BASE_t()
	{
		INFO("CTOR");
		extra = 1;
	}
};

/*!
	PLUS can be down cast to BASE // C++ just adds 8
	BASE must not be upcast to PLUS // we force C++ to subtract 8
	VTBL is 8 byte pointer to VTABLE

	The problem is that C++ insists on having VTBL at OFFS_ZERO

	If C++ simply placed VTBL at the end of BASE
	where PLUS extends it, all would be easy
	the same pointer would work for both

	MORE sub-sub-classes then use VTBL for their pick

	It would still be a "risk" to upcast
	but once that decision is made, the faff of how is easier,
	and the PTR1 == PTR2 works
	and free(PTR) works 
*/
PLUS_t * upcast( BASE_t * BASE )
{
	PLUS_t * FAKE; // 
	return ( PLUS_t *)(		// 2220
	(char *) BASE +			// 2228
     (	(char *) FAKE -			// 1000
	(char *) (BASE_t *)FAKE		// 1008
     )	);
}

int main()
{
	INFO("started");
	PLUS_t * PLUS_one = new PLUS_t();
	BASE_t * BASE_one = PLUS_one;
	PLUS_t * PLUS_two = upcast( BASE_one );

	INFO("PLUS_one ADDR %p", PLUS_one );
	INFO("BASE_one ADDR %p", BASE_one );
	INFO("PLUS_two ADDR %p", PLUS_two );

	delete PLUS_one;

	return 0;
}
