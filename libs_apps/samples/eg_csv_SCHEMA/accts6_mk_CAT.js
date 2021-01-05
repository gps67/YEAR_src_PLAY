
// TODO BUG:
// when desc contains a comma, it isnt handled
// the other code probably does something in CSV load
// the JS allows comma through which fails on the next data load
// FIXME: replaced comma with semicolon

function add_cat( tbl, dotdot, key, desc )
{
//	ACCTS.gdb_invoke()
	tbl[key].dotdot = dotdot
	tbl[key].desc = desc
}

function mk_CAT( dset_CAT )
{
	/*
		The primary level categories are immutable
	*/
	add_cat( dset_CAT, "", "D", "Drawings" )
	add_cat( dset_CAT, "", "T", "Tax" )
	add_cat( dset_CAT, "", "O", "Outgoings" )
	add_cat( dset_CAT, "", "X", "X-FERS" )
	add_cat( dset_CAT, "", "I", "Income" )
	add_cat( dset_CAT, "", "N", "Notes" )

	/*
		sub-categories are much more flexible
		(except for a few coded into the reports, N2 N3 )
	*/
	add_cat( dset_CAT, "D", "D1", "Salary" )
	add_cat( dset_CAT, "D", "D3", "Pension" )
	add_cat( dset_CAT, "D", "D5", "Dividend" )

	add_cat( dset_CAT, "T", "T1", "VAT" )
	add_cat( dset_CAT, "T", "T2", "PAYE+NI" )
	add_cat( dset_CAT, "T", "T3", "TAX" )

	add_cat( dset_CAT, "T2", "T2_PAYE", "PAYE Income Tax" )
	add_cat( dset_CAT, "T2", "T2_NI_ER", "NI Employers part" )
	add_cat( dset_CAT, "T2", "T2_NI_EE", "NI Employees part" )

	add_cat( dset_CAT, "O", "P", "Purchases" )
	add_cat( dset_CAT, "O", "H", "Hardware Asset" )
	add_cat( dset_CAT, "O", "Z", "-DUNNO-" )

	add_cat( dset_CAT, "X", "X1", "PAID Invoice" )
	add_cat( dset_CAT, "X", "X2", "PAID Director Journal" )
	add_cat( dset_CAT, "X", "X3", "PAID VISA" )
	add_cat( dset_CAT, "X", "X4", "PAID Cheque etc" )
	add_cat( dset_CAT, "X", "X5", "OWED NI PAYE; etc" )

	add_cat( dset_CAT, "I", "I1", "Contract" )
	add_cat( dset_CAT, "I", "I2", "Travel Fee" )
	add_cat( dset_CAT, "I", "I3", "Asset Sales" )
	add_cat( dset_CAT, "I", "I4", "Interest etc" ) // breaks to toplevel

	add_cat( dset_CAT, "N", "N1", "231231 = Cheque" )
	add_cat( dset_CAT, "N", "N2", "STMT 100 = VAL" )
	add_cat( dset_CAT, "N", "N3", "note:" )
	add_cat( dset_CAT, "N", "N4", "PREV YEAR etc" )
}

