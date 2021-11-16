// markedSectionEnd.c

void SP_App::markedSectionEnd(const MarkedSectionEndEvent & event)
{
	data_not();
	keep_pos( event.pos );
        app.call_MarkedSectionEnd();
}

