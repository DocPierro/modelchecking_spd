set terminal svg size 1200, 300

set grid lt 4 lc rgb '#909090'
set style fill solid 0.2 noborder
set output 'trace.data.svg'
plot 'trace.data' using 1:2 title  columnheader(2) with lines, \
     'trace.data' using 1:($3-1.5) title  columnheader(3) with lines, \
     'trace.data' using 1:4 title  columnheader(4) with lines, \
     'trace.data' using 1:12 title  columnheader(12) with lines, \
     'trace.data' using 1:($13+1.5) title  columnheader(13) with lines, \
     'trace.data' using 1:($14+3) title  columnheader(14) with lines, \
     'trace.data' using 1:15 title  columnheader(15) with lines

set output
exit
