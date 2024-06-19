{
  open Lexing
  open ParserSimulEdge

  exception SyntaxError  of string
}

let digit = ['0'-'9']
let white = [' ' '\t']+
let newline = '\r' | '\n' | "\r\n"

rule token = parse
    white    {token lexbuf}
  | newline  {new_line lexbuf; token lexbuf}
  | ['/']['/'][^'\n']*    {token lexbuf}
  | "..." {token lexbuf}
  | ['-' '+']?digit+ as lxm  { INT(int_of_string lxm)}
  | ['-' '+']?digit*['.']?digit+(['e' 'E']['-' '+']?digit+)? as lxm  {FLOAT(float_of_string lxm)}
  | "int" {INTKW}
  | "double" {DOUBLEKW}
  | "const" {CONST}
  | "bool" {BOOL}
  | "send" {SEND}
  | "after" {AFTER}
  | "exp"   {EXP}
  | '"'      { read_string (Buffer.create 17) lexbuf }
  | "'" {PRIME}
  | '(' {LPAR}
  | ')' {RPAR}
  | '+' {PLUS}
  | '-' {MINUS}
  | '*' {MULT}
  | '/' {DIV}
  | '[' {LSQBRAK}
  | ']' {RSQBRAK}
  | '{' {LBRAK}
  | '}' {RBRAK}
  | ';' {SEMICOLON}
  | ',' {COMMA}
  | '!' {NOT}
  | "true" {TRUE}
  | "false" {FALSE}
  | '&' {AND}
  | '|' {OR}
  | '=' {EQ}
  | '<' {SL}
  | '>' {SG}
  | "<=" {LE}
  | ">=" {GE}
  | "->" {ARROW}
  | ":" {COLON}
  | "function" {FUNCTION}
  | "end" {ENDFUN}
  | ['a'-'z' 'A'-'Z' '_' '0'-'9']* ['a'-'z' 'A'-'Z'] ['a'-'z' 'A'-'Z' '_' '0'-'9']* as lxm {
    (*let open Type in
	try begin match StringMap.find lxm !mapType with
	    IntT   -> INTNAME(lxm)
	  | BoolT  -> BOOLNAME(lxm)
	  | DoubleT-> DOUBLENAME(lxm)
	end with
	    Not_found ->*) NAME(lxm)
}
  | _ { raise (SyntaxError ("Unexpected char: " ^ Lexing.lexeme lexbuf)) }
  | eof  {EOF}
and read_string buf = parse
  | '"'       { STRING (Buffer.contents buf) }
  | '\\' '/'  { Buffer.add_char buf '/'; read_string buf lexbuf }
  | '\\' '\\' { Buffer.add_char buf '\\'; read_string buf lexbuf }
  | '\\' 'b'  { Buffer.add_char buf '\b'; read_string buf lexbuf }
  | '\\' 'f'  { Buffer.add_char buf '\012'; read_string buf lexbuf }
  | '\\' 'n'  { Buffer.add_char buf '\n'; read_string buf lexbuf }
  | '\\' 'r'  { Buffer.add_char buf '\r'; read_string buf lexbuf }
  | '\\' 't'  { Buffer.add_char buf '\t'; read_string buf lexbuf }
  | [^ '"' '\\']+
    { Buffer.add_string buf (Lexing.lexeme lexbuf);
      read_string buf lexbuf
    }
  | _ { raise (SyntaxError ("Illegal string character: " ^ Lexing.lexeme lexbuf)) }
  | eof { raise (SyntaxError ("String is not terminated")) }
