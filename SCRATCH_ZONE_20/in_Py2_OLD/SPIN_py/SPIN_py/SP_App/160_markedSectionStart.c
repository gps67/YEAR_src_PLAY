// markedSectionStart.c

void SP_App::markedSectionStart(const MarkedSectionStartEvent &event)
{
	data_not();
	keep_pos( event.pos );

	PyObject * list = PyList_New(0); // easier // (event.nParams);
	if(!list) return;
	refhold x1( list );
	for (size_t i = 0; i < event.nParams; i++)
	{
		  AppString para = 0;
		  switch (event.params[i].type) {
			case MarkedSectionStartEvent::Param::temp:
			para = mk_str( "TEMP" );
			break;
			case MarkedSectionStartEvent::Param::include:
			para = mk_str( "INCLUDE" );
			break;
			case MarkedSectionStartEvent::Param::rcdata:
			para = mk_str( "RCDATA" );
			break;
			case MarkedSectionStartEvent::Param::cdata:
			para = mk_str( "CDATA" );
			break;
			case MarkedSectionStartEvent::Param::ignore:
			para = mk_str( "IGNORE" );
			break;
			case MarkedSectionStartEvent::Param::entityRef:
			para = mk_string( event.params[i].entityName );
			// an entity name that is supposed to reduce to above
			// you know it's an entity ref by because it's
			// different from "RCDATA" etc
			break;
		}
			PyList_Append( list, para );
			Py_DECREF( para );
	}

	const char * status = "unset";
	switch (event.status) {
		case MarkedSectionStartEvent::ignore:
			status = "ignore";
		break;
		case MarkedSectionStartEvent::include:
			status = "include";
		break;
		case MarkedSectionStartEvent::cdata:
			status = "cdata";
		break;
		case MarkedSectionStartEvent::rcdata:
			status = "rcdata";
		break;
		default:
			raise_error("marked_section_status");
		break;
	}
	AppString stat = app.mk_str( status );
	refhold x2( stat );

	app.call_MarkedSectionStart( stat, list );
}

