#ifndef file_rename_H
#define file_rename_H

extern
bool file_unlink( const char * old_name );

extern
bool file_link( const char * old_name, const char * new_name );

extern
bool file_rename( const char * old_name, const char * new_name );

#endif

