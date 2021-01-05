#ifndef w_label_H
#define w_label_H

/*!
*/
class w_label : public w_widget0
{
 public:
	GtkLabel * Label() { return (GtkLabel *) w; }
	w_label( str0 str)
	{
		w_set( gtk_label_new( str ));
	}
	void set_text( str0 str)
	{
		gtk_label_set_text( Label(), str );
	}
	void set_text_with_markup( str0 str)
	{
		gtk_label_set_markup( Label(), str );
	}
	void set_justify( GtkJustification just )
	{
		gtk_label_set_justify( Label(), just );
	}
	void justify_center()
	{
		set_justify( GTK_JUSTIFY_CENTER );
	}
	void justify_right()
	{
		set_justify( GTK_JUSTIFY_RIGHT );
	}
};

#endif
