N=wake_on_lan_wol_udp

test: $N.exec
	./$N.exec # should use default builtin fm2
	./$N.exec 4c:bb:58:58:99:7c ams10
	./$N.exec 00:15:b7:11:35:64 boxname
	./$N.exec 00:15:b7:11:35:64 
	./$N.exec badmacoption

$N.exec: $N.c
	g++ -g -o $N.exec $N.c
