#include "tbl_cell_base.h"

// to print amnt
#include "buffer2.h"

namespace ACCTS {

// -- cell_base

str2 * cell_base::get_str2() {
	return new str2( get_str0() );
}

str0 cell_base::get_str0() {
	return "<<-X->>";
}

void cell_base::prints()
{
	e_print("%s", (STR0) get_str0() );
}

// -- cell_str1

	cell_str1::cell_str1( str0 s )
	: data( s )
	{
	}
	str0 cell_str1::get_str0() {
		// if( str2_display_cached ) return str2_display_cached;
		return data;
	}
	bool cell_str1::set_str0(str0 s) {
		data.set( s );
		return TRUE;
	}

// -- cell_date

	bool cell_date::set_str0(str0 s) {
		cache_clear();
		if(s=="-") 
			e_print("expected date got dash '%s'", (STR0) s);
		e_print("set_str0 '%s'", (STR0)s);
		return date.set_year_mm_dd( s );
	}
	void cell_date::set_date( date_glib _date ) {
		cache_clear();
		date = _date;
	}
	str0 cell_date::get_str0() {
		if( str2_display_cached ) return *str2_display_cached;
		static str1 asap;
		date.get_year_mm_dd( asap );
		return asap;
	}
	str2 * cell_date::get_str2()
	{
		if( !str2_display_cached )
			str2_display_cached = new str2( get_str0() );
		return str2_display_cached;
	}
	cell_date::cell_date( str0 s )
	{
		str2_display_cached = NULL;
		date.set_year_mm_dd( s );
	}
	str2 * cell_date::get_YEAR_MM()
	{
		static str1 asap;
		date.get_year_mm( asap );
		return new str2( asap );
	}
	str2 * cell_date::get_DD()
	{
		static str1 asap;
		date.get_dd( asap );
		return new str2( asap );
	}
	IS_DIFF_t cell_date::cmp( cell_date & date2 )
	{
		return date.cmp( date2.date );
	}
	IS_DIFF_t cell_date::cmp( cell_date * date2 )
	{
		if( !date2 ) return IS_MORE;
		return date.cmp( date2->date );
	}

// -- cell_amnt

	bool cell_amnt::set_str0(str0 s) {
		str2_display_cached = NULL;
		return amnt.set( s );
	}

	str0 cell_amnt::get_str0() {
		if( str2_display_cached ) return *str2_display_cached;
#warning this is for amnt to do itself
	static	buffer2 buf;
		buf.clear();

		amnt.get_csv_str( buf );
                return buf;

		// it is still a CSV to be parsed
		// so £ and (GBP) and not used

		amnt.get_curr_str( buf );
		buf.print("(GBP)");
                return buf;
                double d = amnt.pence;
                d = d / 100.0;
                buf.printf( "%.2f", d );
                return buf;
	//	return amnt.get_temp_str() ;
	}
	str2 * cell_amnt::get_str2() {
		if( !str2_display_cached )
			str2_display_cached = new str2( get_str0() );
	//		str2_display_cached = new str2( amnt.get_temp_str() );
		return str2_display_cached;
	}
	void cell_amnt::set_amnt( amnt_t _amnt )
	{
		str2_display_cached = NULL;
		amnt = _amnt;
	}
	cell_amnt::cell_amnt( amnt_t _amnt )
	{
		str2_display_cached = NULL;
		amnt = _amnt;
	}
	cell_amnt::cell_amnt( str0 s )
	{
		str2_display_cached = NULL;
		amnt.set( s );
	}
	IS_DIFF_t cell_amnt::cmp( const cell_amnt & amnt2 )
	{
		return amnt.cmp( amnt2.amnt );
	}
	IS_DIFF_t cell_amnt::cmp( const cell_amnt * amnt2 )
	{
		if( ! amnt2 ) return IS_MORE;
		return amnt.cmp( amnt2->amnt );
	}

// -- cell_percent --

	bool cell_percent::set_str0(str0 s) {
		str2_display_cached = NULL;
		return percent.set( s );
	}
	void cell_percent::set_percent( per_cent_t amnt2 ) {
		str2_display_cached = NULL;
		percent = amnt2 ;
	}
	str0 cell_percent::get_str0() {
		if( str2_display_cached ) return *str2_display_cached;
		return percent.rate_str;
	}
	str2 * cell_percent::get_str2() {
		if( !str2_display_cached )
			str2_display_cached = new str2( percent.rate_str );
		return str2_display_cached;
	}
	cell_percent::cell_percent( str0 s )
	{
		str2_display_cached = NULL;
		percent.set( s );
	}
	IS_DIFF_t cell_percent::cmp( cell_percent & amnt2 )
	{
		return percent.cmp( & amnt2.percent );
	}
	IS_DIFF_t cell_percent::cmp( cell_percent * amnt2 )
	{
		if( ! amnt2 ) return IS_MORE;
		return percent.cmp( & amnt2->percent );
	}


// -- cell_date_rel

	bool cell_date_rel::set_str0(str0 s) {
		cache_clear();
		return date_val.set_year_mm_dd( s );
	}
	str0 cell_date_rel::get_str0() {
		if( str2_display_cached ) return *str2_display_cached;
		int days_diff = date_val - date_rel;
		if( !days_diff ) return "=";
		static char buff[20];
		if( days_diff > 0 )
			sprintf( buff, "+%d", days_diff );
		else
			sprintf( buff, "%d", days_diff );
		return buff;
	}
	str2 * cell_date_rel::get_str2()
	{
		if( !str2_display_cached )
			str2_display_cached = new str2( get_str0() );
		return str2_display_cached;
	}
	/*!
		constructor
	*/
	cell_date_rel::cell_date_rel( date_glib & d0, date_glib & d1 )
	{
		str2_display_cached = NULL;
		date_rel = d0;
		date_val = d1;
		days_diff_cache = 0;
	}
	str2 * cell_date_rel::get_YEAR_MM()
	{
		static str1 asap;
		date_val.get_year_mm( asap );
		return new str2( asap );
	}
	str2 * cell_date_rel::get_DD()
	{
		static str1 asap;
		date_val.get_dd( asap );
		return new str2( asap );
	}
	IS_DIFF_t cell_date_rel::cmp( cell_date_rel & date2 )
	{
		return date_val.cmp( date2.date_val );
	}
	IS_DIFF_t cell_date_rel::cmp( cell_date_rel * date2 )
	{
		if( !date2 ) return IS_MORE;
		return date_val.cmp( date2->date_val );
	}
}; // NAMESPACE
