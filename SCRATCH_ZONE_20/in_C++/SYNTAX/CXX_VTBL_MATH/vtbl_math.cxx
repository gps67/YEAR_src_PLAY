#include "dgb.h"

class BASE_t
{
 // BASE_t does NOT have any virtuals // so no VTBL
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
 // PLUS_t does have virtual // so it ADDS VTBL to BASE as a PREFIX 
 // order: { VTBL BASE extra }
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

caddr_t ZERO = 0;

void show( const char * name, caddr_t PTR )
{
	INFO("%s ADDR %p OFF %d", name, PTR, (int) (PTR - ZERO) );
}

#define shows( name ) show( #name, (caddr_t) name )

int main()
{
	INFO("started");
	PLUS_t * PLUS_one = new PLUS_t();
	BASE_t * BASE_one = PLUS_one;
	PLUS_t * PLUS_two = upcast( BASE_one );

	ZERO = (caddr_t) PLUS_one;

	shows(PLUS_one);
	shows(BASE_one);
	shows(PLUS_two);
	shows(& PLUS_one->extra);

	delete PLUS_one;

	return 0;
}
