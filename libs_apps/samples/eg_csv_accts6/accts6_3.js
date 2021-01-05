#!/usr/bin/env js_accts

print("This is accts6_3.js");

/*
	print_eval( code_str ) runs the code_str, prints it and the result
*/
function print_eval( str ) {
	print( "EVAL: " + str )
	ret = eval( str )
	print( "RESP: " + str + " --> " + ret )
	return ret
}

/*
	print every field of r, which is typically a row , table, dset1 or other

	can also be used to print an array, but dont expect [i] to .i work
*/
function print_row( name, r ) {
	print( "--------------------" );
	print( "print_row( " + name + " ) => {" );
	for( field in r ) {
		print( "  " + name + "." + field + ": "  + r[field] );
	}
	print( "}" );
}

function print_tbl( name, t ) {
	print_row( name, t )
}

/*
	To reduce use of the top level name-space, there is a single
	hook, the "Accts_Module" class name.

	If you run this script on a JavaScript that doesnt have the
	extra code, it should fail here.

	JavaScript has no concept of "module", but it makes sense to me.
	If you had lots of modules, you might have them dynamically loaded
	(like Python does), and this line would be the one to adjust.
*/
a1 = new Accts_Module()

a1.make( "/home/gps/2005/src/libs_apps/_run", "default" )
a1.make( "/home/gps/2005/src/mm3_scan", "all" )

/*
	the Instance of the Accts_Module has functions, use it to
	load a data file (Comma Sep Values) which contains several tables,
	The format is simple but not what any other utility writes :-(

	The loaded tables are held in a NEW data_set "dset1",
	"dset1.VAT" is the VAT table. 
*/
dset2 = a1.load_dset( "/home/gps/2005/src/PRIV_samples/0304.csv" );
dset1 = dset2;
print("loaded");

// two PDF reports in the same run causes some obscure fault

pdf = "/tmp/t.pdf";

print_eval( "dset1.run_reports()" );
// print_eval( "dset1.rep_pdf('Cash_Cat','/tmp/t1.pdf')" );
// print_eval( "dset1.ACCT.pdf_doc( '/tmp/t_ACCT.pdf' )" );
print_eval( "dset1.Categorised.pdf_doc( pdf )" );
print_eval( "a1.view_pdf( '"+pdf+"' )" );
// print_eval( "dset1.CashBook.pdf_doc( '/tmp/t2b.pdf' )" ); // OVERFLOW
// print_eval( "dset1.VatSum.pdf_doc( '/tmp/t2c.pdf' )" );
// print_eval( "dset1.CAT.pdf_doc( '/tmp/t2d.pdf' )" );
a1.exit(0);

print("------------ TABLES in DSET -------------")

for( tblkey in dset1 ) {
	print( "dset1." + tblkey + " = " + dset1[tblkey] )
}


for( tblname in dset1 ) {
	print("------------ ROWS in TABLE "+tblname+" -------------")
	tbl = dset1[tblname]
	for( rowkey in tbl ) {
		print( "dset1." + tblname + "[" + rowkey + "]" + " = " + tbl[rowkey] )
	}
}
print("------------ Loose Tests -------------")

rows = dset1.NWATRANS.rowlist
row = rows[5]
for (x in row ) {
	print( "row." +  x + " = " + row[ x ] )
}

print("------------ Error Tests -------------")

r1.Five=99
r1.Three=88
r1.Nine=99
print_row( "r1", r1 )
print( "DONE");
nenexistant()

