gtk_text.cxx
	main()
		TOPAPP_Holder holder
		new gtk_text_top_app( holder, filename )
gtk_text_top_app.cxx
gtk_text_top_app.h
	gtk_text_top_app( holder, filename )
		menu, furniture
		obj_hold<test_edit> test_edit1;                                           
		obj_hold<w_widget0> text_view1; text_view1 = test_edit->mk_view()

w_text_view.cxx
w_text_view.h
		.view0 = new w_text_view( buff1 )
		scroll_to_endpos()
g_text_tag_table.h
		GtkTextTagTable *tag_table;                                               
g_text_buffer.cxx
g_text_buffer.h
	g_text_buffer
		.buffer = new gtk_text_view OF/FROM text buffer

/*
test_edit.cxx
test_edit.h
	test_edit
		.tags = new g_text_tag_table()
		.buff1 = new g_text_buffer( tags )
		.view0 = new w_text_view( buff1 )
		.view1 = w_win_scrolled( view0 )
*/
