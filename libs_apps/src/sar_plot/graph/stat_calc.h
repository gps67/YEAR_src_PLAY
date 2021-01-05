#ifndef stat_calc_H
#define stat_calc_H

// typedef long double flt80;

#include <math.h>

/*!
	stat_calc calculates the average and standard deviation
	of the points you give it.

	You should give it each point separately (X*X), not after
	any averaging.

*/
class stat_calc : public GRP_sar_plot
{
 public:
	flt80 sum_val;
	flt80 sum_sqr;
	flt80 count;

	stat_calc()
	{
		init();
	}
	void init()
	{
		sum_val = 0.0;
		sum_sqr = 0.0;
		count = 0.0;
	}

	void incl( const flt80 val )
	{
		sum_val += val;
		sum_sqr += val * val;
		count ++;
	}
	void calc_vary( flt80 & avg1, flt80 & vary1 )
	{
		if( count < 1.5 )
		{
			if( count < 1.0 )
			{
				avg1 = 0.0;
				vary1 = 1.0;
				return;
			}
			avg1 = sum_val / count;
			vary1 = avg1 * 3.0;
			return;
		}

		// VARY = SQRT of average difference from mean squared
		// except its / n-1 not n

		flt80 count_minus_1 = count - 1;

		flt80 avg = sum_val / count;
		flt80 vary;
		vary = sum_sqr - (count * avg * avg );
		vary /= count_minus_1;
		vary = sqrt( vary );

		avg1 = avg;
		vary1 = vary;
	}
};

#endif
