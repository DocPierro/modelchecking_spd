#!/usr/bin/ocaml
#load "unix.cma";;
open Printf

let epsilon = 1e-10;;


let nparraljob = 12;;
let joblist = [
  50, 50, 0.34 , 0 , 0 , 2;
  100, 50, 0.34 , 100 , 0 , 2;
  200, 50, 0.34 , 100 , 0 , 2; 


];;

let lauchsim (n,r,mu,h,m,k) =
  ignore (Sys.command (sprintf "ocaml tandem.ml %i %i %f %i %i %i %e" n r mu h m k epsilon));
  printf "Finish tandem.ml %i %i %f %i %i %i %e\n" n r mu h m k epsilon;;



List.iter lauchsim joblist;;


