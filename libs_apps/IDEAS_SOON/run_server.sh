D1=/tools/2004_src/libs/openssl-0.9.7c/apps/
S1=$D1/server.pem
S1=/media/hda9/tools/2000_obj/libs/openssl-0.9.7e/demos/bio/server.pem
S1=/nfs/md7/PKGS/WIN32/HANDSET_WIN32/openssl-1.0.0-beta2/demos/bio/server.pem
set -x
openssl \
s_server \
-cert $S1 \
-state \
"$@"
