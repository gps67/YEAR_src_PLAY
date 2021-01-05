# report1 - category view - "Categorised" table
report2 - cashbook view - "CashBook" table


CashBook
Categorised
VatSum
VatRep		- todo
CatSum		- Cats with writeback subtotals

rep1_pdf	Vat Track = NO_intro, VatSum
rep12_pdf	Multi = intro, CashBook, Categorised
rep_table_pdf	flexible = <TABLE>

rep_pdf_0	lots of functionality, generic rows, titles, Layout_Hdrs::,

The Reports table has this:

	tbl->col_spec_tree = spec_cat

	This is used in ../table/sum_tree.cxx sum_tree_build::s1_tree
	then in sum_tree_build::spec_is_tree_spec( spec )
	then what ??? !!

