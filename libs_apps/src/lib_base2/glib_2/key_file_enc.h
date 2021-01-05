#ifndef key_file_enc_H
#define key_file_enc_H

#include "key_file.h"	// glibs stupid use of 'key'
#include "key_holder.h" // SSL ENCR use of 'key'

/*
	Encryption over a glib ini file (badly named key_file)

	Key_file is in base1 Key_file_enc is in base2
*/

class Key_file_enc : public Key_file
{
 public:
	Key_file_enc( G_rrr & g_rrr );
	bool load_from_encrypted_file( key_holder & evp, const char * filename );
	bool save_into_encrypted_file( key_holder & evp, const char * filename );
	virtual bool load_from_file( const char * filename, bool error_if_absent = true  );
	virtual bool save_into_file( const char * filename );
};


#endif

