set terminal tikz size 8,5

load "gnuplot_prelem.gnu"

set xlabel 'Time'
set ylabel 'Probability of Presence'
set xtics 4000
set yrange [0.0 : 1.2]
set grid lt 4 lc rgb '#909090'
set style fill solid 0.2 noborder
set output 'track28RR.tex'
plot 'track28RR.dat' using 1:($3-1) title 'A17' with lines ls 1,\
      '' using 1:($6-1) title 'A20' with lines ls 2,\
       '' using 1:($9-1) title 'A25' with lines ls 3,\
       '' using 1:($12-1) title 'A28' with lines ls 4	

set output 'track28RL.tex'

plot 'track28RL.dat' using 1:($3-1) title 'A17' with lines ls 1,\
      '' using 1:($6-1) title 'A20' with lines ls 2,\
       '' using 1:($9-1) title 'A25' with lines ls 3,\
       '' using 1:($12-1) title 'A28' with lines ls 4	

set output 'track28LR.tex'

plot 'track28LR.dat' using 1:($3-1) title 'A17' with lines ls 1,\
      '' using 1:($6-1) title 'A20' with lines ls 2,\
       '' using 1:($9-1) title 'A25' with lines ls 3,\
       '' using 1:($12-1) title 'A28' with lines ls 4	

set output 'track28LL.tex'

plot 'track28LL.dat' using 1:($3-1) title 'A17' with lines ls 1,\
      '' using 1:($6-1) title 'A20' with lines ls 2,\
       '' using 1:($9-1) title 'A25' with lines ls 3,\
       '' using 1:($12-1) title 'A28' with lines ls 4	


set output

exit
