#directory "../utils";;
#directory "../../utils";;
#use "mlcall.ml"

let nbatch = 2000;;
let coption = "-v 0 --njob 1 --batch 100 --use-van-der-corput --width 0 ";; (*--use-van-der-corput *)


let log = (Unix.getcwd ())^"/cosmos_test_log";;
init_log log;;

for i =1 to 400 do
  test_coverage "pi2" ((float i)/. 1000.) ["Overflow",0.78539816339] (coption^"--max-run  10000") nbatch;
done;;


(*
test_coverage "pi2" 0.2 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.3 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.4 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;


test_coverage "pi2" 0.01 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.02 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.03 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.04 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.05 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.06 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.07 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.08 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.09 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.10 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.10 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.99 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.95 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.90 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
test_coverage "pi2" 0.50 ["Overflow",0.78539816339] (coption^"--width 0.01") nbatch;;
 *)


(*
test_coverage "pi2" 0.99 ["Overflow",0.78539816339] (coption^"--max-run  100000") nbatch;;
test_coverage "pi2" 0.95 ["Overflow",0.78539816339] (coption^"--max-run  100000") nbatch;;
test_coverage "pi2" 0.90 ["Overflow",0.78539816339] (coption^"--max-run  100000") nbatch;;
test_coverage "pi2" 0.50 ["Overflow",0.78539816339] (coption^"--max-run  100000") nbatch;;
*)
let (succ,failure) = read_log log in
Printf.printf "Test finished: %i success and %i failure\n" (List.length succ)
  (List.length failure);
if List.length failure >0 then (
  Printf.printf "The following test failed:\n";
  List.iter print_endline failure
)
