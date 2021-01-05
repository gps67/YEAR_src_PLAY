set -x
N=CA_ZERO
KP=${N}_key.pem
cat $KP
openssl rsa -in ${KP}
