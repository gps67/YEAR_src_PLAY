// Function for later inclusion

function mk_TABLES( ACCTS )
{
	dset = ACCTS.add_dset()

	/*
		because of guess_col_spec, some tables/cols would be
		auto created, but that code will eventually be reworked.

		Create the tables (with no columns)
	*/
	dset.add_table( "INFO" )
	dset.add_table( "ERA" )
	dset.add_table( "REF" )
	dset.add_table( "CAT" )
	dset.add_table( "ACCT" )
	dset.add_table( "NOTE" )
	dset.add_table( "VAT" )
	dset.add_table( "TRANS" )
	dset.add_table( "XFER" )

	/*
		create the columns, guess_col_spec gets activated here!

		I's sure I added the JS ability to specify col type
		(eg vat_era - links to ERA - though it should have its own).
		Unfortunately, col_spec type is lost (via file)
		and only recovered through guess_ ...
	*/

	dset.INFO.add_col("key")
	dset.INFO.add_col("value")

	dset.ERA.add_col("dotdot")
	dset.ERA.add_col("key")
	dset.ERA.add_col("desc")

	dset.REF.add_col("dotdot")
	dset.REF.add_col("key")
	dset.REF.add_col("desc")

	dset.CAT.add_col("dotdot")
	dset.CAT.add_col("key")
	dset.CAT.add_col("desc")

	dset.ACCT.add_col("key")
	dset.ACCT.add_col("amount")
	dset.ACCT.add_col("desc")
	dset.ACCT.add_col("amnt_closing")

	dset.NOTE.add_col("key")
	dset.NOTE.add_col("desc")

	dset.VAT.add_col("key")
	dset.VAT.add_col("rate")
	dset.VAT.add_col("desc")

	dset.TRANS.add_col("key")
	dset.TRANS.add_col("date_tax")
	dset.TRANS.add_col("date_pay")
	dset.TRANS.add_col("cat")
//	dset.TRANS.add_col("cat2") // remove this experiment ?
	dset.TRANS.add_col("acct")
	dset.TRANS.add_col("img")
	dset.TRANS.add_col("ref")
	dset.TRANS.add_col("desc")
	dset.TRANS.add_col("amnt")
	dset.TRANS.add_col("vat_era")
	dset.TRANS.add_col("vat")

	dset.XFER.add_col("key")
	dset.XFER.add_col("date")
	dset.XFER.add_col("cat")
	dset.XFER.add_col("acct1")
	dset.XFER.add_col("acct2")
	dset.XFER.add_col("img")
	dset.XFER.add_col("ref")
	dset.XFER.add_col("desc")
	dset.XFER.add_col("amnt")

	return dset;
}
