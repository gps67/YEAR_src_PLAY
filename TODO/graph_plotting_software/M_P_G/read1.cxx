#define FILE_IN "mpg.txt"

#define BUFF_SIZE 1024

#include <stdio.h>
#include <errno.h>
#include <cstring>

// typedef int bool;



struct line_data {
	int year;
	int mm;
	int dd;
	float miles;
	float litres;
	float cost;
	char note[ BUFF_SIZE ];

	void clear() {
		year = 0;
		mm = 0;
		dd = 0;
		miles = 0.0;
		litres = 0.0;
		cost = 0.0;
		note[ 0 ] = 0 ;
	}

	void show2() {
		char datestr[16];
		sprintf(datestr, "%4.4d-%2.2d-%2.2d", year, mm, dd ); 
		printf("# SHOW2" );
		printf(" %s,", datestr );
		printf(" %6.0f,", miles );
		printf(" %5.2f,", litres );
		printf(" %5.2f,", cost );
		printf(" %s", note );
		printf("\n");
	}

	bool line_in_line( char * buff ) {
		clear();
		static const char * fmt = "%d-%d-%d, %f, %f, %f,%n";
		int pos = 0;
		int t = sscanf( buff, fmt, 
			& year, & mm, & dd,
			& miles,
			& litres,
			& cost,
			& pos //  note
		);
		if(t<6) {
			show2();
		}
		char * eos = buff + strlen( buff );
		eos--;
		while((eos >= buff) && ((*eos == '\n') || (*eos == '\r'))) {
			*eos = 0;
			eos--;
		}
		// snprintf( note, BUFF_SIZE, "'%d/%d %s'", t, pos, buff+pos );
		snprintf( note, BUFF_SIZE, "%s", buff+pos );
		return true;
	}

	bool row_step( line_data & next ) {
		float d_litres = next.litres;
		float d_miles = next.miles - miles;
		float d_km = d_miles * 1.60934;
		if(d_miles < 1 ) {
			printf(" # SKIPPING # d_miles < 1 # \n");
			return false;
		}
		char datestr[16];
		sprintf(datestr, "%4.4d-%2.2d-%2.2d", next.year,next.mm,  next.dd ); 
		float p_per_mile = 100.0 * next.cost / d_miles; 
		float gallons = d_litres / 4.54609; // litres in a gallon
		float pounds_per_gallon = next.cost / gallons ;
		float pounds_per_litre = next.cost / d_litres ;
		float mpg = d_miles / gallons;
		float d_km_100 = (d_km/100.0); // per 100 km
		float l_100 = d_litres / d_km_100; // litres per 100 km
		printf(" %s", datestr );
		printf(" %6.0f +%3.0f %6.0f", miles, d_miles, next.miles );
		printf(" %5.1f p/mile", p_per_mile );
		printf(" %5.2f £/litre", pounds_per_litre );
		printf(" %5.2f £/gallon", pounds_per_gallon );
		printf(" %5.1f mpg", mpg );
		printf(" %5.1f lp100", l_100 );
		printf(" %s", next.note );
		printf("\n");
		return true;
	}

	void printer() {
		printf("%4.4d-%2.2d-%2.2d", year, mm, dd );
		printf("\n");
	}

};

// 2015-09-11, 91034, 31.11, 33.51, To Jim



int main( int argc, char ** argv )
{
	FILE * fin = fopen( FILE_IN, "r" );
	if(!fin) {
		perror( FILE_IN );
		return errno;
	}

	char buff[ BUFF_SIZE ];
	line_data row_prev;
	line_data row;
	int line_no = 0;
	while( char * s = fgets( buff, BUFF_SIZE, fin ) ) {
		// printf("## %s", buff );
		line_no ++;
		if( line_no == 1 ) continue ; // headers

		if( buff[0] == '#' ) {
			printf(" # SKIPPING # COMMENT ## %s", buff );
			continue ; 
		}

		if( !row.line_in_line( buff )) {
			printf("## BLANK LINE ?? ##\n");
			continue;
		}

		if( line_no == 2 ) {
			row_prev = row;
			printf(" # SKIPPING # FIRST DATA LINE ## %s\n", buff );
			// \n not doubled here not in buff
			continue ; // cant calc
		}

		// row.printer();
		if(!row_prev.row_step( row )) continue;
		row_prev = row;
	}


}
