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

open Parsing;;
let _ = parse_error;;
# 2 "parser.mly"
  open Type

# 35 "parser.ml"
let yytransl_const = [|
  260 (* TRUE *);
  261 (* FALSE *);
  262 (* LPAR *);
  263 (* RPAR *);
  264 (* PLUS *);
  265 (* MINUS *);
  266 (* MULT *);
  267 (* DIV *);
  268 (* LSQBRAK *);
  269 (* RSQBRAK *);
  270 (* EOL *);
  271 (* SEMICOLON *);
  272 (* UNTIL *);
  273 (* EVENTUALLY *);
  274 (* NEXT *);
  275 (* AND *);
  276 (* OR *);
  277 (* NOT *);
  278 (* EQ *);
  279 (* SG *);
  280 (* SL *);
  281 (* GE *);
  282 (* LE *);
    0|]

let yytransl_block = [|
  257 (* INT *);
  258 (* FLOAT *);
  259 (* NAME *);
    0|]

let yylhs = "\255\255\
\001\000\002\000\002\000\002\000\002\000\002\000\002\000\002\000\
\002\000\002\000\004\000\004\000\004\000\004\000\004\000\004\000\
\003\000\003\000\003\000\003\000\003\000\003\000\003\000\007\000\
\007\000\007\000\007\000\007\000\006\000\006\000\006\000\006\000\
\006\000\006\000\005\000\005\000\000\000"

let yylen = "\002\000\
\002\000\003\000\004\000\002\000\003\000\002\000\003\000\003\000\
\003\000\003\000\002\000\002\000\002\000\002\000\002\000\005\000\
\001\000\001\000\003\000\003\000\002\000\003\000\003\000\001\000\
\001\000\001\000\001\000\001\000\001\000\001\000\003\000\003\000\
\003\000\003\000\001\000\001\000\002\000"

let yydefred = "\000\000\
\000\000\000\000\029\000\030\000\017\000\018\000\000\000\000\000\
\000\000\000\000\037\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\021\000\001\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\024\000\025\000\
\026\000\027\000\028\000\000\000\008\000\022\000\031\000\000\000\
\036\000\035\000\000\000\013\000\014\000\012\000\015\000\011\000\
\000\000\000\000\009\000\000\000\000\000\000\000\019\000\000\000\
\000\000\000\000\000\000\033\000\000\000\000\000\000\000\000\000\
\000\000\016\000"

let yydgoto = "\002\000\
\011\000\012\000\013\000\026\000\051\000\014\000\044\000"

let yysindex = "\014\000\
\051\255\000\000\000\000\000\000\000\000\000\000\051\255\041\255\
\041\255\070\255\000\000\142\255\144\255\118\255\021\255\003\255\
\113\255\070\255\047\255\047\255\047\255\047\255\047\255\047\255\
\040\255\070\255\040\255\070\255\000\000\000\000\051\255\051\255\
\041\255\070\255\070\255\015\255\015\255\015\255\000\000\000\000\
\000\000\000\000\000\000\015\255\000\000\000\000\000\000\127\255\
\000\000\000\000\009\255\000\000\000\000\000\000\000\000\000\000\
\040\255\040\255\000\000\016\255\040\255\070\255\000\000\065\255\
\015\255\076\255\076\255\000\000\161\255\047\255\040\255\158\255\
\075\255\000\000"

let yyrindex = "\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\036\255\000\000\071\255\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\096\255\097\255\000\000\006\255\117\255\000\000\000\000\063\255\
\000\000\073\255\093\255\000\000\138\255\000\000\141\255\000\000\
\000\000\000\000"

let yygindex = "\000\000\
\000\000\007\000\255\255\003\000\237\255\249\255\000\000"

let yytablesize = 171
let yytable = "\017\000\
\052\000\053\000\054\000\055\000\056\000\016\000\025\000\027\000\
\029\000\046\000\017\000\028\000\010\000\015\000\001\000\003\000\
\048\000\004\000\033\000\010\000\065\000\034\000\035\000\070\000\
\057\000\010\000\058\000\045\000\066\000\067\000\068\000\061\000\
\063\000\064\000\031\000\062\000\069\000\059\000\060\000\031\000\
\032\000\003\000\004\000\004\000\005\000\006\000\018\000\049\000\
\050\000\004\000\073\000\003\000\019\000\004\000\005\000\006\000\
\007\000\072\000\034\000\035\000\071\000\010\000\020\000\021\000\
\022\000\023\000\024\000\008\000\009\000\020\000\003\000\010\000\
\004\000\005\000\006\000\018\000\020\000\006\000\020\000\032\000\
\032\000\032\000\020\000\034\000\006\000\038\000\032\000\074\000\
\032\000\000\000\010\000\032\000\032\000\000\000\032\000\032\000\
\032\000\032\000\032\000\034\000\034\000\034\000\005\000\007\000\
\000\000\000\000\034\000\000\000\034\000\005\000\007\000\034\000\
\034\000\000\000\034\000\034\000\034\000\034\000\034\000\047\000\
\036\000\037\000\038\000\002\000\000\000\036\000\037\000\038\000\
\000\000\000\000\002\000\000\000\000\000\046\000\039\000\040\000\
\041\000\042\000\043\000\039\000\040\000\041\000\042\000\043\000\
\023\000\034\000\035\000\003\000\000\000\000\000\000\000\023\000\
\000\000\023\000\003\000\030\000\023\000\023\000\000\000\033\000\
\031\000\032\000\034\000\035\000\047\000\036\000\037\000\038\000\
\036\000\037\000\038\000"

