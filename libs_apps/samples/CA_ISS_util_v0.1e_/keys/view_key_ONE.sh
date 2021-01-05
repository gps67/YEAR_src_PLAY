set -x
N=CA_ONE
KP=${N}_key.pem
cat $KP
openssl rsa -in ${KP}
