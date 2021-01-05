#include "err_int.h"
#include "dgb.h"
#include "fd_hold.h"

#include <openssl/err.h>


bool err_int_t:: get_GTK_error( GError * rrr )
{
	return FAIL("TODO");
}
bool err_int_t:: get_G_rrr( G_rrr & g_rrr )
{
	return FAIL("TODO");
}
bool err_int_t:: get_SSL_error() // openSSL // also gives lines + more info ?
{
	bool found = false;
        int e;
        while((e=ERR_get_error()))
        {
		found = true;
		err = e;
                char buf[1024];
                ERR_error_string_n(e,buf,sizeof buf);
		e_zone = "SSL";
		e_name = "NONAMEYET";
		e_str = buf;
                WARN( "# %d # %s", (int) e, (const char *) buf );
                // BUT ERROR IS LOST - squirrel it away somewhere!
        }
//	gdb_invoke(); recursive
	return found; // is_error() or just an SSL error
}


bool err_int_t:: is_key_not_found() // groped for absent item, not an error
{
	return FAIL("TODO");
}
