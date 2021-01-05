PEM=client_req.pem
set -x
openssl req -noout -in $PEM -text \
2>&1 | tee $0.txt_
# openssl x509 -noout -in $PEM -text
