// obj_cctr.c

SP_App::SP_App( SPIN_top * _top) : 
	app(_top),
	buff(500),
	stack( this )
{
	data_trim = 0;
			  
    printf("<!--new SP_App created -->\n");
}

