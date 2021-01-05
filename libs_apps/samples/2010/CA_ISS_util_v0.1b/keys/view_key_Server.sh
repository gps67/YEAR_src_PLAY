set -x
N=C_server
KP=${N}_key.pem
cat $KP
openssl rsa -in ${KP}
