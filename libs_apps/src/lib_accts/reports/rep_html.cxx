// #include "dgb_fail.h"
// #include "dset.h"

#include "tbl_col_spec.h"

#include "rep_html.h"
#include "run_report.h"
#include "buffer2.h"
#include "dir_name_ext.h"


using namespace ACCTS;

///////////

bool run_html_Cash_Cat( ACCTS::data_set * dset, str0 filename )
{

	if(! run_report_1( dset ) ) return false;
	if(! run_report_2( dset ) ) return false;
	if(! run_report_3( dset ) ) return false;
	if(! run_report_4( dset ) ) return false;

	e_print("\nrun_html_Cash_Cat( dset, %s )\n", (STR0) filename );

#if 0
	obj_hold<rep_html> rep_ = new rep_html(dset);
	rep_html & rep = *rep_;
#else
	rep_html rep(dset);
#endif

	if(! rep.opens_titles( filename ) ) return false;

	rep.draw_table( "VAT", "key"  );
	rep.draw_table( "ACCT", "key"  );
	rep.draw_table( "Categorised", "cat"  );
	rep.draw_table( "CashBook", "date_pay"  );
	rep.closes();

	return true;
}

///////////

const char * ML( str0 s0 )
{
	const char * s = s0;
	if( !s ) return "";
	static buffer2 buff;
	buff.clear();
	if(0) buff.put("'");
	while(uchar c1 = *s++)
	{
		switch( c1 )
		{
		 case '<': buff.put( "&lt;" ); break;
		 case '>': buff.put( "&gt;" ); break;
		 case '&': buff.put( "&amp;" ); break;
		 default: buff.put( c1 );
		}
	}
	if(0) buff.put(" ");
	if(0) buff.put("'");
	return (STR0)str0(buff);
}

const char * ML( str2 * s2 )
{
	if(!s2) return "";
	return ML( s2->str );
}

const char * ML( buffer2 & buff )
{
	return ML( str0( buff ) );
}

rep_html::~rep_html()
{
}

rep_html::rep_html( ACCTS::data_set * _dset)
: dset( _dset )
, fd_(new fd_hold_1())
, fd( *fd_ )
, info( _dset )
, p_tree( new sum_tree_build() )
, tree( *p_tree )
{
	img_spec = NULL;
}

bool rep_html::opens( str0 filename, str0 title )
{
	str0 stylesheet = "./style1.css";
	str0 generator = "accts_reports";

	if(! fd.open_RW_CREATE( filename )) return false;
	fd.print("<HTML>\n" );
	fd.print(" <HEAD>\n" );
	fd.print("  <TITLE>%s</TITLE>\n", ML(title) );
	fd.print("  <LINK rel='stylesheet' href='%s' type='text/css'>\n",
		(STR0) stylesheet );
	fd.print("  <META NAME='GENERATOR' CONTENT='%s'>\n",
		(STR0) generator );
	fd.print("<STYLE type=\"text/css\">\n");
	fd.print(
		" TH              { background: yellow } \n"
		" TH.th2          { background: #E0D0E0 } \n"
		" TR              { background: #E0E0E0 } \n"
		" TR.odd          { background: #C0C090; } \n"
		" TR.even         { background: #90C0C0; } \n"
		" TR:hover        { background: #C0C0C0 } \n"
		" TR.show_sums    { background: #C0F0C0 } \n"
	);
	fd.print("</STYLE>\n");
	fd.print(" </HEAD>\n");
	fd.print("<BODY>\n");
	fd.print("\n");

	fd.print("<H2>%s</H2>\n\n", ML( title ));
	return true;
}

bool rep_html::closes()
{
	fd.print("\n</BODY>\n</HTML>\n");
	fd.close();
	return true;
}

