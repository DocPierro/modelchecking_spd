open Xml
open Type
open PrismType
open PetriNet
open Lexing


let print_position outx lexbuf =
  let pos = lexbuf.lex_curr_p in
  Printf.fprintf outx "%s:%d:%d" pos.pos_fname
    pos.pos_lnum (pos.pos_cnum - pos.pos_bol + 1);;

let parseXML xi t = 
  let ei = 
    xi
    |> Xml.parse_string
    |> GrMLParser.parse_Grml_expr t in
  match ei with
      None -> ()
    | Some e -> let e2 = eval e in
		Printf.fprintf stdout "#%a#%a#" printH_expr e2 
		  StochPTPrinter.print_expr e2

		  
let parseXML_distr xi =
  let e2 = 
    xi
    |> Xml.parse_string
    |> (fun x -> GrMLParser.parse_distr [x])  in
  Printf.fprintf stdout "#%a#%a#" StochPTPrinter.printH_distr e2 
    StochPTPrinter.print_distr e2
		  

let parse_expr s name =
  let lexbuf = Lexing.from_channel s in
  lexbuf.lex_curr_p <- { lexbuf.lex_curr_p with pos_fname = name };
  try while true do
      begin try
	      let e = ParserPrism.parseCmd LexerPrism.token lexbuf in
	      begin match e with
		  ParseInt(ei) -> Printf.fprintf stdout "#%a#%a#" printH_expr ei 
		    StochPTPrinter.print_expr ei
		| ParseFloat(ei) -> Printf.fprintf stdout "#%a#%a#" printH_expr ei 
		  StochPTPrinter.print_expr ei
		| ParseDistr(dist,fl) ->
		  let ei = begin match dist,fl with
		      ("EXPONENTIAL",[f]) -> StochasticPetriNet.Exp(f)
		    | ("IMMEDIATE",[]) -> StochasticPetriNet.Imm
		    | ("IMDT",[]) -> StochasticPetriNet.Imm
                    | ("MASSACTION",[f]) -> StochasticPetriNet.Exp(f)
		    | ("DETERMINISTIC",[f]) -> StochasticPetriNet.Det(f)
		    | _ -> failwith ("ill define distribution"^dist)
		  end in
		  Printf.fprintf stdout "#%a#%a#" StochPTPrinter.printH_distr ei 
		    StochPTPrinter.print_distr ei
		| XMLInt(xi) -> parseXML xi (Int 0)
		| XMLFloat(xi) -> parseXML xi (Float 0.0)
		| XMLDistr(xi) -> parseXML_distr xi
		| _ -> raise End_of_file
	      end
	with 
	  | LexerPrism.SyntaxError msg ->
	    Printf.printf "%a: %s\n" print_position lexbuf msg;
	  | Parsing.Parse_error ->
	    Printf.printf "%a: Parsing error: unexpected token:'%s'\n"
	      print_position lexbuf (lexeme lexbuf);
	  | Failure x -> print_endline x
      end;
      print_newline();
    done;
  with
  | End_of_file ->exit 0;;

allInt:=true;;

parse_expr stdin "stdin"
