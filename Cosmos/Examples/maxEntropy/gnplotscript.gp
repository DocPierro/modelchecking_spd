set terminal png size 1000,1000

set style line 1 lt 1 lw 4 lc rgb '#A0A0A0'

set style line 1 lt 1 lw 7 lc rgb '#FFA02F'
set style line 2 lt 1 lw 7 lc rgb '#829600'
set style line 3 lt 1 lw 7 lc rgb '#E05206'
set style line 4 lt 1 lw 7 lc rgb '#BCC775'
set style line 5 lt 3 lw 7 lc rgb '#FFA02F'

set output ARG1

set pointsize 0.1

set tic font ",22"

set xtic 1
set ytic 1

plot 'preplota.dat' using 2:3 notitle with points pt 7 lc rgb '#FFA02F',\
     'preplotb.dat' using 2:3 notitle with points pt 7 lc rgb '#829600',\
     'preplotc.dat' using 2:3 notitle with points pt 7 lc rgb '#800080'
