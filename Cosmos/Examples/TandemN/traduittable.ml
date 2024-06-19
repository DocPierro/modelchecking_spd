#!/opt/local/bin/ocaml

let nd = int_of_string Sys.argv.(3)
and pt = open_in Sys.argv.(1)
and out = open_out Sys.argv.(2);;


let b = ref true in
while !b do
  let p = input_line pt in
  try if "Probabilities (non-zero only) for all states:" = (String.sub p 0 45) then b := false; with _ -> ();
done;;

let rec fs = function 
    | 1 -> "0"
    | x -> fs (x-1)^",0";;

let mapping = Hashtbl.create ~random:false 10000;;
let st = Printf.sprintf "(%s)" (fs nd) in
Hashtbl.add mapping st 0.0;;

(try while true do
    let p = input_line pt in
    let n = String.index p '(' in
    let n2 = String.index p '=' in
    let f = float_of_string (String.sub p (n2+1) (String.length p -(n2+1))) in
    let s = String.sub p n (n2-n) in
    Hashtbl.add mapping s f;
    (*Printf.fprintf out "%s=%e\n" s f;*)
  done;
with
    Not_found -> (););;


let n = Hashtbl.length mapping in
Printf.fprintf out "[%i](" n;
let lastf = Hashtbl.fold (fun _ f os -> match os with
    None -> Some f
  | Some f2 -> (Printf.fprintf out "%g," f2; 
		Some f);
) mapping None in match lastf with
    Some f -> Printf.fprintf out "%g)\n" f
  |_ -> failwith "no state!";;

Hashtbl.fold (fun s _ i -> 
  Printf.fprintf out "%s=%i\n" s i; i+1) mapping 0;;


close_in pt;
close_out out;;
