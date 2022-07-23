set term png size 1200, 800
set output 'CET2.png'
set title 'cetml1659on.dat'
set ylabel 'Centigrade'
plot 'CET2.tmp' using 1:2 with lines title "CET2"
