#!/opt/local/bin/ocaml
#load "unix.cma";;
(*#use "../script.ml" *)
open Printf

let n = int_of_string (Sys.argv.(1));;
let r = int_of_string (Sys.argv.(2));;

let mu = float_of_string (Sys.argv.(3));;

let horizon = int_of_string (Sys.argv.(4));;

let methode = int_of_string (Sys.argv.(5));;

let k = int_of_string (Sys.argv.(6));;

let epsilon = float_of_string (Sys.argv.(7));;

let rho = (1. -. mu) /. 2. ;;

let dir = sprintf "tandem%i_%i_%.2f_%i_%i_%i_%e" n r mu horizon methode k epsilon;;


let genere () =
  let gspnagr = open_out "tandem_agr.gspn" in

  Printf.fprintf gspnagr "const double mu   = %f;
const double rho = %f;
const int r = %i;
" mu rho r;

  Printf.fprintf gspnagr "NbPlaces = %i;\n" (2*k-1);
  Printf.fprintf gspnagr "NbTransitions = %i;\n" (k+1);


  Printf.fprintf gspnagr "PlacesList = { RE_Queue1 ";
  for i = 2 to k do 
    Printf.fprintf gspnagr ",RE_Queue%i , AQueue%i" i i;
  done;
  Printf.fprintf gspnagr " } ;
TransitionsList = { mu";
  for i = 1 to k do 
    Printf.fprintf gspnagr ",rho%i " i ;
  done;
  Printf.fprintf gspnagr " };

Marking= {
(RE_Queue1 ,1);\n";
  for i = 2 to k do 
    Printf.fprintf gspnagr "(RE_Queue%i ,0);(AQueue%i ,r);\n" i i ;
  done;
  Printf.fprintf gspnagr "};

Transitions={
(mu , EXPONENTIAL(mu),1,1,SINGLE);\n";
  for i = 1 to k do 
    Printf.fprintf gspnagr "(rho%i , EXPONENTIAL(rho),1,1,SINGLE);\n" i;
  done;
  Printf.fprintf gspnagr "};

InArcs={
(RE_Queue1    ,rho1,1);\n";
  for i = 2 to k do 
    Printf.fprintf gspnagr "(AQueue%i,rho%i,1);\n(RE_Queue%i,rho%i,1);\n" i (i-1) i i ;
  done;
  Printf.fprintf gspnagr "};

OutArcs={
(mu ,RE_Queue1 ,1);\n";
  for i = 2 to k do 
    Printf.fprintf gspnagr "(rho%i, RE_Queue%i,1);\n(rho%i,AQueue%i,1);\n" (i-1) i i i ;
  done;
  Printf.fprintf gspnagr "};";
  close_out gspnagr;

  let lhaagr = open_out "tandem_agr.lha" in
  Printf.fprintf lhaagr "

NbVariables = 1;
NbLocations = 3;
const int N= %i;
VariablesList = {x} ;

LocationsList = {l1, lp, lm };
AVG(Last(x));
InitialLocations={l1};

FinalLocations={ lp  };

Locations={\t" n;
  let sum = ref "RE_Queue1 " in
  for i =2 to k do
    sum := Printf.sprintf "%s + RE_Queue%i " !sum i;
  done;
  Printf.fprintf lhaagr "
(l1, (%s > 0) & (%s < N) ) ;
(lp, (%s = N)  );
(lm, (%s = 0)  );" !sum !sum !sum !sum;
  Printf.fprintf lhaagr "}; 

Edges={
((l1,l1)  ,ALL,  # , #);
((l1,lp)  ,ALL,  # , {x=1});
((l1,lm)  ,ALL,  # , {x=0});
};";
  close_out lhaagr;


  let gspn = open_out "tandem.gspn" in

  Printf.fprintf gspn "const double mu   = %f;
const double rho = %f;
const int r = %i;
" mu rho r;

  Printf.fprintf gspn "NbPlaces = %i;\n" (k+1);
  Printf.fprintf gspn "NbTransitions = %i;\n" (k+3);


  Printf.fprintf gspn "PlacesList = { RE_Queue1 ";
  for i = 2 to k do 
    Printf.fprintf gspn ",RE_Queue%i " i;
  done;
  Printf.fprintf gspn ", Puit } ;
TransitionsList = { mu";
  for i = 1 to k do 
    Printf.fprintf gspn ",rho%i " i ;
  done;
  Printf.fprintf gspn ", selfloop, Puittrans };

Marking= {
(RE_Queue1 ,1);\n";
  for i = 2 to k do 
    Printf.fprintf gspn "(RE_Queue%i ,0);\n" i ;
  done;
  Printf.fprintf gspn "(Puit ,0);
};

Transitions={
(mu , EXPONENTIAL(mu),1,1,SINGLE);\n";
  for i = 1 to k do 
    Printf.fprintf gspn "(rho%i , EXPONENTIAL(rho),1,1,SINGLE);\n" i;
  done;
  Printf.fprintf gspn "(selfloop, EXPONENTIAL(0*RE_Queue1),1,1,SINGLE);
(Puittrans, EXPONENTIAL(0*RE_Queue1),1,1,SINGLE);
};

InArcs={
(RE_Queue1    ,rho1,1);\n";
  for i = 1 to k do 
    Printf.fprintf gspn "(RE_Queue%i,rho%i,1);\n" i i ;
  done;
  Printf.fprintf gspn "};

OutArcs={
(mu ,RE_Queue1 ,1);\n";
  for i = 2 to k do 
    Printf.fprintf gspn "(rho%i, RE_Queue%i,1);\n" (i-1) i ;
  done;
  Printf.fprintf gspn "(Puittrans, Puit,1);
};";
  close_out gspn;

  let lha = open_out "tandem.lha" in
  Printf.fprintf lha "

NbVariables = 1;
NbLocations = 3;
const int N= %i;
VariablesList = {x} ;

LocationsList = {l1, lp, lm };
AVG(Last(x));
InitialLocations={l1};

FinalLocations={ lp,lm };

Locations={
(l1, (%s > 0) & (%s < N) & (Puit = 0)) ;
(lp, (%s = N) & (Puit = 0) );
(lm, (%s = 0) | (Puit > 0) );
}; 

Edges={
((l1,l1)  ,ALL,  # , #);
((l1,lp)  ,ALL,  # , {x=1});
((l1,lm)  ,ALL,  # , {x=0});
};" n !sum !sum !sum !sum;
  close_out lha;

  let sm = open_out "tandem_agr.sm" in

  Printf.fprintf sm "ctmc
const int n = %i;
const int u = %i;
const int r = %i;

const double mu = %f;
const double rho = %f;" n horizon r mu rho;
Printf.fprintf sm "
module file1
	buffer1 :  [0..n] init 1;
	[lambda] buffer1<n -> mu: (buffer1'=buffer1+1) ;
	[rho1] buffer1>0 -> rho: (buffer1'=buffer1-1) ;
endmodule

module file2
	buffer2 : [0..r] init 0;
	[rho1] buffer2<r -> 1: (buffer2'=buffer2+1) ;
	[rho2] buffer2>0 -> rho: (buffer2'=buffer2-1) ;
endmodule

";

for i =3 to k do 
  Printf.fprintf sm "module file%i = file2[buffer2=buffer%i, rho1=rho%i, rho2=rho%i] endmodule\n" i i (i-1) i;
done;
let sum2 = ref "buffer1 " in
  for i =2 to k do
    sum2 := Printf.sprintf "%s + buffer%i " !sum2 i;
  done;
Printf.fprintf sm"
formula win = %s=n;
formula notloose = %s>0;" !sum2 !sum2;

close_out sm;

  let com1 =  "Cosmos tandem_agr.gspn tandem_agr.lha -s > logcosmos  2>&1" in
  print_endline com1;
  ignore (Sys.command com1);

let prism1 = "prism tandem_agr.sm ../tandem.csl -fixdl -v -maxiters 100000000 > prismout" in
  print_endline prism1;
  ignore (Sys.command prism1);

  ignore (Sys.command "../traduittable.ml prismout");;



let exist = try 
	      Unix.mkdir dir 0o770;
	      false;
  with _ -> true;;

Unix.chdir dir;;

if not exist then genere ();;


let com2 = Printf.sprintf "Cosmos tandem.gspn tandem.lha --batch 100 --max-run 1000 -c -b %i --set-Horizon %i --epsilon %e --level 0.99999 --njob 10 --gppcmd clang++ --gppflags --coverage > logcosmosCalcParal  2>&1" methode horizon epsilon;; 

let com3 = Printf.sprintf "Cosmos tandem.gspn tandem.lha --batch 1000 --width 0.1 --gppcmd clang++ --gppflags --coverage > logcosmosCalc  2>&1";;

print_endline com3;;
Sys.command com3;;


Sys.command "grep Memory logcosmosCalc";;
print_newline ();
Sys.command "grep Mean logcosmosCalc";;

