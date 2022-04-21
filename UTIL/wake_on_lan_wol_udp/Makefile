N=wake_on_lan_wol_udp

test: $N.exec
	./$N.exec 00:15:b7:11:35:64 boxname
	./$N.exec 00:15:b7:11:35:64 
	./$N.exec badmacoption

$N.exec: $N.c
	g++ -g -o $N.exec $N.c
