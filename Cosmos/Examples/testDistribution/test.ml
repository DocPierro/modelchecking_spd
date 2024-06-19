#use "../testTeamCity.ml";;

raw_cosmos "testdeterministic.grml -v 4 --tmp-status only-gen | head -n 50 | grep -v Binary > testout" true;;
if (Sys.command "diff testout defaultOutpoutv4") <> 0
then update_log false "testOutput"
else update_log true "testOutput" ;;

test_cosmos "TestWeight" "TestWeight.grml" "" "--loop 2.1 --max-run 10000" ["Throughput_D0", 0.043395909; "Throughput_D2", 0.43279409] ;;

test_cosmos "TestWeight2" "TestWeight2.gspn" "" "--loop '#19' --max-run 10000" ["Throughput_t1", 17.27; "Throughput_t0", 1.727] ;;

test_cosmos "lognormal" "testlognormal.grml" "testdistrib.lha" ""
  ["MeanVal", 2.636323945294441 ; "VarVal", 4.626008723827832];;

test_cosmos "geometric" "testgeometric.grml" "testdistrib.lha" ""
  ["MeanVal",(0.7 /. 0.98) ; "VarVal",(0.7 *. 0.7 *. 0.02 /. (0.98 *. 0.98))];;

test_cosmos "uniform" "testuniform.grml" "testdistrib.lha" ""
  ["MeanVal",(1.0)];;

test_cosmos "exponential" "testexponential.grml" "testdistrib.lha" ""
  ["MeanVal",(0.5); "VarVal",(1. /. 4.)];;

test_cosmos "deterministic" "testdeterministic.grml" "testdistrib.lha" ""
  ["MeanVal",(2.0)];;

test_cosmos "triangle" "testtriangle.grml" "testdistrib.lha" ""
  ["MeanVal",(8.0 /. 3.)];;
(*;"VarVal", (1.0+.9.0+.16.-. 3.0 -. 4.0 -. 12.0)/.18.0 *)

test_cosmos "erlang" "testerlang.grml" "testdistrib.lha" ""
  ["MeanVal",(5.0/.0.7)];;

test_cosmos "Weibull" "testweibull.grml" "testdistrib.lha" ""
            ["MeanVal", 4.0 (* lambda * gamma (1+1/k) = 2*gamma(1+2)*);
             "VarVal",  80.0 (* lambda^2 * gamma(1+2/k) - E^2 = 4*gamma(5)-4^2 *)];;

test_cosmos "Pareto" "testpareto.grml" "testdistrib.lha" ""
            ["MeanVal", 0.75 (* k*x_m/(k-1) = 3/2 * 0.5 *);
             "VarVal",  3.0 /. 16.0 (* k*x_m^2 / (k-1)^2(k-1) = 3*0.5^2/2^2 *)];;

test_cosmos "gamma" "testgamma.grml" "testdistrib.lha" ""
  ["MeanVal",(5.0*.0.7)];;

test_cosmos "discrete uniform" "testdiscreteuniform.grml" "testdistrib.lha" ""
  ["MeanVal",(8.0)];;

test_cosmos "age memory" "testagememory.grml" "testagememory.lha" "--max-run 10000"
  ["MeanVal",(12.0)];;

test_cosmos "normal" "testnormal.grml" "testdistrib.lha" "--max-run 10000"
["MeanVal",(10.0); "VarVal",(2.0*.2.0)];;

test_cosmos "Userdefined" "testuserdefine.grml" "testdistrib.lha" "--max-run 1000"
["MeanVal",(4.0); "VarVal",(5.33321869776)];;

test_cosmos "Userdefined" "testuserdefinepoly.grml" "testdistrib.lha" "--max-run 1000"
["MeanVal",(4.0); "VarVal",(5.33321869776)];;

test_cosmos "TokenInitialGuard"  "tokenInitGuard.grml" "testdistrib.lha" "--max-run 1000"
["MeanVal",(1.0); "VarVal",(1.0)];;
