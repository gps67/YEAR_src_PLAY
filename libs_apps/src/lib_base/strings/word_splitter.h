#ifndef word_splitter_H
#define word_splitter_H

#include "buffer3.h"
#include "str2_reused.h"
#include "dyn_array.h"


/*!
	Split a line into an ARGV of words {blocks}

	TODO: word holds 'quote' "used" or doesnt strip
	(currently it strips and forgets, which is not really enough)

	TODO: better definition of {block syntax} rather than repeat parse.

	TODO: incremental parsing, when at depth-3

	TODO: balance between context free rules, and CMD says expect BLOB

	TODO: etc
*/
class word_splitter : public GRP_lib_base
{
 public:
	buffer2 word_space;
	str0 curr_word_start;
	dyn_array<str0> word_list; // str0 never allocates or releases

	bool parse_error; // to know when not just part line
	bool remove_newlines_added_with_curlies;

	word_splitter();
	~word_splitter();
	void clear();
	bool split_by_spaces( str0 line, int len = 0 );
	bool split_by_spaces_and_quotes( str0 line, int len = 0 );

	bool split_next_line_from( buffer3 & buf );

	void dgb_show( const char * name = NULL );

	int N()
	{
		return word_list.N();
	}

	str0 operator[] ( int idx )
	{
		return word_list[idx];
	}

	//
	// some extras for proto parse
	//
	bool scan_close_brace_line( buffer3 &in );
	bool scan_text_line_from_buffer_to_buffer(  buffer3 &in, blk1 & line ); // no CRLF
	bool scan_base64_line_from_buffer_to_blob(  buffer3 &in, blk1 & blob );
	bool split_by_spaces_and_quotes_multi_line( buffer3 & buf );
};

// TODO
bool sublex_split_one_dot( str0 whole, buffer2 & left, buffer2 & right );

#endif
