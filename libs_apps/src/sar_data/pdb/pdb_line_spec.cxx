
#include "pdb_line_spec.h"
#include "mem_line_spec.h"
#include "pdb_string_pool.h"

/*!
	set the pdb_line_spec's value from a mem_line_spec
*/
void pdb_line_spec::set_from(
	pdb_string_pool * string_pool,
	mem_line_spec * spec
) {
	linetype = spec->linetype;
	site = string_pool->find( spec->site );
	host = string_pool->find( spec->host );
	dev  = string_pool->find( spec->dev );
	cmd  = string_pool->find( spec->cmd );
	opt  = string_pool->find( spec->opt );
	attr = string_pool->find( spec->attr );
	avg  = string_pool->find( spec->avg );
	dt   = spec->dt;
	if(0) {
	// this is called from find_add_line() - to build a temp MEM == val
		fprintf(stderr, "pdb_line_spec::set_from() " );
		spec->print1(stderr);
		gdb_break_point();
	}
	if( site < 0 )
		gdb_break_point();
	if( site < 0 )
		e_print("pdb_line_spec::set_from() -1 %s\n", spec->attr.str );
	else
		if(0) print2("set_from(mem_line_spec)");
}

/*!
	set the pdb_line_spec's value from another pdb_line_spec
*/
void pdb_line_spec::set_from(
	const pdb_line_spec * spec
) {
	linetype = spec->linetype;
	site = spec->site;
	host = spec->host;
	dev  = spec->dev;
	cmd  = spec->cmd;
	opt  = spec->opt;
	attr = spec->attr;
	dt   = spec->dt;
	avg  = spec->avg;
	print2("set_from(pdb_line_spec)");
}

/*!
	debugging print
*/
void pdb_line_spec::print2( const char * msg )
{
	fprintf(stderr,"%s: "
		"site=%d "
		"host=%d "
		"dev=%d "
		"cmd=%d "
		"opt=%d "
		"attr=%d "
		"dt=%d "
		"avg=%d \n",

		msg,
		site,
		host,
		dev,
		cmd,
		opt,
		attr,
		dt,
		avg 
	);
}

/*!
	convert this pdb_line_spec into a mem_line_spec
*/
void pdb_line_spec::get_into(
	pdb_string_pool * string_pool,
	mem_line_spec * spec
) {

//	if( site <0 ) gdb_break_point();

	if(!spec) {
		print2("## get_into(mem_line_spec NULL)");
		return;
	}
	spec->linetype = linetype;
	spec->site.set( string_pool->get_str( site ));
	spec->host.set( string_pool->get_str( host ));
	spec->dev.set(  string_pool->get_str(  dev ));
	spec->cmd.set(  string_pool->get_str(  cmd ));
	spec->opt.set(  string_pool->get_str(  opt ));
	spec->attr.set( string_pool->get_str( attr ));
	spec->avg.set(  string_pool->get_str(  avg ));
	spec->set_dt( dt );
//	spec->dt   = dt;
	spec->recalc_hash_key();
	if(0) print2("get_into(mem_line_spec)");
	return;
}

