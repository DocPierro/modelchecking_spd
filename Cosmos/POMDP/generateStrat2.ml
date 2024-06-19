open Str;;
#load "str.cma";;

let dots = regexp ":\t";;
let confintdel = regexp "\\[\\| , \\|\\]";;

type result = { mutable mean : float;
                mutable stdDev : float;
                mutable confInt: float*float;
              };;

let parse_result f =
  let fs = open_in f in
  let result = {  mean= 0. ;  stdDev = 0. ; confInt = 0. , 0. } in
  (try while true do
      let str = input_line fs in
      let ls = split dots str in
      match ls with
        | "Prism Result" :: v :: [] -> let x = float_of_string v in
				       result.mean <- x;
				       result.confInt <- (x-.0.0001 , x+. 0.0001)
        | "Estimated value" :: v :: [] -> result.mean <- (float_of_string v)
        | "Standard deviation" :: v :: [] -> result.stdDev <- (float_of_string v)
        | "Confidence interval" :: v :: [] ->
          (match split confintdel v with
            | a::b::_ -> result.confInt <- (float_of_string a , float_of_string b)
            | _ -> Printf.printf "Fail to parse confidence interval %s\n" v)
        | "Binomiale confidence interval" :: v :: [] ->
          (match split confintdel v with
            | a::b::_ -> result.confInt <- (float_of_string a , float_of_string b)
            | _ -> Printf.printf "Fail to parse confidence interval %s\n" v)
        | _ -> ()
    done
   with
       End_of_file -> close_in fs);
  result;;

let invoke_cosmos lha w =
  let cmd = Printf.sprintf "../bin/Cosmos --width %f -v 0 --njob 12 --batch 100 generated.gspn %s" w lha in
  (*print_endline cmd;*)
  ignore (Sys.command cmd);
  parse_result  "Result.res";;  


let rec insere_l a b = function
  | [] -> [(a,b)]
  | (ta,tb)::q when ta<a -> (a,b)::(ta,tb)::q
  | t::q -> t::(insere_l a b q);;

let apply_fst l s n =
  for k=0 to n-1 do
    let _,(i,x,j) = List.nth l k in
    s.(i).(x).(j) <- not s.(i).(x).(j)
  done;;

let generateLHA strat file ron reward discounted horizon =
  let fd = open_out file in
  let fp = Printf.fprintf in

  let n = Array.length strat in
  let xLastMax = Array.length strat.(0) -1 in

  fp fd "NbVariables = %i;\n" 2;
  fp fd "NbLocations = %i;\n" (n*(xLastMax+1)+1);

  fp fd "const double H = %f;\n" horizon;

  fp fd "VariablesList = {time, Reward} ;\n";
  fp fd "LocationsList = { lf ";
  for i = 0 to n-1 do for x = 0 to xLastMax do 
      fp fd ", lp%i_%i" i x;
    done; done;
  fp fd "};\n";

  fp fd "Reward=AVG(Last(Reward));\n";
  fp fd "InitialLocations = {lp0_0};\n";
  fp fd "FinalLocations = {lf};\n";
  fp fd "Locations={\n";
  fp fd "\t(lf, TRUE );\n";
  for i = 0 to n-1 do 
    fp fd "\t(lp%i_0, V%i =1 );\n" i i ;
    for x = 1 to xLastMax do 
      fp fd "\t(lp%i_%i, TRUE );\n" i x ;
    done;
  done;
  fp fd "};\n";
  fp fd "Edges={\n";
  for i = 0 to n-1 do
    for x = 0 to xLastMax do
      fp fd "\t((lp%i_%i,lf),ALL, time>=H, {Reward=Reward/H} );\n" i x;
    done;
  done;
  
  for i = 0 to n-1 do
    for x = 0 to xLastMax do
      let nbSensorOn = Array.fold_left  (fun x b -> if b then x+1 else x) 0 strat.(i).(x) in
      fp fd "\t((lp%i_%i ,lp%i_%i), ALL" i x i (min (x+1) xLastMax);
      
      let b =ref false in
      for j = 1 to n-1 do 
	if (strat.(i).(x).(j)) then
	  for xnb =0 to n-1 do
	    if !b then fp fd " ," else fp fd "\\{";
	    b := true;
	    fp fd "Tr%i_%i " xnb j;
	  done;
      done;
      if !b then fp fd "}";
      fp fd ", time <= H,";
      fp fd "{time = time+1, Reward = Reward + %i}" (ron*nbSensorOn);

      fp fd ");\n";
    

      for j =0 to n-1 do
	if strat.(i).(x).(j) then begin
	  fp fd "\t((lp%i_%i, lp%i_0) , ALL, time <= H, {" i x j;
	  fp fd "time = time+1, Reward = Reward + %i}" ((ron*nbSensorOn) + reward.(j));
	  fp fd ");\n";
	end;
      done;
    done;
  done;
  fp fd "};\n";
  close_out fd;;

