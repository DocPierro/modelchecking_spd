set terminal pngcairo font "arial,10" fontscale 1.0 size 1000, 500 
set output 'dataout.png'

set grid lc rgb "black"

set style fill solid 0.2 noborder


plot 'dataout.dat' \
     using 1:2:4 w filledcu ls 1 notitle, \
     '' using 1:3 notitle w lines ls 1 lw 2
     