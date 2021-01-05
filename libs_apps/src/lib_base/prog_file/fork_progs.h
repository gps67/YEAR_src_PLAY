#ifndef fork_progs_H
#define fork_progs_H

bool fork_xpdf( str0 filename );
bool fork_xpdf_page( str0 filename, int pageno );
bool fork_netscape( str0 filename );
bool fork_make( str0 dir, str0 target );

#endif
