Cosmos idhp_model_heart_realtime_syn.slx --loop 50000 --trace-place "SIG_VgetOut,SIG_VPOut,SIG_AgetOut,SIG_APOut,EMPTY_AP,EMPTY_AS,EMPTY_VP,EMPTY_VS" --output-trace test.dat 0.0 --output-dot dotgraph.pdf --gnuplot-driver svg --tmp-status 2 --gppflags -O0 --batch 1 --max-run 1 --seed 1 $1

gnuplot gnuplotscript.gp