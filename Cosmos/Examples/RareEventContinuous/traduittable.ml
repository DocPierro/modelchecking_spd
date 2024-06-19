#!/opt/local/bin/ocaml

let pt = open_in Sys.argv.(1)
and out = open_out "mu_table" in

let b = ref true in
while !b do
  let p = input_line pt in
  try if "Probabilities (non-zero only) for all states:" = (String.sub p 0 45) then b := false; with _ -> ();
done;

(try while true do
    let p = input_line pt in
    let n = String.index p '(' in
    Printf.fprintf out "%s\n" (String.sub p n (String.length p -n));
  done;
with
    Not_found -> (););
close_in pt;
close_out out;;
