// endProlog.c

void SP_App::endProlog(const EndPrologEvent &event)
{
	data_not();
	keep_pos( event.pos );
	app.call_PrologEnd();
}

