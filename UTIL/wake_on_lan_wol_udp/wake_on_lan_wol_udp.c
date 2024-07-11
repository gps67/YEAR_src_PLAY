
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
	// hex byte is PAIR of hex digits
	// get binary value into BYTE val
	if( !is_hex_byte( P, val )) return false;
	// check for colon ":"
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

	// convert str_mac_P("FF:FF:FF:FF:FF:FF") to binary 6-bytes addr[6]
	if( !is_hex_byte_colon( P+0, addr[0] )) return false;
	if( !is_hex_byte_colon( P+3, addr[1] )) return false;
	if( !is_hex_byte_colon( P+6, addr[2] )) return false;
	if( !is_hex_byte_colon( P+9, addr[3] )) return false;
	if( !is_hex_byte_colon( P+12, addr[4] )) return false;
	if( !is_hex_byte(       P+15, addr[5] )) return false;
	// do not check that text continues with NON_HEX nor another :
	return true;
}

bool must_be_mac( const char * mac_str )
{
	// convert mac_str to binary addr[6] 
	u8 addr[6];
	if( !is_mac( mac_str, addr )) {
		// PARSE of MAC failed
		printf("FAILED from %s\n", mac_str );
		return false;
	}
	// debugging print // HI to LO byte order
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
	// convert mac_str to binary addr[6] 
	u8 addr[6];
	if( is_mac( mac_str, addr )) {
		// debugging print // probably all zeros
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
		// debugging print // or mac_str not binary
		printf("# OK not MAC from %s\n", mac_str );
		return true;
	}
}

bool send_wake( const char * mac_str, const char * cmnt ) {

	// start by checking our ARG mac_str
	// it should be a MAC like "74:d4:35:fb:3f:6f"
	if(!must_be_mac(mac_str)) {
		printf("FAILED is_mac('%s')\n", mac_str );
		return false;
	}

	// wiki: 6 bytes of password
	// unused option

	// sock is the transmitting UDP socket
	int sock;

	// open sock IP4 UDP
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("socket");
		return false;
	}
	
	// set sock to BROADCAST 
	int optval = 1; // true - switch on
	if( setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof optval) < 0) {
		perror("setsockopt SO_BROADCAST");
		close(sock);
		return false;
	}

	// set broadcast address port 9
	struct sockaddr_in dest_addr;
//	unsigned port = 6; // ok fm2
	unsigned port = 9; // ok fm2
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	dest_addr.sin_port = htons(port);

	// check again PARSE MAC and convert max_str to 6 binary bytes
	u8 eth_addr[6];
	if( !is_mac( mac_str, eth_addr )) {
		printf("FAILED is_mac('%s')\n", mac_str );
		return false;
	}

	// build PACKET 
	// IP4_UDP header added by SOCK
	// 6 bytes of FF
	// 6 bytes of MAC - repeated 16 times
	
	u8 packet_message[106];
	u8 * message_ptr = packet_message;
	memset(message_ptr, 0xFF, 6);
	message_ptr += 6;
	for (int i=0; i<16; i++ ) {
		memcpy(message_ptr, eth_addr, 6);
		message_ptr += 6;
	}

	// transmit PACKET to SEGMENT to BROADCAST dest_addr from sock
	if (sendto(
		sock,
		(char *)packet_message,
		sizeof packet_message,
		0,
		(struct sockaddr *)&dest_addr,
		sizeof dest_addr
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
	//	if(!send_wake("00:15:b7:11:35:64", "M7" )) return -errno;
		if(!send_wake("b8:ca:3a:cc:64:4a", "e6320" )) return -errno;
		if(!send_wake("22:16:4d:04:17:dd", "x79-1g" )) return -errno;
		if(!send_wake("00:e0:4c:69:5d:c6", "x79-2g" )) return -errno;
		if(!send_wake("74:d4:35:fb:3f:6f ", "fm2" )) return -errno;

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

