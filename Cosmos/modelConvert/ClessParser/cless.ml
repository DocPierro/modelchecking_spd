open Lexing

let print_location fmt (b, e) =
  Format.fprintf fmt "File \"%s\", line %d, characters %d-%d:@."
    b.pos_fname b.pos_lnum (b.pos_cnum - b.pos_bol) (e.pos_cnum - b.pos_bol)

let dummy_location = Lexing.dummy_pos, Lexing.dummy_pos

let of_lexbuf entry lexbuf =
  try
    entry Cless_lexer.token lexbuf
  with
    | Parsing.Parse_error ->
        let loc = lexeme_start_p lexbuf, lexeme_end_p lexbuf in
        raise (Cless_ast.Parse_error (loc, "Parse error"))

let of_channel entry ch =
  of_lexbuf entry (Lexing.from_channel ch)

let of_string entry str =
  of_lexbuf entry (Lexing.from_string str)

let of_function entry f =
  of_lexbuf entry (Lexing.from_function f)

let of_file entry file =
  let ch = open_in file in
  try
    let lexbuf = Lexing.from_channel ch in
    lexbuf.lex_curr_p <- { lexbuf.lex_curr_p with pos_fname = file };
    let result = of_lexbuf entry lexbuf in
    close_in ch;
    result
  with exn ->
    close_in ch;
    raise exn

let expr_list_of_channel = of_channel Cless_parser.expr_list
let expr_list_of_file = of_file Cless_parser.expr_list
let expr_list_of_function = of_function Cless_parser.expr_list
let expr_list_of_string = of_string Cless_parser.expr_list

let file_of_channel = of_channel Cless_parser.file
let file_of_file = of_file Cless_parser.file
let file_of_function = of_function Cless_parser.file
let file_of_string = of_string Cless_parser.file

open Format

let print_int _ fmt x = fprintf fmt "%d" x
let print_string _ fmt x = fprintf fmt "%S" x
let print_bool _ fmt x = fprintf fmt "%B" x
let print_unit _ fmt () = fprintf fmt "()"
let print_node a par fmt x = a par fmt x.Cless_ast.node

let print_list a _ fmt x =
  match x with
    | [] ->
        fprintf fmt "[]";
    | x :: r ->
        fprintf fmt "@[<hv 2>[ ";
        fprintf fmt "%a" (a false) x;
        List.iter (fprintf fmt ";@ %a" (a false)) r;
        fprintf fmt " ]@]"

let print_option a par fmt x =
  match x with
    | None -> fprintf fmt "None"
    | Some x ->
        if par then fprintf fmt "(";
        fprintf fmt "@[<hv 2>Some@ %a@]" (a true) x;
        if par then fprintf fmt ")"

let rec print_expr_list par fmt x =
  (print_list (print_node print_expr_list_case)) par fmt x

and print_expr par fmt x =
  (print_node print_expr_list_case) par fmt x

and print_gen_expr par fmt x =
  (print_node print_expr_list_varassign) par fmt x

and print_bool_simple_expr par fmt x =
  (print_node print_expr_list_booldeffull) par fmt x

and print_int_simple_expr par fmt x =
  (print_node print_expr_list_booldeffull_eq) par fmt x

and print_file par fmt x =
  (print_list (print_node print_expr_list_case)) par fmt x

