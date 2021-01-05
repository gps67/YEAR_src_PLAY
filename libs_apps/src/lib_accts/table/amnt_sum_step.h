#ifndef amnt_sum_step_H
#define amnt_sum_step_H

#include "amnt_t.h"
#include "amnt_sum_step.h"
namespace ACCTS {

/*!
	amnt_sum_step is a running total, each add(amnt) or sub(amnt)
	is added to the sum, and the used flag set.

	Incase two vals are added in one row, val adds the row-sub-tot
	amounts per step, and sum adds the total.

	When not used, the output report will probably contain a dash,
	but the running total is still there. The caller has to do that.
*/
struct amnt_sum_step : public GRP_lib_accts
{
	amnt_t sum;
	amnt_t val;
	bool used;

	amnt_sum_step()
	: sum()
	, val()
	{
		used = false;
	}
	amnt_sum_step( amnt_t v )
	: sum()
	, val()
	{
		sum = v;
		used = false;
	}
	void add( const amnt_t & v )
	{
		used = true;
		val.add( v );
		sum.add( v );
	}
	void sub( const amnt_t & v )
	{
		used = true;
		val.sub( v );
		sum.sub( v );
	}
	void step()
	{
		val.set_zero();
		used = false;
	}
};

}; //NAMESPACE
#endif
