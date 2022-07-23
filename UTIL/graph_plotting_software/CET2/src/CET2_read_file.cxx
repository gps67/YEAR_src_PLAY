
// 600K

#define CET_filenam_ "../CET_datafile"
#define CET_filename "../cetml1659on.dat" 
// haha // defining { filename } messed up every include after that uses it!

// #include "file_line_in.h"
#include <stdio.h>
#include "dgb.h"
#include "buffer1.h"

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
	int line_no;

	// CET file has 7 line header
	const static int line_no_7 = 7; // 7 header liners NON DATA

	// CET file uses -99.99 as marker // approx float
	bool val_is_absent_marker( double val ) {
		return val <= -99.9;
	}

	bool got_year_mm_dd_val( int year, int mm, int dd, double val )
	{
		static int days_in_months[] = {
		0, // SKIP mm==0 
		 31, 28, 31, 30, // FEB APR
		 31, 30, 31, 31, // JUN 
		 30, 31, 30, 31  // SEP NOV
		};

		if( val_is_absent_marker( val )) { // beware float match
			if( dd > days_in_months[mm] )
				return true;
			// leapyear data still gets through
			// but -999 is not noticed on Feb-29 on leap year
			// which never happens :-)
		// visual inspection shows there are NO -999 values in data
			FAIL("value %f <= -99.9 data line %d ", val, line_no );
			exit(1);
		}
		// since there is no -99.9 data, convert it to float
	// SOME DATA uses INT/10 others use FLOAT
	//	double val2 = val / 10.0;
		double val2 = val;
	//	printf("%d-%2.2d-%2.2d %4d\n", year,mm,dd, val );
		printf("%d-%2.2d-%2.2d %5.1f\n", year,mm,dd, val2 );
		// the data will need sorting 
		// currently it is YEAR, DD, MM
		// plain ascii sort will work, because YEAR-MM-DD sorts
		// iso something or other
		return true;
	}

	char * drop_CRLF( const char * line, buffer1 & holder ) { // not edited in situ
		holder.set( line );
		holder.trim_trailing_blanks(); // more than eoln ?
		holder.trailing_nul();
		return (char*)holder.buff;
	}

 	bool got_line( char * line )
	{
		buffer1 holder;
		line = drop_CRLF( line, holder );
	//	line contains CRLF or LF
	//	printf("%s", line );
		const char * fmt = "%d"	// year
		" %lf %lf %lf %lf"		// jan feb mar apr
		" %lf %lf %lf %lf"		// may jun jul aug
		" %lf %lf %lf %lf"		// sep oct nov dec
		" %lf";			// avg
		double v_mm[12+1]; // from 1 so that mm is natural, stops AFTER 12
		int year;
		int t= sscanf(line, fmt,
			&year, 
			&v_mm[1], &v_mm[2],  &v_mm[3], &v_mm[4],
			&v_mm[5], &v_mm[6],  &v_mm[7], &v_mm[8],
			&v_mm[9], &v_mm[10], &v_mm[11], &v_mm[12],
			&v_mm[0]
		);
		if(t == 14 ) {
		 bool vals_absent = false;
		 int dd = 15;
		 for( int mm=1; mm<13; mm++ )  { // [0[12 becomes [1[13
		  if( vals_absent ) {
			if( val_is_absent_marker( v_mm[mm] )) {
				// more absent months in last year
			} else {
				return FAIL("data found after absent");
				// CET file does not have absent months
				// except last year 
			}
		  } else {
			if( val_is_absent_marker( v_mm[mm] )) {
				vals_absent = true;
				WARN("absent data marker");
			} else {
				got_year_mm_dd_val( year, mm, dd, v_mm[mm]);
			}
		  }
		 } // for
		} else {
		 if( line_no <= line_no_7 ) {
			INFO("HEADER LINE line %d line %s", line_no,  line );
		 } else {
		       return
			FAIL("fmt t== %d line %d line '%s'",t, line_no,  line );
		 }
		}
		return true;
	}

 	bool run( const char * filename ) {

		line_no = 0; // starts from 1
		FILE * f = fopen( filename, "r" );
		if(!f) return FAIL("fopen %s", filename );
		char line[1024]; // 80 + CRLF //
		char * S;
		while( S = fgets(line, sizeof(line), f) ) {
			line_no ++; 
			if(! got_line( S ) ) 
				return FAIL_FAILED();
		}

		return true;
	}
};

int main( int argc, char ** argv ) {

	CET_LINE_BY_LINE_t fd1;
	fd1.run( CET_filename );

	return errno;
}
