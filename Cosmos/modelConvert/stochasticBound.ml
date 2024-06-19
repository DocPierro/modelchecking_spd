open Type

type format = Prism | Dot | Pdf
type fonctCM = Bornes |Debug| GaussSeidel of string  |IntervalP | IsMC | MarkovC |VecInv of string*int                       

let input = ref stdin
let output = ref "out"
let inname = ref "stdin"
let typeFormat = ref Prism
let outputFormat = ref [Dot]
let statespace = ref ""
let verbose = ref 1
let application = ref [MarkovC]
let label = ref ""
		     
let suffix_of_filename s =
  let fa = String.rindex s '.'+1 in
  (String.sub s 0  (fa -1)),(
  String.sub s fa (String.length s - fa))

let nbarg = ref 0
	       
let _ = 
  Arg.parse [
	     "--pdf",Arg.Unit (fun () -> outputFormat:= Pdf:: !outputFormat),"Output as PDF";
	     "--state-space", Arg.Set_string statespace, "Compute state space of the model";
	     "-v",Arg.Set_int verbose,"Set verbose level default 1";
	     "--debug",Arg.Unit (fun () -> application:= Debug:: !application), " Calcul PP PM PSUP PINF "; 
	     "--gauss-seidel",Arg.String (fun (s) -> application:= (GaussSeidel s ):: !application), "Use gaussSeidel Algorithm on MDP, parametres mdp and int ";
	     "--markov-chain",Arg.Unit (fun () -> application:= IsMC:: !application), " Test if it is a Markov Chains"; 
	     "--vec-inv",Arg.Tuple ([	Arg.String (fun s ->label :=s );
			 		Arg.Int (fun nbcoup -> application:= (VecInv (!label, nbcoup)):: !application) 
			 	   ])  , " Calcule Pi like  Pi = 1t (1-P)¹";  
	     "--borne-st",Arg.Unit (fun () -> application:= Bornes:: !application), " PstarSup  et PstarInf";
	     "--imc",Arg.Unit (fun () -> application:= IntervalP:: !application), " Interval Markov Chains of the mdp ->  Calcul P+ and P-";    
    ]
            (function s -> incr nbarg; match !nbarg with
              | 1 -> inname:= s;
	             ( let o,suf = suffix_of_filename s in 
	               output := o;
	               match suf with 
	                 "sm" | "pm" | "nm" | "prism" -> typeFormat := Prism
	                 | _-> failwith "Unsupported file format"
	             )
              | 2 -> output := s
              | _ -> failwith "Do not know what to do with extra arguments.") 
    "usage";;

let _ =
  if !verbose>1 then begin
    Printf.printf "Verbose level: %i\n" !verbose;
   end;

logout := open_out (!output^".log");;

let _ =
  print_endline ("Opening "^ !inname);
  (*let s = Printf.sprintf "prism %s -exportprism %s.expanded  -nobuild > /dev/null" !inname !inname in
  Printf.printf "Using prism to expand file :%s\n" s;
  if( Sys.command s = 0) then
      inname := !inname ^ ".expanded"
    else Printf.printf "Fail to start prism to expand the file\n";*)
  input := open_in !inname;

  let prismfile = PrismTrans.read_prism !input !inname in
  let model = PrismTrans.prism_to_pt prismfile in
   let mdp =try
                       let net2 = PlainStochasticPetriNet.of_spn model in
                       (PlainStochasticPetriNet.SemanticGSPN.get_lts
                          ?labelling:(Some prismfile.PrismType.labels)
                          ?formulas:(Some prismfile.PrismType.formulas) net2)
                     with
                       FailToEvaluate -> Printf.fprintf stdout "fail to evaluate as Plain SPN\n";
                       Simulation.SemanticSPT.get_lts  ?labelling:(Some prismfile.PrismType.labels) ?formulas:(Some prismfile.PrismType.formulas)model
                   in
  
  Printf.printf  "State-space size:%i\n" (Array.length mdp.LTS.states);
  if not (Sys.file_exists  "result") then  Printf.printf "Création result :  %d \n " (Sys.command "mkdir result") ;
  LTS.computeBound mdp; 
	LTS.print_dot "result/DEBUG.dot" mdp; 
	
 
  print_endline "Finish transformation, start writing";
	
  List.iter (function 
             | Dot ->
                if !statespace <> "" then 
                  LTS.print_dot (!statespace^".dot") mdp;
	        StochPTPrinter.print_spt_dot ((!output)^".dot") model [] []
             | Pdf -> 
	        StochPTPrinter.print_spt_dot ((!output)^".dot") model [] [];
	        ignore @@ Sys.command (Printf.sprintf "dot -Tpdf %s.dot -o %s.pdf" !output !output);
                if !statespace <> "" then  
                  ignore @@ Sys.command (Printf.sprintf "dot -Tpdf %s.dot -o %s.pdf" !statespace !statespace)
             | _ -> failwith "Cannot export"
    ) !outputFormat; 

	List.iter (function 
		   |IsMC ->  Printf.printf " Chaine de markov ?  %b\n " (LTS.isMarkovChain mdp)
		   |GaussSeidel i ->  LTS.print_dot "mdp.dot" mdp; LTS.applyGaussSeidel  mdp i (!inname)
		   |Debug ->  LTS.printAppli mdp 1 
		   |VecInv (lab, nb) ->  LTS.applyVecInv mdp lab nb  
		   |Bornes -> LTS.printAppli mdp 3 
		   |IntervalP -> LTS.printAppli mdp 4 
		   |_ -> ()

	) !application
