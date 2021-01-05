set -x
N=CA_ZONE
KP=${N}_key.pem
cat $KP
openssl rsa -in ${KP}
