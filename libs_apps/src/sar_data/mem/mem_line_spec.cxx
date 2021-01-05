// #include "line_id_tbl.h"

#include <string.h> // memcpy
#include <time.h> // ctime
#include "my_hash.h"
#include "mem_line_spec.h"

str0 spec_attr_raw::name()
{
	return mem_line_spec::get_attr_name( *this );
}

spec_attr_raw::spec_attr_raw( str0 name )
{
	spec_attr_raw search (0);
	while(1)
	{
		// if( 0==strcmp( name, search.name() ) ) {
		if( name == search.name() ) {
			raw_attr = search.raw_attr;
			return;
		}
		if( search.is_leaf_node() ) break;
		search.raw_attr++;
	}
	e_print("spec_attr_raw(%s) - no such name\n", name.sget() );
	raw_attr = -1;

	throw name;
}

mem_line_spec::mem_line_spec()
: linetype()
, dt( 0 )
{
	set_dt( dt );
}
// mem_line_spec::mem_line_spec( const mem_line_spec & _spec )
// {
// 	copy_from( &_spec );
// }

mem_line_spec::~mem_line_spec()
{
}

void mem_line_spec::set_dt( int _dt )
{
	dt = _dt;
	char buff[30];
	sprintf( buff, "%d", dt );
	dt_as_str.set( (str0) buff );
}

void mem_line_spec::copy_from( const mem_line_spec * spec2 )
{
	// rework as copy constructor
	linetype = spec2->linetype;
	site.set( spec2->site );
	host.set( spec2->host );
	 cmd.set( spec2->cmd );
	 opt.set( spec2->opt );
	 dev.set( spec2->dev );
	attr.set( spec2->attr );
	 avg.set( spec2->avg );
 //	  dt    = spec2->dt;
	set_dt( spec2->dt );
	recalc_hash_key();
}

uint mem_line_spec::recalc_hash_key()
{
	hash.init();
	hash.incl( site.get_hash_key() );
        hash.incl( host.get_hash_key() );
        hash.incl(  cmd.get_hash_key() );
        hash.incl(  opt.get_hash_key() );
        hash.incl(  dev.get_hash_key() );
        hash.incl( attr.get_hash_key() );
        hash.incl(  avg.get_hash_key() );
        hash.incl( dt );
	return hash.hash;
}

uint mem_line_spec::get_hash_key()
{
	if(! hash.hash ) recalc_hash_key();
	return hash.hash;
}

bool mem_line_spec::is_same_as( mem_line_spec * b )
{
	// mostly used after hash key match
	// put more obvious differences to the top
	// would benefit from shared strings
	if( this == b ) return TRUE;
	if( dt != b->dt ) { return FALSE; };
	if( !attr.is_same_as( b->attr ) ) { return FALSE; };
	if( !host.is_same_as( b->host ) ) { return FALSE; };
	if( ! cmd.is_same_as( b->cmd  ) ) { return FALSE; };
	if( ! opt.is_same_as( b->opt  ) ) { return FALSE; };
	if( ! dev.is_same_as( b->dev  ) ) { return FALSE; };
	if( ! avg.is_same_as( b->avg  ) ) { return FALSE; };
	if( !site.is_same_as( b->site ) ) { return FALSE; };
	if(  linetype != b->linetype    ) { return FALSE; };
	return TRUE;
}

str0 mem_line_spec::get_attr( const spec_attr_raw & depth ) const
{
	// depth = map_depth( depth );

	switch( depth.raw_attr ) {
	 case 0: return site;
	 case 1: return host;
	 case 2: return cmd;
	 case 3: return opt;
	 case 4: return dev;
	 case 5: return attr;
	 case 6: return dt_as_str;
	 case 7: return avg;
	 default:
		e_print("mem_line_spec::get_attr()  bad depth %d\n", depth.raw_attr );
		return (str0) "ERROR-DEPTH";
	}
}

// STATIC
str0 mem_line_spec::get_attr_name( const spec_attr_raw depth )
{
	// depth = map_depth( depth );
	switch( depth.raw_attr ) {
	 case 0: return "site";
	 case 1: return "host";
	 case 2: return "cmd";
	 case 3: return "opt";
	 case 4: return "dev";
	 case 5: return "attr";
	 case 6: return "dt";
	 case 7: return "avg";
	 default:
		e_print("mem_line_spec::get_attr_name()  bad depth %d\n", depth.raw_attr );
		return "ERROR-DEPTH";
	}
}

IS_DIFF_t mem_line_spec::cmp_at_depth(
	const mem_line_spec & spec2,
	const spec_attr_raw depth
)
{
	switch( depth.raw_attr ) {
	 case 0:
		return site.cmp( spec2.site );

	 case 1:
		return host.cmp( spec2.host );

	 case 2:
		return cmd.cmp( spec2.cmd );

	 case 3:
		return opt.cmp( spec2.opt );

	 case 4:
		return dev.cmp( spec2.dev );

	 case 5:
		return attr.cmp( spec2.attr );

	 case 6:

		if( dt < spec2.dt ) return IS_SAME;
		if( dt > spec2.dt ) return IS_MORE;
		return IS_SAME;

	 case 7:
		return avg.cmp( spec2.avg );

	 default:
		e_print("mem_line_spec::cmp_at_depth()  bad depth %d\n", depth.raw_attr );
		return IS_SAME;
	}

}

IS_DIFF_t mem_line_spec::cmp( const mem_line_spec & spec2 )
{
	for( int i = 0; i < N_depths(); i++ )
	{
		IS_DIFF_t diff = cmp_at_depth( spec2, i );
		if( diff != IS_SAME ) return diff;
	}
	return IS_SAME;
}

void mem_line_spec::print1( FILE * f ) const
{
	fprintf(f,"%s %s %s %s %s %s %d %s\n",
		site.sget(),
		host.sget(),
		cmd.sget(),
		opt.sget(),
		dev.sget(),
		attr.sget(),
		dt,
		avg.sget()
	);
}


