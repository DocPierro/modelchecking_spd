set terminal pngcairo font "arial,10" fontscale 1.0 size 2000, 600 
set output 'dataout.png'

set grid lc rgb "black"
set style fill solid 0.2 noborder

#set logscale y

plot 'logtraj2' \
      using 1:2 title columnheader(2) w lines ls 1 lw 2, \
      '' using 1:(10000 * $3) title columnheader(3) w lines ls 2 lw 2, \
      '' using 1:4 title columnheader(4) w lines ls 3 lw 2, \
      '' using 1:(100 * $5) title columnheader(5) w lines ls 4 lw 2