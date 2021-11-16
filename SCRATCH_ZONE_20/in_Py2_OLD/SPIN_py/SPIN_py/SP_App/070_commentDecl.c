// commentDecl.c

void SP_App::commentDecl(const CommentDeclEvent &event)
{
	data_not();
	keep_pos2( event.pos ); // if(OEP_change) send_location();

//  if (!inInstance_) return;

	// convert pseudo type AppStrngList to a real utility type
	// AppStringList_builder (holds ref to self ...)

	int N = event.nComments; 
	PyObject * ary = PyList_New( N );
	if(!ary) {
		vret(0); // ie print err and call error1
		return;
	}
	refhold x1( ary );
	for( int i = 0; i < N; i++ ) {
		// comments don't go through buff - so not wrapped
		AppString str = app.mk_string_drop_lf( event.comments[i] );
		if(!str) { vret(0); return; } // malloc error
		PyList_SetItem( ary, i, str ); // LURK eats REF of str
	}
	app.call_Comments( ary );

// refhold //	Py_DECREF(ary);
}

