#ifndef pdb_line_spec_H
#define pdb_line_spec_H


class mem_line_spec;
class pdb_string_pool;

#include "misc.h"

#include "mem_line_type.h"

/*!
	pdb_line_spec is analagous to a mem_line_spec,
	but specific to the NS_POST file.

	One line_type is used for both/all line types,
	the linetype tells you what data to expect.
*/
struct pdb_line_spec : public GRP_sar_data
{
 public:
	// Point_or_Series 
	// mem_line_type::Point_or_Series linetype;
	mem_line_type linetype;
	int site;
	int host;
	int dev;
	int cmd;
	int opt;
	int attr;
	int dt;
	int avg;

	/*
		this is barely a NS_POST object, but ??
		it needs an INERT contructor
		Durint relocating reload - it was splatted
	*/
	pdb_line_spec()
	{
		// init_minus_one();
		// e_print("pdb_line_spec() site=%d\n", site );
		return;
	}

	void init_minus_one()
	{

		const static int M1 = -2;
		linetype.init_M1();
		site = M1;
		host = M1;
		dev = M1;
		cmd = M1;
		opt = M1;
		attr = M1;
		dt = M1;
		avg = M1;
	}

	// MUST NOT be the default constructor !!
	pdb_line_spec( int x1 )
	{
		init_minus_one();
	}

	~pdb_line_spec()
	{
		init_minus_one();
	}

	void set_from(
		const pdb_line_spec * spec
	);

	void set_from(
		pdb_string_pool * _string_pool,
		mem_line_spec * spec
	);

	void get_into(
		pdb_string_pool * _string_pool,
		mem_line_spec * spec
	);

	void print2( const char * msg );

	bool equals( pdb_line_spec * spec2 )
	{
		if( attr != spec2->attr ) return FALSE;
		if( site != spec2->site ) return FALSE;
		if( host != spec2->host ) return FALSE;
		if( dev  != spec2->dev  ) return FALSE;
		if( cmd  != spec2->cmd  ) return FALSE;
		if( opt  != spec2->opt  ) return FALSE;
		if( attr != spec2->attr ) return FALSE;
		if( dt   != spec2->dt   ) return FALSE;
		if( avg  != spec2->avg  ) return FALSE;
		if( linetype != spec2->linetype ) return FALSE;
		return TRUE;
	}

};

#endif
