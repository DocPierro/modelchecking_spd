{
  open Lexing
  open Preparser

  exception SyntaxError  of string
}

let digit = ['0'-'9']
let white = [' ' '\t']+
let newline = '\r' | '\n' | "\r\n"

rule token = parse
    white    {token lexbuf}
  | newline  {new_line lexbuf; token lexbuf}
  | ['/']['/'][^'\n']*    {token lexbuf}
  | digit+ as lxm  { INT(int_of_string lxm)}
  | digit*['.']?digit+(['e' 'E']['-' '+']?digit+)? as lxm  {FLOAT(float_of_string lxm)}
  | "ctmc" | "dtmc" | "pta" | "mdp" {CTMC} 
  | "int" {INTKW}
  | "clock" {CLOCK}
  | "double" {DOUBLEKW}
  | "const" {CONST}
  | "formula" {FORMULA}
  | "module" {MODULE} | "endmodule" {ENDMODULE}
  | "rewards" {REWARDS} | "endrewards" {ENDREWARDS}
  | "invariant" {INVARIANT} | "endinvariant" {ENDINVARIANT}
  | "label" {LABEL}
  | "init" {INIT} | "endinit" {ENDINIT}
  | "bool" {BOOL}
  | "floor" {FLOOR} | "ceil" {CEIL}
  | '"'      { read_string (Buffer.create 17) lexbuf }
  | ".." {RANGE}
  | "'" {PRIME}
  | '(' {LPAR}
  | ')' {RPAR}
  | '+' {PLUS}
  | '-' {MINUS}
  | '*' {MULT}
  | '/' {DIV}
  | '?' {QMARK}
  | '[' {LSQBRAK}
  | ']' {RSQBRAK}
  | ';' {SEMICOLON}
  | ',' {COMMA}
  | '!' {NOT}
  | "!=" {NEQ}
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
  | ['a'-'z' 'A'-'Z'] ['a'-'z' 'A'-'Z' '_' '0'-'9']* as lxm {
    NAME(lxm)
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
