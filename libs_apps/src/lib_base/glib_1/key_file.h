#ifndef key_file_H
#define key_file_H

#ifdef do_it_slow_but_sure
#include <glib.h>
// #include "g_rrr.h"
#else
typedef struct _GKeyFile GKeyFile;
// class G_rrr;
#endif
#include "str1.h"
#include "g_rrr.h"
#include "blk1.h"
#include "dgb_fail.h"

/*!
	glib provides a .ini file handler for config.ini

	NB stupid name

	It has lots of internatinalisation options,
	but I am only interested in LANG=C

	NB case sensitive
*/
class Key_file
{
	GKeyFile * kf;
	G_rrr & g_rrr;
	str1 file_one;
	bool must_be_encrypted;

 public:
	virtual ~Key_file();
	Key_file( G_rrr & g_rrr );
	bool has_key( const char * group, const char * item );
	bool bind(
		bool write,
		const char * group,
		const char * item,
		str1 & value,
		const char * cmnt_if_absent,
		bool is_multi_line = false
	);
	bool get_value(
		const char * group,
		const char * item,
		str1 & value,
		const char * deflt = NULL, // "" means OK NULL means error
		bool is_multi_line = false
	);
	bool set_value(
		const char * group,
		const char * item,
		str1 & value,
		bool is_multi_line = false
	);
	bool del_item(
		const char * group,
		const char * item
	);

#if 0
	bool load_from_encrypted_file( key_holder & evp, const char * filename );
	bool save_into_encrypted_file( key_holder & evp, const char * filename );
#endif

	virtual bool load_from_file( const char * filename, bool error_if_absent = true  );
	virtual bool save_into_file( const char * filename );
	bool load_from_buf( blk1 & buf );
	bool save_into_buf( blk1 & buf );
	bool save() {
		WARN("Using save() might stop save_encrypted(keyz)");
		return save_into_file( file_one.get_str() );
	}
	bool set_comment_for_item(
		const char * group,
		const char * item,
		const char * cmnt
	);
	bool set_comment_for_group(
		const char * group,
		const char * cmnt
	);
	bool set_comment_for_file(
		const char * cmnt
	);
	GError ** grrr() { return g_rrr.grrr(); }

};

#endif
