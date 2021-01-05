#ifndef line_spec_H
#define line_spec_H

// #include "glib.h" // GHashTable

#include <stdio.h> // FILE *
#include "str1.h"
#include "is_diff.h"
#include "my_hash.h"

class spec_attr_mapped;
struct mem_line_spec;
struct spec_attr_mapper;

#include "mem_line_type.h"


/*
	a mem_line_spec is a list of attributes that describe a line

		site, host, cmd, opt, attr, dev, dt, avg
		linetype

	These can be accessed by array index, or by name, except
	linetype, which is not a selector, its a characteristic.
	
	a mem_line_tree (elsewhere) is a tree which can have its
	depths re-arranged according to any (valid) sequence of fields.
	To help with that:

		spec_attr_raw - holds the real depth (0==site)

		spec_attr_mapped - holds the virtual depth

	In all cases -1 means root-depth, and 7 means leaf-depth
	(this limits tree to exactly 8 levels).

	Hence "attr_id" is spelled "depth"
*/

class spec_attr_raw : public GRP_sar_data
{
	static const int N8 = 8;
	friend class spec_attr_mapped;
	friend struct spec_attr_mapper;
	friend struct mem_line_spec;

	int raw_attr;
	spec_attr_raw( int a0 )
	{
		raw_attr = a0;
	}
	bool is_leaf_node() const
	{
		return( raw_attr + 1 >= N8 );
	}
 public:
	str0 name();
	spec_attr_raw( str0 name );
};

class spec_attr_mapped : public GRP_sar_data
{
	static const int N8 = 8;
	friend class spec_attr_raw;
	friend struct spec_attr_mapper;
	friend struct mem_line_spec;
 public:
	int map_attr;

	spec_attr_mapped( int a0 )
	{
		map_attr = a0;
	}
	bool is_leaf_node() const
	{
		return( map_attr + 1 >= N8 );
	}
	bool is_root_node() const
	{
		return( map_attr == -1 );
	}
	spec_attr_mapped prev() const
	{
		return spec_attr_mapped( map_attr - 1 );
	}
	spec_attr_mapped next() const
	{
		return spec_attr_mapped( map_attr + 1 );
	}
 static
	spec_attr_mapped root()
	{
		return spec_attr_mapped( -1 );
	}
	bool operator==( const spec_attr_mapped & attr2 ) const
	{
		return map_attr == attr2.map_attr;
	}
};

struct spec_attr_mapper : public GRP_sar_data
{
	static const int N8 = 8;
	int map[N8];
	spec_attr_raw un_map( const spec_attr_mapped & a1 ) const
	{
		return spec_attr_raw( map[ a1.map_attr ] );
	}

	spec_attr_mapper(
		int map1[N8]
	) {
		for(int i=0; i<N8; i++ )
		{
			map[i] = map1[i];
		}
	}

	spec_attr_mapper(
		str0 map1[N8]
	) {
		for(int i=0; i<N8; i++ )
		{
			spec_attr_raw search( map1[i] );
			map[i] = search.raw_attr;
		}
	}

};

struct mem_line_spec : public GRP_sar_data
{

	static const int N8 = 8;

	mem_line_type linetype;
	str1 site;
	str1 host;
	str1 cmd;
	str1 opt;
	str1 dev;
	str1 attr;
	str1 avg;
	int  dt;
	str1 dt_as_str;

	void set_dt( int _dt );

	my_hash hash;

	 mem_line_spec();
	~mem_line_spec();
	mem_line_spec( const mem_line_spec & spec2 ) {
		copy_from( & spec2 );
	}

	void copy_from( const mem_line_spec * spec2 );

	uint recalc_hash_key();
	uint get_hash_key();
	bool is_same_as( mem_line_spec * spec2 );

	void print1(FILE * out) const;

	IS_DIFF_t cmp( const mem_line_spec & spec2 );
	IS_DIFF_t cmp_at_depth(
		const mem_line_spec & spec2,
		const spec_attr_raw depth
	);

 static
	int N_depths() { return N8; } // 0..7
	str0 get_attr( const spec_attr_raw & depth ) const;
 static
	str0 get_attr_name( const spec_attr_raw depth );

};

#endif
