// sdata.c

void SP_App::sdata(const SdataEvent &event)
{
	data_not();
	space_not(); // presuming sdata isn't a space !
	keep_pos( event.pos );
	AppString text = app.mk_string(event.text);
	AppString name = app.mk_string(event.entityName);
	refhold x1( text );
	refhold x2( name );
	app.call_sdata( name, text );
}


