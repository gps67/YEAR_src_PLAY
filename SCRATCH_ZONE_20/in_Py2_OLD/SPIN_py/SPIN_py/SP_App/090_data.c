// data.c

/*
	data_not is called frequently to end a POSSIBLE data area
	if any data is in the buffer, the inline macro calls data_not_fn
	Stripping spaces might lead to empty, which is never sent.
	NB buff is "self-clearing"
*/

void SP_App::data_not_fn()
{
	if( 1 && data_trim ) {

		// leading spaces are rare due to putc_prev eating initial sp
		if( buff.trim_leading_space() ) {
			app.call_DataSpace();
		}

		// trailing is often, but not majority, and truncate is fast
		int trail = buff.trim_trailing_space();

		// send the data (if theres any left)
		app.call_Data( buff );

		// send that trailing space!
		if( trail ) {
			app.call_DataSpace();
		}
	} else {
		app.call_Data( buff );
	}
}

/*
	SP tells us there is some text data,
	add it to the buff
*/
void SP_App::data(const DataEvent &event)
{
	buff.data_string( event.data );
}

