#!/usr/bin/env js_accts

file = "demo_schema1.csv"
pdf1 = "/tmp/demo_schema_1.pdf";
pdf2 = "/tmp/demo_schema_2.pdf";

a1 = new Accts_Module()

dset1 = a1.load_dset( file );
dset1.run_reports(); // internal ones
dset1.rep_pdf('Cash_Cat',pdf1);
a1.view_pdf( pdf1 );

a1.exit(0);
