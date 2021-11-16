// startDtd.c


void SP_App::
startDtd (const StartDtdEvent & event)
{
  // data_not(); // surely no data before dtd!
  keep_pos (event.pos);
  send_location ();		// the DTD decl starts within the document ...

  if (!event.haveExternalId)
    {
      AppString m = app.mk_str ("missing");
      refhold x1 (m);
      app.call_DtdStart (m, m, m, m, m);
      return;
    }

  text_collector_utf decl_line;

  AppExternalId eid = mk_externalId (event.externalId);
  AppString name = app.mk_string (event.name);	//LINUXDOC

  AppString path = 0;		// fixup later
  AppString syspub = 0;

  decl_line.data_str ("<!DOCTYPE ");
  decl_line.data_string (event.name);
  decl_line.data_char (' ');
  if (event.externalId.havePublicId)
    {
      syspub = app.mk_str ("PUBLIC");
      path = app.mk_string (event.externalId.publicId);
      decl_line.data_str ("PUBLIC \"");
      decl_line.data_string (event.externalId.publicId);
      decl_line.data_char ('"');
    }
  else if (event.externalId.haveSystemId)
    {
      syspub = app.mk_str ("SYSTEM");
      decl_line.data_str ("SYSTEM");
	{ 
	  path = app.mk_string (event.externalId.systemId);
	  // calc quote for path
	  const CharString & str = event.externalId.systemId;
	  bool q1 = 0;
	  bool q2 = 0;
	  for (size_t i = 0; i < str.len; i++)
	  {
	    if (str.ptr[i] == '\'') q1 = 1;
	    if (str.ptr[i] == '"') q2 = 1;
	  }
	  if (q1 && q2) { raise_error("path uses both quotes"); }
	  char qc = '\'';
	  if( q1 ) qc = '"';
	  decl_line.data_char (' ');
	  decl_line.data_char (qc);
	  decl_line.data_string (event.externalId.systemId);
	  decl_line.data_char (qc);
	}			// have sysid

    }				// system
     else if (event.externalId.haveGeneratedSystemId)
      {
	path = app.mk_str ("--Generated--");
	syspub = app.mk_str ("SYSTEM");
	decl_line.data_str ("SYSTEM");
	decl_line.data_char (' ');
	decl_line.data_string (event.externalId.generatedSystemId);
      } else {
		raise_error( "No sys id to speak of" );
		syspub = app.mk_str ("--MISSING--");
		path = app.mk_str ("--MISSING--");
      }

  decl_line.data_char ('>');

  AppString decl = decl_line.mk_string ();

 if(!name) raise_error("DTD no name");
 if(!syspub) raise_error("DTD no syspub");
 if(!path) raise_error("DTD no path");
 if(!decl) raise_error("DTD no decl");
 if(!eid) raise_error("DTD no eid");

  app.call_DtdStart(
	name,
	syspub,
	path,
	decl,
	eid
  );

// ie Py_DECREF each of these
  refhold x1 (name);
  refhold x2 (syspub);
  refhold x3 (path);
  refhold x4 (decl);
  refhold x5 (eid);
}

void SP_App::
endDtd (const EndDtdEvent & event)
{
  keep_pos (event.pos);
  send_location ();
  AppString obj = mk_string (event.name);
  refhold x1 (obj);
  app.call_DtdEnd (obj);
}
