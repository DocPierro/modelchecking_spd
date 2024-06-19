#directory "../utils";;
#directory "../../utils";;
#use "mlcall.ml"

let nbatch = 100;;
let coption = "-v 0 --njob 1 --batch 100 ";; (*--use-van-der-corput *)


let log = (Unix.getcwd ())^"/cosmos_test_log";;
init_log log;;

test_coverage "tqn" "tqn" 0.99 ["Overflow",0.1752] (coption^"--width 0.01") nbatch;;
test_coverage "tqn" "tqn" 0.95 ["Overflow",0.1752] (coption^"--width 0.01") nbatch;;
test_coverage "tqn" "tqn" 0.90 ["Overflow",0.1752] (coption^"--width 0.01") nbatch;;
test_coverage "tqn" "tqn" 0.50 ["Overflow",0.1752] (coption^"--width 0.01") nbatch;;

test_coverage "tqn" "tqn" 0.99 ["Overflow",0.1752] (coption^"--max-run  100000") nbatch;;
test_coverage "tqn" "tqn" 0.95 ["Overflow",0.1752] (coption^"--max-run  100000") nbatch;;
test_coverage "tqn" "tqn" 0.90 ["Overflow",0.1752] (coption^"--max-run  100000") nbatch;;
test_coverage "tqn" "tqn" 0.50 ["Overflow",0.1752] (coption^"--max-run  100000") nbatch;;


let (succ,failure) = read_log log in
Printf.printf "Test finished: %i success and %i failure\n" (List.length succ)
  (List.length failure);
if List.length failure >0 then (
  Printf.printf "The following test failed:\n";
  List.iter print_endline failure
)
