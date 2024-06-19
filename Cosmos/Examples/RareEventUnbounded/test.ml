#use "../testTeamCity.ml";;

test_cosmos_grml "tandem" ["HASLFormula",0.0089] "--max-run 20000";;

test_cosmos "formula" "tandem.grml" "" "--formula \"(Queue1+Queue2>0) U (Queue1+ Queue2 = 5)\" --max-run 20000 " ["Probability",0.0089];;

test_cosmos "formula" "tandem.grml" "" " --formula \"(Queue1+Queue2>0) U[3;10] (Queue1+ Queue2 = 5)\" --max-run 20000 " ["Probability",0.00372183348544772] ;;

test_cosmos "formula" "tandem.grml" "" "--formula \"F=10 (Queue1+ Queue2 = 5)\" --max-run 20000 " ["Probability",0.00176838079048024];;

(*
0.063742;;
*)
