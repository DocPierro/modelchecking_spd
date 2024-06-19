#!/usr/bin/ocaml
#load "unix.cma";;
open Printf

let nparraljob = 12;;
let joblist = [
  1000, 10, 0.8 , 1000 , 1 ;
  1000, 10, 0.8 , 2000 , 1 ; 
  1000, 10, 0.8 , 3000 , 1 ;
  1000, 10, 0.8 , 5000, 1 ;
];;

let lauchsim (n,r,mu,h,m) =
  ignore (Sys.command (sprintf "./tandem.ml %i %i %f %i %i" n r mu h m));
  printf "Finish tandem.ml %i %i %f %i %i\n" n r mu h m;;



List.iter lauchsim joblist;;


