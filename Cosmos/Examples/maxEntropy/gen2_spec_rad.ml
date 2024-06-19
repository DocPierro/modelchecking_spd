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
  for i = 2 to nbcomp do
    Printf.fprintf model "module c%i = c1 [state1=state%i,x1=x%i ] endmodule\n" i i i;
  done;
  close_out model;
  ignore (Sys.command (Printf.sprintf "sage script_radius.sage %s.prism specrad_of_%s %i" name name polysize));;
(*timeConflict "spec_rad_3_1_60" 3 1 40;;
timeConflict "spec_rad_5_1_60" 5 1 40;;
timeConflict "spec_rad_7_1_60" 7 1 80;;*)
timeConflict "spec_rad_5_1_30" 11 1 30;;
(*timeConflict "first4_1_4" 4 1 4;;
timeConflict "first5_1_5" 5 1 5;;
timeConflict "first6_1_6" 6 1 6;;
timeConflict "first7_1_7" 7 1 7;;
timeConflict "first8_1_8" 8 1 8;;
timeConflict "first9_1_9" 9 1 9;;
timeConflict "first5_1_5" 5 1 5;;
timeConflict "first5_1_10" 5 1 10;;
timeConflict "first5_1_20" 5 1 20;;*)
0.310778544880998, 0.296475448212538
