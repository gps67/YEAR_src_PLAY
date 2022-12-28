
#include "tty_stty_1.h"
#include "dgb.h" // FAIL
#include <unistd.h>
#include "buffer2.h"


namespace BASE1 {


 void show_flag( STR0 item_name, char iocl, tcflag_t val, tcflag_t mask, STR0 flag_name ) {
 	STR0 bit_val = "NO";
 	if( val & mask ) {
		bit_val = "YES";
	}
 	e_print( "%10s %c %-8s %s\n", item_name, iocl,  flag_name, bit_val );
 }

 void show_char( STR0 item_name, int idx, STR0 idx_name, char chr_val ) {

 	buffer1 chr_str;
 	chr_str.print( "%10s %-8s ", item_name, idx_name, chr_val, (STR0) chr_str );
	chr_str.print("x%02X %2d ", chr_val, chr_val );
	if(!chr_val) {
		chr_str.print("NUL");
	}
	else if(chr_val<=26) {
		chr_str.print("CTRL-%c",chr_val+'@');
	}
	else if(chr_val==127) {
		chr_str.print("DEL");
	}
 	
 	e_print( "%s\n", (STR0) chr_str );
 }

#define i_show_flag( mask_macro_name ) \
	show_flag( label, 'I', tio.c_iflag, mask_macro_name, #mask_macro_name )

#define o_show_flag( mask_macro_name ) \
	show_flag( label, 'O', tio.c_oflag, mask_macro_name, #mask_macro_name )

#define c_show_flag( mask_macro_name ) \
	show_flag( label, 'C', tio.c_cflag, mask_macro_name, #mask_macro_name )

#define l_show_flag( mask_macro_name ) \
	show_flag( label, 'L', tio.c_lflag, mask_macro_name, #mask_macro_name )

#define	m_show_char( char_pos_name ) \
	show_char( label, char_pos_name, #char_pos_name, tio.c_cc[char_pos_name] )

 tty_stty_t::
 tty_stty_t()
 {
 	fd_tty = -1;
 }

 bool
 tty_stty_t::
 expect_is_open()
 {
 	if( fd_tty < 0 ) {
		return FAIL("fd_tty is %d", fd_tty);
	}
 	return true;
 }


 bool
 tty_stty_t::
 tio_get()
 {
 	if(!expect_is_open()) return FAIL_FAILED();
        if( -1 == tcgetattr(fd_tty, &tio)) {
		return FAIL("tcgetattr(%d,tio)",fd_tty);
	}
 	return PASS("OK");
 }


 bool
 tty_stty_t::
 show_info(STR0 label)
 {
 	INFO("label '%s'", label );
	i_show_flag( BRKINT );
	i_show_flag( IGNPAR );
	i_show_flag( PARMRK );
	i_show_flag( INPCK );
	i_show_flag( ISTRIP );
	i_show_flag( INLCR );
	i_show_flag( IGNCR );
	i_show_flag( ICRNL );
	i_show_flag( IUCLC );
	i_show_flag( IXON );
	i_show_flag( IXANY );
	i_show_flag( IXOFF );
	i_show_flag( IMAXBEL );
	i_show_flag( IUTF8 );

	o_show_flag( OPOST );
	o_show_flag( OLCUC );
	o_show_flag( ONLCR );
	o_show_flag( OCRNL );
	o_show_flag( ONOCR );
	o_show_flag( ONLRET );
	o_show_flag( OFILL );
	o_show_flag( OFDEL );
	o_show_flag( NLDLY );
	o_show_flag( CRDLY );
	o_show_flag( TABDLY ); // values 
	o_show_flag( BSDLY ); // values 
	o_show_flag( VTDLY ); // values 
	o_show_flag( FFDLY ); // values 

	c_show_flag( CBAUD );
	c_show_flag( CBAUDEX );
	c_show_flag( CSIZE ); // values
	c_show_flag( CSTOPB );
	c_show_flag( CREAD );
	c_show_flag( PARENB );
	c_show_flag( PARODD );
	c_show_flag( HUPCL );
	c_show_flag( CLOCAL );
//	c_show_flag( LOBLK ); // not POSIX not LINUX
	c_show_flag( CIBAUD );
	c_show_flag( CMSPAR );
	c_show_flag( CRTSCTS );

	l_show_flag( ISIG );
	l_show_flag( ICANON );
	l_show_flag( XCASE );
	l_show_flag( ECHO );
	l_show_flag( ECHOE );
	l_show_flag( ECHOK );
	l_show_flag( ECHONL );
	l_show_flag( ECHOCTL );
	l_show_flag( ECHOPRT );
	l_show_flag( ECHOKE );
//	l_show_flag( DEFECHO ) // not POSIX not LINUX;
	l_show_flag( FLUSHO );
	l_show_flag( NOFLSH );
	l_show_flag( TOSTOP );
	l_show_flag( PENDIN );
	l_show_flag( IEXTEN );


	m_show_char( VDISCARD );
//	m_show_char( VDSUSP ); // not POSIX not LINUX;
	m_show_char( VEOF );
	m_show_char( VEOL );
	m_show_char( VEOL2 );
	m_show_char( VERASE );
	m_show_char( VINTR );
	m_show_char( VKILL );
	m_show_char( VLNEXT );
	m_show_char( VMIN );
	m_show_char( VQUIT );
	m_show_char( VREPRINT );
	m_show_char( VSTART );
//	m_show_char( VSTATUS ); // not POSIX not LINUX;
	m_show_char( VSTOP );
	m_show_char( VSUSP );
//	m_show_char( VSWTCH ); // not POSIX not LINUX;
	m_show_char( VTIME );
	m_show_char( VWERASE );


 	return true;
 }

 bool
 tty_stty_t::
 set_fd( int fd )
 {
 	fd_tty = fd;
 	if(!tio_get()) return FAIL_FAILED();
 	return FAIL("TODO");
 }

 bool
 tty_stty_t::
 tio_set()
 {
 	if(!expect_is_open()) return FAIL_FAILED();
 	return FAIL("TODO");
 }

 bool
 tty_stty_t::
 stty_saner()
 {
 	if(!expect_is_open()) return FAIL_FAILED();
 	return FAIL("TODO");
 }

}; // namespace


using namespace BASE1;

bool stty_test1()
{
	tty_stty_t stty;
//	stty.fd_tty = 0;
	stty.set_fd(0);
	stty.show_info("fd_zero");
	if(!stty.stty_saner()) 
		return FAIL_FAILED();
	return PASS("OK");
}
