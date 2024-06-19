set terminal png size 800,800
#set terminal tkcanvas

set style line 1 lt 1 lw 4 lc rgb '#A0A0A0'

set style line 1 lt 1 lw 7 lc rgb '#FFA02F'
set style line 2 lt 1 lw 7 lc rgb '#829600'
set style line 3 lt 1 lw 7 lc rgb '#E05206'
set style line 4 lt 1 lw 7 lc rgb '#BCC775'
set style line 5 lt 3 lw 7 lc rgb '#FFA02F'

set output "test.png"

set pointsize 0.3 

set tic font ",22"

set xtic 1
set ytic 1

plot 'data4NoConv.txt' using  2:8 notitle with points pt 7 lc rgb '#FFA02F'
#splot 'data2NoConv' using 3:6:9 notitle with points pt 7 lc rgb '#FFA02F'
