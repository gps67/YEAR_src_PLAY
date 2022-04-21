
/*
	A broadcast frame
	anywhere in its payload
	6 bytes of FF
	16 x repetitions of 6 bytes MAC address

	== 102 bytes

	and protocol 
	usually IP4.udp.to_port(0) 0 or 7 or 9

*/
/*
	Right click on win7 adapter
	select properties

	Advanced [Tab]
	Wake on Magic Packet	Enabled

	Power Management [Tab]
	Tick, tick tick
	Wake on Magic Packet	Enabled

	Control Panel
	 Programs and features
	  Turn Windows Features on or off
	   Simple TCP/IP services (echo daytime ...) TICK

	Administrator tools
	   Simple TCP/IP services 
		 Restart

	Control Panel
	 Windows Firewall
	  Advanced Settings
	   Inbound Rule
	    New Rule
		... port 9

	BIOS
		must be set there


	(above words are well old - probably nonsense by now)
*/

#include <stdio.h>
#include <string.h> // memset
#include <unistd.h> // close

#include <arpa/inet.h>
#include <sys/socket.h>

typedef unsigned char u8;

#include <errno.h>

bool is_hex_digit( char c, int & val  )
{
	if(( c >= '0' ) && ( c <= '9' )) {
		val = c - '0';
		return true;
	}
	if(( c >= 'A' ) && ( c <= 'F' )) {
		val = c + 10 - 'A' ;
		return true;
	}
	if(( c >= 'a' ) && ( c <= 'f' )) {
		val = c + 10 - 'a' ;
		return true;
	}
	return false;
}

bool is_hex_byte( const char * P , u8 & val )
{
	int nibble1;
	int nibble2;
	if( !is_hex_digit( P[0], nibble1 ) ) return false;
	if( !is_hex_digit( P[1], nibble2 ) ) return false;
	val = (nibble1 << 4) + nibble2;
	return true;
}

bool is_hex_byte_colon( const char * P, u8 & val ) {
	if( !is_hex_byte( P, val )) return false;
	if( P[2] != ':' ) return false;
	return true;
}

bool is_space( u8 * P )
{
	if(!P) return false;
	switch(*P) {
	 case ' ': return true;
	 case '\t': return true;
	}
	// if(*P < ' ') WARN("NOT EXPECTING CR LF etc");
	return false;
}

bool is_mac( const char * P, u8 * addr ) {

	if( !is_hex_byte_colon( P+0, addr[0] )) return false;
	if( !is_hex_byte_colon( P+3, addr[1] )) return false;
	if( !is_hex_byte_colon( P+6, addr[2] )) return false;
	if( !is_hex_byte_colon( P+9, addr[3] )) return false;
	if( !is_hex_byte_colon( P+12, addr[4] )) return false;
	if( !is_hex_byte(       P+15, addr[5] )) return false;
	return true;
}

bool must_be_mac( const char * mac_str )
{
	u8 addr[6];
	if( !is_mac( mac_str, addr )) {
		printf("FAILED from %s\n", mac_str );
		return false;
	}
	if(0) printf("# OK MAC %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X from %s\n",
		addr[0],
		addr[1],
		addr[2],
		addr[3],
		addr[4],
		addr[5],
		mac_str
	);
	return true;
}

bool must_not_be_mac( const char * mac_str )
{
	u8 addr[6];
	if( is_mac( mac_str, addr )) {
		printf("MAC %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X from %s\n",
			addr[0],
			addr[1],
			addr[2],
			addr[3],
			addr[4],
			addr[5],
			mac_str
		);
		printf("FAILED because it is from %s\n", mac_str );
		return false;
	} else {
		printf("# OK not MAC from %s\n", mac_str );
		return true;
	}
}

bool send_wake( const char * mac_str, const char * cmnt ) {

	u8 eth_addr[6];
	if( !is_mac( mac_str, eth_addr )) {
		printf("FAILED is_mac('%s')\n", mac_str );
		return false;
	}
	must_be_mac(mac_str);

	u8 message[106];
	u8 * message_ptr = message;
	memset(message_ptr, 0xFF, 6);
	message_ptr += 6;
	for (int i=0; i<16; i++ ) {
		memcpy(message_ptr, eth_addr, 6);
		message_ptr += 6;
	}

	// wiki: 6 bytes of password
	// unused option

	int sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("socket");
		return false;
	}

	int optval = 1; // true - switch on
	if( setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof optval) < 0) {
		perror("setsockopt SO_BROADCAST");
		close(sock);
		return false;
	}

	struct sockaddr_in addr;
//	unsigned port = 6; // ok fm2
	unsigned port = 9; // ok fm2
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	addr.sin_port = htons(port);

	// perror("BEFORE");
	if (sendto(
		sock,
		(char *)message,
		sizeof message,
		0,
		(struct sockaddr *)&addr,
		sizeof addr
	) < 0) {
		perror("sendto");
		close(sock);
		return false;
	}

	close(sock);
	printf("# OK # WAKE sent to %s // %s\n", mac_str, cmnt );
	return true;
}


int main( int argc, char * argv[] )
{
	if( argc == 1 ) {
		// test call without args
		if(!send_wake("00:15:b7:11:35:64", "M7" )) return -errno;
	} else {
		int arg_i = 1;
		while( arg_i < argc ) {
			if( (arg_i + 1) < argc ) {
				if(!send_wake(argv[arg_i], argv[arg_i+1]))
					return -errno;
				arg_i += 2;
			} else {
				if(!send_wake(argv[arg_i], "NONAME"))
					return -errno;
				arg_i ++;
			}
		}
	}
	return -errno;

 if(0) {
	must_be_mac("5c:26:0a:68:e7:40");
	must_not_be_mac("5c 26:0a:68:e7:40");
	must_not_be_mac("zz:26:0a:68:e7:4o");
 }
	return -errno;
}

