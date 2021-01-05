#include "P5.h"
#include "dgb.h"
#include "e_print.h"

namespace BASE1 {
namespace Z4_P5 {

bool EDGE_valid( EDGE E )
{
	if(E<0) return FAIL("EDGE < 0");
	if(E>E5_N) return FAIL("EDGE > 4 (5 edges four zones)");
	return true;
}

bool ZONE_valid( ZONE Z )
{
	if(Z<0) return FAIL("ZONE < 0");
	if(Z>=Z4_N) return FAIL("ZONE > 4 (5 edges four zones)");
	return true;
}

bool convert_EDGE( EDGE & E, int from )
{
	E = (EDGE) from;
	if(!EDGE_valid(E)) {
		E = E0;
		return false;
	}
	return true;
}

bool convert_ZONE( ZONE & Z, int from )
{
	Z = (ZONE) from;
	if(ZONE_valid(Z)) {
		return true;
	} else {
		Z = Z0;
		return false;
	}
	return true;
}

}; }; // NAMESPACE S
