#directory "../utils";;
#directory "../../utils";;
#use "mlcall.ml"

let nbatch = 500;;
let coption = "-v 0 --njob 1 --batch 100 --width 0 --max-run 10000 --use-van-der-corput"

let oracle = [| 1.00000000000000;
 1.00000000000000;
 0.833333333333333;
 0.677777777777778;
 0.549603174603175;
 0.445511463844797;
 0.361119795842018;
 0.292712844181098;
 0.237264109408620;
 0.192319044532528;
 0.155887945951208;
 0.126358009605531;
 0.102421944768345;
 0.0830201013991180;
 0.0672935595189322;
 0.0545461048157152;
 0.0442134072240584;
 0.0358380380224183;
 0.0290492194548042;
 0.0235464103923743;
 0.0190860013718688;
 0.0154705299999763;
 0.0125399392893751;
 0.0101644919328204;
 0.00823902683005082;
 0.00667830360385381;
 0.00541322900692287;
 0.00438779816246769;
 0.00355661522723921;
 0.00288288371667391;
 0.00233677752381298;
 0.00189412051697234;
 0.00153531626192699;
 0.00124448048739018;
 0.00100873788801733;
 0.000817652134390304;
 0.000662763856512875;
 0.000537216147337960;
 0.000435451007360469;
 0.000352963291872089;
 0.000286101268118208;
 0.000231904953018485;
 0.000187975074658829;
 0.000152366856477515;
 0.000123503921969312;
 0.000100108508467213;
 0.0000811449005645362;
 0.0000657735789739089;
 0.0000533140549922338;
 0.0000432147452526865;
 0.0000350285531184344 |]
  
let log = (Unix.getcwd ())^"/cosmos_test_log";;
init_log log;;


  for k = 5 to 50 do
    for i = 90 to 90 do 
      test_coverage_full "ThickTwinMC" "thick_twin.prism.grml" "prop1.lha" ((float i)/.100.0) ["Overflow", oracle.(k) ] (Printf.sprintf "%s --const 'bo=%i' --set-Horizon %i" coption (2*k) (2*k+1)) nbatch;
    done;
  done;;

let (succ,failure) = read_log log in
Printf.printf "Test finished: %i success and %i failure\n" (List.length succ)
  (List.length failure);
if List.length failure >0 then (
  Printf.printf "The following test failed:\n";
  List.iter print_endline failure
)