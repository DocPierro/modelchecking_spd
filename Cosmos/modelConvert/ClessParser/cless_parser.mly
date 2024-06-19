%{
  open Parsing
  open Cless_ast

  let loc () =
    symbol_start_pos (), symbol_end_pos ()

  let node x = {
    loc = loc ();
    node = x;
  }
%}

%token BANG
%token DIV
%token EOF
%token EQUAL
%token GT
%token <string> IDENT
%token <int> INT
%token KWbool
%token KWelse
%token KWfalse
%token KWif
%token KWint
%token KWtrue
%token KWwhile
%token LBRACE
%token LPAR
%token LT
%token MINUS
%token PLUS
%token RBRACE
%token RPAR
%token SEMI
%token STAR

%left PLUS MINUS
%left STAR DIV

%type <Cless_ast.expr_list> expr_list
%start expr_list

%type <Cless_ast.file> file
%start file
%%

file: | expr_list EOF { $1 };

int_simple_expr:
  | INT { (node (Const $1)) }
  | IDENT { (node (Var $1)) }
  | int_simple_expr PLUS int_simple_expr { (node (Plus ($1, $3))) }
  | int_simple_expr MINUS int_simple_expr { (node (Minus ($1, $3))) }
  | int_simple_expr STAR int_simple_expr { (node (Mult ($1, $3))) }
  | LPAR int_simple_expr RPAR { (node (Par $2)) };

bool_simple_expr:
  | KWtrue { (node True) }
  | KWfalse { (node False) }
  | BANG bool_simple_expr { (node (Neg $2)) }
  | IDENT { (node (BoolVar $1)) }
  | int_simple_expr EQUAL EQUAL int_simple_expr { (node (Eq ($1, $4))) }
  | int_simple_expr BANG EQUAL int_simple_expr { (node (Neq ($1, $4))) }
  | int_simple_expr LT EQUAL int_simple_expr { (node (Leq ($1, $4))) }
  | int_simple_expr GT EQUAL int_simple_expr { (node (Geq ($1, $4))) }
  | int_simple_expr LT int_simple_expr { (node (LL ($1, $3))) }
  | int_simple_expr GT int_simple_expr { (node (GG ($1, $3))) };

gen_expr:
  | int_simple_expr { (node (Int $1)) }
  | bool_simple_expr { (node (Bool $1)) };

expr:
  | IDENT EQUAL gen_expr SEMI { (node (VarAssign ($1, $3))) }
  | KWint IDENT EQUAL int_simple_expr SEMI { (node (IntdefFull ($2, $4))) }
  | KWint IDENT SEMI { (node (IntdefEmpty $2)) }
  | KWbool
    IDENT
    EQUAL
    bool_simple_expr
    SEMI
    { (node (BooldefFull ($2, $4))) }
  | KWbool IDENT SEMI { (node (BooldefEmpty $2)) }
  | KWif
    LPAR
    bool_simple_expr
    RPAR
    LBRACE
    expr_list
    RBRACE
    { (node (If ($3, $6))) }
  | KWif
    LPAR
    bool_simple_expr
    RPAR
    LBRACE
    expr_list
    RBRACE
    KWelse
    LBRACE
    expr_list
    RBRACE
    { (node (IfElse ($3, $6, $10))) }
  | KWwhile
    LPAR
    bool_simple_expr
    RPAR
    LBRACE
    expr_list
    RBRACE
    { (node (While ($3, $6))) };

expr_list: | parsini_0 { $1 };

parsini_0: | expr parsini_0 { ($1 :: $2) } | expr { ($1 :: []) };
