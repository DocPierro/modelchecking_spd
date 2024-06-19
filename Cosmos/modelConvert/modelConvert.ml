open Type

type format = Prism | Pnml | Dot | Marcie | Stateflow | Pdf | GrML | CLess | Simulink

let input = ref stdin
let output = ref "out"
let inname = ref "stdin"
let typeFormat = ref Prism
let outputFormat = ref [Dot;Marcie]
let const_file = ref ""
let verbose = ref 1
let inHibitor = ref true
let traceSize = ref 0
let simule = ref 0
let labels = ref ""
let statespace = ref ""
let stochasticBound = ref false
		     
let suffix_of_filename s =
  let fa = String.rindex s '.'+1 in
  (String.sub s 0  (fa -1)),(
  String.sub s fa (String.length s - fa))

let nbarg = ref 0

	       
let _ = 
  Arg.parse ["--light",Arg.Set StateflowType.lightSim,"light simulator";
	     "--pdf",Arg.Unit (fun () -> outputFormat:= Pdf:: !outputFormat),"Output as PDF";
	     "--prism",Arg.Unit (fun () -> outputFormat:= Prism:: !outputFormat),"Output in Prism File Format";
	     "--pnml",Arg.Unit (fun () -> outputFormat:= Pnml:: !outputFormat),"Output in Pnml File Format";
	     "--grml",Arg.Unit (fun () -> outputFormat:= GrML:: !outputFormat),"Output in Pnml File Format";
	     "--andl",Arg.Unit (fun () -> outputFormat:= Marcie:: !outputFormat),"Output in Marcie File Format";
	     "--stoch",Arg.Set StateflowType.modelStoch,"Use probabilistic delay";
	     "--trace",Arg.Set_int traceSize, "Generate a trace of the model";
	     "--simule",Arg.Set_int simule, "Simulate trajectories";
	     "--state-space", Arg.Set_string statespace, "Compute state space of the model";
             "--stochastic-bound", Arg.Set stochasticBound, "Compute bounding matrix";
	     "--no-erlang",Arg.Clear StateflowType.useerlang,"Replace erlang distribution by exponentials";
	     "--no-imm",Arg.Set StateflowType.doremoveImm,"Remove Instantaneous transition in prims model";
	     "--no-inhib",Arg.Clear inHibitor,"Remove inhibitor arcs";
	     "--erlang-step",Arg.Set_int StateflowTrans.erlangstep,"Number of erlang step for stochastic model";
	     "-v",Arg.Set_int verbose,"Set verbose level default 1";
	     "--add-reward",Arg.Set StateflowType.add_reward, "Add reward transition to each non immediate transition";
	     "--mdp-strat", Arg.Set_string Simulation.mdpstrat, "MDP strategy load/save file";
	     "--mdp-det-strat", Arg.Set Simulation.detstrat, "Use deterministic strategy";
             "--mdp-prism-strat", Arg.Set_string LoadAdversary.strat_name, "Load a prism strategy";
             "--labels", Arg.Set_string labels, "add labels to the models";
	    ]
    (function s -> incr nbarg; match !nbarg with
      1 -> inname:= s;
	( let o,suf = suffix_of_filename s in 
	  output := o;
	  match suf with
	    "sm" | "pm" | "nm" | "prism" -> typeFormat := Prism
	    | "c" -> typeFormat := CLess
	| "pnml" -> typeFormat := Pnml
	| "slx" -> 
	  typeFormat := Stateflow;
	  const_file := "const.m"
	| "gml" | "grml" -> typeFormat := GrML
	| _-> failwith "Unsupported file format"
	)
    | 2 -> output := s
    | _ -> failwith "Do not know what to do with extra arguments.") 
    "usage";;

let _ = Simulation.MdpOp.read_strat ();;
let _ = Simulation.PrismMdpOp.read_strat ();;

  
(*if Array.length Sys.argv <>2 then
  outputFormat := (List.filter (fun x -> x<>Pdf && x <> Dot) !outputFormat);;
*)

let _ =
  if !verbose>1 then begin
    Printf.printf "Verbose level: %i\n" !verbose;
    Printf.printf "Light Sim: %b\n" !StateflowType.lightSim;
    Printf.printf "Stochastic transition: %b\n" !StateflowType.modelStoch;
    Printf.printf "Use Erlang: %b\n" !StateflowType.useerlang;
  end;

logout := open_out (!output^".log");;

let prism_labelling = ref [];;
let prism_formulas = ref [];;
  
