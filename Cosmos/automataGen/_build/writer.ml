open Type
open Automata
open Printf

let rec write_linConstr = function
  | [] -> ()
  | (i,c,fexpr)::q ->
    printf "vc%i" i;
    print_cmp c;
    print_float_expr fexpr;
    if List.length q >0 then printf "&";
    write_linConstr q

let rec write_flow = function
  | [] -> ()
  | (i,fexpr)::q ->
    printf "vc%i:" i;
    print_float_expr fexpr;
    if List.length q >0 then printf ",";
    write_flow q


let writeAutomata a =
  printf "NbVariables = %i;\n" (a.nbContVar + a.nbDiscVar);
  printf "NbLocations = %i;\n" a.nbLoc;
  
  printf "VariablesList = {";
  for i = 0 to a.nbContVar-1 do
    if i>0 then printf ",";
    print_float_expr (ContinuousVar(i));
  done;
  for i = 0 to a.nbDiscVar-1 do
    if i>0 || a.nbContVar>0  then printf ",";
    print_string " DISC ";
    print_float_expr (DiscreteVar(i));
  done;
  printf "};\n";
  
  printf "LocationsList = {";
  for i = 0 to a.nbLoc-1 do
    if i>0 then printf ",";
    printf "l%i" i;
  done;
  printf "};\n";
  
  List.iter print_HASL a.haslForm;

  printf "InitialLocations = { l%i };\n" a.init;

  printf "FinalLocations = {";
  print_list (fun x ->printf "l%i" x) (fun () -> print_string ",") a.final;
  printf "};\n";

  printf "Locations = {\n";
  for i = 0 to a.nbLoc-1 do
    printf "(l%i," i;
    begin 
      try print_sf (List.assoc i a.invariant)
      with Not_found -> printf "TRUE"; 
    end;
    begin
    try let lflows = List.assoc i a.flows in 
	if lflows <> [] then (
	  printf ",(";
	  write_flow lflows;
	  printf ")";
	)
    with Not_found -> ();
    end;
    printf ");\n"
  done;
  printf "};\n";

  printf "Edges = {\n";
  List.iter (fun (l1,tt,tuc,tud,l2) -> 
    printf "((l%i,l%i)," l1 l2;
    (match tt with
      Synch(lab,sf) -> print_labels lab;
	printf ",";
	if sf = True then print_string "#"
	else print_sf sf;
    | Autonomous(lconstr) -> printf "#,";
       write_linConstr lconstr;
    );
    printf ",";
    if tuc = [] && tud =[] then printf "#"
    else (
      printf "{";
      print_list (fun (v,u) -> printf "vc%i = " v; print_float_expr u;) (fun () -> print_string ",") tuc;
      if tuc  <> [] && tud  <> [] then print_string ",";
      print_list (fun (v,u) -> printf "vd%i = " v; print_float_expr u;) (fun () -> print_string ",") tud;
      printf "}";
    );
    printf ");\n"
  ) a.trans;
  printf "};\n";
