{
  open Lexing
  open Cless_ast
  open Cless_parser

  let parse_error lexbuf s =
    Printf.ksprintf
      (fun s ->
         let loc = lexeme_start_p lexbuf, lexeme_end_p lexbuf in
         raise (Parse_error (loc, s)))
      s

  let identifier lexbuf = function
    | "bool" -> KWbool
    | "else" -> KWelse
    | "false" -> KWfalse
    | "if" -> KWif
    | "int" -> KWint
    | "true" -> KWtrue
    | "while" -> KWwhile
    | s -> IDENT s
}

rule token = parse
  | [' ' '\t' '\r']* { token lexbuf }
  | '\n' { new_line lexbuf; token lexbuf }
  | "(*" { comment lexbuf; token lexbuf }
  | ['a'-'z' 'A'-'Z' '_'] ['a'-'z' 'A'-'Z' '_' '0'-'9']* as x
      { identifier lexbuf x }
  | "!" { BANG }
  | "(" { LPAR }
  | ")" { RPAR }
  | "*" { STAR }
  | "+" { PLUS }
  | "-" { MINUS }
  | "/" { DIV }
  | ";" { SEMI }
  | "<" { LT }
  | "=" { EQUAL }
  | ">" { GT }
  | "{" { LBRACE }
  | "}" { RBRACE }
  | ['0'-'9']+ as x
      { try
          INT (int_of_string x)
        with Failure x ->
          parse_error lexbuf "Invalid integer literal: `%s'" x }
  | eof { EOF }
  | _ as c { parse_error lexbuf "Invalid character: `%c'" c }

and comment = parse
  | "*)" { }
  | "(*" { comment lexbuf; comment lexbuf }
  | '\n' { new_line lexbuf; comment lexbuf }
  | eof { parse_error lexbuf "Unterminated comment" }
  | _ { comment lexbuf }