let _ =
  print_endline ("Opening "^ !inname);
  begin match !typeFormat with
  | Prism -> 
    let s = Printf.sprintf "prism %s -exportprism %s.expanded  -nobuild > /dev/null" !inname !inname in
    Printf.printf "Using prism to expand file :%s\n" s;
    (*if( Sys.command s = 0) then
      inname := !inname ^ ".expanded"
    else Printf.printf "Fail to start prism to expand the file\n";*)
    input := open_in !inname;
    PrismTrans.read_prism !input !inname
    |< PrismType.print_prism !logout
    |< (fun fp -> prism_labelling := fp.PrismType.labels)
    |< (fun fp -> prism_formulas := fp.PrismType.formulas)
    |> PrismTrans.prism_to_pt
  | Pnml ->
     (*IFNDEF HAS_XML THEN
      failwith "XML library required to read PNML" 
      ELSE*)
      let xmlf = Pnmlparser.tree_of_pnml !inname in
	  let net = PetriNet.Net.create () in
	  let open StochasticPetriNet in
	  net.PetriNet.Net.def <- Some {intconst=[]; floatconst=[]; clock=[]; labels=[];formulas=[]; classDef=[]; printer = (fun _ _ -> ()); };
	  Pnmlparser.net_of_tree net xmlf;
	  net
  (* ENDIF *)
  | GrML -> (try
  (*  IFNDEF HAS_XML THEN
      failwith "XML library required to read GrML" 
      ELSE *)
  let xmlf = GrMLParser.tree_of_pnml !inname in
      let net = PetriNet.Net.create () in
       let open StochasticPetriNet in
       net.PetriNet.Net.def <- (Some {
				 intconst=[];
				 floatconst=[];
				 clock=[];
                                 labels=[];formulas=[];
                                 classDef=[];
				 printer = (fun _ _ -> ())
			       });
      GrMLParser.net_of_tree net xmlf;
      net
          with
            Xml.Error(e) as exn -> output_string stderr ((Xml.error e)^"\n"); raise exn)
  (* ENDIF *)
  | Stateflow -> 
   (* IFNDEF HAS_XML THEN
      failwith "XML library required to read Stateflow file" 
      ELSE IFNDEF HAS_ZIP THEN
      failwith "ZIP library required to read Stateflow file" 
      ELSE*)
      begin 
	Zip.open_in !inname
	|> (fun z -> (
	    try Zip.find_entry z "simulink/stateflow.xml"
	    with Not_found -> Zip.find_entry z "simulink/blockdiagram.xml")
		     |> Zip.read_entry z)
	|> Xml.parse_string
	|> Stateflowparser.modulist_of_tree []
	|> StateflowTrans.expand_trans
	|> List.map StateflowTrans.flatten_module
	(*|> List.map StateflowTrans.flatten_state_ssid*)

	|< List.iter (StateflowType.print_module !logout)
	|> List.map StateflowTrans.incr_state
	|> StateflowTrans.find_combinaison
	|< (fun x -> if List.mem Prism !outputFormat then
	    x 
	       |> (fun x -> if !StateflowType.doremoveImm then 
		   List.map StateflowTrans.removeImm x else x)
	       |> StateflowTrans.print_prism_module ((!output)^".sm") !const_file)
	|> List.map StateflowTrans.prune_unread2
      	|> (fun x -> List.fold_left StateflowTrans.combine_modu2 (List.hd x) (List.tl x))
	|> StateflowTrans.expand_trans2
    (*|> (fun x->Stateflowparser.print_simulink_dot2 ((!output)^".dot") [x])*)
    (*|< Stateflowparser.prune*) 
	|> StateflowTrans.stochNet_of_modu !const_file
      end
     (* ENDIF
      ENDIF *)
  | CLess ->
     let ast = try
	 Cless.expr_list_of_file !inname
	 |< (fun x ->ignore (List.fold_left Cless2PT.sanity StringMap.empty x))
       with
       | Cless_ast.Parse_error(l,m) as x -> Cless.print_location Format.std_formatter l;
					    print_endline m;
					    raise x
       | Cless2PT.Type_error(l,m) as x -> Cless.print_location Format.std_formatter l;
				 print_endline m;
				 raise x;
     in
	     let net = PetriNet.Net.create () in
	     let pstart = PetriNet.Net.add_place net ("PSTA") (Int(1),Some (Int(1))) in
	     let pend = PetriNet.Net.add_place net ("PEND") (Int(0),Some (Int(1))) in
     ignore(Cless2PT.build_spt_list net StringMap.empty pstart pend ast);
     net
  | _ -> failwith "Input format not yet supported" end
  |< (fun _-> print_endline "Finish parsing, start transformation")
  |> (fun x-> if !StateflowType.useerlang then x else StochasticPetriNet.remove_erlang x)
  (*|> (fun x-> if !add_reward then StochasticPetriNet.add_reward_struct x; x)*)

  |< (fun net -> if !statespace <> "" || !stochasticBound then
		   let lts = try
                       let net2 = PlainStochasticPetriNet.of_spn net in
                       (PlainStochasticPetriNet.SemanticGSPN.get_lts ?labelling:(Some (!prism_labelling)) ?formulas:(Some (!prism_formulas)) net2)
                     with
                       FailToEvaluate -> Printf.fprintf stdout "fail to evaluate as Plain SPN\n";
                       Simulation.SemanticSPT.get_lts  ?labelling:(Some (!prism_labelling)) ?formulas:(Some (!prism_formulas)) net
                   in
                   if !statespace <> "" then begin
                       LTS.print_dot (!statespace^".dot") lts;
		       Printf.printf  "State-space states :%i\n" (Array.length lts.LTS.states);
                       Printf.printf  "State-space transitions:%i\n"
                                      (Array.fold_left (fun i t ->
                                           List.fold_left (fun j e -> j+List.length e.LTS.distr) i t 
                                         ) 0 lts.LTS.transitions);
                       (*LTS.compact_mdp lts;*)
                     end;
                   if !stochasticBound then LTS.computeBound lts;
                   (*for i = 0 to PetriNet.Data.size net.PetriNet.Net.place -1 do 
	             PetriNet.Data.unsafe i
	             |> PetriNet.Data.acca net.PetriNet.Net.place
	             |> fst
	             |> print_string;
                     print_string ", ";
                   done;
                   print_newline ();
                   List.iter (fun x ->
			      Array.iter (fun y ->
					  Simulation.SptOp.print_marking stdout y;
					 output_string stdout ", ") x; print_newline ()) list*)
     )    
  |< (fun net -> if !simule<> 0 then let open Simulation.SemanticSPT in
				     Printf.printf "Simulate %n trajectories:\n" !simule;
                                     let nbsucc = ref 0 in
                                     for i = 1 to !simule / 100 do                                     
				       let succ =
				         simulate net 100
				         |> List.filter (function Some true -> true |_-> false)
				         |> List.length in
                                       Printf.printf "Result: %f" ((float succ) /. (100.0));
                                       print_newline ();
                                       nbsucc := !nbsucc + succ
                                     done;
                                     Printf.printf "Final Result: %f\n" ((float !nbsucc) /. (float !simule));
                                     (*Simulation.MdpOp.consolidate_strat ();*)
                                     Simulation.MdpOp.print_state_mdp ()
                                     
     )
 (* |< (fun _ -> let open LoadAdversary in
               if !strat_name <>"" then
                 let full_strat = loadFullStrat (!strat_name) in
                 StateMap.iter (fun m s -> Printf.printf "(%a) -> %s\n" (print_int_array ",") m s) full_strat.strat
     )*)

  
  |< (fun net -> if !traceSize <> 0 then let open Simulation.SemanticSPT in
     try
	print_endline "Trace:";
	let m0 = init net in	  
	print_marking net stdout m0;
	print_newline ();
	(*let en = enabled net m0 in
	List.iter (fun t -> print_endline (fst (PetriNet.Data.acca net.PetriNet.Net.transition t))) en;*)
	let l,result = trace  net m0 !traceSize in
	List.iter (fun (en,t,m) ->
		   let pt x = (PetriNet.Data.acca net.PetriNet.Net.transition x) in
		   print_string "{";
		   List.iter (fun x -> let name,distr = pt x in Printf.printf "%s(%a), " name (Simulation.SptOp.print_distr net m) distr) en;
		   print_string "}->";
		   print_endline (fst (pt t));
		   print_marking net stdout m;
		   print_newline ()) l;
	match result with
	  None -> print_endline "Unfinished trajectory"
	| Some(true) -> print_endline "Accepting trajectory"
	| Some(false) -> print_endline "Rejecting trajectory"
      with
	Illegal_fire(tr,(pm,_),v) ->
	let tn,_ = PetriNet.Data.acca net.PetriNet.Net.transition tr in
	Printf.printf "tr:%s m:%a v:%a\n" tn printH_expr pm printH_expr v)
  |> (fun x -> if !inHibitor then x else StochasticPetriNet.remove_inhibitor x)
  |> (fun net -> 
      print_endline "Finish transformation, start writing";
      List.iter (function 
      | Dot ->
	StochPTPrinter.print_spt_dot ((!output)^".dot") net [] []
      | Pdf -> 
	StochPTPrinter.print_spt_dot ((!output)^".dot") net [] [];
	ignore @@ Sys.command (Printf.sprintf "dot -Tpdf %s.dot -o %s.pdf" !output !output);
        if !statespace <> "" then  
          ignore @@ Sys.command (Printf.sprintf "dot -Tpdf %s.dot -o %s.pdf" !statespace !statespace);
      | GrML ->
	StochPTPrinter.print_spt ((!output)^".grml") net
      | Pnml ->
	 net
	 |> StochPTPrinter.print_pnml ((!output)^".pnml")
      | Marcie -> 
	StochPTPrinter.print_spt_marcie ((!output)^".andl") net
      | Prism when !typeFormat = Stateflow -> ()
      | _ -> print_endline "Output format not supported"
      ) !outputFormat;
      print_endline "Finish writing.";
    );;
