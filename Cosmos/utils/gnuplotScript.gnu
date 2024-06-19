set terminal pngcairo font "arial,10" fontscale 1.0 size 500, 200 
set output 'dataout.png'

set grid lc rgb "black"

set style fill solid 0.2 noborder

plot for [i=1:5] 'dataout.dat' \
      using 1:4*i+1:4*i+2 w filledcu ls i title columnheader(4*i), \
     for [i=1:5] '' using 1:4*i+1 notitle with lines lw 1 lc rgb "black", \
     for [i=1:5] '' using 1:4*i+2 notitle with lines lw 1 lc rgb "black", \
     for [i=1:5] '' using 1:4*i-1 title columnheader(4*i-1) w lines ls i lw 2