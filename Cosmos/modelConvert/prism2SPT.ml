open Type

type format = Prism | Pnml | Dot | Marcie | Simulink | Pdf | GrML

let input = ref stdin
let output = ref "out"
let inname = ref "stdin"
let typeFormat = ref Prism
let outputFormat = ref [GrML;Dot;Pdf;Marcie]

let suffix_of_filename s =
  let fa = String.rindex s '.'+1 in
  (String.sub s 0  (fa -1)),(
  String.sub s fa (String.length s - fa))

let nbarg = ref 0

let _ = 
  Arg.parse []
    (function s -> incr nbarg; match !nbarg with
      1 -> inname:= s;
	( let o,suf = suffix_of_filename s in 
	  output := o;
	  match suf with
	  "sm" | "pm" -> typeFormat := Prism
	| _-> failwith "Unsupported file format"
	)
    | 2 -> output := s
    | _ -> failwith "Do not know what to do with extra arguments.") 
    "usage";;
  
if Array.length Sys.argv =2 then
  outputFormat := Pdf :: (List.filter (fun x -> x<>Pdf && x <> Dot) !outputFormat)

let _ =
  print_endline ("Opening "^ !inname);
  begin match !typeFormat with
  | Prism -> 
    input := open_in !inname;
    PrismTrans.read_prism !input !inname
  |> PrismTrans.prism_to_pt
  | _ -> failwith "Format not yet supported" end
	      |> (fun net -> 
		print_endline "Finish parsing, start writing";
		List.iter (function 
	        | Dot ->
		  StochPTPrinter.print_spt_dot ((!output)^".dot") net [] []
		| Pdf -> 
		  StochPTPrinter.print_spt_dot ((!output)^".dot") net [] [];
		  ignore (Sys.command (Printf.sprintf "dot -Tpdf %s.dot -o %s.pdf" !output !output))
		| GrML ->
		  StochPTPrinter.print_spt ((!output)^".grml") net
		| Marcie -> 
		  StochPTPrinter.print_spt_marcie ((!output)^".andl") net
		| _ -> print_endline "Output format not supported"
		) !outputFormat;
		print_endline "Finish writing.";
	      );;
