#!/usr/bin/env python

from socket import *
import time

s = socket( AF_INET, SOCK_DGRAM )
s.bind(('', 0))
# s.setsockopt(SOL_SOCKET, SO_BROADCAST, 1)

host = '127.0.0.1'
port = 1514 
if 0:
	host = 'log2'
	port = 514
addr_dest = (host, port )
# s.connect( addr_dest )
s.sendto( "Data line" , addr_dest )
s.sendto( "And then another line" , addr_dest )
time.sleep(2)
s.close()