(*  let fdi = open_in file in
  while true do 
      let line = input_line fdi in
      print_endline line;
  done;;*)


let allOn n m =
  Array.init n (fun i -> 
    Array.init m (fun x -> 
      Array.init n (fun j -> j>0)
    )
  );;


let iter_strat s r =
  let l = ref [] in
  generateLHA s "test" (-1) r 0 1000.;
  let roldStrat = invoke_cosmos "test" 0.002 in
  let snew =
    Array.init (Array.length s) (fun i ->
      Array.init (Array.length s.(0)) (fun x -> 
	Array.init (Array.length s.(0).(0)) (fun j ->
	  if j = 0 || i=0 then false
	  else begin 
	    s.(i).(x).(j) <- not s.(i).(x).(j);
	    generateLHA s "test" (-1) r 0 1000.;
	    let rneg = invoke_cosmos "test" 0.01 in
	    s.(i).(x).(j) <- not s.(i).(x).(j);
	    l := insere_l rneg.mean (i,x,j) !l;

	    Printf.printf "Test Sensor %i %i %i -> " i x j; 
	    print_string (string_of_bool s.(i).(x).(j));
	    Printf.printf "-> result old: %f result neg: %f ->" roldStrat.mean rneg.mean;
	    let rb = (if roldStrat.mean >= rneg.mean then s.(i).(x).(j) else not s.(i).(x).(j)) in
	    print_string (string_of_bool rb);
	    if rb <> s.(i).(x).(j) then print_string "  SWITCH ";
	    print_newline ();
	    rb
	  end;
	)
      )
    ) in (snew, !l);;

let print_bool b =
  if b then print_string "true "
  else print_string "false";;

let print_strat s =
  for i = 0 to Array.length s-1 do
    print_string "[| ";
    for j=0 to Array.length s.(0).(0)-1 do
      print_string "[| ";
      for x =0 to Array.length s.(0)-1 do
	if x>0 then print_string " ; ";
	print_bool s.(i).(x).(j);
      done;
      print_string "|]";
    done;
    print_endline "|]";
  done;;

(*generateLHA (allOn 6 2)  "test" (-1) [|0;15;15;15;15;15|] 0 1000.;;*)

(*invoke_cosmos "test";;*)

let buildit n r = 
  let stratFile = open_out "StratCaml" in
  let stratref =  ref (allOn (n+1) 2) in
  output_value stratFile !stratref;
  for i = 0 to 20 do
    let _,l = iter_strat !stratref (Array.create (n+1) r) in
    let s2 = !stratref in
    if List.length l = 0 then failwith "No more switch";
    apply_fst l s2 ((List.length l)/3);
    output_value stratFile s2;
    print_strat s2;
    stratref := s2
  done;
  close_out stratFile;;

let print_all_strat () = 
  let fs = open_in "StratCaml" in
  while true do 
    let s = input_value fs in
    print_strat s;
    print_newline ();
  done;;

buildit (int_of_string Sys.argv.(1)) (int_of_string Sys.argv.(2));;
