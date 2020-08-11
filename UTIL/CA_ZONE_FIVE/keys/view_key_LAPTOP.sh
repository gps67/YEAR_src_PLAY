set -x
N=C_LAPTOP
KP=${N}_key.pem
cat $KP
openssl rsa -in ${KP}
