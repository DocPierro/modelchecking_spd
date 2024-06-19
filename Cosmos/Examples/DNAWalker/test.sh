#!/bin/sh

Cosmos track18Block1.grml --sampling 5000 10.0 --trace-place "a1,a2,a3,a4,a9,a10,a11,a12,a8" --output-PDFCDF test.dat --njob 8 --max-run 100000

Cosmos track28.grml --formula "(a20=0 & a25=0 & a28=0)  U<12000 a17>0" --njob 8 --level 0.95 --gppcmd clang++ --max-run 100000000

Cosmos lozange.grml --sampling 120000 1000 --trace-place "a100" --output-graph "finishloz.dat" --gnuplot-driver png --gppflags -O0 --max-run 100000 --njob 2


Cosmos lozange.grml lozange.lha --gppflags -O0 --max-run 100000 --const maxtime=120000 --njob 2 --tmp-status 2 --HASL-formula "Deadlock=CDF(Last(deadlocktime),1000,0,120000);Finish=CDF(Last(timefinishcorrect),1000,0,120000);" --output-graph "cdfdata.dat" --gnuplot-driver png 

Cosmos lozangeBlock.grml lozangeBlock.lha --gppflags -O0 --max-run 100000 --const maxtime=120000 --njob 2 --tmp-status 2 --HASL-formula "Deadlock=CDF(Last(deadlocktime),1000,0,120000);Finish=CDF(Last(timefinishcorrect),1000,0,120000);" --output-graph "cdfdataBlock.dat" --gnuplot-driver png 
