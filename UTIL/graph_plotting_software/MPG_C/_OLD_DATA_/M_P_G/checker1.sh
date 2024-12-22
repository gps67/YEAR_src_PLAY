set -x
gawk -F, 'NF!=5 {print $0}' < mpg.txt 
