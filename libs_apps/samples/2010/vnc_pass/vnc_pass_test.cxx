#include "bit_flip.h"
#include "buffer2.h"
#include "SSL_global.h"
#include <unistd.h>
#include "util_buf.h"
#include "evp_cipher.h"
#include <openssl/des.h>
#include "auth_pw.h"


const char * stock =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

#define PASSLEN 8
bool C_make_pass(
        int uid,
        buffer2 & filename,
        buffer2 & pass,
        buffer2 & pass_hex
)
{
        if(uid <= 0 ) return FAIL("uid == %d", uid );
	int gid = 1000;
        int stock_len = strlen(stock);
	buffer2 msg;

	//
	// SHOW correct answer
	// this is what vncpasswd makes from abcdefgh
	char test_pass_abcdefgh[] = { 0xdb, 0x8a, 0xb0, 0x25, 0x65, 0xe3, 0xb4, 0xc2 };
	buffer2 pass_abcdefgh;
	pass_abcdefgh.set( test_pass_abcdefgh, 8 );
	pass_abcdefgh.dgb_dump( "VNCPASSWD makes this correct target value" );

	//
	// DEMO ENCRYPT
	//
	buffer2 pw_vnc;
	if(!AUTH:: PW_UTIL_VNC:: encrypt_vncpass( pw_vnc, "abcdefgh" ))
		return FAIL_FAILED();
	pw_vnc.dgb_dump( "EVP routines from abcdefgh" );

	//
	// DEMO ENCRYPT TIGHT
	//
	buffer2 pw_vnc_tight;
	if(!AUTH:: PW_UTIL_VNC:: encrypt_vncpass_TIGHT( pw_vnc_tight, "abcdefgh" ))
		return FAIL_FAILED();
	pw_vnc_tight.dgb_dump( "encrypt_vncpass_TIGHT routines from abcdefgh" );

	//
	// DEMO DECRYPT
	//
	buffer2 pw_vnc_plain;
	if(!AUTH:: PW_UTIL_VNC:: decrypt_vncpass( pw_vnc, pw_vnc_plain ))
		return FAIL_FAILED();
	pw_vnc_plain.dgb_dump( "EVP routines back to ascii abc... " );

	//
	// DEMO issue
	//
        // obtain an ASCII printable but fairly dense password
        pass.clear();
	if(!AUTH::PW_UTIL_VNC::RAND_pass8_dense_vnc( pass ))return FAIL_FAILED();
	//
	// DEMO ENCRYPT - of ISSUE
	//
	buffer2 pw_vnc_stored;
	if(!AUTH:: PW_UTIL_VNC:: encrypt_vncpass( pw_vnc_stored, (STR0) pass ))
		return FAIL_FAILED();
	pw_vnc_stored.dgb_dump( "The issued password - in a file" );

	// home
        filename.clear();
      //  filename.print( "/home/.vnc/vncpass_%d", uid );
        filename.print( "/tmp/vncpass_%d", uid );

	INFO("WRITING to %s", (STR0) filename );
	int mask = 0400; // file permission
        if(!blk_write_to_file_mask_uid_gid( pw_vnc_stored, filename, mask, uid, gid )) {
		WARN("That would have failed if you ran it a non root a second time");
                return FAIL_FAILED();
	}

        // read vnc's mangled password back in
        buffer2 vnc_pass;
	int MAX1K = 1;
        if(!blk_read_entire_file( vnc_pass, filename,  MAX1K )) // 1K max
                return FAIL_FAILED();

        // convert to 2 HEX digits
        int len = vnc_pass.nbytes_used;
        pass_hex.clear();
        for( int i=0; i<len; i++) {
                 if(!pass_hex.print("%02X", vnc_pass.buff[i]))
                        return FAIL_FAILED();
        }

}

bool
bool_main(int argc, char *argv[])
{
	// fire up and stir the entropy sub system
	obj_hold<SSL_global_server_eg>  _ssl_serv = new SSL_global_server_eg();
	buffer2 filename;
	buffer2 pass;
	buffer2 pass_hex;
	if(! C_make_pass(
		1001,
		filename,
		pass,
		pass_hex
	)) return FAIL_FAILED();
	return PASS("OK");
}

int
main(int argc, char *argv[])
{
	PASS("pre_start error FAIL fail = OK");
	if( bool_main( argc, argv ) ) return 0;
	return 1;
}

