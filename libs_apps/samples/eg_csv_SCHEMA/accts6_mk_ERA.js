

function ERA_quarter_year_desc( dset_ERA, year_name, quarter_name, QN, year, m1, nm ) 
{
	list_mm = [ "12",
		"01", "02", "03", "04", "05", "06",
		"07", "08", "09", "10", "11", "12"
	]
	list_mon = [ "Dec",
		"Jan", "Feb", "Mar", "Apr",
		"May", "Jun", "Jul", "Aug",
		"Sep", "Oct", "Nov", "Dec"
	]
	list_month = [ "December",
		"January",	"February",	"March",
		"April",	"May",		"June",
		"July",		"August",	"September",
		"October",	"November",	"December"
	]

	quarter_desc = "VAT " + QN
	m = m1
	while( nm > 0 )
	{
		month_desc = year + " " + list_month[ m ]
		quarter_desc += " " + list_mon[ m ]
		year_mm = year + "-" + list_mm[ m ]
		dset_ERA[ year_mm ].desc = month_desc
		dset_ERA[ year_mm ].dotdot = quarter_name
		// NEXT
		nm -= 1;
		m += 1
		if( m > 12 ) 
		{
			m = 1;
			year++
		}
	}

	dset_ERA[ quarter_name ].dotdot = year_name
	dset_ERA[ quarter_name ].desc = quarter_desc
}

function mk_ERA( dset_ERA )
{

	// Each quarter has:
	// dset.ERA["2001_Q1"].dotdot = "year_0102"
	// dset.ERA["2001_Q1"].desc = "VAT Q1 Feb Mar Apr"
	//
	// Each Month has
	// dset_ERA[ "2001-02" ].desc = "2001 February"
	// dset_ERA[ "2001-02" ].dotdot = "2001_Q1"

	dset_ERA = dset.ERA
	dset_ERA["year_0001"].desc = "Year 2000 - 2001"
	dset_ERA["year_0102"].desc = "Year 2001 - 2002"
	dset_ERA["year_0203"].desc = "Year 2002 - 2003"
	dset_ERA["year_0304"].desc = "Year 2003 - 2004"
	dset_ERA["year_0405"].desc = "Year 2004 - 2005"
	dset_ERA["year_0506"].desc = "Year 2005 - 2006"

	ERA_quarter_year_desc( dset_ERA, "year_0001", "2000_Q1", "Q1", 2000,  3, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0001", "2000_Q2", "Q2", 2000,  6, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0001", "2000_Q3", "Q3", 2000,  9, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0001", "2000_Q4", "Q4", 2000, 12, 2 )
	// VAT quarter straddles Account Years (out of alignment)
	ERA_quarter_year_desc( dset_ERA, "year_0102", "2001_Q0", "Q0", 2001, 2, 1 )
	ERA_quarter_year_desc( dset_ERA, "year_0102", "2001_Q1", "Q1", 2001, 3, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0102", "2001_Q2", "Q2", 2001, 6, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0102", "2001_Q3", "Q3", 2001, 9, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0102", "2001_Q4", "Q4", 2001, 12, 2 )

	ERA_quarter_year_desc( dset_ERA, "year_0203", "2002_Q0", "Q0", 2002, 2, 1 )
	ERA_quarter_year_desc( dset_ERA, "year_0203", "2002_Q1", "Q1", 2002, 3, 2 )
	ERA_quarter_year_desc( dset_ERA, "year_0203", "2002_Q2", "Q2", 2002, 5, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0203", "2002_Q3", "Q3", 2002, 8, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0203", "2002_Q4", "Q4", 2002, 11, 3 )

	ERA_quarter_year_desc( dset_ERA, "year_0304", "2003_Q1", "Q1", 2003, 2, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0304", "2003_Q2", "Q2", 2003, 5, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0304", "2003_Q3", "Q3", 2003, 8, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0304", "2003_Q4", "Q4", 2003, 11, 3 )

	ERA_quarter_year_desc( dset_ERA, "year_0405", "2004_Q1", "Q1", 2004, 2, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0405", "2004_Q2", "Q2", 2004, 5, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0405", "2004_Q3", "Q3", 2004, 8, 3 )
	ERA_quarter_year_desc( dset_ERA, "year_0405", "2004_Q4", "Q4", 2004, 11, 3 )

	ERA_quarter_year_desc( dset_ERA, "year_0506", "2005_Q1", "Q1", 2005, 2, 3 )
}
