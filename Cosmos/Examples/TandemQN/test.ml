#use "../testTeamCity.ml";;

test_cosmos_gspn "tqn" ["Overflow",0.1752; "maxVal",5.401088487352618] "--max-run 5000  --HASL-formula 'maxVal=AVG(Max(x1));'";; 

try ignore (exec_cosmos  "tqn.gspn" "tqn.lha" "--gppflags '-O0 -Wno-return-type -std=gnu++11 -g --coverage' --output-trace toto.dat 0.0 --max-run 1 --gracefull-ending" true)
with Sys_error(_) -> ();;
                
