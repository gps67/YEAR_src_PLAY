SPIN_elem_info * SP_App::find_elem_info(const StartElementEvent &event)
{
	// move this down hoping it disappears?
	AppString name = app.mk_string_elem(event.gi);
	if(!name) return NULL;
	refhold x1( name );

	SPIN_elem_info * info = app.look_elem_info( name );

	// IF it exists and its been used before - return it asis
	if(info && ! info->get_on_flag( IS_first_use ) )
		return info;

	// Even if it isnt used before, it was created - PROPERLY!
	if( info )
	{
		info -> clr_on_flag( IS_first_use );
		return info;
	}

	/*
		info ==> never used before, but pre-declared in dictionary
		NULL ==> not yet in dictionary
	*/

	// these three values come from SP
	const char * ctyp = "contentType-error";

	/*
		There is not an exact content-type to element-type match.

		phrase is like inline but element breaks word.

		Frankly, I claim to support, but never use part-word
		emphasis, so e-b-w is better everywhere.
	*/

	Enum_Element_Type et = EL_inline;
	switch (event.contentType) {
	 case StartElementEvent::cdata:   ctyp = "cdata";   et = EL_inline; break;
	 case StartElementEvent::rcdata:  ctyp = "rcdata";  et = EL_pre;    break; // VERB
	 case StartElementEvent::empty:   ctyp = "empty";   et = EL_empty;  break; // BR HR
	 case StartElementEvent::mixed:   ctyp = "mixed";   et = EL_phrase; break; // P TT ITEM
	 case StartElementEvent::element: ctyp = "element"; et = EL_block;  break; // ARTICLE ITEMIZE
	 default: break;
	}

	bool ebw = true;

	info = app.make_elem_info(
		name,
		et,
		ctyp,
		ebw
	);
	if(!info) {
		app.report_error(); 
		app.raise_error("No elem_info"); // for what name?
		return NULL;
	}
	return info;
}
