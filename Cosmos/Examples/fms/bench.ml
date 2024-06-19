#use "../testTeamCity.ml";;

(*let lt = [ 10 ; 25 ; (*50;*) 100 ; (*200;*) 400; (*800;*) 1600; (*6400;*) 12800; (*25600;*) 102400]*)
let lt = [ 10 ; 50 ; (*50;*) 100 ; (*200;*) 400; (*800;*) 1600; 6400; (*12800;*) 25600; 102400 ]
let lt2 = [ (*10 ;*) 50 ; (*50;*) 100 ; (*200;*) 400; (*800;*) 1600; 6400; (*12800;*) 25600; 102400 ]
let lp = [ "Policy1.grml"; "Policy2.grml" ]
let prop = "rho1.grml"
let commonopt = "--width 0.01 --batch 10 -v 2 --njob 10 "

let mu10 = -0.68305
let sigma10 = 0.83255
let mu15 = -0.11487
let sigma15 = 0.60640
let mu20 = 0.24510
let sigma20 = 0.47238

let iterexpfun l name p prop opt =
  List.iter (fun i ->
    execSavedCosmos ~prefix:false (
      (Printf.sprintf "%s_%s_%i" name p i),p,prop,
      (Printf.sprintf "%s --const 'T=%i'" opt i ))
  ) l

let iterexp = iterexpfun lt
let iterexp2= iterexpfun lt2

(*
let _ = List.iter (fun p ->
  iterexp "Table1" p "rho1.grml" (commonopt^" --const 'l1=3,l2=3,failrate=0'") )
  lp

let _ = List.iter (fun p ->
  iterexp "Table2" p "rho2.grml" (commonopt^" --const 'l1=3,l2=3,failrate=0'") )
  lp

(* phi2(sym) *)
let _ = List.iter (fun p ->
  iterexp "Table3phi2Sym" p "rho2.grml" (commonopt^(Printf.sprintf " --const 'l1=3,l2=3,failrate=0,mu1=%f,sigma1=%f,mu2=%f,sigma2=%f'" mu15 sigma15 mu15 sigma15) )) lp

(* phi2(Asym) S2 *)
let _ =
  iterexp "Table3phi2ASym" "Policy2.grml" "rho2.grml" (commonopt^(Printf.sprintf " --const 'l1=3,l2=3,failrate=0,mu1=%f,sigma1=%f,mu2=%f,sigma2=%f'" mu10 sigma10 mu20 sigma20))

(* phi2(Asym) S1 *)
let _ =
  iterexp "Table3phi2ASym3_5" "Policy1.grml" "rho2.grml" (commonopt^(Printf.sprintf " --const 'l1=3,l2=5,failrate=0,mu1=%f,sigma1=%f,mu2=%f,sigma2=%f'" mu10 sigma10 mu20 sigma20))

(* phi2(Asym) S1 *)
let _ =
  iterexp "Table3phi2ASym3_4" "Policy1.grml" "rho2.grml" (commonopt^(Printf.sprintf " --const 'l1=3,l2=4,failrate=0,mu1=%f,sigma1=%f,mu2=%f,sigma2=%f'" mu10 sigma10 mu20 sigma20))

(* phi2(Asym) S1 *)
let _ =
  iterexp "Table3phi2ASym3_3" "Policy1.grml" "rho2.grml" (commonopt^(Printf.sprintf " --const 'l1=3,l2=3,failrate=0,mu1=%f,sigma1=%f,mu2=%f,sigma2=%f'" mu10 sigma10 mu20 sigma20))

(* phi2(Asym) S1 *)
let _ =
  iterexp "Table3phi2ASym2_3" "Policy1.grml" "rho2.grml" (commonopt^(Printf.sprintf " --const 'l1=2,l2=3,failrate=0,mu1=%f,sigma1=%f,mu2=%f,sigma2=%f'" mu10 sigma10 mu20 sigma20))

(* phi2(Asym) S1 *)
let _ =
  iterexp "Table3phi2ASym1_3" "Policy1.grml" "rho2.grml" (commonopt^(Printf.sprintf " --const 'l1=1,l2=3,failrate=0,mu1=%f,sigma1=%f,mu2=%f,sigma2=%f'" mu10 sigma10 mu20 sigma20))

(* phi2(Asym) S1 *)
let _ =
  iterexp "Table3phi2ASym1_4" "Policy1.grml" "rho2.grml" (commonopt^(Printf.sprintf " --const 'l1=1,l2=4,failrate=0,mu1=%f,sigma1=%f,mu2=%f,sigma2=%f'" mu10 sigma10 mu20 sigma20))

(* phi2(Asym) S1 *)
let _ =
  iterexp "Table3phi2ASym1_5" "Policy1.grml" "rho2.grml" (commonopt^(Printf.sprintf " --const 'l1=1,l2=5,failrate=0,mu1=%f,sigma1=%f,mu2=%f,sigma2=%f'" mu10 sigma10 mu20 sigma20))
*)

let _ = List.iter (fun p ->
  iterexp "Table4_fault_" p "rho2.grml" (commonopt^" --const 'l1=3,l2=3,failrate=0.016' --width 0.1") )
  lp


let _ = List.iter (fun p ->
  iterexp2 "Table5_nofault_" p "rho3.grml" (commonopt^" --const 'l1=3,l2=3,failrate=0.0' ") )
  lp

let _ = List.iter (fun p ->
  iterexp2 "Table5_fault_" p "rho3.grml" (commonopt^" --const 'l1=3,l2=3,failrate=0.016' ") )
  lp

