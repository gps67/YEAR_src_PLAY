// subdocEntityRef.c

// this doesn't get called by the generic interface
// but the API processes the subdoc

void SP_App::subdocEntityRef(const SubdocEntityRefEvent &event)
{
	data_not();
	keep_pos( event.pos );
	AppEntity entity = mk_entity( event.entity );
	refhold x1( entity );
	app.call_SubdocEntityRef( entity );
}

