#!/usr/bin/env js_accts

print("This is accts6_1.js");

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
	print every field of r, which is typically a row , table, dset or other
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
 MODULE:
	To reduce use of the top level name-space, there is a single
	hook, the "Accts_Module" class name.

	If you run this script on a JavaScript that doesnt have the
	extra code, it should fail here.

	JavaScript has no concept of "module", but it makes sense to me.
	If you had lots of modules, you might have them dynamically loaded
	(like Python does), and this line would be the one to adjust.
*/
print_eval(	"a1 = new Accts_Module()	") // init module

/*
 DEMO:
	create a file step by step
*/
print_eval(	"dset1 = a1.add_dset()		") // create dset
print_eval(	"t1 = dset1.add_table('TBL1')	") // create table
print_eval(	"t1.add_col('key')		") // declare column
print_eval(	"t1.add_col('amnt_pre_vat')	") // declare column
print_eval(	"t1.add_col('vat')		") // declare column
print_eval(	"r1 = t1.add_row('abc')		") // add a row
print_eval(	"r1.amnt_pre_vat = 99.5		") // set field
print_eval(	"r1.vat = 'v'			") // set field
print_eval(	"t1.abc.amnt_pre_vat		") // set field
print_row(	"t1.abc", t1.abc )		   // print ...
print_eval(	"dset1.save_file('t1.csv')	") // save

/*
 DEMO:
	Load a file, then run some steps on it
	access fields and things

	the Instance of the Accts_Module has functions, use it to
	load a data file (Comma Sep Values) which contains several tables,
	The format is simple but not what any other utility writes :-(

	The loaded tables are held in a NEW data_set "dset",
	"dset.VAT" is the VAT table. 
*/
print("load_dset");
dset2 = a1.load_dset( "/home/gps/2005/src/PRIV_samples/0203.csv" );
dset  = a1.load_dset( "/home/gps/2005/src/PRIV_samples/0203.csv" );
/*
	Its OK to call load_file twice,
	The headers line is skipped properly,
	and date overwrites existing data
*/
print("loaded");

// dset2 
print_eval( "dset2.TRANS" )		// get table of dset
print_eval( "dset2.VAT.v" )		// get row of table of dset


/*
	a series of simple tests, presuming accts style data loaded
*/
print_eval( "dset.VAT" )		// get table of dset
print_eval( "dset.VAT.v" )		// get row of table of dset
print_row( "dset.VAT.v", dset.VAT.v )	// print that row, field by field
print_eval( "dset.VAT.v.rate" )		// get field of row
print_eval( "dset.VAT.v.rate = 20" )	// set field of row
print_eval( "dset.VAT.v.rate" )		// show change
print_eval( "dset.VAT.rowlist" )	// _builtin_ name of list (not func)
print_row( "dset.VAT.rowlist", dset.VAT.rowlist )

// this crashes with PROPERTY range error!
// for( key in dset.VAT.rowlist ) {
// 	print( " key: " + key )
// 	print_row( "dset.VAT."+key, dset.VAT[key] )
// }

list1 = dset.VAT.rowlist
for( key in list1 ) {
	print( " key: " + key )
	item1 = list1[key]
	print( " item: " + item1 )
	print_row( "list1[key]", item1 )
}

print_eval( "dset.CAT.S4" )
print_eval( "dset.CAT.S4.desc" )
print_eval( "dset.CAT.S4.dotdot" )
print_eval( "dset.CAT.S4.dotdot.desc" )
print_eval( "dset.CAT.S.desc" )
print_eval( "dset.TRANS" )
//print_eval( "dset.TRANS.rowlist" )
print_eval( "dset.TRANS.rowlist[5]" )
print_eval( "dset.TRANS.rowlist[5].vat" )
print_eval( "dset.TRANS.rowlist[5].vat.rate" )
t = dset.TRANS.rowlist[5]
print_eval( "t.vat.rate" )
print_row( "t", t )
print_eval( "t.amnt" )
print_eval( "t.date_tax" )
// # nenexistant()

print_row( "dset", dset )
print_row( "dset.VAT", dset.VAT )


for( attr in a1 ) {
	print( "a1." + attr );
}
// dset = a1.load_dset( "../PRIV_samples/scheme1b.csz" );
// dset = a1.load_dset( "/tmp/rpt1.csv" );

print("------------ TABLES in DSET -------------")

for( tblkey in dset ) {
	print( "dset." + tblkey + " = " + dset[tblkey] )
}


for( tblname in dset ) {
	print("------------ ATTRS of TABLE "+tblname+" -------------")
	tbl = dset[tblname]
	for( objattr in tbl ) {
		print( "dset." + tblname + "[" + objattr + "]" + " = " + tbl[objattr] )
	}
}
print("------------ Loose Tests -------------")

rows = dset.NWATRANS.rowlist
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

