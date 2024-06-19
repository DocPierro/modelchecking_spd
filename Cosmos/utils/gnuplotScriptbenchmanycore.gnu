set terminal pngcairo font "arial,10" fontscale 1.0 size 1000, 700 


set grid lc rgb "black"

set style fill solid 0.2 noborder

set output 'dataout1.png'
plot 'bench.dat' \
     using 1:2 title 'simulation time'

set output 'dataout2.png'
plot 'bench.dat' \
    using 1:3 title 'speed'

set output 'dataout3.png'
plot 'bench.dat' \
     using 1:4 title 'cpu time'