// externalDataEntityRef.c

void SP_App::externalDataEntityRef(const ExternalDataEntityRefEvent &event)
{
	data_not();
	keep_pos( event.pos );
	AppEntity obj = mk_entity(event.entity);
	if(!obj) { vret(0); return; }
	refhold x1(obj);
  	app.call_ExternalDataEntityRef( obj );
}
