
#include <openssl/err.h>
#include <openssl/ssl.h>

extern "C" int gtk_init(int * p_argc, char *** p_argv );
extern "C" int gtk_main();
extern     void e_print_init_console();

// DONE // #include "SSL_global.h"
// DONE // #include "X509_cert.h"
#include "fd_ssl_client.h"

//#define PFX "/home/gps/2004/src/IDEAS/openssl-0.9.7c/apps/"
// #define PFX "/media/md7/GPS/AIR/PKGS/HANDSET_WIN32/openssl-1.0.0-beta2/apps/"
// #define PFX "/nfs/md7/PKGS/HANDSET_WIN32/openssl-1.0.0-beta2/apps/"
#define PFX "/nfs/md7/PKGS/PKGS_Build_LGX/openssl-1.0.0-beta3/apps/"

int main(int argc, char ** argv )
{
	gtk_init(&argc, &argv);

	e_print_init_console();

	SSL_global_base SSL;
	// OK this function is an old one
	// it can be brought back, but there is a new way
	// but at least it coompiles and links
	// or would do if this function existed
	
	SSL.use_key_cert_files(
		PFX "client.pem",
		PFX "client.pem"
	);


	// spin the RANDOM wheel here ?

	fd_ssl_client::test1( SSL );

	gtk_main();

	return 0;
}
