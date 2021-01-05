#!/usr/bin/env js_accts

file = "demo_schema1.csv"

a1 = new Accts_Module()

dset1 = a1.load_dset( file );
dset1.save_file( file );

a1.exit(0);

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
