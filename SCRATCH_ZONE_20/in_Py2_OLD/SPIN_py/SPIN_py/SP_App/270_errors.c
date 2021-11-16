void    SP_App::send_location( )
{
	L = Location( OEP, pos_recent );
	AppLocation locn = mk_location();
	refhold x1( locn );
	app.call_set_location( locn );
}

void    SP_App::keep_pos( Position pos )
{
	pos_recent = pos;
	// comment calls keep_pos2
	// so you don't have to
	// keep_pos2( pos );
}

void    SP_App::keep_pos2( Position pos )
{
	pos_recent = pos;
/*
	this is a mute point - the noise is excessive with psplit
	but otherwise, there is no way of getting the DTD's actual
	filename (calculate from catalogue dir). With this option
	an OEP_change is generated, and the next event that gets
	pos can generate a Location (pos 0 is valid but wrong)

	To get the DTD filename, find a comment in that file
*/
	if(OEP_change) {
		OEP_change = 0;
		send_location();
	}
}

AppLocation SP_App::mk_location( void )
{
	AppString entityName   = mk_string( L.entityName );
	AppString filename     = mk_string( L.filename );
	refhold x1( entityName );
	refhold x2( filename );
	return( app.mk_location (
		entityName,
		filename,
		L.lineNumber,
		L.columnNumber,
		L.byteOffset,
		L.entityOffset,
		(long) L.other  // void *
	));
}

void SP_App::error(const ErrorEvent & event)
{
	data_not();
	keep_pos( event.pos );

	send_location();
	AppLocation locn = mk_location();
	refhold x1( locn );

	const char * s = "unset";
	switch (event.type ) {
	 /*  */ case ErrorEvent::info:       s = "info";
	 break; case ErrorEvent::warning:    s = "warning";
	 break; case ErrorEvent::quantity:   s = "quantity";
	 break; case ErrorEvent::idref:      s = "idref";
	 break; case ErrorEvent::capacity:   s = "capacity";
	 break; case ErrorEvent::otherError: s = "otherError";
	}
	AppString etype = mk_str( s );
	refhold x2( etype );

	// for some reason message just doesn't happen
	// but it's probably a parameter somewhere ...
	AppString message = mk_string( event.message );
	refhold x3( message );

	app.call_ErrorEvent( locn, etype, message );
}

void SP_App::openEntityChange(const OpenEntityPtr & ptr )
{
	// seems that OEP==ptr always!
	// even when switching to DTD?
	// if(OEP == ptr ) return;
	OEP = ptr;
	OEP_change = 1; // wait for pos to make a Location
if(0)	printf("(openEntityChange)");
}
