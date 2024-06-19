#!/opt/local/bin/ocaml
#load "unix.cma";;
(*#use "../script.ml" *)
open Printf

let n = int_of_string (Sys.argv.(1));;
let r = int_of_string (Sys.argv.(2));;

let mu = float_of_string (Sys.argv.(3));;

let horizon = int_of_string (Sys.argv.(4));;

let methode = int_of_string (Sys.argv.(5));;

let rho1 = (1. -. mu) /. 2. ;;
let rho2 = (1. -. mu) /. 2. ;;

let dir = sprintf "tandem%i_%i_%.2f_%i_%i" n r mu horizon methode;;


let genere () =
  let gspnagr = open_out "tandem_agr.gspn" in

  Printf.fprintf gspnagr "const double mu   = %f;
const double rho1 = %f;
const double rho2   = %f;
const int r = %i;
" mu rho1 rho2 r;

  Printf.fprintf gspnagr "
NbPlaces = 3;
NbTransitions = 3;

PlacesList = { Queue1, Queue2 ,AQueue2 } ;
TransitionsList = { mu, rho1, rho2};

Marking= {
(Queue1 ,1);
(Queue2 ,0);
(AQueue2,r);
};

Transitions={
(mu , EXPONENTIAL(mu),1,1,SINGLE);
(rho1  , EXPONENTIAL(rho1),1,1,SINGLE);
(rho2 , EXPONENTIAL(rho2),1,1,SINGLE);
};


InArcs={
(Queue1    ,rho1,1);
(AQueue2   ,rho1,1);
(Queue2   ,rho2,1);
};

OutArcs={
(mu ,Queue1   ,1);
(rho1,Queue2   ,1);
(rho2,AQueue2  ,1);
};
";
  close_out gspnagr;

  let lhaagr = open_out "tandem_agr.lha" in
  Printf.fprintf lhaagr "

NbVariables = 1;
NbLocations = 3;
const int N= %i;
VariablesList = {x} ;

LocationsList = {l1, lp, lm };AVG(Last(x));
InitialLocations={l1};

FinalLocations={ lp  };

Locations={
(l1, (Queue1 + Queue2 > 0) & (Queue1 + Queue2 < N) ) ;
(lp, (Queue1 + Queue2 = N)  );
(lm, (Queue1 + Queue2 = 0)  );
}; 

Edges={
((l1,l1)  ,ALL,  # , #);
((l1,lp)  ,ALL,  # , {x=1});
((l1,lm)  ,ALL,  # , {x=0});
};" n;
  close_out lhaagr;

  let gspn = open_out "tandem.gspn" in
  Printf.fprintf gspn "
const double Lambda   = %f;
const double rho1 = %f;
const double rho2   = %f;
const int r = %i;
" mu rho1 rho2 r;

  Printf.fprintf gspn "
NbPlaces = 7;
NbTransitions = 8;

PlacesList = { Queue1, Queue2 , RE_Queue1, RE_Queue2 , RE_AntiQueue2 ,ComplQueue2,Puit} ;
TransitionsList = { LambdaT , rho1T ,rho1Tp,dummyrho1 , rho2T ,rho2Tp,dummyrho2, Puittrans};

Marking= {
(Queue1 ,1);
(Queue2 ,0);
(RE_Queue1   ,1);
(RE_Queue2   ,0);
(RE_AntiQueue2   ,r);
(ComplQueue2 ,0);
(Puit ,0);
};

Transitions={
(LambdaT , EXPONENTIAL(Lambda),1,1,SINGLE);
(rho1T  , EXPONENTIAL(rho1),1,1,SINGLE);
(rho2T , EXPONENTIAL(rho2),1,1,SINGLE);
(rho1Tp  , EXPONENTIAL(rho1),1,1,SINGLE);
(rho2Tp , EXPONENTIAL(rho2),1,1,SINGLE);
(dummyrho1  , EXPONENTIAL(rho1),1,1,SINGLE);
(dummyrho2  , EXPONENTIAL(rho1),1,1,SINGLE);
(Puittrans, EXPONENTIAL(0*Queue1),1,1,SINGLE);
};


InArcs={
(Queue1    ,rho1T,1);
(RE_Queue1 ,rho1T,1);
(RE_AntiQueue2,rho1T,1);

(Queue1    ,rho1Tp,1);

(Queue2   ,rho2T,1);
(RE_Queue2,rho2T,1);

(Queue2      ,rho2Tp,1);
(RE_Queue1   ,rho2Tp,1);
(ComplQueue2 ,rho2Tp,1);
};

OutArcs={
(LambdaT,Queue1   ,1);
(LambdaT,RE_Queue1,1);

(rho1T,Queue2   ,1);
(rho1T,RE_Queue2,1);

(rho1Tp,Queue2 ,1);
(rho1Tp,ComplQueue2,1);

(rho2T,RE_AntiQueue2 ,1);

(Puittrans, Puit,1);
};

InhibitorArcs={
(RE_AntiQueue2,rho1Tp,1);
(RE_AntiQueue2,rho2Tp,1);
(ComplQueue2,rho2T,1);
(Queue1,dummyrho1,1);
(Queue2,dummyrho2,1);
};";
  close_out gspn;

  let lha = open_out "tandem.lha" in
  Printf.fprintf lha "

NbVariables = 1;
NbLocations = 3;
const int N= %i;
VariablesList = {x} ;

LocationsList = {l1, lp, lm };AVG(Last(x));
InitialLocations={l1};

FinalLocations={ lp,lm };

Locations={
(l1, (Queue1 + Queue2 > 0) & (Queue1 + Queue2 < N) & (Puit = 0)) ;
(lp, (Queue1 + Queue2 = N) & (Puit = 0) );
(lm, (Queue1 + Queue2 = 0) | (Puit > 0) );
}; 

Edges={
((l1,l1)  ,ALL,  # , #);
((l1,lp)  ,ALL,  # , {x=1});
((l1,lm)  ,ALL,  # , {x=0});
};" n;
  close_out lha;
  
  let com1 =  "CosmosGPP tandem_agr.gspn tandem_agr.lha -s > logcosmos  2>&1" in
  print_endline com1;
  ignore (Sys.command com1);;



let exist = try 
	      Unix.mkdir dir 0o770;
	      false;
  with _ -> true;;

Unix.chdir dir;;

if not exist then genere ();;


let com2 = Printf.sprintf "CosmosGPP tandem.gspn tandem.lha --batch 1000 --max-run 1000 -b %i --set-Horizon %i > logcosmosCalc  2>&1" methode horizon;; 

print_endline com2;;
Sys.command com2;;


Sys.command "grep Memory logcosmosCalc";;
print_newline ();
Sys.command "grep Mean logcosmosCalc";;

