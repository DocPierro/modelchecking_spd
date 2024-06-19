#use "tandem.ml";;

assert (Array.length Sys.argv>3);;
let n = int_of_string Sys.argv.(1);;
let n2 = int_of_string Sys.argv.(2);;
let r = int_of_string Sys.argv.(3);;
assert(n>=1);;
let lambda = float_of_string Sys.argv.(4);;
let rho = (1. -. lambda) /. (float n);;
let batch = int_of_string Sys.argv.(5);;

generate n n2 r lambda (function | _ -> rho);;
execSavedCosmos ~prefix:false ("StateIndepIS","tandemDTMC.grml","tandemIS.lha",Printf.sprintf " --width 0 --max-run %i --level %f" batch 0.95 );;
