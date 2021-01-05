
function mk_VAT( dset_VAT )
{
	/*
		UK VAT rate, and codes
	*/

	dset_VAT.e.rate = 0
	dset_VAT.w.rate = 0
	dset_VAT.x.rate = 0
	dset_VAT.z.rate = 0
	dset_VAT.v.rate = 17.5

	dset_VAT.e.desc = "euro"
	dset_VAT.w.desc = "world"
	dset_VAT.x.desc = "excluded"
	dset_VAT.z.desc = "zero"
	dset_VAT.v.desc = "uk Vat"
	dset_VAT["0"].desc = "Zero Rated" // zero is not ident ?
}
