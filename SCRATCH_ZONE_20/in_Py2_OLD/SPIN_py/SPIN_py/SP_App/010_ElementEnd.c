// ElementEnd.c

void SP_App::endElement(const EndElementEvent &event)
{
	// data_not(); // done a few lines down, before / after somthing
	keep_pos( event.pos );
	// need to know of this end tag breaks word - acts as space
	SPIN_elem_info * elm = stack.match_top_name(event.gi);
	if(!elm) {
		raise_error("match_top_name failed!\n");
		vret(0);
		raise_error("EndElement - name not found - how was it opened?\n");
		return;
	}
	refhold x1( (AppObject) elm );

	int do_space_migration = 1 ; // should be a variable
	if(data_trim && buff.wrap) { // any manipulation allowed at all
		if(elm->element_breaks_word) {
			buff.data_char( ' ' ); // should trim next data zone!
			buff.trim_trailing_space();	// remove last sp T/F
			data_not();
		} else { // inline - allow space migration over end tag
			if(do_space_migration)
			{
				int space_found = buff.trim_trailing_space();
				data_not();
				if( space_found ) // put it back
				{
					buff.putc_prev = '!'; // matters
					buff.data_char( ' ' ); // replace
				}
			} else {
				data_not();
			}
		}
	} else {
		data_not();
	}

	AppElem tree = stack.pop(); // LURK before or after call ??
	if(!tree)
	{
		raise_error("ElementEnd/pop - NULL tree");
		return;
	}
	refhold x2( tree );
	app.call_ElementEnd( tree );

	// ElementEnd at end of document HAS TO call tree_add
	// ACTUALLY IT MIGHT NOT WANT TO - simple normalisations do, but ...
	// a DOC is a single top element --> single tree element
	// which adds entire doc to stack[0].zone (sole item)

	// NB no more events from SP means this is our last chance!

	if( stack.sp == 0 )
	{
		if(0) printf("ElementEnd.c Detecting Stack Now Empty: </%s>\n",
			PyString_AsString( elm->name ) );
		AppElem z = stack.item[0].zone; // should be list of one item
		switch ( PyList_Size( z )) {
		  case 0:
		  	raise_error("EndDocument - zero Size");
		  break;
		  case 1:
			// convert list of one to single element/tree (top elem)
		  	z = PyList_GetItem( z, 0 ); // NO INCREF?
			app.call_DocumentEnd( z );
		  break;
		 default:
		  	raise_error("EndDocument - Size should be 1");
			app.call_DocumentEnd( z );
		}
	}
}

