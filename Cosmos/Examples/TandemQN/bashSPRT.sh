#SPRT
/Applications/prism-4.0.2-osx64/bin/prism tandem.sm -const c=5 tandem.csl -sim -simwidth 0.001 -simconf 0.005

~/Downloads/ymer-3.0.9/ymer tandem.sm tandem.csl --const c=5 --delta 0.001 -A 0.005 -B 0.005 

Cosmos tqn.gspn tqnSPRT.lha --level 0.005

#Confint

Cosmos tqn.gspn tqn.lha --max-run 66000 --const T=200
marcie --net-file tqn.andl --ltl-file tqn.ltl