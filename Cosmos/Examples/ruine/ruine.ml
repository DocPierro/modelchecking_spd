#!/opt/local/bin/ocaml
#load "unix.cma";;
(*#use "../script.ml"*)
open Printf
 
let n = int_of_string (Sys.argv.(1));;
let l1= (int_of_string (Sys.argv.(2))) / 2;;
let l2= l1;;

let p = float_of_string (Sys.argv.(3));;
let q = 1. -. p;;

let horizon = float_of_string (Sys.argv.(4));;
let methode = int_of_string (Sys.argv.(5));;
let epsilon = float_of_string (Sys.argv.(6));;

let dir = sprintf "ruine%i_%i_%f_%i" n (l1+l2) horizon methode;;

let log = open_out "logscript";;

let genere () =
let gspn = open_out "ruine.gspn" in

Printf.fprintf gspn "
const double p = %f;
const double q = %f;
const int l =%i;
const int n =%i;

NbPlaces = %i;
NbTransitions = %i;

PlacesList = { " p q (l1+l2) n ((l1+l2)*n*2+1) ((l1+l2-2)*n*2 +2);
for i =1 to n do
  for j = 1 to l1+l2 do
    Printf.fprintf gspn "RE_Place%i_%i, " i j;
    Printf.fprintf gspn "AntiPlace%i_%i, " i j;
  done;
done;

(*for j = 1 to l1+l2 do
  Printf.fprintf gspn "RE_nPlace%i" j;
  if j< l1+l2 or true then Printf.fprintf gspn ", ";
  done;*)
Printf.fprintf gspn "Puit } ;

TransitionsList = { ";
for i =1 to n do
  for j=1 to l1+l2-1 do
    if j>1 then Printf.fprintf gspn "Avance%i_%i, " i j;
    if j<l1+l2-1 then Printf.fprintf gspn "Recule%i_%i, " i j;
  done;
done;
Printf.fprintf gspn "selfloop,";
Printf.fprintf gspn "Puittrans } ;

Marking = { ";
for i =1 to n do
  for j =1 to l1+l2 do
    if j = l1 then 
      Printf.fprintf gspn "(RE_Place%i_%i,1);(AntiPlace%i_%i,0); " i j i j
    else 
      Printf.fprintf gspn "(RE_Place%i_%i,0);(AntiPlace%i_%i,1); " i j i j;
  done;
done;
(*for j = 1 to l1+l2 do
  if j = l1 then Printf.fprintf gspn "(RE_nPlace%i,%i);" j n
  else Printf.fprintf gspn "(RE_nPlace%i,0);" j;
done;*)
Printf.fprintf gspn "(Puit,0); } ;

Transitions = {";

for i =1 to n do
  for j=1 to l1+l2-1 do
    if j>1 then Printf.fprintf gspn "(Avance%i_%i,EXPONENTIAL(p),1,1,SINGLE);\n " i j;
    if j<l1+l2-1 then Printf.fprintf gspn "(Recule%i_%i,EXPONENTIAL(q),1,1,SINGLE);\n " i j;
  done;
done;
Printf.fprintf gspn "(selfloop,EXPONENTIAL(0*RE_Place1_1),1,1,SINGLE);\n ";
Printf.fprintf gspn "(Puittrans ,EXPONENTIAL(0*RE_Place1_1),1,1,SINGLE);\n   } ;

InArcs = { ";
for i =1 to n do
  for j = 1 to l1+l2-1 do 
    if j> 1 then (
      Printf.fprintf gspn "(RE_Place%i_%i, Avance%i_%i,1);\n " i j i j;
      Printf.fprintf gspn "(AntiPlace%i_%i, Avance%i_%i,1);\n " i (j+1) i j;
    (*Printf.fprintf gspn "(RE_nPlace%i, Avance%i_%i,1);\n " j i j;*)
      if i<n then Printf.fprintf gspn "(AntiPlace%i_%i, Avance%i_%i,1);\n " (i+1) j i j);
      
    if j< l1+l2-1 then (
      Printf.fprintf gspn "(AntiPlace%i_%i, Recule%i_%i,1);\n " i j i j;
      Printf.fprintf gspn "(RE_Place%i_%i, Recule%i_%i,1);\n " i (j+1) i j;
      (*Printf.fprintf gspn "(RE_nPlace%i, Recule%i_%i,1);\n " (j+1) i j;*)
    )
  done;
done;
(*Printf.fprintf gspn "(RE_nPlace%i, Absorbe,1);\n " (l1+l2);*)
Printf.fprintf gspn " } ;

OutArcs = { ";

for i =1 to n do
  for j = 1 to l1+l2-1 do 
    if j>1 then (
      Printf.fprintf gspn "(Avance%i_%i, RE_Place%i_%i,1);\n " i j i (j+1);
      (*Printf.fprintf gspn "(Avance%i_%i, RE_nPlace%i,1);\n " i j (j+1);*)
      Printf.fprintf gspn "(Avance%i_%i, AntiPlace%i_%i,1);\n " i j i j;
      if i<n then Printf.fprintf gspn "(Avance%i_%i, AntiPlace%i_%i,1);\n " i j (i+1) j;
    );

    if j<l1+l2-1 then (
      Printf.fprintf gspn "(Recule%i_%i, RE_Place%i_%i,1);\n " i j i j;
      (*Printf.fprintf gspn "(Recule%i_%i, RE_nPlace%i,1);\n " i j j;*)
      Printf.fprintf gspn "(Recule%i_%i, AntiPlace%i_%i,1);\n " i j i (j+1)
    );
  done;
done;

Printf.fprintf gspn "(Puittrans, Puit,1); } ;";
close_out gspn;

let lha = open_out "ruine.lha" in
let loosestate = ref "RE_Place1_1"
and winstate = ref (sprintf "RE_Place1_%i" (l1+l2)) in
for i=2 to n do
  winstate := sprintf "%s + RE_Place%i_%i" !winstate i (l1+l2);
  loosestate := sprintf "%s + RE_Place%i_1" !loosestate i;
done; 

Printf.fprintf lha "
NbVariables = 1;
NbLocations = 3;
";
Printf.fprintf lha "const int N=%i;" n;
Printf.fprintf lha "

VariablesList = {x} ;

LocationsList = {li, lp ,lm};
";

Printf.fprintf lha "AVG(Last(x));";

Printf.fprintf lha "
InitialLocations={li};

FinalLocations={ lp, lm };

Locations={\n";
Printf.fprintf lha "(li, ( %s + %s < N) & (Puit=0) );\n" !winstate !loosestate ;
Printf.fprintf lha "(lp, ( %s + %s = N) & (%s <  %s) & (Puit = 0));\n" !winstate !loosestate !loosestate !winstate;
Printf.fprintf lha "(lm, (%s + %s = N) & (%s >= %s) | (Puit > 0));\n" !winstate !loosestate !loosestate !winstate;

Printf.fprintf lha "
}; 

Edges={
((li,li)  ,ALL ,  # , #);
((li,lp)  ,ALL ,  # , {x=1});
((li,lm)  ,ALL ,  # , {x=0});
};";

close_out lha;

let gspn = open_out "ruine_agr.gspn" in

fprintf gspn "
const double p = %f;
const double q = %f;
const int l = %i;
const int n = %i;

NbPlaces = %i;
NbTransitions = %i;

PlacesList = { " p q (l1+l2) n (l1+l2) ((l1+l2-2)*2);

for j = 1 to l1+l2 do
  fprintf gspn "RE_Place%i" j;
  if j< l1+l2  then fprintf gspn ", ";
done;
Printf.fprintf gspn " } ;

TransitionsList = { ";
  for j=1 to l1+l2-2 do
    fprintf gspn "Avance%i, " (j+1);
    fprintf gspn "Recule%i " j;
    if j< l1+l2-2  then fprintf gspn ", ";
  done;
Printf.fprintf gspn " } ;

Marking = { ";
  for j =1 to l1+l2 do
    if j = l1 then 
      Printf.fprintf gspn "(RE_Place%i,%i); " j n
    else 
      Printf.fprintf gspn "(RE_Place%i,0); " j;
  done;

Printf.fprintf gspn " } ;

Transitions = {";

  for j=1 to l1+l2-2 do
    fprintf gspn "(Avance%i,EXPONENTIAL(p*RE_Place%i),1,1,SINGLE);\n " (j+1) (j+1);
    fprintf gspn "(Recule%i,EXPONENTIAL(q*RE_Place%i),1,1,SINGLE);\n " j (j+1);
  done;

fprintf gspn "   } ;

InArcs = { ";

  for j = 1 to l1+l2-2 do 
    Printf.fprintf gspn "(RE_Place%i, Avance%i,1);\n " (j+1) (j+1);
    Printf.fprintf gspn "(RE_Place%i, Recule%i,1);\n " (j+1) (j);
  done;
Printf.fprintf gspn " } ;

OutArcs = { ";

  for j = 1 to l1+l2-2 do 
    Printf.fprintf gspn "(Avance%i, RE_Place%i,1);\n " (j+1) (j+2);
    Printf.fprintf gspn "(Recule%i, RE_Place%i,1);\n " j j;
  done;

Printf.fprintf gspn " } ;";

close_out gspn;

let lha_agr = open_out "ruine_agr.lha" in
Printf.fprintf lha_agr "
NbVariables = 1;
NbLocations = 3;
const int N=%i;

VariablesList = {x} ;

LocationsList = {li, lp ,lm};
AVG(Last(x));
InitialLocations={li};

FinalLocations={ lp };

Locations={
(li, ( RE_Place1 + RE_Place%i < N) );
(lp, ( RE_Place1 + RE_Place%i = N) & (RE_Place1 <  RE_Place%i));
(lm, ( RE_Place1 + RE_Place%i = N) & (RE_Place1 >= RE_Place%i));

}; 

Edges={
((li,li)  ,ALL ,  # , #);
((li,lp)  ,ALL ,  # , {x=1});
((li,lm)  ,ALL ,  # , {x=0});
};" n (l1+l2) (l1+l2) (l1+l2) (l1+l2) (l1+l2);
close_out lha_agr;

let prisme = open_out "ruine.sm" in
Printf.fprintf prisme "ctmc

const int n = %i;
const int l1 = %i;
const int l2 = %i;

const double p = %f;
const double q = %f;


module Ruine
" n l1 l2 p q;

for i = 0 to n-1 do
Printf.fprintf prisme 
  "\tJoueur%i : [0..(l1+l2-1)] init l1-1;\n" i;
done;

(*for j = 0 to l1+l2-1 do
Printf.fprintf prisme 
  "\tnJoueur%i : [0..n] init %i;\n" j (if j=l1-1 then n else 0);
done;*)

for i = 0 to n-1 do
  if i< n-1 then Printf.fprintf prisme 
    "\t[Avance%i] Joueur%i>0 & Joueur%i<(l1+l2-1) & Joueur%i != Joueur%i -> p : (Joueur%i'=Joueur%i+1);\n" i i i i (i+1) i i
  else Printf.fprintf prisme 
    "\t[Avance%i] Joueur%i>0 & Joueur%i<(l1+l2-1) -> p : (Joueur%i'=Joueur%i+1);\n" i i i i i;

  Printf.fprintf prisme 
    "\t[Recule%i] Joueur%i>0 & Joueur%i<(l1+l2-1) -> q : (Joueur%i'=Joueur%i-1);\n" i i i i i;
  
done;

Printf.fprintf prisme "endmodule
formula dl = ( Joueur0 = 0 | Joueur0 = (l1+l2-1) )";
for i =1 to n-1 do Printf.fprintf prisme "& ( Joueur%i = 0 | Joueur%i = (l1+l2-1) )" i i; done;

Printf.fprintf prisme ";
formula win = dl & ( Joueur0 ";
for i =1 to n-1 do Printf.fprintf prisme "+ Joueur%i" i; done;
Printf.fprintf prisme ">((l1+l2-1)*n)/2);
formula loose = dl & !win;";

close_out prisme;

let prisme_decol = open_out "ruine_agr.sm" in
Printf.fprintf prisme_decol "ctmc
const int n = %i;

const double p = %f;
const double q = %f;

module File
" n p q;
for i = 1 to l1 +l2 do
  if i = l1 then Printf.fprintf prisme_decol "\t place%i : [0..n] init n;\n" i
  else Printf.fprintf prisme_decol "\t place%i : [0..n] init 0;\n" i;
done;

for i=1 to l1+l2-1 do
  if i>1 then  Printf.fprintf prisme_decol "\t [Avance%i] place%i>0 & place%i<n -> place%i*p : (place%i'=place%i -1) & (place%i'= place%i +1);\n" i i (i+1) i i i (i+1) (i+1);

  if i<l1+l2-1 then Printf.fprintf prisme_decol "\t [Recule%i] place%i>0 & place%i<n -> place%i*q : (place%i'=place%i -1) & (place%i'= place%i +1);\n" i (i+1) i (i+1) (i+1) (i+1) i i;

done;
Printf.fprintf prisme_decol "\t[deadlock] true -> 1 : (place1'=place1);
endmodule

formula win = (place%i+place%i=n) & (place%i>place%i);\n" (l1+l2) 1 (l1+l2) 1;
Printf.fprintf prisme_decol "formula loose = (place%i+place%i=n) & (place%i<=place%i);\n" (l1+l2) 1 (l1+l2) 1;
close_out prisme_decol;

let com1 =  "Cosmos ruine_agr.gspn ruine_agr.lha -s > logcosmos  2>&1" in
  print_endline com1;
  ignore (Sys.command com1);;

output_string log "genere ok\n";

(*execu "prism ruine_agr.sm ../ruine.csl -fixdl -v -maxiters 100000000 > prismout";;
output_string log "prisme ok\n";;

execu "../traduittable.ml prismout";;
output_string log "traduction ok\n";;*)

close_out log;;

let exist = try 
	      Unix.mkdir dir 0o770;
	      false;
  with _ -> true;;

Unix.chdir dir;;

if not exist then genere ();;


let com2 = Printf.sprintf "Cosmos ruine.gspn ruine.lha --batch 1000 --width 0.1 -c -b %i --set-Horizon %f --epsilon %e > logcosmosCalc  2>&1" methode horizon epsilon;; 

print_endline com2;;
Sys.command com2;;


Sys.command "grep Memory logcosmosCalc";;
print_newline ();
Sys.command "grep Mean logcosmosCalc";;

