
// 600K

#define CET_filename "../CET_datafile"
// haha // defining { filename } messed up every include after that uses it!

// #include "file_line_in.h"
#include <stdio.h>
#include "dgb.h"

/*!
	CET_LINE_BY_LINE_t does both jobs,
	loading parsing and processing the data

	It opens the file, and parses it into lines
	It sends each line to itself,
	and parses it into words (12 months data of day 31)
	YEAR DD Jan Feb ... 
	word is 'C *10, or -999

	direct fgets would be simpler
	this is to show how far libs_apps has lapsed
	and how the update needs DESIGN
*/
class CET_LINE_BY_LINE_t // : public file_line_in
{
 public:
	bool got_year_mm_dd_val( int year, int mm, int dd, int val )
	{
		static int days_in_months[] = {
		0, // SKIP mm==0 
		 31, 28, 31, 30, // FEB APR
		 31, 30, 31, 31, // JUN 
		 30, 31, 30, 31  // SEP NOV
		};
		if( val == -999 ) {
			if( dd > days_in_months[mm] )
				return true;
			// leapyear data still gets through
			// but -999 is not noticed on Feb-29 on leap year
			// which never happens :-)
		// visual inspection shows there are NO -999 values in data
			FAIL("-999");
			exit(1);
		}
		// since there is no -99.9 data, convert it to float
		float val2 = val / 10.0;
	//	printf("%d-%2.2d-%2.2d %4d\n", year,mm,dd, val );
		printf("%d-%2.2d-%2.2d %5.1f\n", year,mm,dd, val2 );
		// the data will need sorting 
		// currently it is YEAR, DD, MM
		// plain ascii sort will work, because YEAR-MM-DD sorts
		// iso something or other
		return true;
	}

 	bool got_line( char * line )
	{
	//	printf("%s", line );
		const char * fmt = "%d %d"
		" %d %d %d %d"
		" %d %d %d %d"
		" %d %d %d %d";
		int v_mm[12+1]; // from 1 so that mm is natural, stops AFTER 12
		int year, dd;
		int t= sscanf(line, fmt,
			&year, &dd, 
			&v_mm[1], &v_mm[2],  &v_mm[3], &v_mm[4],
			&v_mm[5], &v_mm[6],  &v_mm[7], &v_mm[8],
			&v_mm[9], &v_mm[10], &v_mm[11], &v_mm[12]
		);
		if(t == 14 ) {
		for( int mm=1; mm<13; mm++ )  { // [0[12 becomes [1[13
			got_year_mm_dd_val( year, mm, dd, v_mm[mm]);
		}
		} else {
			FAIL("fmt t== %d",t);
		}
		return true;
	}

 	bool run( const char * filename ) {

		FILE * f = fopen( filename, "r" );
		char line[1024]; // 80 + CRLF //
		char * S;
		while( S = fgets(line, sizeof(line), f) ) {
			got_line( S );
		}

		return true;
	}
};

int main( int argc, char ** argv ) {

	CET_LINE_BY_LINE_t fd1;
	fd1.run( CET_filename );

	return errno;
}