let yycheck = "\007\000\
\020\000\021\000\022\000\023\000\024\000\007\000\008\000\009\000\
\010\000\007\001\018\000\009\000\007\001\007\000\001\000\001\001\
\018\000\003\001\016\001\014\001\006\001\019\001\020\001\015\001\
\026\000\020\001\028\000\007\001\036\000\037\000\038\000\033\000\
\034\000\035\000\019\001\033\000\044\000\031\000\032\000\019\001\
\020\001\001\001\007\001\003\001\004\001\005\001\006\001\001\001\
\002\001\014\001\070\000\001\001\012\001\003\001\004\001\005\001\
\006\001\065\000\019\001\020\001\062\000\021\001\022\001\023\001\
\024\001\025\001\026\001\017\001\018\001\007\001\001\001\021\001\
\003\001\004\001\005\001\006\001\014\001\007\001\016\001\007\001\
\008\001\009\001\020\001\019\001\014\001\010\001\014\001\013\001\
\016\001\255\255\021\001\019\001\020\001\255\255\022\001\023\001\
\024\001\025\001\026\001\007\001\008\001\009\001\007\001\007\001\
\255\255\255\255\014\001\255\255\016\001\014\001\014\001\019\001\
\020\001\255\255\022\001\023\001\024\001\025\001\026\001\007\001\
\008\001\009\001\010\001\007\001\255\255\008\001\009\001\010\001\
\255\255\255\255\014\001\255\255\255\255\007\001\022\001\023\001\
\024\001\025\001\026\001\022\001\023\001\024\001\025\001\026\001\
\007\001\019\001\020\001\007\001\255\255\255\255\255\255\014\001\
\255\255\016\001\014\001\014\001\019\001\020\001\255\255\016\001\
\019\001\020\001\019\001\020\001\007\001\008\001\009\001\010\001\
\008\001\009\001\010\001"

let yynames_const = "\
  TRUE\000\
  FALSE\000\
  LPAR\000\
  RPAR\000\
  PLUS\000\
  MINUS\000\
  MULT\000\
  DIV\000\
  LSQBRAK\000\
  RSQBRAK\000\
  EOL\000\
  SEMICOLON\000\
  UNTIL\000\
  EVENTUALLY\000\
  NEXT\000\
  AND\000\
  OR\000\
  NOT\000\
  EQ\000\
  SG\000\
  SL\000\
  GE\000\
  LE\000\
  "

let yynames_block = "\
  INT\000\
  FLOAT\000\
  NAME\000\
  "

