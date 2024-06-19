#!/usr/bin/ocaml
#load "unix.cma";;
open Printf

let epsilon = 1e-10;;
let p= 0.3;;
let width = 0.5;;

let nparraljob = 12;;
let joblist = [
(*  2 , 10 , 10.0 , 3 ;
  2 , 10 , 10.0 , 2 ;
  2 , 10 , 10.0 , 1 ;

  3 , 10 , 15.0 , 3 ;
  3 , 10 , 15.0 , 2 ;
  3 , 10 , 15.0 , 1 ;

  4 , 10 , 20.0 , 3 ;
  4 , 10 , 20.0 , 2 ;
  4 , 10 , 20.0 , 1 ;

  5 , 10 , 25.0 , 3 ;
  5 , 10 , 25.0 , 2 ;
  5 , 10 , 25.0 , 1 ;

  6 , 10 , 30.0 , 3 ;
  6 , 10 , 30.0 , 2 ;
  6 , 10 , 30.0 , 1 ;

  7 , 10 , 35.0 , 3 ;
  7 , 10 , 35.0 , 2 ;
  7 , 10 , 35.0 , 1 ;

  8 , 10 , 40.0 , 3 ;
  8 , 10 , 40.0 , 2 ;
  8 , 10 , 40.0 , 1 ;

  9 , 10 , 45.0 , 3 ;
  9 , 10 , 45.0 , 2 ;
  9 , 10 , 45.0 , 1 ;*)

  12 , 10 , 60.0 , 2 ;

];;

let lauchsim (n,l,h,m) =
  ignore (Sys.command (sprintf "ocaml ruine2.ml %i %i %f %f %i %e %f" n l p h m epsilon width));;



List.iter lauchsim joblist;;


