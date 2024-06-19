#use "../testTeamCity.ml";;


try ignore (exec_cosmos  "tandem_agr.gspn" "tandem_agr.lha" "-s --gppflags '-O0 -Wno-return-type -std=gnu++11 -g --coverage' --gracefull-ending" true)
with Sys_error(_) -> ();;

test_cosmos_gspn "tandem" ["",9.95012414790993e-16] "-b 1 --set-Horizon 100 --max-run 5000 --batch 5000";;
test_cosmos_gspn "tandem" ["",9.95012414790993e-16] "-b 2 --set-Horizon 100 --max-run 5000 --batch 5000";;
test_cosmos_gspn "tandem" ["",9.95012414790993e-16] "-b 3 --set-Horizon 100 --max-run 5000 --batch 5000";;
(* Old value : 3.687633e-14 *)



test_cosmos_gspn "tandem" ["",2.210017e-52] "-b 1 --set-Horizon 10  --max-run 5000 --batch 5000 -c";;
test_cosmos_gspn "tandem"  ["",3.99e-23] "-b 1 --set-Horizon 50  --max-run 500 --batch 500 -c";;

