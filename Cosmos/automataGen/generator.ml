open Automata


let _ =
  let input = ref stdin in
  if Array.length Sys.argv >1 then
    input := open_in Sys.argv.(1);
  try
    let lexbuf = Lexing.from_channel !input  in
    while true do
      try
	let result = Parser.main Lexer.token lexbuf in
	print_string "%Initial formula:";
	Type.print_formula result;
	print_newline ();
	let a = Automata.automata_of_formula result in
	Writer.writeAutomata a;
	print_newline ();
      with Parsing.Parse_error -> print_endline "Fail to parse!"
    done
  with Lexer.Eof ->
    exit 0


(*
 let a =  Automata.automata_of_pltrlist ["Queue1";"Queue2"] [] 0.0 10.0 (int_of_string (Sys.argv.(2))) in
    Writer.writeAutomata a;
	print_newline ();
*)