and print_expr_list_booldeffull_eq par fmt = function
  | Cless_ast.Const x1 ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>Const@ %a@]" (print_int true) x1;
      if par then fprintf fmt ")"
  | Cless_ast.Minus (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>Minus (@,";
      ((print_node print_expr_list_booldeffull_eq) false) fmt x1;
      fprintf fmt ",@ %a" ((print_node print_expr_list_booldeffull_eq) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.Mult (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>Mult (@,";
      ((print_node print_expr_list_booldeffull_eq) false) fmt x1;
      fprintf fmt ",@ %a" ((print_node print_expr_list_booldeffull_eq) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.Par x1 ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>Par@ %a@]" ((print_node
                                         print_expr_list_booldeffull_eq) true) x1;
      if par then fprintf fmt ")"
  | Cless_ast.Plus (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>Plus (@,";
      ((print_node print_expr_list_booldeffull_eq) false) fmt x1;
      fprintf fmt ",@ %a" ((print_node print_expr_list_booldeffull_eq) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.Var x1 ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>Var@ %a@]" (print_string true) x1;
      if par then fprintf fmt ")"

and print_expr_list_booldeffull par fmt = function
  | Cless_ast.BoolVar x1 ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>BoolVar@ %a@]" (print_string true) x1;
      if par then fprintf fmt ")"
  | Cless_ast.Eq (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>Eq (@,";
      ((print_node print_expr_list_booldeffull_eq) false) fmt x1;
      fprintf fmt ",@ %a" ((print_node print_expr_list_booldeffull_eq) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.False -> fprintf fmt "False"
  | Cless_ast.GG (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>GG (@,";
      ((print_node print_expr_list_booldeffull_eq) false) fmt x1;
      fprintf fmt ",@ %a" ((print_node print_expr_list_booldeffull_eq) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.Geq (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>Geq (@,";
      ((print_node print_expr_list_booldeffull_eq) false) fmt x1;
      fprintf fmt ",@ %a" ((print_node print_expr_list_booldeffull_eq) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.LL (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>LL (@,";
      ((print_node print_expr_list_booldeffull_eq) false) fmt x1;
      fprintf fmt ",@ %a" ((print_node print_expr_list_booldeffull_eq) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.Leq (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>Leq (@,";
      ((print_node print_expr_list_booldeffull_eq) false) fmt x1;
      fprintf fmt ",@ %a" ((print_node print_expr_list_booldeffull_eq) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.Neg x1 ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>Neg@ %a@]" ((print_node
                                         print_expr_list_booldeffull) true) x1;
      if par then fprintf fmt ")"
  | Cless_ast.Neq (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>Neq (@,";
      ((print_node print_expr_list_booldeffull_eq) false) fmt x1;
      fprintf fmt ",@ %a" ((print_node print_expr_list_booldeffull_eq) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.True -> fprintf fmt "True"

and print_expr_list_varassign par fmt = function
  | Cless_ast.Bool x1 ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>Bool@ %a@]" ((print_node
                                          print_expr_list_booldeffull) true) x1;
      if par then fprintf fmt ")"
  | Cless_ast.Int x1 ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>Int@ %a@]" ((print_node
                                         print_expr_list_booldeffull_eq) true) x1;
      if par then fprintf fmt ")"

and print_expr_list_case par fmt = function
  | Cless_ast.BooldefEmpty x1 ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>BooldefEmpty@ %a@]" (print_string true) x1;
      if par then fprintf fmt ")"
  | Cless_ast.BooldefFull (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>BooldefFull (@,";
      (print_string false) fmt x1;
      fprintf fmt ",@ %a" ((print_node print_expr_list_booldeffull) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.If (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>If (@,";
      ((print_node print_expr_list_booldeffull) false) fmt x1;
      fprintf fmt ",@ %a" ((print_list (print_node print_expr_list_case)) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.IfElse (x1, x2, x3) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>IfElse (@,";
      ((print_node print_expr_list_booldeffull) false) fmt x1;
      fprintf fmt ",@ %a" ((print_list (print_node print_expr_list_case)) false) x2;
      fprintf fmt ",@ %a" ((print_list (print_node print_expr_list_case)) false) x3;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.IntdefEmpty x1 ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>IntdefEmpty@ %a@]" (print_string true) x1;
      if par then fprintf fmt ")"
  | Cless_ast.IntdefFull (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>IntdefFull (@,";
      (print_string false) fmt x1;
      fprintf fmt ",@ %a" ((print_node print_expr_list_booldeffull_eq) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.VarAssign (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>VarAssign (@,";
      (print_string false) fmt x1;
      fprintf fmt ",@ %a" ((print_node print_expr_list_varassign) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
  | Cless_ast.While (x1, x2) ->
      if par then fprintf fmt "(";
      fprintf fmt "@[<hv 2>While (@,";
      ((print_node print_expr_list_booldeffull) false) fmt x1;
      fprintf fmt ",@ %a" ((print_list (print_node print_expr_list_case)) false) x2;
      fprintf fmt ")@]";
      if par then fprintf fmt ")"
