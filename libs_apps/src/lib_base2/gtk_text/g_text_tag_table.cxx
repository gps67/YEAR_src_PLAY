#include "g_text_tag_table.h"

#include "w_widget0.h"

g_text_tag_table::g_text_tag_table() {
	tag_table = gtk_text_tag_table_new();
}

GtkTextTag * g_text_tag_table::lookup( str0 tagname )
{
	tag1 = gtk_text_tag_table_lookup( TAG_TABLE(), (STR0) tagname );
	return tag1; // poss NULL
}

//! useful to set properties on tag1
void g_text_tag_table::set_tag1( GtkTextTag * _tag1 )
{
	tag1 = _tag1;
}

void g_text_tag_table::remove( GtkTextTag * _tag1 ) 
{
	if( !_tag1 ) _tag1 = tag1; // default
	if( tag1 == _tag1 ) tag1 = NULL; // beyond use
	gtk_text_tag_table_remove( TAG_TABLE(), _tag1 );
}

GtkTextTag * g_text_tag_table::create_anon()
{
	tag1 = gtk_text_tag_new( NULL );
	gtk_text_tag_table_add( TAG_TABLE(), tag1 );
	return tag1;
}

GtkTextTag * g_text_tag_table::create( str0 tagname )
{
/*
	tag1 = gtk_text_tag_table_lookup( TAG_TABLE(), (STR0) tagname );
	if( tag1 ) {
		e_print("Tag '%s' already exists\n", (STR0) tagname );
		return tag1;
	}
*/
	tag1 = gtk_text_tag_new( (STR0) tagname );
	if(tag1) {
		gtk_text_tag_table_add( TAG_TABLE(), tag1 );
		if(0) e_print("tag_table::create(%s) OK()\n", (STR0) tagname );
	} else {
		e_print("tag_table::create(%s) tag_new() failed\n", (STR0) tagname );
	}
	return tag1; // poss NULL
}

void g_text_tag_table::set_tag_prop( str0 prop, str0 val )
{
	if(0) e_print("(%p)set_tag_prop(%s,%s)\n", tag1, (STR0) prop, (STR0) val );
	g_object_set( tag1, (STR0) prop, (STR0) val, NULL );
}

void g_text_tag_table::set_tag_prop( str0 prop, int val )
{
	g_object_set( tag1, (STR0) prop,        val, NULL );
}

void g_text_tag_table::set_tag_prop_obj( str0 prop, GtkObject * val )
{
	g_object_set( tag1, (STR0) prop,        val, NULL );
}

/*
   "name"                 gchararray           : Read / Write / Construct Only
   "background"           gchararray           : Write
   "foreground"           gchararray           : Write
   "background-gdk"       GdkColor             : Read / Write
   "foreground-gdk"       GdkColor             : Read / Write
   "background-stipple"   GdkPixmap            : Read / Write
   "foreground-stipple"   GdkPixmap            : Read / Write
   "font"                 gchararray           : Read / Write
   "font-desc"            PangoFontDescription : Read / Write
   "family"               gchararray           : Read / Write
   "style"                PangoStyle           : Read / Write
   "variant"              PangoVariant         : Read / Write
   "weight"               gint                 : Read / Write
   "stretch"              PangoStretch         : Read / Write
   "size"                 gint                 : Read / Write
   "size-points"          gdouble              : Read / Write
   "scale"                gdouble              : Read / Write
   "pixels-above-lines"   gint                 : Read / Write
   "pixels-below-lines"   gint                 : Read / Write
   "pixels-inside-wrap"   gint                 : Read / Write
   "editable"             gboolean             : Read / Write
   "wrap-mode"            GtkWrapMode          : Read / Write
   "justification"        GtkJustification     : Read / Write
   "direction"            GtkTextDirection     : Read / Write
   "left-margin"          gint                 : Read / Write
   "indent"               gint                 : Read / Write
   "strikethrough"        gboolean             : Read / Write
   "right-margin"         gint                 : Read / Write
   "underline"            PangoUnderline       : Read / Write
   "rise"                 gint                 : Read / Write
   "background-full-height" gboolean             : Read / Write
   "language"             gchararray           : Read / Write
   "tabs"                 PangoTabArray        : Read / Write
   "invisible"            gboolean             : Read / Write
   "background-set"       gboolean             : Read / Write
   "foreground-set"       gboolean             : Read / Write
   "background-stipple-set" gboolean             : Read / Write
   "foreground-stipple-set" gboolean             : Read / Write
   "family-set"           gboolean             : Read / Write
   "style-set"            gboolean             : Read / Write
   "variant-set"          gboolean             : Read / Write
   "weight-set"           gboolean             : Read / Write
   "stretch-set"          gboolean             : Read / Write
   "size-set"             gboolean             : Read / Write
   "scale-set"            gboolean             : Read / Write
   "pixels-above-lines-set" gboolean             : Read / Write
   "pixels-below-lines-set" gboolean             : Read / Write
   "pixels-inside-wrap-set" gboolean             : Read / Write
   "editable-set"         gboolean             : Read / Write
   "wrap-mode-set"        gboolean             : Read / Write
   "justification-set"    gboolean             : Read / Write
   "left-margin-set"      gboolean             : Read / Write
   "indent-set"           gboolean             : Read / Write
   "strikethrough-set"    gboolean             : Read / Write
   "right-margin-set"     gboolean             : Read / Write
   "underline-set"        gboolean             : Read / Write
   "rise-set"             gboolean             : Read / Write
   "background-full-height-set" gboolean             : Read / Write
   "language-set"         gboolean             : Read / Write
   "tabs-set"             gboolean             : Read / Write
   "invisible-set"        gboolean             : Read / Write
*/


