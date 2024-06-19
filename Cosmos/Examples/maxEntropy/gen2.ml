#directory "../../utils"

#use "mlcall.ml";;


let timeConflict name nbstate nbcomp polysize =
  let model = open_out (name^".prism") in
  Printf.fprintf model 
"pta

const int n = %i;
const int wt = 3;

module c1
	state1: [1..n] init 1;
	x1: clock;

	invariant
		  ( state1<n => x1<2) 
		& ( state1=n => x1 < wt)
	endinvariant

	[] state1<n-1 & x1<2 -> 1.0 : (state1'=state1+1);
	[] state1=n-1 & x1<1 -> 1.0 : (state1'=n);
	[] state1=n-1 & x1>1 & x1<2  -> 1.0 : (state1'=1)&(x1'=0);	
	[] state1<n & x1<2 -> 1.0 : (state1'=1);
	[] state1=n & x1<wt -> 1.0 : (state1'=1)&(x1'=0);
endmodule\n" nbstate;
  let target = ref ("state1="^(string_of_int nbstate)) in
  for i = 2 to nbcomp do
    Printf.fprintf model "module c%i = c1 [state1=state%i,x1=x%i ] endmodule\n" i i i;
    target := Printf.sprintf "%s,state%i=%i" !target i nbstate;
  done;
  close_out model;
  let prop = open_out (name^".lha") in
  Printf.fprintf prop
"VariablesList = {time,DISC countT} ;
LocationsList = {l0, l1,l2};

timeReach=AVG(Last(time));
stepReach=AVG(Last(countT));

InitialLocations={l0};
FinalLocations={l2};
Locations={
(l0, TRUE, (time:1));
(l1, TargetState =0 , (time:1 ));
(l2, TargetState >0);
};
Edges={
((l0,l0),ALL,time<= 0 ,#);
((l0,l1),#,time=0 ,{time=0});
((l1,l1),ALL,#,{countT = countT+1 });
((l1,l2),ALL,#,#);
};";
  close_out prop;
  ignore (Sys.command (Printf.sprintf "sage script_tocosmos.sage %s.prism %s.grml %i -uniform \".*(%s).*\"" name name polysize !target));
  execSavedCosmos ~prefix:false (name,name^".grml",name^".lha"," --njob 2 --max-run 20000 --gppflags \"-O0\" --batch 0");
(*  ignore (Sys.command (Printf.sprintf "sage script_tocosmos.sage %s.prism %s.grml 1 -isotropic \".*(%s).*\"" name name !target)); *)
  execSavedCosmos ~prefix:false ("Iso_"^name,"Iso_"^name^".grml",name^".lha"," --njob 2 --max-run 20000 --gppflags \"-O0\" --batch 0");;
  
printHeadCsv ();;

(* First Table *)
timeConflict "firstbis5_1_10" 5 1 10;;
(*timeConflict "first4_1_4" 4 1 13;;
timeConflict "first5_1_5" 5 1 13;;
timeConflict "first6_1_6" 6 1 13;;
timeConflict "first7_1_7" 7 1 13;;
timeConflict "first8_1_8" 8 1 13;;
timeConflict "first9_1_9" 9 1 13;;
timeConflict "first10_1_10" 10 1 13;;
timeConflict "first11_1_11" 11 1 13;;
timeConflict "first12_1_12" 12 1 13;;
timeConflict "first13_1_13" 13 1 13;;
*)

(* Second Table *)
for i = 2 to 5 do 
    timeConflict (Printf.sprintf "second%i_2_15" i) i 2 7;
done;
