Cosmos idhp_model_heart_realtime_syn.slx PMTDetection3.grml --trace-place "SIG_VgetOut,SIG_VPOut,SIG_AgetOut,SIG_APOut,EMPTY_AP,EMPTY_AS,EMPTY_VP,EMPTY_VS,countEp,t2" --output-trace test.dat 0.0 --gnuplot-driver svg  --tmp-status 2 --gppflags -O0 --batch 1 --max-run 1 --const "PM_TPVARP=195,SA_ectopD=999999999,VRG_ectoD=9999999999,PM_TURI=350,T=3600000" $1
 

gnuplot gnuplotscript.gp