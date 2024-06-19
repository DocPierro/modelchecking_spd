#directory "../../utils"
(*#mod_use "../../modelConvert/type.ml"
#mod_use "../../modelConvert/PetriNet.ml"
#mod_use "../../modelConvert/StochasticPetriNet.ml"
#mod_use "../../modelConvert/StochPTPrinter.ml"*)

#directory "../../modelConvert/_build"
#directory "../../modelConvert/Prism"
#directory "../../modelConvert/_build/Prism"

#load "type.cmo"
#load "prismType.cmo"
#load "parserPrism.cmo"
#load "lexerPrism.cmo"
#load "PetriNet.cmo"
#load "StochasticPetriNet.cmo"
#load "StochPTPrinter.cmo"


open Type
open PetriNet 
open StochasticPetriNet

(*#use "StochasticPetriNet.ml"*)
#use "mlcall.ml";;

type anchorT = Init | Final | Norm

let sq2 = (sqrt 2.0)/.2.0;;
let sq3 = (sqrt 3.0)/.2.0;;


let mapsq3 l =
  List.map (fun (x,y,z,(p1,p2)) -> (x,y,z,(p1*.sq3,p2*.sq3))) l

let foldi f c t =
  let acc = ref c in
  for i = 0 to Array.length t-1 do
    acc:=f !acc i t.(i);
  done;
  !acc;;

let dist (x1,y1) (x2,y2) =
  sqrt ((x1 -.x2)*.(x1 -.x2) +. (y1 -.y2)*.(y1 -.y2))


let generate_lha fpath li obj =
  let safe = ref " TRUE "
  and blockade = ref " FALSE " in
  List.iter (fun (n,t,i,_) -> 
    if t= Final then safe := Printf.sprintf " %s & (A%i<2)" !safe n
    else if i=0 && t<> Init then blockade := Printf.sprintf " %s | (A%i=2)" !blockade n
  ) li;
  let f = open_out fpath in
(*\%corrBlock=AVG(Last(corrblock))/AVG(Last(useblocked));*)
  Printf.fprintf f "
const double maxtime=12000;
VariablesList = { vc0, vc1, DISC vd0 , DISC vd1, DISC vd2, DISC vd3, DISC timefinishcorrect, DISC timefinish, DISC useblocked, DISC corrblock, DISC lasttranstime, DISC deadlocktime};
LocationsList = {lii, li, li2, lfc,lf,lnf1,ldl,lnf2};
FinishCorrect=AVG(Last(vd0));
Finish=AVG(Last(vd1));
DeadLock=AVG(Last(vd2));
Step=AVG(Last(vd3));
Blockade=AVG(Last(vc1));
%%DeadLockTime=CDF(Last(deadlocktime),100.0,0.0,maxtime);
%%FinishTime=CDF(Last(timefinishcorrect),100.0,0.0,maxtime);
uB=AVG(Last(corrblock))/AVG(Last(vd0));
InitialLocations = { lii };
FinalLocations = {lfc,lf,ldl,lnf2};
Locations = {
(lii,TRUE,(vc0:1.));
(li,((%s) & !(%s)),(vc0:1.));
(li2,((%s) & (%s)),(vc0:1., vc1:1.));
(lfc,(%s));
(lf,( !(%s) & !(%s) ));
(lnf1,TRUE,(vc0:1.));
(ldl,TRUE);
(lnf2,TRUE);
};
Edges = {
((lii,lii),ALL,vc0=0,#);
((lii,li),ALL,vc0>=0.000000000001,{vd3=vd3+1,timefinish=maxtime,timefinishcorrect=maxtime,deadlocktime=maxtime});
((lii,li2),ALL,vc0>=0.000000000001,{vd3=vd3+1,timefinish=maxtime,timefinishcorrect=maxtime,deadlocktime=maxtime,useblocked=1});
((li,li),ALL,#,{lasttranstime=vc0,vd3=vd3+1});
((li2,li2),ALL,#,{lasttranstime=vc0,vd3=vd3+1});
((li,li2),ALL,#,{lasttranstime=vc0,vd3=vd3+1,useblocked=1});
((li2,li),ALL,#,{lasttranstime=vc0,vd3=vd3+1});
((li,lfc),ALL,#, {vd0=1,vd1=1,vd3=vd3+1, timefinishcorrect=vc0,corrblock=useblocked });
((li2,lfc),ALL,#,{vd0=1,vd1=1,vd3=vd3+1, timefinishcorrect=vc0,corrblock=useblocked });
((li,lf),ALL,#,{vd1=1,vd3=vd3+1, timefinish=vc0});
((li2,lf),ALL,#,{vd1=1,vd3=vd3+1, timefinish=vc0});
((li,lnf1),#,vc0=maxtime,#);
((li2,lnf1),#,vc0=maxtime,#);
((lii,lfc),ALL,#,{vd0=1,vd1=1,vd3=vd3+1, timefinishcorrect=vc0});
((lii,lf),ALL,#,{vd1=1,vd3=vd3+1, timefinish=vc0});
((lii,lnf1),#,vc0=maxtime,#);
((lnf1,lnf2),ALL,#,#);
((lnf1,ldl),#,vc0=1000000000,{deadlocktime=lasttranstime,vd2=1});
};" !safe !blockade !safe !blockade obj !safe obj ;
  close_out f;;
  

let generate_pctl fpath li obj =
  let safe = ref " true "
  and blockade = ref " false " in
  List.iter (fun (n,t,i,_) -> 
    if t= Final then safe := Printf.sprintf " %s & (A%i<2)" !safe n
    else if i=0 && t<> Init then blockade := Printf.sprintf " %s | (A%i=2)" !blockade n
  ) li;
  let f = open_out fpath in
  Printf.fprintf f "P=? [ (%s)  U[0,12000] (%s) ]\n" !safe obj;
  Printf.fprintf f "R{\"steps\"}=? [ C<=12000 ]\n";
  close_out f

let generate_csl fpath li obj =
  let safe = ref " true "
  and blockade = ref " false " in
  List.iter (fun (n,t,i,_) -> 
    if t= Final then safe := Printf.sprintf " %s & [A%i<2]" !safe n
    else if i=0 && t<> Init then blockade := Printf.sprintf " %s | [A%i=2]" !blockade n
  ) li;
  let f = open_out fpath in
  Printf.fprintf f "P=? [ [%s]  U[0,12000] [%s] ]\n" !safe obj;
  close_out f

let imm_trans imm weight =
  if imm then (Imm, (Float weight),(Float 1.0))
  else let rate = 1000000.0 in 
       (Exp (Float (weight*.rate)),(Float 1.0),(Float 1.0))

let gen_spn2 ?(genimm=true) ?(gentrans=true) ?(genfailure=true) ?(genloop=true)  fpath li ks failure=
  let ksi = ks/.3. in
  print_endline ("Generate2 "^fpath);
  let net = Net.create () in
  List.iter (fun (n,t,i,_) ->
    Data.add (("A"^(string_of_int n)),((if t=Init then Int 2 else Int i),Some (Int 2))) net.Net.place;
    if i=0 && t <> Init && genfailure then begin
      Data.add (("B"^(string_of_int n)), (Int 1,Some (Int 1))) net.Net.place;
      Data.add ("tb"^(string_of_int n) ,(imm_trans genimm failure)) net.Net.transition;
      Data.add ("tAb"^(string_of_int n) ,(imm_trans genimm (1.0-.failure))) net.Net.transition;
      Net.add_arc net ("B"^(string_of_int n)) ("tb"^(string_of_int n)) (Int 1);
      Net.add_arc net ("B"^(string_of_int n)) ("tAb"^(string_of_int n)) (Int 1);
      Net.add_arc net ("tb"^(string_of_int n)) ("A"^(string_of_int n)) (Int 1);

      (* For Marcie *)
      (*Net.add_inhibArc net ("A"^(string_of_int n)) ("tb"^(string_of_int n)) (Int 1);*)
      
    (*cluster.(n-1) <- ("b"^(string_of_int n)) 
      :: ("tb"^(string_of_int n))
      :: ("tAb"^(string_of_int n))
      :: cluster.(n-1);*) 
    end
  ) li;
  if gentrans then begin
  List.iter (fun (n1,t1,i1,p1) ->
    List.iter (fun (n2,t2,i2,p2) -> 
      let r = (match t1 with 
	  Init -> ksi
	| Final -> 0.0
	| _ -> ks) in
      let r2 = (match dist p1 p2 with
	| x when x< 0.1 -> 0.0
	| x when x< 1.5 -> r
	| x when x< 2.5 -> r/.50.0
	| x when x< 3.87 -> r/.100.0
	| _->0.0 ) in
      let r3 = (match t2 with 
	  Final -> r2 /. 10.
	| _ -> r2) in
      if r3 <> 0.0 then
	let tl = Printf.sprintf "t%i_%i" n1 n2 in
	Data.add (tl,(Exp (Float r3),Float 1.0,Float 1.0)) net.Net.transition;
	Net.add_arc net ("A"^(string_of_int n1)) tl (Int 2);
	Net.add_arc net ("A"^(string_of_int n2)) tl (Int 1);
	Net.add_arc net tl ("A"^(string_of_int n2)) (Int 2);

	(* For Marcie *)
	(*Net.add_inhibArc net ("A"^(string_of_int n2)) tl (Int 2);*)

	(*cluster.(n1-1) <- tl :: cluster.(n1-1);*) 
      ) li ) li; 
  end;

  if genloop then 
    List.iter (fun (n1,t1,i1,p1) ->
      if t1 = Final then (
	let tl = Printf.sprintf "tloop%i" n1 in 
	Data.add (tl,(Exp (Float 0.000000001),Float 1.0,Float 1.0)) net.Net.transition;
	Net.add_arc net ("A"^(string_of_int n1)) tl (Int 2);
	Net.add_arc net tl ("A"^(string_of_int n1)) (Int 2);)
    ) li;  
  net;;
  
let dist = 0.675 ;;
let dist2 = 1.15;;

let generate_spn fpath li2 ks failure obj =
  let li = mapsq3 li2 in
  let net = gen_spn2 ~gentrans:true ~genloop:true ~genfailure:true ~genimm:true fpath li ks failure in
  StochPTPrinter.print_spt (fpath^".grml") net;
  StochPTPrinter.print_pnml (fpath^".pnml") net;
  generate_lha (fpath^".lha") li obj;
  StochPTPrinter.print_spt_marcie (fpath^".andl") net;
  generate_csl (fpath^".csl") li obj;
  (*print_prism_module (fpath^".sm") net;*)
  generate_pctl (fpath^".pctl") li obj;
  StochPTPrinter.print_spt_dot ~showlabel:true (fpath^".dot") net []
        (List.fold_left (fun q (n,_,_,(px1,py1)) ->
	  let px,py = (px1*.dist2,py1*.dist2) in
	  (("A"^(string_of_int n)),(px,py))::
	    (("tloop"^(string_of_int n)),(px,py+.(copysign 1.0 py)))::
	    (("tb"^(string_of_int n)),(px,py-.(copysign dist py)))::
	    (("tAb"^(string_of_int n)),(px,py-.(copysign (3.*.dist) py)))::
	    (("B"^(string_of_int n)),(px,py-.(copysign (2.0*.dist) py)))::q) [] li);
 (*execSavedCosmos ~prefix:false (fpath,fpath^".grml",fpath^".lha"," --njob 2 --max-run 20000 --batch 1000 --width 0");;*)
(*  ignore (Sys.command (Printf.sprintf "prism %s.sm %s.pctl --sim --simsamples 2000000" fpath fpath));;*)
 (* ignore (Sys.command (Printf.sprintf "/usr/bin/time -l prism %s.sm %s.pctl" fpath fpath));;*)
 (* ignore (Sys.command (Printf.sprintf "/usr/bin/time -v prism %s.sm %s.pctl -transientmethod fau -faudelta 1E-10 -fauepsilon 1E-8" fpath fpath));;*)
 (* ignore (Sys.command (Printf.sprintf "./marcie --net-file %s.andl" (* --csl-file %s.csl --approximative fpath*) fpath));;*)
  ignore (Sys.command (Printf.sprintf "dot -Kfdp -Tpdf %s.dot -o %s.pdf" fpath fpath));;
(*  execSavedCosmos ~prefix:false (fpath,fpath^".grml",fpath^".lha"," --njob 2");;*)

let gen28 f l1 r1 l2 r2 obj =
generate_spn f [ 
  (1,Init,0,(0.0,0.0)); 
  (2,Norm,1,(0.0,-1.0)); 
  (3,Norm,1,(0.0,-2.0));
  (4,Norm,1,(0.0,-3.0));
  (5,Norm,l1,(-.1.0,-.3.5));
  (6,Norm,l1,(-.2.0,-.4.0)); 
  (7,Norm,1 ,(-.3.0,-.4.5));
  (8,Norm,1 ,(-.4.0,-.5.0));
  (9,Norm,r1,(  1.0,-.3.5));
  (10,Norm,r1,( 2.0,-.4.0));
  (11,Norm,1 ,( 3.0,-.4.5));
  (12,Norm,1 ,( 4.0,-.5.0));
  
  (13,Norm,1 ,(-.4.0,-6.0));
  (14,Norm,1 ,(-.4.0,-7.0));
  (15,Norm,l2,(-.5.0,-.7.5));
  (16,Norm,l2,(-.6.0,-.8.0)); 
  (17,Final,1,(-.7.0,-.8.5));
  (18,Norm,r2,(-.3.0,-.7.5));
  (19,Norm,r2,(-.2.0,-.8.0));
  (20,Final,1,(-.1.0,-.8.5));

  (21,Norm,1 ,(4.0,-6.0));
  (22,Norm,1 ,(4.0,-7.0));
  (23,Norm,l2,(3.0,-.7.5));
  (24,Norm,l2,(2.0,-.8.0)); 
  (25,Final,1,(1.0,-.8.5));
  (26,Norm,r2,(5.0,-.7.5));
  (27,Norm,r2,(6.0,-.8.0));
  (28,Final,1,(7.0,-.8.5));

] 0.009 0.3 obj ;; 


let gen_xor f x y =
  let obj = if x <> y then 
    (*generate_lha (f^".lha") "a7=0 & a17=0" "a17>0" "a7>0"*)
    "A17=2"
  else  
    (*generate_lha (f^".lha") "a7=0 & a17=0" "a7>0" "a17>0";*)
      "A7=2" in
  let  xi = if x then 1 else 0
  and nxi = if x then 0 else 1
  and  yi = if y then 1 else 0
  and nyi = if y then 0 else 1 in
  generate_spn f (
    [ (1 ,Init,0  ,(   0.0,  0.0)); 
      (2 ,Norm,nxi ,(-.1.0,  0.5)); 
      (3 ,Norm,nxi ,(-.2.0,  1.0));
      (4 ,Norm,nyi ,(-.2.0,  2.0));
      (5 ,Norm,nyi ,(-.1.0,  2.5));
      (6 ,Norm,1   ,(  0.0,  3.0)); 
      (7 ,Final,1  ,(  1.0,  2.5));
      (8 ,Norm,1   ,(  2.0,  2.0));
      (9 ,Norm,1   ,(  3.0,  1.5));
      (10,Norm,yi  ,(  3.0,  0.5));
      (11,Norm,yi  ,(  3.0,-.0.5));
      (12,Norm,xi  ,(  2.0,-.1.0));
      (13,Norm,xi  ,(  1.0,-.0.5));
      (14,Norm,nyi ,(  2.0,-.2.0));
      (15,Norm,nyi ,(  1.0,-.2.5));
      (16,Norm,1   ,(  0.0,-.3.0));
      (17,Final,1  ,(-.1.0,-.2.5));
      (18,Norm,1   ,(-.2.0,-.2.0));
      (19,Norm,1   ,(-.3.0,-.1.5));
      (20,Norm,yi  ,(-.3.0,-.0.5));
      (21,Norm,yi  ,(-.3.0,  0.5));
]) 0.009 0.3 obj;;

let gen_xor_large f x y =
  let obj = if x <> y then 
    (*generate_lha (f^".lha") "a7=0 & a17=0" "a17>0" "a7>0"*)
    "A22=2"
  else  
    (*generate_lha (f^".lha") "a7=0 & a17=0" "a7>0" "a17>0";*)
    "A9=2" in
  let  xi = if x then 1 else 0
  and nxi = if x then 0 else 1
  and  yi = if y then 1 else 0
  and nyi = if y then 0 else 1 in
  generate_spn f ( 
    [ (1 ,Init,0  ,(   0.0,  0.0)); 
      (2 ,Norm,nxi ,(-.1.0,  0.5)); 
      (3 ,Norm,nxi ,(-.2.0,  1.0));
      (4 ,Norm,nyi ,(-.2.0,  2.0));
      (5 ,Norm,nyi ,(-.2.0,  3.0));
      (6 ,Norm,1   ,(-.1.0,  3.5)); 
      (7 ,Norm,1   ,(  0.0,  4.0));
      (8 ,Norm,1   ,(  1.0,  3.5));
      (9 ,Final,1  ,(  2.0,  3.0));
      (10,Norm,1   ,(  3.0,  2.5));
      (11,Norm,1   ,(  4.0,  2.0));
      (12,Norm,1   ,(  4.0,  1.0));
      (13,Norm,yi  ,(  4.0,  0.0));
      (14,Norm,yi  ,(  3.0,-.0.5));
      (15,Norm,xi  ,(  2.0,-.1.0));
      (16,Norm,xi  ,(  1.0,-.0.5));
      (17,Norm,nyi ,(  2.0,-.2.0));
      (18,Norm,nyi ,(  2.0,-.3.0));
      (19,Norm,1   ,(  1.0,-.3.5));
      (20,Norm,1   ,(  0.0,-.4.0));
      (21,Norm,1   ,(-.1.0,-.3.5));
      (22,Final,1  ,(-.2.0,-.3.0));
      (23,Norm,1   ,(-.3.0,-.2.5));
      (24,Norm,1   ,(-.4.0,-.2.0));
      (25,Norm,1   ,(-.4.0,-.1.0));
      (26,Norm,yi  ,(-.4.0,-.0.0));
      (27,Norm,yi  ,(-.3.0,  0.5));
]) 0.009 0.3 obj;;


let lozange f n m fb =
  let accl = ref [] in
  for i = 1 to n do
    for j = 1 to m do
      let t = if i=1 && j=1 then Init else if
	  i=n && j =m then Final else Norm in
      accl := (i+n*(j-1) ,t ,(fb i j)   ,( 1.0*.(float (j-i)) , 0.5*.(float (i+j)))) :: !accl;
    done;
  done;
  generate_spn f !accl 0.009 0.3 (Printf.sprintf "A%i=2" (n*m));;



(*
generate_spn "ex" [
  (1,Init,0,(0.0,0.0)); 
  (2,Final,1,(2.0,0.0));] 
0.009 0.3 "A2=2";; 


generate_spn "ex2" [
  (1,Init,0,(0.0,0.0)); 
  (2,Norm,1,(2.0,0.0));
  (3,Final,1,(4.0,0.0));] 
0.009 0.3 "A3=2";; 


(*generate_lha "control.lha" "a8<2" "a8=2" "FALSE";*)
generate_spn "control" [
  (1,Init ,0,(0.0,0.0)); 
  (2,Norm ,1,(0.5,-1.0)); 
  (3,Norm ,1,(1.0,-2.0));
  (4,Norm ,1,(1.5,-3.0));
  (5,Norm ,1,(2.0,-.4.0));
  (6,Norm ,1,(2.5,-.5.0)); 
  (7,Norm ,1,(3.0,-.6.0));
  (8,Final,1,(3.5,-.7.0))]
0.009 0.3 "A8=2";; 


(*generate_lha "controlMissing1.lha" "a8<2" "a8=2" "FALSE";*)
generate_spn "controlMissing1" [
   (1,Init ,0,(0.0,0.0)); 
  (2,Norm ,1,(0.5,-1.0)); 
  (3,Norm ,1,(1.0,-2.0));
  (5,Norm ,1,(2.0,-.4.0));
  (6,Norm ,1,(2.5,-.5.0)); 
  (7,Norm ,1,(3.0,-.6.0));
  (8,Final,1,(3.5,-.7.0))]
0.009 0.3 "A8=2";; 

(*generate_lha "controlMissing2.lha" "a8<2" "a8=2" "FALSE";*)
generate_spn "controlMissing2" [
   (1,Init ,0,(0.0,0.0)); 
  (2,Norm ,1,(0.5,-1.0)); 
  (3,Norm ,1,(1.0,-2.0));
  (6,Norm ,1,(2.5,-.5.0)); 
  (7,Norm ,1,(3.0,-.6.0));
  (8,Final,1,(3.5,-.7.0))]
0.009 0.3 "A8=2";; 

(*generate_lha "controlMissing7.lha" "a8<2" "a8=2" "FALSE";*)
generate_spn "controlMissing7" [
  (1,Init ,0,(0.0,0.0)); 
  (2,Norm ,1,(0.5,-1.0)); 
  (3,Norm ,1,(1.0,-2.0));
  (4,Norm ,1,(1.5,-3.0));
  (5,Norm ,1,(2.0,-.4.0));
  (6,Norm ,1,(2.5,-.5.0)); 
  (8,Final,1,(3.5,-.7.0))]
0.009 0.3 "A8=2";; 

 *)

(*generate_lha "track12Block1.lha" "a8<2 & a12<2" "a12=2" "a8=2";*)
generate_spn "track12Block1" [ 
  (1,Init,0,(  0.0,0.0)); 
  (2,Norm,1,(  0.0,-1.0)); 
  (3,Norm,1,(  0.0,-2.0));
  (4,Norm,1,(  0.0,-3.0));
  (5,Norm,0,(-.1.0,-.3.5));
  (6,Norm,1,(-.2.0,-.4.0)); 
  (7,Norm,1,(-.3.0,-.4.5));
 (8,Final,1,(-.4.0,-.5.0));
  (9,Norm,1,(  1.0,-.3.5));
  (10,Norm,1,( 2.0,-.4.0));
  (11,Norm,1,( 3.0,-.4.5));
  (12,Final,1,(4.0,-.5.0))] 0.009 0.3 "A12=2";;

(*generate_lha "track12Block2.lha" "a8=0 & a12=0" "a12>0" "a8>0";
generate_lha "track12Block2.lha" "a8<2 & a12<2" "a12=2" "a8=2";*)
generate_spn "track12Block2" [ (1,Init,0,(0.0,0.0)); 
			 (2,Norm,1,(0.0,1.0)); 
			 (3,Norm,1,(0.0,2.0));
			 (4,Norm,1,(0.0,3.0));
			 (5,Norm,0,(-.1.0,3.5));
			 (6,Norm,0,(-.2.0,4.0)); 
			 (7,Norm,1,(-.3.0,4.5));
			(8,Final,1,(-.4.0,5.0));
			 (9,Norm,1,(  1.0,3.5));
			 (10,Norm,1,( 2.0,4.0));
			 (11,Norm,1,( 3.0,4.5));
			 (12,Final,1,(4.0,5.0));] 0.009 0.3 "A12=2";; 

(*generate_lha "track12BlockBoth.lha" "a8=0 & a12=0" "a12>0" "a8>0";
generate_lha "track12BlockBoth.lha" "a8<2 & a12<2" "a12=2" "a8=2";*)
generate_spn "track12BlockBoth" [ (1,Init,0,(0.0,0.0)); 
			 (2,Norm,1,(0.0,-1.0)); 
			 (3,Norm,1,(0.0,-2.0));
			 (4,Norm,1,(0.0,-3.0));
			 (5,Norm,0,(-.1.0,-.3.5));
			 (6,Norm,1,(-.2.0,-.4.0)); 
			 (7,Norm,1,(-.3.0,-.4.5));
			(8,Final,1,(-.4.0,-.5.0));
			 (9,Norm,0,(  1.0,-.3.5));
			 (10,Norm,1,( 2.0,-.4.0));
			 (11,Norm,1,( 3.0,-.4.5));
			 (12,Final,1,(4.0,-.5.0));] 0.009 0.3 "A8=2";; 


(*generate_lha "track28LL.lha" "a17<2 & a20<2 & a25<2 & a28<2" 
 "a17>1" "a20>1 | a25>1 | a28>1";;*)
gen28 "track28LL" 1 0 1 0 "a17=2";;

(*generate_lha "track28LR.lha" "a17<2 & a20<2 & a25<2 & a28<2" 
  "a20>1" "a17>1 | a25 >1 | a28>1";;*)
gen28 "track28LR" 1 0 0 1 "a20=2";;

(*generate_lha "track28RL.lha" "a17<2 & a20<2 & a25<2 & a28<2" 
  "a25>1" "a20>1 | a17 >1 | a28>1";;*)
gen28 "track28RL" 0 1 1 0 "a25=2";;

(*generate_lha "track28RR.lha" "a17<2 & a20<2 & a25<2 & a28<2" 
  "a28>1" "a20>1 | a25 >1 | a17>1";;*)
gen28 "track28RR" 0 1 0 1 "a28=2";;

(*
gen_xor "ringLL" true true;;
gen_xor "ringRR" false false;;
*)

gen_xor "ringLL" true true;;
gen_xor "ringRL" false true;;
gen_xor "ringLR" true false;;
gen_xor "ringRR" false false;;

gen_xor_large "ringLLLarge" true true;;
gen_xor_large "ringRLLarge" false true;;
gen_xor_large "ringLRLarge" true false;;
gen_xor_large "ringRRLarge" false false;;


let redondantChoice bl br = 
  generate_spn (Printf.sprintf "redondantChoice%i%i" bl br) [ (1,Init,0,(0.0,0.0)); 
				   (2,Norm,1,(0.0,1.0)); 
				   (3,Norm,1,(0.0,2.0));
				   (4,Norm,1,(0.0,3.0));

				   (5,Norm,bl,(-.1.0,3.5));
				   (6,Norm,bl,(-.2.0,4.0)); 
				   (7,Norm,1,(-.3.0,4.5));
				   (8,Norm,1,(-.3.0,5.5));

				   (9,Norm,br,(  1.0,3.5));
				   (10,Norm,br,( 2.0,4.0));
				   (11,Norm,1,( 3.0,4.5));
				   (12,Norm,1,(3.0,5.5));
				   
				   (13,Norm,bl,(-3.0,6.5));
				   (14,Norm,bl,(-3.0,7.5));
				   (15,Norm,bl,(-3.0,8.5));
				   (16,Norm,bl,(-3.0,9.5));
				   (17,Norm,1,(-3.0,10.5));
				   
				   (18,Norm,br,(3.0,6.5));
				   (19,Norm,br,(3.0,7.5));
				   (20,Norm,br,(3.0,8.5));
				   (21,Norm,br,(3.0,9.5));
				   (22,Norm,1,(3.0,10.5));
				   
				   (23,Norm,br,(-2.0,6.5));
				   (24,Norm,br,(-1.0,7.0));
				   (25,Norm,1,(0.0,7.5));
				   (26,Norm,br,(1.0,8.0));
				   (27,Norm,br,(2.0,8.5));
				   
				   (28,Norm,bl,(2.0,6.5));
				   (29,Norm,bl,(1.0,7.0));
				   (30,Norm,bl,(-1.0,8.0));
				   (31,Norm,bl,(-2.0,8.5));

				   (32,Final,1,(-3.0,11.5));
				   (33,Final,1,(3.0,11.5));

] 0.009 0.3 (if bl=1 then "A32=2" else "A33=2");; 

(*
redondantChoice 1 0;;
redondantChoice 0 1;;
*)

lozange "lozange" 10 10 (fun _ _ -> 1);;
 
let ra x = (float x) -. 5.5;;
let ra2 x y = (ra x)*.(ra x) +. (ra y)*.(ra y);;

let raman x = abs_float ((float x) -. 5.5);;
let ra3 x y = max (raman x) (raman y);;

(*lozange "lozangeBlock" 10 10 (fun x y -> 
  max 0 ( (min 1 (int_of_float ((ra2 x y)/.20.1)))));;
*)

lozange "lozangeBlock" 10 10 (fun x y -> 
  max 0 ( (min 1 (int_of_float ((ra3 x y)/.4.0)))));;


