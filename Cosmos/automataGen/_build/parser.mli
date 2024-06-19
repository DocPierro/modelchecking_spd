type token =
  | INT of (int)
  | FLOAT of (float)
  | NAME of (string)
  | TRUE
  | FALSE
  | LPAR
  | RPAR
  | PLUS
  | MINUS
  | MULT
  | DIV
  | LSQBRAK
  | RSQBRAK
  | EOL
  | SEMICOLON
  | UNTIL
  | EVENTUALLY
  | NEXT
  | AND
  | OR
  | NOT
  | EQ
  | SG
  | SL
  | GE
  | LE

val main :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> Type.formula
