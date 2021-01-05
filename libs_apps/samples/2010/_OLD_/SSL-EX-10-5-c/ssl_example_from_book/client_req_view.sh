PEM=client_req.pem
set -x
openssl req -noout -in $PEM -text
# openssl x509 -noout -in $PEM -text
