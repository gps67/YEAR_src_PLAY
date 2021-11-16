// nonSgmlChar.c

void SP_App::nonSgmlChar(const NonSgmlCharEvent &event)
{
	// data_not(); // merge char into buffer
	keep_pos( event.pos ); // or leave pos as start of data?
	buff.data_Char( event.c ); // LURK non 8859
}
