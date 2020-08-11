PEM=client_cert.pem
PEM=keys/C_LAPTOP_cert.pem
set -x
openssl x509 -noout -in $PEM -text
2>&1 | tee $0.txt_
# openssl x509 -noout -in $PEM -text
# openssl req -noout -in $PEM -text \
