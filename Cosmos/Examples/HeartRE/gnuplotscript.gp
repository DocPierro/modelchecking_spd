set terminal svg size 10000,200

set style line 1 lt 1 lw 1 lc rgb '#FFA02F'
set style line 2 lt 4 lw 1 lc rgb '#829600'
set style line 3 lt 3 lw 1 lc rgb '#E05206'
set style line 4 lt 3 lw 1 lc rgb '#5052F8'
set style line 5 lt 3 lw 1 lc rgb '#A032F8'
set style line 6 lt 3 lw 1 lc rgb '#003208'


set grid lt 4 lc rgb '#909090'
set style fill solid 0.2 noborder
set output 'test.dat.svg'
plot  'tmp/tmpdatafilecomb.dat' \
       using 1:($3*0.5) title  columnheader(3) with lines ls 1\
   ,'' using 1:(-$4*0.5) title  columnheader(4) with lines ls 2\
   ,'' using 1:($5) title  columnheader(5) with lines ls 3\
   ,'' using 1:(-$6) title  columnheader(6) with lines ls 4

set output
exit
