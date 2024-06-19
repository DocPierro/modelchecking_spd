(** Cless abstract syntax tree *)

(** {2 Locations} *)

type location = Lexing.position * Lexing.position

type 'a node = {
  node: 'a;
  loc: location;
}

exception Parse_error of location * string

(** {2 Entry Points Types} *)

type expr_list_booldeffull_eq =
  | Var of string
  | Plus of expr_list_booldeffull_eq node * expr_list_booldeffull_eq node
  | Par of expr_list_booldeffull_eq node
  | Mult of expr_list_booldeffull_eq node * expr_list_booldeffull_eq node
  | Minus of expr_list_booldeffull_eq node * expr_list_booldeffull_eq node
  | Const of int

and expr_list_booldeffull =
  | True
  | Neq of expr_list_booldeffull_eq node * expr_list_booldeffull_eq node
  | Neg of expr_list_booldeffull node
  | Leq of expr_list_booldeffull_eq node * expr_list_booldeffull_eq node
  | LL of expr_list_booldeffull_eq node * expr_list_booldeffull_eq node
  | Geq of expr_list_booldeffull_eq node * expr_list_booldeffull_eq node
  | GG of expr_list_booldeffull_eq node * expr_list_booldeffull_eq node
  | False
  | Eq of expr_list_booldeffull_eq node * expr_list_booldeffull_eq node
  | BoolVar of string

and expr_list_varassign =
  | Int of expr_list_booldeffull_eq node | Bool of expr_list_booldeffull node

and expr_list_case =
  | While of expr_list_booldeffull node * expr_list
  | VarAssign of string * expr_list_varassign node
  | IntdefFull of string * expr_list_booldeffull_eq node
  | IntdefEmpty of string
  | IfElse of expr_list_booldeffull node * expr_list * expr_list
  | If of expr_list_booldeffull node * expr_list
  | BooldefFull of string * expr_list_booldeffull node
  | BooldefEmpty of string

and expr_list = expr_list_case node list

and file = expr_list_case node list
