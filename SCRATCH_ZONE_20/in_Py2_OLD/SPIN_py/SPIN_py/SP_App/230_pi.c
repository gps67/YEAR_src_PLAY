// pi.c

void SP_App::pi(const PiEvent &event)
{
	data_not();
	keep_pos( event.pos );
	AppString data = mk_string( event.data );
	AppString entityName = mk_string( event.entityName );
	refhold x1( data );
	refhold x2( entityName );
	app.call_PI( data, entityName );
	return;
}