bool rep_html::opens_titles( str0 filename )
{
	str0 title = "Default Title: Information Cascade Ltd";
	if(info.fetch_row((str0)"title"))
	{
		title = str0( info._value );
	}

	// above fold to this
	info.fetch_value_default( title, "title", "DEFAULT TITLE");

	if(! opens( filename, title ) ) return false;

	// help_INFO info( dset );

	str0 year_start_str = "1999-12-31"; // looks suspect ?
	if(info.fetch_row("year_start"))
	{
		year_start_str = info._value;
	}
	date_glib year_start;
	year_start.set_year_mm_dd( year_start_str );


	str0 next_year_start_str = "1999-12-31"; // looks suspect ?
	if(info.fetch_row("next_year_start"))
	{
		next_year_start_str = info._value;
	}
	date_glib next_year_start;
	next_year_start.set_year_mm_dd( next_year_start_str );

	// this replaces above
	if(!info.must_fetch_date_year_start( year_start ))
		return FAIL_FAILED();
	if(!info.must_fetch_date_next_year_start( next_year_start ))
		return FAIL_FAILED();

/*
	date_glib next_year_start;
	int year, mm, dd;
	year_start.get_year_mm_dd( year, mm, dd );
	next_year_start.set_year_mm_dd( year+1, mm, dd );
*/

	date_glib year_end;
	year_end = next_year_start;
	year_end.add_days( -1 );


	fd.print("<H2>Year Start: %s</H2>\n", year_start.tmp_year_mm_dd() );

	fd.print("<H2>Year End: %s</H2>\n", year_end.tmp_year_mm_dd() );
	return true;
}

void rep_html::draw_td( str2 * data, str0 align, str0 clas, str0 href )
{
	str0 s = "";
	if( data ) s = data->str;
	draw_td( s, align, clas, href );
}

void rep_html::draw_td( str0 data, str0 align, str0 clas, str0 href )
{
	if( data == "-" ) data = "";
	if( data == "." ) data = "";
	str0 blank="&nbsp;";
	if( data.is_empty() )
	{
		if( href.is_empty() )
			align="";
		else
			data = "ALINK";
	}

	fd.print(" <td");
	if( align.isnt_empty() )
		fd.print(" align='%s'", (STR0) align );
	if( clas.isnt_empty() )
		fd.print(" class='%s'", (STR0) clas );
	fd.print(">");

	if(data.is_empty() )
		fd.print("%s", (STR0) blank );
	else
	 if( href.is_empty() )
		fd.print("%s", ML( data ) );
	 else
		fd.print("<A HREF='%s'>%s</A>", (STR0) href, ML( data ) );
	fd.print("</td>\n");
}

void rep_html::draw_tr_th()
{
	buffer2 buff;
	fd.print("<tr>");
	for( int i=0; i<ncol; i++ )
	{
		col_spec * spec = col_list[i];
		if(!spec) continue;
		if( spec->is_hidden ) continue;
		buff.clear();
		const char * s = (STR0) str0( spec->name );
		while( char c = *s++ ) {
			switch( c )
			{
			 case '_': buff.put( "<BR>" ); break; // CONVERT
			 case '<': buff.put( "&lt;" ); break;
			 case '>': buff.put( "&gt;" ); break;
			 case '&': buff.put( "&amp;" ); break;
			 default:
				 buff.put( c );
			}
		}
		fd.print("<th>%s</th>", (STR0) str0( buff )); // NO ML CONVERTION
	}
	fd.print("</tr>\n");
}

/*!
	TODO: rework as put text into a buffer, not return val from fn
*/
str0 rep_html::href_for_row( row_inst * row )
{
	static buffer2 buff;
	if( !img_spec ) return "";
	// TODO: this should provide the relative path
	str2 * s2 = img_spec->get_str2_for_display( row );
	if(!s2) return "";
	if(!*s2) return "";
	if( *s2 == "-" ) return "";
	buff.clear();
	buff.put( "img/" );
	buff.put( s2 );
	buff.put( ".png" );
	return buff;
}