let yyact = [|
  (fun _ -> failwith "parser")
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'expr) in
    Obj.repr(
# 36 "parser.mly"
           (_1)
# 212 "parser.ml"
               : Type.formula))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'stateCondition) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'stateCondition) in
    Obj.repr(
# 40 "parser.mly"
                                      (Until(_1,_3) )
# 220 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : 'stateCondition) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'timeinterval) in
    let _4 = (Parsing.peek_val __caml_parser_env 0 : 'stateCondition) in
    Obj.repr(
# 41 "parser.mly"
                                                    (BoundedUntil(_1,_3,_4))
# 229 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'stateCondition) in
    Obj.repr(
# 42 "parser.mly"
                             (Until(True,_2) )
# 236 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'timeinterval) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'stateCondition) in
    Obj.repr(
# 43 "parser.mly"
                                          (BoundedUntil(True,_2,_3) )
# 244 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'stateCondition) in
    Obj.repr(
# 44 "parser.mly"
                       (Next(_2))
# 251 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'timeinterval) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'stateCondition) in
    Obj.repr(
# 45 "parser.mly"
                                    (BoundedNext(_2,_3))
# 259 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'expr) in
    Obj.repr(
# 46 "parser.mly"
                  (_2)
# 266 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'expr) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 47 "parser.mly"
                 (AndF(_1,_3))
# 274 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'expr) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 48 "parser.mly"
                (OrF(_1,_3))
# 282 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'floatexpr) in
    Obj.repr(
# 53 "parser.mly"
                ((Float(0.0),_2))
# 289 "parser.ml"
               : 'timeinterval))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'floatexpr) in
    Obj.repr(
# 54 "parser.mly"
                ((Float(0.0),_2))
# 296 "parser.ml"
               : 'timeinterval))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'floatexpr) in
    Obj.repr(
# 55 "parser.mly"
                ((_2,_2))
# 303 "parser.ml"
               : 'timeinterval))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'floatexpr) in
    Obj.repr(
# 56 "parser.mly"
                ((_2,Infty))
# 310 "parser.ml"
               : 'timeinterval))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'floatexpr) in
    Obj.repr(
# 57 "parser.mly"
                ((_2,Infty))
# 317 "parser.ml"
               : 'timeinterval))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 3 : 'floatexpr) in
    let _4 = (Parsing.peek_val __caml_parser_env 1 : 'floatexpr) in
    Obj.repr(
# 58 "parser.mly"
                                                 ((_2,_4))
# 325 "parser.ml"
               : 'timeinterval))
; (fun __caml_parser_env ->
    Obj.repr(
# 61 "parser.mly"
          (True)
# 331 "parser.ml"
               : 'stateCondition))
; (fun __caml_parser_env ->
    Obj.repr(
# 62 "parser.mly"
           (False)
# 337 "parser.ml"
               : 'stateCondition))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'stateCondition) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'stateCondition) in
    Obj.repr(
# 63 "parser.mly"
                                     (And(_1,_3))
# 345 "parser.ml"
               : 'stateCondition))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'stateCondition) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'stateCondition) in
    Obj.repr(
# 64 "parser.mly"
                                    (Or(_1,_3))
# 353 "parser.ml"
               : 'stateCondition))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'stateCondition) in
    Obj.repr(
# 65 "parser.mly"
                      (Not(_2))
# 360 "parser.ml"
               : 'stateCondition))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'stateCondition) in
    Obj.repr(
# 66 "parser.mly"
                            (_2)
# 367 "parser.ml"
               : 'stateCondition))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'intexpr) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'cmp) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'intexpr) in
    Obj.repr(
# 67 "parser.mly"
                        ( IntAtom(_1,_2,_3) )
# 376 "parser.ml"
               : 'stateCondition))
; (fun __caml_parser_env ->
    Obj.repr(
# 71 "parser.mly"
     (EQ)
# 382 "parser.ml"
               : 'cmp))
; (fun __caml_parser_env ->
    Obj.repr(
# 72 "parser.mly"
      (SG)
# 388 "parser.ml"
               : 'cmp))
; (fun __caml_parser_env ->
    Obj.repr(
# 73 "parser.mly"
      (SL)
# 394 "parser.ml"
               : 'cmp))
; (fun __caml_parser_env ->
    Obj.repr(
# 74 "parser.mly"
      (GE)
# 400 "parser.ml"
               : 'cmp))
; (fun __caml_parser_env ->
    Obj.repr(
# 75 "parser.mly"
      (LE)
# 406 "parser.ml"
               : 'cmp))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : int) in
    Obj.repr(
# 79 "parser.mly"
      (Int(_1))
# 413 "parser.ml"
               : 'intexpr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 80 "parser.mly"
        (IntName(_1))
# 420 "parser.ml"
               : 'intexpr))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'intexpr) in
    Obj.repr(
# 81 "parser.mly"
                     (_2)
# 427 "parser.ml"
               : 'intexpr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'intexpr) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'intexpr) in
    Obj.repr(
# 82 "parser.mly"
                        (Plus(_1,_3))
# 435 "parser.ml"
               : 'intexpr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'intexpr) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'intexpr) in
    Obj.repr(
# 83 "parser.mly"
                        (Mult(_1,_3))
# 443 "parser.ml"
               : 'intexpr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'intexpr) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'intexpr) in
    Obj.repr(
# 84 "parser.mly"
                         (Minus(_1,_3))
# 451 "parser.ml"
               : 'intexpr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : float) in
    Obj.repr(
# 88 "parser.mly"
         (Float(_1))
# 458 "parser.ml"
               : 'floatexpr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : int) in
    Obj.repr(
# 89 "parser.mly"
       (Float(float_of_int _1))
# 465 "parser.ml"
               : 'floatexpr))
(* Entry main *)
; (fun __caml_parser_env -> raise (Parsing.YYexit (Parsing.peek_val __caml_parser_env 0)))
|]
let yytables =
  { Parsing.actions=yyact;
    Parsing.transl_const=yytransl_const;
    Parsing.transl_block=yytransl_block;
    Parsing.lhs=yylhs;
    Parsing.len=yylen;
    Parsing.defred=yydefred;
    Parsing.dgoto=yydgoto;
    Parsing.sindex=yysindex;
    Parsing.rindex=yyrindex;
    Parsing.gindex=yygindex;
    Parsing.tablesize=yytablesize;
    Parsing.table=yytable;
    Parsing.check=yycheck;
    Parsing.error_function=parse_error;
    Parsing.names_const=yynames_const;
    Parsing.names_block=yynames_block }
let main (lexfun : Lexing.lexbuf -> token) (lexbuf : Lexing.lexbuf) =
   (Parsing.yyparse yytables 1 lexfun lexbuf : Type.formula)
