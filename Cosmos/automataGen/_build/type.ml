let rec print_list f c = function
  | [] -> ()
  | [t] -> f t;
  | t::q -> f t; c (); print_list f c q

type formula =
  Until of stateFormula*stateFormula
| BoundedUntil of stateFormula*(floatExpr*floatExpr)*stateFormula
| Next of stateFormula
| BoundedNext of (floatExpr*floatExpr)*stateFormula
| AndF of (formula*formula)
| OrF of (formula*formula) 

and stateFormula = True | False | Not of stateFormula 
		   | And of stateFormula*stateFormula | Or of stateFormula*stateFormula
		   | IntAtom of intExpr*cmp*intExpr | RealAtom of floatExpr*cmp*floatExpr 

and cmp = EQ | SG | SL | GE | LE | NEQ
and intExpr = IntName of string | Int of int 
	      | Plus of intExpr*intExpr | Minus of intExpr*intExpr
	      | Mult of intExpr*intExpr
and floatExpr = FloatName of string | Float of float | Infty
		| ContinuousVar of int
		| DiscreteVar of int
		| CastInt of intExpr
		| MultF of floatExpr*floatExpr
		| PlusF of floatExpr*floatExpr 
		| MinusF of floatExpr*floatExpr
		| DivF of floatExpr*floatExpr;;

let negcmp = function
  | EQ -> NEQ
  | NEQ -> EQ
  | SG -> LE
  | SL -> GE
  | GE -> SL
  | LE -> SG

let rec print_timeInt (fexpr1,fexpr2) =
  print_string "[";
  print_float_expr fexpr1;
  print_string ";";
  print_float_expr fexpr2;
  print_string "]"

and print_formula = function
  |Until(sf1,sf2) -> 
    print_string "(";
    print_sf sf1;
    print_string " U ";
    print_sf sf2;
    print_string ")"
  |BoundedUntil(sf1,timeInt,sf2) ->
    print_string "(";
    print_sf sf1;
    print_string " U";
    print_timeInt timeInt;
    print_sf sf2;
    print_string ")"
  |Next(sf) ->
    print_string "(";
    print_string " X";
    print_sf sf;
    print_string ")"
  |BoundedNext(timeInt,sf) ->
    print_string "(";
    print_string " X";
    print_timeInt timeInt;
    print_sf sf;
    print_string ")"
  |AndF(f1,f2) ->
    print_string "(";
    print_formula f1;
    print_string " & ";
    print_formula f2;
    print_string ")"
  |OrF(f1,f2) ->
    print_string "(";
    print_formula f1;
    print_string " | ";
    print_formula f2;
    print_string ")"

and print_sf = function
  | True -> print_string "TRUE"
  | False -> print_string "TRUE"
  | Not(sf) -> print_string "!(";
    print_sf sf;
    print_string ")"
  | And(sf1,sf2) ->
    print_string "(";
    print_sf sf1;
    print_string " & ";
    print_sf sf2;
    print_string ")"
  | Or(sf1,sf2) ->
    print_string "(";
    print_sf sf1;
    print_string " | ";
    print_sf sf2;
    print_string ")"
  | IntAtom(iexpr1,cmp,iexpr2) -> 
    print_int_expr iexpr1;
    print_cmp cmp;
    print_int_expr iexpr2;
  | RealAtom(fexpr1,cmp,fexpr2) -> 
    print_float_expr fexpr1;
    print_cmp cmp;
    print_float_expr fexpr2

and print_cmp = function
  | EQ -> print_string "="
  | NEQ -> print_string "!="
  | SG -> print_string ">"
  | SL -> print_string "<"
  | GE -> print_string ">="
  | LE -> print_string "<="

and print_int_expr = function
  | IntName(s) -> print_string s;
  | Int(i) -> print_int i;
  | Plus(e1,e2) -> print_string "(";
    print_int_expr e1;
    print_string "+";
    print_int_expr e2;
    print_string ")"
  | Minus(e1,e2) -> print_string "(";
    print_int_expr e1;
    print_string "-";
    print_int_expr e2;
    print_string ")"
  | Mult(e1,e2) -> print_string "(";
    print_int_expr e1;
    print_string "*";
    print_int_expr e2;
    print_string ")"

and print_float_expr = function
  | FloatName(x) -> print_string x;
  | ContinuousVar(x) -> print_string " vc"; print_int x;
  | DiscreteVar(x) -> print_string " vd"; print_int x;
  | Float(x) -> print_float x;
  | Infty -> print_string " 1E200 ";
  | CastInt(x) -> print_int_expr x;
  | PlusF(e1,e2) -> print_string "(";
    print_float_expr e1;
    print_string "+";
    print_float_expr e2;
    print_string ")"
  | MinusF(e1,e2) -> print_string "(";
    print_float_expr e1;
    print_string "-";
    print_float_expr e2;
    print_string ")"
  | MultF(e1,e2) -> print_string "(";
    print_float_expr e1;
    print_string "*";
    print_float_expr e2;
    print_string ")"
  | DivF(e1,e2) -> print_string "(";
    print_float_expr e1;
    print_string "/";
    print_float_expr e2;
    print_string ")"
  
