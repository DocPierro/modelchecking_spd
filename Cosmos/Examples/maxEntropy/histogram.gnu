set terminal png
set output ARG1

set xrange [-0.5 :*]

binwidth = 1
binstart = -0.5
load 'hist.fct'
plot 'histdata.raw' i 0 @hist ls 1,\
      '' i 0 @hist2 ls 2
