#!/usr/bin/ocaml
#load "unix.cma";;

for i = 48 to 1000 do
  let com = Printf.sprintf "Cosmos tandem.gspn tandem.lha -b 1 --set-Horizon %i --batch 5000 --max-run 5000 > loggraph%i" i i in
  Sys.command com;
  Sys.command "grep \"value:\" test | sed \"s/Estimated value: //\" >> resultgraph";
done;


