#!/usr/bin/env ocaml

#directory "../../utils"

#use "mlcall.ml";;
open Printf;;

(*script for experiment*)

let gen_diff_to_reach name n b timeC wt npoly = 
  let f = open_out (name^"_C.prism") in
    Printf.fprintf f "pta
    const int n = %i;
    const int B = %i;
    const int timeC = %i;
    const int wt = %i;

    module c1
        state1: [1..n] init 1;
        x: clock;

        invariant
                  ( state1<n => x<B) 
                & ( state1=n => x < wt)
        endinvariant

        [a] state1<n-1 & x<B -> 1.0 : (state1'=state1+1);
        [b] state1=n-1 & x<timeC -> 1.0 : (state1'=n);
        [c] state1=n-1 & x>timeC & x<B  -> 1.0 : (state1'=1)&(x'=0);    
        [d] state1<n & x<B -> 1.0 : (state1'=1);
        [e] state1=n & x<wt -> 1.0 : (state1'=1)&(x'=0);
endmodule" n b timeC wt;
    close_out f;
    let cmd = Printf.sprintf "sage script_tocosmos_PTA.sage %s_C.prism %s_C.grml %i" name name npoly in
    print_endline cmd;
    assert (0=Sys.command cmd);

    let cmd2 = Printf.sprintf "python script_lha_alzen.py %i 1000 %i %i > %s_A.lha" n wt b name in
    print_endline cmd2;
    assert (0=Sys.command cmd2)

let gen_faillure_repair name n b minRep npoly cWeight dline=
  let f = open_out (name^"_C.prism") in
  Printf.fprintf f  "pta
    const int n = %i;
    const int B = %i;
    const int minRep = %i;

    module c1
        state1: [1..n+1] init 1;
        x: clock;
        y: clock;

        invariant
              ( x<B) & (state1<=n | x< %i ) & ( x < B-(n-state1)*minRep | state1>n )
        endinvariant

        [a] state1<n & x< B-(n-state1)*minRep  & y>minRep-> 1.0 : (state1'=state1+1)&(y'=0);
        [b] state1<n+1 & state1>1 & x<B-(n-state1-1)*minRep  -> 1.0 : (state1'=state1-1);
        [b] state1=1 & x<B-(n-state1)*minRep  -> 1.0 : (state1'=1);
        [c] state1=n & x<B -> 1.0 : (state1'=n+1)&(x'=0)&(y'=0);
        [c] state1=n+1 -> 1.0 : (state1'=n+1)&(x'=0)&(y'=0);
        [b] state1=n+1 -> 1.0 : (state1'=1)&(x'=0)&(y'=0);
endmodule" n b minRep cWeight;
  close_out f;
  let cmd = Printf.sprintf "sage script_tocosmos_PTA.sage %s_C.prism %s_C.grml %i %i" name name npoly dline in
  print_endline cmd;
  assert (0=Sys.command cmd)


let gen_faillure_repairB name c1 c2 =
  let f = open_out (name^"_B.lha") in
  Printf.fprintf f  "NOTDETERMINISTIC
const int c1 =%i;
const int c2 = %i;

VariablesList = {DISC countT, t1,t2};
LocationsList = {li,l1,l2,lf, li2,l12,l22,lf2};

reach=PROB;
reachCond=PROB(lf);

InitialLocations={li,li2};
FinalLocations={lf2};
Locations={
(li, TRUE, (t1:1,t2:1));
(l1, TRUE, (t1:1,t2:1));
(l2, TRUE, (t1:1,t2:1));
(lf, TRUE, (t1:1,t2:li2));

(1, TRUE, (t1:1,t2:1));
(l12, TRUE, (t1:1,t2:1));
(l22, TRUE, (t1:1,t2:1));
(lf2, TRUE, (t1:1,t2:1));
};

Edges={
((li,li),ALL\\{t_a,t_b,t_c},countT <=Dline ,#);
((l1,l1),ALL\\{t_a,t_b,t_c},countT <=Dline & t1<=c2 ,#);
((l2,l2),ALL\\{t_a,t_b,t_c},countT <=Dline & t1<=c2 ,#);
((lf,lf),ALL\\{t_a,t_b,t_c},countT <=Dline ,#);

((li,li),{t_a,t_b,t_c}, # ,{countT = countT+1});
((li,l1),{t_a}, # ,{countT = countT+1,t1=0});
((l1,l1),{t_a,t_b,t_c}, t1<=c2 ,{countT = countT+1});
((l1,l2),{t_c}, t1>=c1& t1<=c2 ,{countT = countT+1,t2=0});
((l2,l2),{t_a,t_b,t_c}, t1<=c2 ,{countT = countT+1});
((l2,lf),{t_a}, t2>=c1&t1<=c2 ,{countT = countT+1});
((lf,lf),{t_a,t_b,t_c}, # ,{countT = countT+1});


((li2,li2),ALL\\{t_a,t_b,t_c},countT <=Dline ,#);
((l12,l12),ALL\\{t_a,t_b,t_c},countT <=Dline & t1<=c2 ,#);
((l22,l22),ALL\\{t_a,t_b,t_c},countT <=Dline & t1<=c2 ,#);
((lf2,lf2),ALL\\{t_a,t_b,t_c},countT <=Dline ,#);

((li2,li2),{t_a,t_b,t_c}, # ,{countT = countT+1});
((li2,l12),{t_a}, # ,{countT = countT+1,t1=0});
((l12,l12),{t_a,t_b,t_c}, t1<=c2 ,{countT = countT+1});
((l12,l22),{t_c}, t1>=c1& t1<=c2 ,{countT = countT+1});
((l22,l22),{t_a,t_b,t_c}, t1<=c2 ,{countT = countT+1});
((l22,lf2),{t_a}, t1>=2*c1&t1<=c2 ,{countT = countT+1});
((lf2,lf2),{t_a,t_b,t_c}, # ,{countT = countT+1});
};
" c1 c2;
  close_out f


let gen_faillure_third name n m k dline c1 c2 evt bB=
  let f = open_out (name^"_A.lha") in
  fprintf f  "NOTDETERMINISTIC
const int nstep =%i;
const int c1 = %i;
const int c2 = %i;
const int evt = %i;
const int B= %i;

VariablesList = {DISC countT,DISC pos, t1 " m c1 c2 evt bB;
  let tstr = ref "t1:1" in
  let tstr2 = ref "countT=0" in
  for i =1 to n do 
    fprintf f ", time%i,ctime%i,DISC compA%i,DISC compB%i" i i i i; 
    tstr := (!tstr) ^", time"^ (string_of_int i)^":1"^", ctime"^ (string_of_int i)^":1";
    tstr2 := (!tstr2) ^", compA"^ (string_of_int i)^"=0, compB"^ (string_of_int i)^"=0, time"^ (string_of_int i)
    ^"=1, ctime"^ (string_of_int i)^"=0";
  done;
  fprintf f "
 };
LocationsList = {li,l1,l2,l3,lf,li2,l12,l22,lf2};

reach=PROB;
reachCond=PROB(lf2);

InitialLocations={li,li2};
FinalLocations={lf};
Locations={
(li, TRUE, (%s));
(l1, TRUE, (%s));
(l2, TRUE, (%s));
(l3, TRUE, (%s));
(lf, TRUE, (%s));

(li2, TRUE, (t1:1));
(l12, TRUE, (t1:1));
(l22, TRUE, (t1:1));
(lf2, TRUE, (t1:1));
};

Edges={
((li,l1),ALL,#,{%s});
((l1,l1),ALL\\{t_a,t_b,t_c},countT <=Dline ,#);
((l2,l2),ALL\\{t_a,t_b,t_c},countT <=Dline ,#);
((l3,l3),ALL\\{t_a,t_b,t_c},countT <=Dline ,#);
((lf,lf),ALL\\{t_a,t_b,t_c},countT <=Dline ,#);\n" !tstr !tstr !tstr !tstr !tstr !tstr2;
  fprintf f "((l1,l1),{t_b}, pos>=1 ,{countT = countT+1,pos=pos-1});\n";
  fprintf f "((l1,l1),{t_b}, pos<=0 ,{countT = countT+1});\n";
  for i =1 to n do
    fprintf f "\n((l1,l2),{t_a}, # ,{countT = countT+1,compA%i=compA%i+1,time%i=0,ctime%i=0,pos=pos+1});\n" i i i i;
    fprintf f "((l2,l2),{t_a}, time%i>=2 & compA%i<=nstep-1 & ctime%i<=evt ,{countT = countT+1,compA%i=compA%i+1,time%i=0,ctime%i=0,pos=pos+1});\n" i i i i i i i;
    fprintf f "((l2,l2),{t_b}, compB%i<=%i & compA%i>=1& ctime%i<=evt  & pos<=0,{countT = countT+1,compB%i=compB%i+1,compA%i=compA%i-1,ctime%i=0});\n" i (k-1) i i i i i i i;
    fprintf f "((l2,l2),{t_b}, compB%i<=%i & compA%i<=0& ctime%i<=evt & pos<=0,{countT = countT+1,compB%i=compB%i+1,ctime%i=0});\n" i (k-1) i i i i i;
    fprintf f "((l2,l2),{t_b}, compB%i<=%i & compA%i>=1& ctime%i<=evt & pos>=1,{countT = countT+1,compB%i=compB%i+1,compA%i=compA%i-1,ctime%i=0,pos=pos-1});\n" i (k-1) i i i i i i i;
    fprintf f "((l2,l2),{t_b}, compB%i<=%i & compA%i<=0& ctime%i<=evt & pos>=1,{countT = countT+1,compB%i=compB%i+1,ctime%i=0,pos=pos-1});\n" i (k-1) i i i i i;
  done;
  fprintf f "((l2,l3),{t_c},compA1>=nstep & t1<= B";
  for i =2 to n do fprintf f "& compA%i>=nstep" i done;
  fprintf f ",{countT=countT+1,t1=0";
  for i =1 to n do fprintf f ", compA%i=0, compB%i=0" i i done;
  fprintf f "});\n((l3,l3),{t_c},#,{countT = countT+1,t1=0});\n";
  fprintf f "((l3,l1),{t_b},#,{countT=countT+1";
  for i =1 to n do fprintf f ",ctime%i=0" i done;
  fprintf f "});\n" ;

  fprintf f "((l1,lf),{t_a,t_b,t_c}, countT=Dline+1 ,#);
((l2,lf),{t_a,t_b,t_c}, countT=Dline ,#);
((l3,lf),{t_a,t_b,t_c}, countT=Dline ,#);
((lf,lf),{t_a,t_b,t_c}, countT=Dline ,#);



((li2,li2),ALL\\{t_a,t_b,t_c},countT <=Dline ,#);
((l12,l12),ALL\\{t_a,t_b,t_c},countT <=Dline & t1<=c2 ,#);
((l22,l22),ALL\\{t_a,t_b,t_c},countT <=Dline & t1<=c2 ,#);
((lf2,lf2),ALL\\{t_a,t_b,t_c},countT <=Dline ,#);

((li2,li2),{t_a,t_b,t_c}, # ,{countT = countT+1});
((li2,l12),{t_a}, # ,{countT = countT+1,t1=0});
((l12,l12),{t_a,t_b,t_c}, t1<=c2 ,{countT = countT+1});
((l12,l22),{t_c}, t1>=c1& t1<=c2 ,{countT = countT+1});
((l22,l22),{t_a,t_b,t_c}, t1<=c2 ,{countT = countT+1});
((l22,lf2),{t_a}, t1>=2*c1&t1<=c2 ,{countT = countT+1});
((lf2,lf2),{t_a,t_b,t_c}, # ,{countT = countT+1});

};


";
  close_out f


(* Running Example *)
let exp () =
    let name= "twoears" in
      let cmd = Printf.sprintf "sage script_tocosmos_PTA.sage %s.prism %s.grml %i 50" name name 5 in
      ignore (Sys.command cmd);
      execSavedCosmos ~prefix:false (name,name^".grml","runningExB.lha"," --njob 1 --batch 1000 --width 0.01");;

(*exp ();;*)

let run1 n b timeC wt npoly =
  let name = "exp1_"^string_of_int n in
    gen_diff_to_reach name n b timeC wt npoly;
    execSavedCosmos ~prefix:false (name,name^"_C.grml",name^"_A.lha"," --njob 2 --max-run 10000 --batch 1000 --width 0");
    execSavedCosmos ~prefix:false ("Iso_"^name,"Iso_"^name^"_C.grml",name^"_A.lha"," --njob 2 --max-run 10000 --batch 1000 --width 0")

(* EXP1 *)

let exp1 () =
    run1 6 20 10 1000 6;
    run1 7 20 10 1000 7;
    run1 8 20 10 1000 8;
    run1 9 20 10 1000 9;
    run1 10 20 10 1000 10;;

(*exp1 ()*)

let run2 n b minRep npoly c1 c2 dline =
  let name = "exp2_"^string_of_int n in
  (*gen_faillure_repair name n b minRep npoly;
  gen_faillure_repairB name c1 c2 dline;*)
  execSavedCosmos ~prefix:false (name,name^"_C.grml",name^"_B.lha"," --njob 2 --max-run 100000 --batch 1000 --width 0");
  execSavedCosmos ~prefix:false ("Iso_"^name,"Iso_"^name^"_C.grml",name^"_B.lha"," --njob 2 --max-run 100000 --batch 1000 --width 0");;


(* EXP2 *)
let exp2 () =
  let b = 10
	    
  and minRep = 1 
  and npoly = 8
  and c1 = 9
  and c2= 20
  and dline = 100 in
  for i = 2 to 7 do
    run2 i b minRep npoly c1 c2 dline;
  done;;

(*gen_faillure_repair "exp2" 5 12 1 10;*)
(*gen_faillure_third "exp2_2" 2 2 2 50 9 20 8;*)
(*exp2 ();;*)


let run3 n b minRep npoly c1 c2 dline nm evt=
  let name = "exp3_"^string_of_int n in
  gen_faillure_repair name (n+1) b minRep npoly 2 dline;
  (*gen_faillure_repairB name c1 c2 dline;*)
  gen_faillure_third name nm ((n/2)) 2 dline c1 c2 evt b;
  execSavedCosmos ~prefix:false (name,name^"_C.grml",name^"_A.lha"," --njob 4 --max-run 100000 --batch 100 --width 0 --tmp-status 2");
  execSavedCosmos ~prefix:false ("Iso_"^name,"Iso_"^name^"_C.grml",name^"_A.lha"," --njob 4 --max-run 100000 --batch 100 --width 0");;

let timeold f x =
    let t = Sys.time() in
    let fx = f x in
    Printf.printf "Execution time: %fs\n" (Sys.time() -. t);
    fx

let time f x =
    let start = Unix.gettimeofday ()
    in let res = f x
    in let stop = Unix.gettimeofday ()
    in let () = Printf.printf "Execution time: %fs\n%!" (stop -. start)
    in
       res

(* Running Example *)
let runExp () =
  let cmd = Printf.sprintf "sage script_tocosmos_PTA.sage %s %s.grml %i %i" "twoears.prism" "twoears.prism" 5 50 in
  print_endline cmd;
  assert (0=Sys.command cmd);
  execSavedCosmos ~prefix:false ("Running Example","twoears.prism.grml","runningExB.lha"," --max-run 10000 --width 0")



(* EXP3 *)
let exp3 () =
  let b = 12
  and minRep = 1 
  and npoly = 14
  and c1 = 9
  and c2= 20
  and dline = 50 in

  run3 (4) b minRep npoly c1 c2 dline 2 8;;
  (*for i = 2 to 2 do
    time (run3 (2*i) (8+2*i) minRep (2*i+5) c1 c2 dline 2) 8;
  done;;*)

(*let _ = runExp ();;*)


let _ = exp3 ();;

