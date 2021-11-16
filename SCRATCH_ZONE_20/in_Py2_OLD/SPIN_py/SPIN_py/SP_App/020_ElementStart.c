// ElementStart.c

// LURK: Attributes have "isdefaulted" info, and notation,
// some extra handling required -- as are test cases

AppAttr SP_App::mk_cdata_attr( const Attribute &a )
{
	AppString name = app.mk_string_attr( a.name );
	refhold x1( name );
	if(!name) {
		return NULL;
	}
	text_collector_utf text(50); // plus another 50
#define xTRACER
#ifdef TRACER
	text.data_string( "CDATA:" );
#endif
	if (a.defaulted == Attribute::definition) {
#ifdef TRACER
		text.data_string( "DEFAULTED:" );
#endif
	// tokens only valid for tokenised ;-)
	// CORE // text.data_string( a.tokens );
		for (size_t i = 0; i < a.nCdataChunks; i++) {
			text.data_string( a.cdataChunks[i].data );
			if( a.cdataChunks[i].isSdata )
			text.data_string( a.cdataChunks[i].entityName );
		}
	}

	if (a.defaulted != Attribute::definition) {
	for (size_t j = 0; j < a.nCdataChunks; j++)
	  if (a.cdataChunks[j].isSdata) {
	    if(0) {
		    text.data_byte( '&' );
		    text.data_string( a.cdataChunks[j].entityName );
		    text.data_byte( ';' );
	    } else {
		    // LURK - sdata .data might not be what you want
		    text.data_string( a.cdataChunks[j].data );
		    // LURK - sdata .data might not exist
	    }
	  } else if (a.cdataChunks[j].isNonSgml)
	    text.data_Char( a.cdataChunks[j].nonSgmlChar );
	  else
	    text.data_string( a.cdataChunks[j].data );
	}
	AppString data = text.mk_string();
	refhold x2( data );
	return app.mk_attr( name, data );
}

AppAttr SP_App::mk_tokenised_attr( const Attribute &a ) {
#if 1
	if (a.defaulted == Attribute::definition) {
		return NULL;
	}
#endif
	AppString name = app.mk_string_attr( a.name );
# if 1
	text_collector_utf text(50); // plus another 50
// tracer
	bool noise = false;
	if(noise) text.data_string( "(TOKENISED)" );
	text.data_string( a.tokens );
	if (a.defaulted == Attribute::definition) {
	  if(noise) text.data_string( "(defaulted)" );
	} else {
	  if(noise) text.data_string( "(notdefault)" );
	  if (a.isGroup && a.name == a.tokens)
		; // os() << ' ' << a.tokens;
	  else
		; // os() << ' ' << a.name << '=' << '"' << a.tokens << '"';
	}
	AppString data = text.mk_string();
#else
	AppString data = app.mk_string( a.tokens );
#endif
	refhold x1( name );
	refhold x2( data );
	// placeholder for unused logic
	if (a.defaulted != Attribute::definition) {
	  if (a.isGroup && a.name == a.tokens)
		; // os() << ' ' << a.tokens;
	  else
		; // os() << ' ' << a.name << '=' << '"' << a.tokens << '"';
	}
	return app.mk_attr( name, data );
}

void SP_App::startElement(const StartElementEvent &event)
{
	// data_not(); // done a few lines down // effects trailing space
	keep_pos( event.pos );

	SPIN_elem_info * info = find_elem_info( event ); // elem name
	if(!info) { vret(0); return; }
	refhold x1( (AppObject) info );

	AppAttrList atts = PyList_New(0);
	refhold x4( atts );
	for (size_t i = 0; i < event.nAttributes; i++) {
	    AppAttr attr = NULL;
	    const Attribute &a = event.attributes[i];
	    switch (a.type) {
	    case Attribute::cdata:
		attr = mk_cdata_attr( a );
	      break;
	    case Attribute::tokenized:
		attr = mk_tokenised_attr( a );
	      break;
	    default:
	      break;
	    }
	    if( attr ) PyList_Append( atts, attr );
	}

	if(info->element_breaks_word && data_trim && buff.wrap) {
		buff.data_char( ' ' ); // should trim next data zone!
		buff.trim_trailing_space();	// remove last sp T/F
	}
	data_not();

	/* there is an additional case:
		text
		SP(1)
		<htmlurl> -- inline - so NOT element_breaks_word
		SP(2)
		text
	Because the htmlurl was EMPTY, no data was sent to text_collector,
	which considers SP1 to still be active when SP2 is seen, the "text"
	inbetween didn't cancel SP1 because there wasn't any, hence SP2 gets
	eaten.

	REMEDY:
		WHEN applic AFTER data_not() cancel SP1

	most empty elements break words and GENERATE spaces
	which are then eaten in anticipation of them later generating
	*/

	if( info->IS_empty() && !info->element_breaks_word )
	{
		space_not();
		// buff.putc_prev = '!'; // anything but space
	}

// stack is going to get pushed - whatever
// so do before calling script

	stack.push( (SPIN_elem_info *) info, atts );
	app.call_ElementStart( info, atts );

//auto	Py_DECREF(atts);
//auto	Py_DECREF(ctype);
//auto	Py_DECREF( (PyObject *)info );
}

