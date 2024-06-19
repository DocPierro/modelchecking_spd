Cosmos idhp_model_heart_realtime_syn.slx --loop 600000 --trace-place "SIG_VgetOut,SIG_VPOut,SIG_AgetOut,SIG_APOut,EMPTY_AP,EMPTY_AS,EMPTY_VP,EMPTY_VS" --output-trace test.dat 0.0 --gnuplot-driver svg  --tmp-status 2 --gppflags -O0 --batch 1 --max-run 1 $1

gnuplot gnuplotscript.gp