// ignoredChars.c

void SP_App::ignoredChars(const IgnoredCharsEvent &event)
{
	// marked section - ignore
	data_not();
	keep_pos( event.pos );
	AppString str = mk_string( event.data );
	refhold x1( str );
	app.call_IgnoredChars( str );
}

