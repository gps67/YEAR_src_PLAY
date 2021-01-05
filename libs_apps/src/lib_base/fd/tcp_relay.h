#ifndef tcp_relay_H
#define tcp_relay_H

#include "ints.h" // GRP_lib_base
#include "sock_addr_base.h" // (sa_ptr,sa_len)


namespace tcp_relay {

 struct u32_hilo 
 {
        u32 sto; 

	u32 get_u32_cpu() { return ntoh( sto ); }
	u32 set_u32_cpu(u32 val) { sto = hton( val ); }

	u32 get_u32_hilo() { return sto; }
	u32 set_u32_hilo(u32 val) { sto = val; }
 };

 struct u16_hilo 
 {
        u16 sto; 

	u16 get_u16_cpu() { return ntoh( sto ); }
	u16 set_u16_cpu(u16 val) { sto = hton( val ); }

	u16 get_u16_hilo() { return sto; }
	u16 set_u16_hilo(u16 val) { sto = val; }
 };

 struct TCP_IP4_addr {
	 u16_hilo IP_port;
	 u32_hilo IP_addr;
 };
// alternatives ... sock_addr_ip	

class EXTRA_INFO_base : public GRP_lib_base
{
};

class tcp_relay_base : public GRP_lib_base
{
 public:
	fd & fd_left;
	fd & fd_right;

	tcp_relay_base() // all components do own init_null
	{
	}

	virtual bool set_fd_left( fd & _fd_left, EXTRA_INFO_base * EXTRA_INFO = NULL )
	{
		// your code can validate 
		fd_left = _fd_left; // set_from( _fd_left );
		return true;
	}

	virtual bool fd_accepted( LISTERNER,fd,inet_info);
	
};

};
#endif
