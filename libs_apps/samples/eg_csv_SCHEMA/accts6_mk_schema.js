#!/usr/bin/env js_many

// js_many is a JavaScript interpreter, with (many) added module(s)
// js_many is js_accts, but also other things (not accts)
// you must put js_many on the PATH, so that /usr/bin/env finds it
// I create a symb-link from $HOME/bin2 to the development dir
// It in turn, must find lib_XXX.so (I used rpath, but you must ... something )

/*
	The actual 'schema' is hardcoded in src/lib_accts/accts/dset.cxx
	col_spec * data_set::guess_col_spec( table * tbl, str2 * colname )              
	and maybe a few places that expect it to work (reports)

	This script generates a datafile that conforms to that schema
*/

ACCTS = new Accts_Module()

ACCTS.load_script( "accts6_mk_TABLES.js" )
ACCTS.load_script( "accts6_mk_ERA.js" )
ACCTS.load_script( "accts6_mk_CAT.js" )
ACCTS.load_script( "accts6_mk_VAT.js" )

/*
	create a set of tables (probably with guess_spec from name)
*/
dset = mk_TABLES( ACCTS )

/*
	save the headers with no rows at all!
*/
dset.save_file("demo_schema_blank.csv")


/*
	add some data, using the function loaded above

	Some fields are mandatory (for the reports etc),
	and the expected type must be correct
	e.g. year_start.value must be a valid "YEAR-MM-DD" string

	The ERA's are VAT eras, where each year has quarters, has months,
	but on an irregular basis. The company year is not calender
	(UK is daft beyind belief), and the VAT zones can be requested
	to re-align (if the request goes the wrong way, you have to
	re-request another change). The demo data, shows how that
	can be managed (introduce dotdot values that match).

	The company year is truncated at start-end, and recorded
	in the INFO (year_start_next is hidden and auto calculated)

	The CAT tree is another area of confusion.
	I personally prefer it to be based on D-T-O-X-I-N,
	but the annual companies house report suggests something else.
*/

/*
	The FRSSE standard (book) says:  (nothing useful)

		sub-heading to use on P + L report

	It also uses terms freely without qualification,
	eg "capitalisation", is that like evaporation or crystalisation?
	It sounds like a convertion, so it takes from which column,
	and adds to which column(s) ? Or is it converting from cash
	to asset?

	Its used many times, but never clarified. Basically the book
	is a companion to a degree course, with a number of supporting
	points, but no actual consistency. If you know what to do,
	you have sufficient buoyency to repeat what you ever did,
	and the book will sound "familiar", giving a (false) sense of
	optimisism, and completeness, but if you dont already know,
	it doesnt specify what to do (a load of self-satisfying w**k)
*/

mk_ERA( dset.ERA )
mk_CAT( dset.CAT )
mk_VAT( dset.VAT )

/*
	The necessary INFO fields are:
*/

dset.INFO.year_start.value = "2003-02-01"
dset.INFO.title.value = "My Annual Accounts"

function ACCT_name_amnt_desc( dset_ACCT, name, amnt, desc )
{
	// note how row is created by referring to it!
	// row = dset_ACCT.add_row( name )
	// row.desc = ...
	dset_ACCT[ name ].desc = desc
	dset_ACCT[ name ].amount = amnt
}
ACCT_name_amnt_desc( dset.ACCT, "bank",     57.50, "Cheque account" )
ACCT_name_amnt_desc( dset.ACCT, "ccard",     0.00, "Credit Card" )
ACCT_name_amnt_desc( dset.ACCT, "director",  0.00, "Director Cash Journal" )

/*
	Its MUCH easier to do all this from a file, but for demo
*/
function TRANS( tbl, date_tax, date_pay, cat, acct, img, ref, desc, amnt, vat_era, vat )
{
	var row = tbl.add_row("+") // gets an anonymous key
	row.date_tax = date_tax
	row.date_pay = date_pay
	row.cat      = cat
	row.acct     = acct
	row.img      = img
	row.ref      = ref
	row.desc     = desc
	row.amnt     = amnt
	row.vat_era  = vat_era
	row.vat      = vat
}

// This shows how tricky it is to match fields
TRANS( dset.TRANS, "2003-02-01", "2003-02-03", "N2", "director", "", "", "Year Start", 0.0, "2003_Q1", "x" )

T = dset.TRANS.add_row("+")
	T.date_tax = "2004-01-31"
	T.cat = "N2"
	T.acct = "t"
	T.vat_era = "2003_Q4"	// Jan is in last QTR of 2003, really !
	T.vat = "x"
	T.desc = "Year End"

T = dset.XFER.add_row("+")
	T.date = "2004-01-31"
	T.cat = "X2"
	// company pays NI for employee
	// shown in accounts as transfer from t to g !!!
	// thats because ITS A LOAN that other payments fixup!!
	T.acct1 = "t"
	T.acct2 = "g"
	T.amnt = 100.0
	T.desc = "NI"

dset.save_file("demo_schema1.csv")

/*
	sometimes the autoloader adjusts the data
	so load and save again, to diff the results

	dset2=ACCTS.load_dset("demo_schema1.csv")
	dset2.save_file("demo_schema2.csv")

*/





