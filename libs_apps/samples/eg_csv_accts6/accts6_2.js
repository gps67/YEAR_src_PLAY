#!/usr/bin/env js_accts

print("This is accts6_2.js");

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


// dset1 = a1.add_dset();
// t1 = dset1.add_table("TBL1");

/*
	the Instance of the Accts_Module has functions, use it to
	load a data file (Comma Sep Values) which contains several tables,
	The format is simple but not what any other utility writes :-(

	The loaded tables are held in a NEW data_set "dset1",
	"dset1.VAT" is the VAT table. 
*/
dset2 = a1.load_dset( "/home/gps/2005/src/PRIV_samples/0203.csv" );
print("loaded");
dset1 = a1.load_dset( "/home/gps/2005/src/PRIV_samples/0203.csv" );
print("loaded");
/*
	Its a mistake to call load_file twice,
	as the headers line looks like a data line
*/

/*
	a series of simple tests, presuming accts style data loaded
*/
print_eval( "dset2.VAT.v" )
print_eval( "dset2.TRANS" )
print_eval( "dset1.VAT" )
print_eval( "dset1.VAT.v" )
print_row( "dset1.VAT.v", dset1.VAT.v )
print_eval( "dset1.VAT.v.rate" )
print_eval( "dset1.VAT.v.rate = 20" )
print_eval( "dset1.VAT.v.rate" )
print_eval( "dset1.VAT.rowlist" )
print(" -this list is NOT a row . It will get expanded-\n")
print_row( "dset1.VAT.rowlist", dset1.VAT.rowlist )
// print_row( "dset1.VAT.rowlist[1]", dset1.VAT.rowlist[1] )

// this crashes with PROPERTY range error!
// for( key in dset1.VAT.rowlist ) {
// 	print( " key: " + key )
// 	print_row( "dset1.VAT."+key, dset1.VAT[key] )
// }

// show each row in rowlist (all VAT vable)

list1 = dset1.VAT.rowlist
for( key in list1 ) {
	print( " key: " + key )
	item1 = list1[key]
	print( " item: " + item1 )
	print_row( "list1[key]", item1 )
}

print_eval( "dset1.CAT.S4" )
print_eval( "dset1.CAT.S4.desc" )
print_eval( "dset1.CAT.S4.dotdot" )
print_eval( "dset1.CAT.S4.dotdot.desc" )
print_eval( "dset1.CAT.S.desc" )
print_eval( "dset1.TRANS" )
//print_eval( "dset1.TRANS.rowlist" )
print_eval( "dset1.TRANS.rowlist[5]" )
print_eval( "dset1.TRANS.rowlist[5].vat" )
print_eval( "dset1.TRANS.rowlist[5].vat.rate" )
t = dset1.TRANS.rowlist[5]
print_eval( "t.vat.rate" )
print_row( "t", t )
print_eval( "t.amnt" )
print_eval( "t.date_tax" )
// # nenexistant()

print_row( "dset1", dset1 )
print_row( "dset1.VAT", dset1.VAT )


for( attr in a1 ) {
	print( "a1." + attr );
}
// dset1 = a1.load_dset( "../samples/scheme1b.csz" );
// dset1 = a1.load_dset( "/tmp/rpt1.csv" );

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