void rep_html::draw_tr_td( row_inst * row )
{
	str0 empty = "";
	str0 tr_class= empty;
	str0 href= empty;
	if( row->row_flags & ROW_show_sums )
	{
		tr_class=" class='show_sums'";
	}

	fd.print( "<tr%s>\n", (STR0) tr_class );

	for( int j=0; j<ncol; j++ )
	{
		col_spec * spec = col_list[j];
		if( spec == img_spec )
			href = href_for_row( row );
		else
			href = empty;
		if( tree.spec_is_desc( spec ) )
			; // href = href_for_row( row );
		str0 opts = empty;
		str0 align = empty;

		switch( just_align[j].val ) {
		 case align_left:
		 case align_none:
		 break;
		 case align_right:
			align = "right";
		 break;
		 case align_center:
			align = "center";
		 break;
		}

		str0 clas = empty;
		draw_td( spec->get_str2_for_display( row ), align, clas, href );
	}

	fd.print( "</tr>\n" );
}

void rep_html::draw_tr_group_heading( tree_node * grp )
{
	if(!grp->group) return; // skip "table" header
	str2 * s = tree.get_key_desc( grp );
//	fd.print("<tr><th align=left colspan=%d bgcolor='white'>%s</th></tr>\n", ncol, ML(s) );
	fd.print("<tr><th align=left colspan=%d class='th2'>%s</th></tr>\n", ncol, ML(s) );
}

bool rep_html::draw_table(
	str0 tablename,
	str0 treecol
)
{

	// str0 totcol = "";
	// bool _landscape = true;
	// bool _full_key_path = true;
	// int interior_depth = 2;
	// bool interleave = true;
	// bool show_leaves = true;

	tbl = dset->get_table( tablename );
	if(!tbl)
	{
		e_print("No such table %s\n", (STR0)tablename );
		return false;
	}

	// can be NULL - used to detect == (other may not be NULL)
	img_spec = tbl->get_col_spec ("img" );

	col_spec * col_spec_tree_1;
	col_spec_tree_1 = tbl->get_col_spec( treecol );
	if(!col_spec_tree_1)
	{
		e_print("No such column %s\n", (STR0)treecol );
		return false;
	}
	tree.re_init_2( col_spec_tree_1 ); // late_init - constructor

	fd.print("<h2>%s</h2>\n", ML( tablename ) ); // ML conv

	build_col_list();
	build_tree();

	fd.print("<table border=-1>");
	draw_tr_th(); // headers

#if 1
	obj_list<tree_node> list2;
	bool depthwise = true;
	tree.flat_list_locn( list2, depthwise );
	int ngrp = list2.N();
	for( int i=0; i<ngrp; i++ )
	{
		tree_node * grp = list2[i];
		draw_tr_group_heading( grp );
		int nrow = grp->children_leaf.N();
		for( int j=0; j<nrow; j++ )
		{
			row_inst * row = grp->children_leaf[j];
			draw_tr_td( row );
		}
	}
#else
	obj_list<row_inst> list1;
	tree.flat_list_leaf( list1 );
	int nrow = list1.N();
	for( int i=0; i<nrow; i++ )
	{
		// row_inst * row = list1[i];
		draw_tr_td( list1[i] );

	}
#endif

	fd.print("</table>");

	return true;
}


/*!
	Build the default list of columns for tbl
*/
void rep_html::build_col_list()
{
	col_list.clear();
	just_align.clear();

	ncol = tbl->col_specs.N(); // is_hidden reduces this

	col_list.get_space( ncol );
	just_align.get_space( ncol );

	for( int j=0; j<ncol; j++ )
	{
		col_spec * spec = tbl->col_specs[j];
		if( !spec ) continue;
		if( spec->is_hidden ) continue;

		align_val aligns = spec -> get_align();

		col_list.append( spec );
		just_align.append( aligns );
	}
	ncol = col_list.N();
}

void rep_html::build_tree()
{
	int nr = tbl->rows_list.N();
	for( int i=0; i<nr; i++ )
	{
		row_inst * row = tbl->rows_list[i];
		tree.add_one_row( row );
	}
	tree.sort_interior();
}
