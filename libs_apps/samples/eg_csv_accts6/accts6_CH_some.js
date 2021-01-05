#!/usr/bin/env js_accts

print("This is accts6_CH_some.js");

a1 = new Accts_Module()

dset_CH = a1.load_dset( "/home/gps/2005/src_PRIV/PRIV_samples/tmp_CH1_archive_draft.csv" );
print("#" )

list = [ "0001", "0001", "0102", "0203", "0304", "0405", "0506" ]

dset_curr = ""

for( pos in list )
{
	Y0Y1 = list[ pos ]
	dset_prev = dset_curr;
	print(" Y0Y1: " + Y0Y1 )
	csv_curr = "/home/gps/2005/src/PRIV_samples/" + Y0Y1 + ".csv"
	print(" csv_curr: " + csv_curr )
	dset_curr = a1.load_dset( csv_curr );
	print("#")

	if( dset_prev == "" ) {
		print("Skipping on Y0Y1 == " + Y0Y1 );
		continue;
	}

	pdf = "/tmp/CH_" + Y0Y1 + ".pdf";
	print(" Y0Y1: " + Y0Y1 )
	dset_CH.rep_pdf('CH', pdf, dset_curr, dset_prev );
}

a1.exit()

