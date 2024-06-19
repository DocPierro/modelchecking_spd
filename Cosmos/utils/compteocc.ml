#!/usr/bin/ocaml 
#load "unix.cma";;

let s = Sys.argv.(1);;

let n = 10.0;;

let fin = open_in s in

let tabl = Hashtbl.create 10 in
let mean = ref 0. in
let i = ref 0 in

(try while true do
      let v = float_of_string (input_line fin) in
      incr i;
      mean := !mean +.v ;
      if true or  v>0. then
	let logv = log v in
	let approx = int_of_float (n *. logv) in
	if Hashtbl.mem tabl approx then
	  let i = Hashtbl.find tabl approx in
	  Hashtbl.replace tabl approx (i+1)
	else Hashtbl.add tabl approx 1;
  done with
    | End_of_file -> ()
    | x -> raise x;);

close_in fin;
let mean2 = !mean /. (float !i) in
let var = ref 0. in
let fin2 = open_in s in

(try while true do
      let v = float_of_string (input_line fin2) in
      var  := !var +. (v -. mean2) ** 2. ;
  done with
    | End_of_file -> ()
    | x -> raise x;);

close_in fin2;


Printf.printf "Nb trajectory: %i mean: " !i;
print_float mean2;
Printf.printf " Variance ";
print_float (!var /. (float !i));
Printf.printf " : ";
print_newline ();

let fout = open_out "loghisto2.dat" in
Hashtbl.iter (fun x y ->
  let s= string_of_float (exp ((float x)/. n)) in
  let s2 = string_of_float ((exp ((float x)/. n )*. (float y))) in
  let s3 = string_of_float (float y) in
  Printf.fprintf fout "%s %s %s\n" s s2 s3 ) tabl; 
close_out fout;;
      
Sys.command "gnuplot histogramme.plt";;
(*Sys.command "pdflatex histogramme.tex";;
Sys.command "evince histogramme.pdf";;*)

(*Unix.execvp "gnuplot" [|""; "histogramme.plt"|];;*)
