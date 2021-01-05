#include "buffer3.h" 

/*!
	reduce the front gap to zero
*/
void buffer3:: circ_copy_down_fn()
{
	if(offs_front)
	{
		INFO("offs_front was %d", offs_front);
		del_copy_down( offs_front );
		offs_front = 0;
	}
}

/*!
	the data can move down
*/
void buffer3:: circ_check_fn()
{
		// nothing to do
		if( !offs_front ) return;

		// limit front gap to 10K
		if( offs_front > (1024*10) ) {
			circ_copy_down();
			return;
		}

		int data_len = nbytes_used - offs_front;
		int back_gap = nbytes_alloc - nbytes_used;
		int fore_gap = (int) offs_front;

		// circ_buffer empty or error
		if( data_len <= 0 ) {
			WARN("EMPTY discovered here");
			if( data_len < 0 ) gdb_invoke(true);
			if( data_len < 0 ) FAIL("negative");
			circ_clear();
			return;
		}

		if( data_len > (1024*64)) {
			STEP("plenty of data in buffer");
			return;
		}

		if( fore_gap > (int) (nbytes_alloc/2)) {
			STEP("past half way");
			circ_copy_down();
			return;
		}

		if( fore_gap > (back_gap+(1024*4))) {
			// problem with full big buffer adjusting every byte
			STEP("more gap than space");
			circ_copy_down();
			return;
		}

		if( data_len < 100 ) {
			// would be nice if copy down still left 4K alignments
			circ_copy_down();
			return;
		}
}

void buffer3::dgb_show( const char * s1, const char * s2 )
{
//	if(!s2) s2 = "lost/data/space";
	if(!s2) s2 = "buff";
	if(!s1) s1 = "streamname";
	buffer2 s;
	s.print( "%-16s %8s lost/data/space %4d %4d %4d",
		s1,
		s2,
		circ_lost_space_unavail(),
		circ_data_avail(),
		circ_space_avail()
	);

	int N = circ_data_avail();
	if(N) {
 if(1)	s.print( "\n# DATA # " ); // cheats TRACE system, but makes it readable
	s.put( " '" );
	u8 * P = circ_addr_rd();
	int N_max = 40;
	bool truncated = false;
	if (N > N_max) {
		N = N_max;
		truncated = true;
	}
	for(int i=0; i<N; i++) {
		u8 c1 = *P++;
		u8 c2 = c1;
		const char * cname = NULL;
		switch(c1) {
		 case 0x00: cname = "NUL"; break;
		 case 0x09: cname = "TAB"; break;
		 case 0x0D: cname = "CR";  break;
		 case 0x0A: cname = "LF";  break;
		 case 0x7F: cname = "DEL"; break;
		 case 0x0C: cname = "FF";  break;
		}
		if(cname) {
			s.print("(%s)", cname );
			continue;
		}
		if(c1<' ') c1 = 0;
		if(c1>126) c1 = 0;
		if(c1)
			s.put_byte( c1 );
		else
			s.print( "(%2.2X)", c2 );
	}
	if(truncated)
		s.put( " ...'" );
	else
		s.put( "'");
	}
	INFO( "%s", (STR0) s );
}

