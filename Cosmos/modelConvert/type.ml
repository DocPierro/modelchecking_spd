let (|>>) x f = match x with 
    Some y -> f y
  | None -> None

let (|>>>) x f = match x with 
    Some y -> begin try Some (f y) with _ -> None end
  | None -> None

let (|>>|) x v = match x with
    Some y -> y
  | None -> v

let (|<) x f = let () = f x in x
let (|<>|) f (x,y) = f x y

let opHd = function
  | t::_ -> Some t
  | [] -> None

let list_create n f =
  let l = ref [] in
  for i=0 to n-1 do
    l := (f i)::!l;
  done;
  !l
            
let fix_point f x =
  let y = ref x 
  and z = ref (f x) in
  while !y <> !z do
    let t = f !z in
    y := !z;
    z := t;
  done;
  !z

let merge_option f x y = match (x,y) with
    None, x -> x
  | x, None -> x
  | Some a,Some b -> Some (f a b)

let logout = ref stdout

let rec string_of_list sep f = function
  | [] -> ""
  | [t] -> f t
  | t::q -> Printf.sprintf "%s%s%s" (f t) sep @@ string_of_list sep f q

let print_list f sep file l =
  output_string file (string_of_list sep f l)

let rec print_list2 fu sep file = function
  | [] -> ()
  | [t] -> fu file t
  | t::q -> Printf.fprintf file "%a%s%a" fu t sep (print_list2 fu sep) q
            

                
let rec selectL fb = function
  | [] -> raise Not_found
  | t::q when fb t -> (t,q)
  | t::q -> let t2,q2 = selectL fb q in
	    (t2,t::q2)
 
let print_option2 def f so  =
  match so with None -> output_string f def | Some s -> Printf.fprintf f "%s" s

let string_of_option def so  =
  match so with None -> def | Some s -> s

let print_option f so  = print_option2 "" f so

type cmp = EQ | SG | SL | GE | LE | NEQ

type _ expr' =
  | Bool  : bool -> bool expr'
  | Int   : int -> int expr'
  | Float : float -> float expr'
  | Not : bool expr' -> bool expr'
  | And : bool expr' * bool expr' -> bool expr'
  | Or  : bool expr' * bool expr' -> bool expr'
  | IntAtom : int expr' * cmp * int expr' -> bool expr'
  | FloatAtom : float expr' * cmp * float expr' -> bool expr'
  | BoolAtom : bool expr' * cmp * bool expr' -> bool expr'
  | Plus : 'a expr' * 'a expr' -> 'a expr'
  | Minus : 'a expr' * 'a expr' -> 'a expr'
  | Mult  : 'a expr' * 'a expr' -> 'a expr'
  | Floor : float expr' -> int expr'
  | Ceil  : float expr' -> int expr'
  | CastInt : int expr' -> float expr'
  | CastBool: bool expr' -> int expr'
  | IntName : string -> int expr'
  | FloatName : string -> float expr'
  | BoolName : string -> bool expr'
  | Div : float expr' *  float expr' -> float expr'
  | Exp : float expr' -> float expr'
  | FunCall : string*(('a expr') list) -> 'a expr'
  | If : (bool expr' * 'a expr' * 'a expr') -> 'a expr'

type full_expr =
  BoolExpr of bool expr'
| IntExpr of int expr'
| FloatExpr of float expr'
                                                  
type cmdAttr =
  | Close
  | ParseInt of int expr'
  | ParseFloat of float expr'
  | ParseBool of bool expr'
  | ParseDistr of string*((float expr') list) 
  | XMLInt of string
  | XMLFloat of string
  | XMLBool of string
  | XMLDistr of string
      
      
let rec iterFloat f y = 
  let ri = iterFloat f in match y with
    | FloatName(x) -> f y;
    | Float(x) -> f y;
    | CastInt(x) -> f y;
    | Plus(e1,e2) -> Plus(ri e1,ri e2) |> f
    | Minus(e1,e2) -> Minus(ri e1,ri e2) |> f
    | Mult(e1,e2) -> Mult(ri e1,ri e2) |> f
    | Div(e1,e2) -> Div(ri e1,ri e2) |> f
    | Exp(x) -> Exp(ri x) |> f 
    | FunCall(fu,l) -> FunCall(fu,List.map ri l) |> f
    | If(c,e1,e2) -> If(c,ri e1,ri e2) |> f
	       
let neg_cmp = function 
  | EQ -> NEQ  
  | SG -> LE
  | SL -> GE
  | GE -> SL
  | LE -> SG
  | NEQ-> EQ

let eval_cmp x y = function
    EQ -> x=y
  | NEQ -> x!=y
  | SG -> x>y
  | SL -> x<y
  | GE -> x>=y
  | LE -> x<=y
	    
let rec neg_bool = function
  | Bool x -> Bool (not x)
  | BoolName x -> Not (BoolName x)
  | Not e -> e
  | IntAtom (ie1,cmp,ie2) -> IntAtom (ie1,neg_cmp cmp,ie2)
  | FloatAtom (ie1,cmp,ie2) -> FloatAtom (ie1,neg_cmp cmp,ie2)
  | BoolAtom (ie1,cmp,ie2) -> BoolAtom (ie1,neg_cmp cmp,ie2)
  | And (e1,e2) -> Or (neg_bool e1,neg_bool e2)
  | Mult (e1,e2) -> Or (neg_bool e1,neg_bool e2)
  | Or (e1,e2) -> And (neg_bool e1,neg_bool e2)
  | Plus (e1,e2) -> And (neg_bool e1,neg_bool e2)
  | Minus (e1,e2) -> Or (neg_bool e1,e2)
  | FunCall (e,l) -> Not (FunCall (e,l))
  | If(c,e1,e2) -> If(c,neg_bool e1,neg_bool e2)

let rec eval: type a.
		   ?iname:(string -> int expr' option) ->
		   ?fname:(string -> float expr' option) ->
		   ?bname:(string -> bool expr' option) ->
		   a expr' -> a expr' = fun
    ?iname:(iv=(fun _ -> None))
    ?fname:(fv=(fun _ -> None))
    ?bname:(bv=(fun _ -> None)) x ->
  let ev x = eval ~iname:iv ~fname:fv ~bname:bv x in
  match x with
  | IntName(s) -> (match iv s with Some e -> ev e | None -> x)
  | FloatName(s) -> (match fv s with Some e -> ev e | None -> x)
  | BoolName(s) -> (match bv s with Some e -> ev e | None -> x)
  | Plus(e1,e2) -> (match (ev e1),(ev e2) with
      Int y,Int z -> Int (y+z) 
    | Float y,Float z -> Float (y+.z)
    | Bool y,Bool z -> Bool (y || z)
    | ee1,ee2 -> begin match ee2 with
      Int 0 -> ee1
      | Int y when y <0 -> Minus(ee1,Int (-y))
      | Float 0.0 -> ee1
      | Float y when y<0.0 -> Minus(ee1,Float (-.y))
      |_-> Plus(ee1,ee2)
		 end)
  | Not(be) -> (match ev be with Bool(b) -> Bool(not b) | x -> Not(x))
  | And(e1,e2) -> (match (ev e1),(ev e2) with
    | Bool y,z -> if y then z else Bool false
    | y,Bool z -> if z then y else Bool false
    | ee1,ee2 -> And(ee1,ee2)) 
  | Or(e1,e2) -> (match (ev e1),(ev e2) with
    | Bool y,z -> if not y then z else Bool true
    | y,Bool z -> if not z then y else Bool true
    | ee1,ee2 -> Or(ee1,ee2))
 | BoolAtom(e1,cmp,e2) -> (match (ev e1),cmp,(ev e2) with
    | Bool y,EQ,z -> if y then z else ev (Not z)
    | Bool y,NEQ,z -> if not y then z else  z
    | z,EQ,Bool y -> if y then z else ev (Not z)
    | z,NEQ,Bool y -> if not y then z else  z
    | y,EQ,z -> Or( And(y,z), (And( ev (Not y),ev (Not z)) ))
    | ee1,cmp,ee2 -> BoolAtom(ee1,cmp,ee2))
  | Mult(e1,e2) -> (match (ev e1),(ev e2) with
    Int y,Int z -> Int (y*z) 
    | Float y,Float z -> Float (y*.z)
    | Float y,z -> if y=1.0 then z else (if y=0.0 then Float 0.0 else Mult (Float y,z))
    | z,Float y -> if y=1.0 then z else (if y=0.0 then Float 0.0 else Mult (Float y,z))
    | Int y,z -> if y=1 then z else (if y=0 then Int 0 else Mult (Int y,z))
    | z,Int y -> if y=1 then z else (if y=0 then Int 0 else Mult (Int y,z))

    | Bool y,Bool z -> Bool (y && z)
    | ee1,ee2 -> Mult(ee1,ee2))
  | Div(e1,e2) -> (match (ev e1),(ev e2) with
    | Float y,Float z -> Float (y/.z)
    | z,Float 1.0 -> z
    | ee1,ee2 -> Div(ee1,ee2))
  | Minus(e1,e2) -> (match (ev e1),(ev e2) with
      Int y,Int z -> Int (y-z) 
    | Float y,Float z -> Float (y-.z) 
    | ee1,ee2 -> Minus(ee1,ee2))
  | CastInt(e) -> (match ev e with 
    | Int i -> Float (float i)
    | y -> CastInt(y))
  | IntAtom(ie1,c,ie2) -> (match (ev ie1,ev ie2) with
			  | Int i1,Int i2 -> Bool (eval_cmp i1 i2 c)
			  | x,y -> IntAtom(x,c,y))
  | FloatAtom(fe1,c,fe2) -> (match (ev fe1,ev fe2) with
			  | Float f1,Float f2 -> Bool (eval_cmp f1 f2 c)
			  | x,y -> FloatAtom(x,c,y))
  | Bool b -> Bool b
  | Int i -> Int i
  | Float f -> Float f
  | Floor(f) -> (match ev f with Float(fv) -> Int(int_of_float (floor fv))
			       | x -> Floor(x))
  | Ceil(f) -> (match ev f with Float(fv) -> Int(int_of_float (ceil fv))
			      | x -> Floor(x))
  | CastBool(be) -> (match ev be with Bool(true) -> Int(1) | Bool(false) -> Int(0)
			      | x -> CastBool(x))
  | Exp(f) -> (match ev f with Float(fv) -> Float(exp fv)
			      | x -> Exp(x))
  | If(c,e1,e2) -> (match ev c with Bool(true) -> ev e1 | Bool(false) -> ev e2
				    | x -> If(x,ev e1,ev e2))
  | FunCall("max",[e1;e2]) -> (match (ev e1),(ev e2) with
                                 Int y,Int z -> Int (max y z)
                               | Float y,Float z -> Float (max y z)
                               | ee1,ee2 -> FunCall("max",[ee1;ee2]))
  | FunCall("min",[e1;e2]) -> (match (ev e1),(ev e2) with
                                 Int y,Int z -> Int (min y z)
                               | Float y,Float z -> Float (min y z)
                               | ee1,ee2 -> FunCall("max",[ee1;ee2]))
  | x -> x
		
let print_rename f =
  List.iter (fun (x,y) -> Printf.fprintf f " %s->%s " x y) 

let rename_op rn = function None -> None | Some a -> Some (rn a)

let rec rename_expr: type a. (string ->string) -> a expr' -> a expr' = fun rn e -> let rnr: type a. a expr' -> a expr' = fun x -> rename_expr rn x in match e with
    | IntName(s) -> IntName (rn s);
    | Int(i) -> Int(i)
    | Plus(e1,e2) -> Plus(rnr e1,rnr e2)
    | Minus(e1,e2) -> Minus(rnr e1,rnr e2)
    | Mult(e1,e2) -> Mult(rnr e1,rnr e2)
    | Div(e1,e2) -> Div(rnr e1,rnr e2)
    | Exp(e) -> Exp(rnr e)
    | Ceil(e) -> Ceil(rnr e)
    | Floor(e) -> Floor(rnr e)
    | Bool x -> Bool x
    | BoolName(x) -> BoolName(rn x) 
    | Not(e1) -> Not (rnr e1) 
    | And(e1,e2) -> And(rnr e1,rnr e2)
    | Or(e1,e2) -> Or(rnr e1,rnr e2)
    | IntAtom(ie,c,ie2) -> IntAtom(rnr ie,c,rnr ie2)
    | FloatAtom(ie,c,ie2) -> FloatAtom(rnr ie,c,rnr ie2)
    | BoolAtom(ie,c,ie2) -> BoolAtom(rnr ie,c,rnr ie2)
    | Float(f) -> Float(f)
    | FloatName(x) -> FloatName(rn x) 
    | CastInt(x) -> CastInt (rnr x)
    | CastBool(x) -> CastBool (rnr x)
    | FunCall(x,fl) -> FunCall(x,List.map rnr fl) 
    | If(c,e1,e2) -> If(rnr c,rnr e1, rnr e2)

let rec replace_expr vi e = 
  let rn x = replace_expr vi x in
  match e with
    | IntName(s) -> (match vi s with Some(x) -> x | _ -> IntName (s))
    | Int(i) -> Int(i)
    | Plus(e1,e2) -> Plus(rn e1,rn e2)
    | Minus(e1,e2) -> Minus(rn e1,rn e2)
    | Mult(e1,e2) -> Mult(rn e1,rn e2)
    | CastBool(x) -> CastBool (x)
    | FunCall(x,fl) -> FunCall(x,fl) 
    | If(c,e1,e2) -> If( c, e1,  e2)
    | Floor(f) -> Floor(f)
    | Ceil(f) -> Ceil(f)

let ez x = match eval x with
         | Int 0 -> true
         | _ -> false
let gez x = match eval x with
  | Int y when y>=0 -> true
  | _ -> false
let sgz x = match eval x with
  | Int y when y>0 -> true
  | _ -> false

let incr_int z = eval (Plus(z,Int 1))

module MultiSet (Ot:Map.OrderedType) = struct
  include Map.Make(Ot)
  let add_multi e t =
    try let i = find e t in
	remove e t 
	|> add e (i+1) 
    with
      Not_found -> add e 1 t
end

module StringSet = Set.Make(String)
module IntSet = Set.Make(struct type t = int let compare=compare end)
module StringMap = MultiSet(String)

module IntSQ = struct 
  type t = int * int 
  let compare = compare 
end

module IntSQMulti = MultiSet(IntSQ)

let rec split_eq = function
    [] -> [],[]
  | [t] -> [t],[]
  | t1::t2::q -> let r1,r2 = split_eq q in
		 (t1::r1,t2::r2)

let rec conjunction = function
    [] -> Bool(true)
  | [t] -> t
  | l -> let l1,l2= split_eq l in
	 And( conjunction l1, conjunction l2)

let rec disjunction = function
    [] -> Bool(false)
  | [t] -> t
  | l -> let l1,l2= split_eq l in
	 Or( disjunction l1, disjunction l2)
			    
let rec simp_bool : bool expr' -> bool expr' = fun x -> match x with
  | Bool(x) -> Bool(x)
  | Or(x,y) -> Or(simp_bool x,simp_bool y)
  | And(x,y) -> And(simp_bool x,simp_bool y)
  | Not(Not(x)) -> simp_bool x
  | Not(Bool(x)) -> Bool (not x)
  | Not(Or(x,y)) -> And(simp_bool (Not(x)),simp_bool (Not(y)))
  | Not(And(x,y)) -> Or(simp_bool (Not(x)),simp_bool (Not(y)))
  | Not(IntAtom (ie1,cmp,ie2)) -> IntAtom (ie1,neg_cmp cmp,ie2)
  | Not(FloatAtom (ie1,cmp,ie2)) -> FloatAtom (ie1,neg_cmp cmp,ie2)
  | Not(x) -> Not(simp_bool x)
  | x -> x

module BoolSet = Set.Make (struct
			      type t=bool expr'
			      let compare t1 t2 = compare t1 t2
			    end)
let exists_pair f s =
  BoolSet.exists (fun x ->
		  let _,_,r = BoolSet.split x s in
		  BoolSet.exists (fun y -> f x y) r) s
			  
module BoolSetSet = Set.Make (struct
			      type t=BoolSet.t
			      let compare t1 t2 = BoolSet.compare t1 t2
			    end)

let disjunction_set s =
  let l = BoolSet.fold (fun x l -> x::l) s [] in disjunction l
let conjunction_set s =
  let l = BoolSet.fold (fun x l -> x::l) s [] in conjunction l 
			  
let rec form_dij_set : bool expr' -> BoolSetSet.t = fun x ->
  match x with
  | Or(x,y) -> BoolSetSet.union (form_dij_set x) (form_dij_set y)
  | And(e1,e2) ->
     let l1 = form_dij_set e1 in
     let l2 = form_dij_set e2 in
     BoolSetSet.fold (fun t1 sdij1 ->
		      BoolSetSet.fold (fun t2 sdij2 ->
				       BoolSetSet.add (BoolSet.union t1 t2) sdij2) l1 sdij1)
		    l2 BoolSetSet.empty
  | x -> BoolSetSet.singleton (BoolSet.singleton x)


let form_dij e =
  e
  |> form_dij_set
  |> BoolSetSet.filter (fun c ->
			not @@ exists_pair (fun a1 a2 ->
					    (simp_bool a1) = (simp_bool (Not a2))) c)
  |> (fun s -> BoolSetSet.filter (fun c ->
				  not @@ BoolSetSet.exists (fun c2 -> c<>c2 && BoolSet.subset c2 c) s) s) 
  |> BoolSetSet.elements
  |> List.map conjunction_set
			      
let rec dependency : type a . StringSet.t -> a expr' -> StringSet.t = fun set x -> 
  match x with
  | BoolName(x) -> StringSet.add x set
  | IntName(x) -> StringSet.add x set
  | FloatName(x) -> StringSet.add x set
  | Bool(_) -> set
  | Int(_) -> set
  | Float(_) -> set
  | Not(b) -> dependency set b
  | Exp(b) -> dependency set b
  | Floor(b) -> dependency set b  
  | Ceil(b) -> dependency set b
  | CastInt(b) -> dependency set b			  
  | CastBool(b) -> dependency set b
  | And(b1,b2) -> dependency (dependency set b1) b2
  | Or(b1,b2) -> dependency (dependency set b1) b2
  | Plus(b1,b2) -> dependency (dependency set b1) b2
  | Minus(b1,b2) -> dependency (dependency set b1) b2
  | Mult(b1,b2) -> dependency (dependency set b1) b2
  | Div(b1,b2) -> dependency (dependency set b1) b2
  | IntAtom(i1,_,i2) -> dependency (dependency set i1) i2
  | FloatAtom(i1,_,i2) -> dependency (dependency set i1) i2
  | BoolAtom(i1,_,i2) -> dependency (dependency set i1) i2
  | FunCall(s,l) -> List.fold_left dependency set l
  | If(b,e1,e2) ->
     let set2 = dependency set b in
     let set3 = dependency set2 e1 in
      dependency set3 e2				   
				     
			    
let add_set t s =
  StringSet.fold StringMap.add_multi t s

let print_set sep f s=
  ignore (StringSet.fold (fun x c -> if c then output_string f sep;
    output_string f x;true) s false)

let print_multi sep f s=
  ignore (StringMap.fold (fun x y c -> if c then output_string f sep;
    Printf.fprintf f "%s:%i" x y;true) s false)

let printH_cmp f = function 
  | EQ -> output_string f "="  
  | SG -> output_string f ">" 
  | SL -> output_string f "<" 
  | GE -> output_string f ">=" 
  | LE -> output_string f "<=" 
  | NEQ-> output_string f "!="
                        
                        
let print_pretty_float = function
  | x when x = 0.5 -> "1/2"
  | x when x = 1./.3. -> "1/3"
  | x when x = 2./.3. ->  "2/3"
  | x when x = 1./.4. ->  "1/4"
  | x when x = 3./.4. ->  "3/4"
  | x when x = 1./.5. ->  "1/5"
  | x when x = 2./.5. ->  "2/5"
  | x when x = 3./.5. ->  "3/5"
  | x when x = 4./.5. ->  "4/5"            
  | x when x = 1./.6. ->  "1/6"
  | x when x = 5./.6. ->  "5/6"
  | x when x = 1./.7. ->  "1/7"                            
  | x when x = 1./.8. ->  "1/8"
  | x when x = 3./.8. ->  "3/8"
  | x when x = 5./.8. ->  "5/8"
  | x when x = 7./.8. ->  "7/8"
  | x when x = 1./.9. ->  "1/9"
  | x when x = 2./.9. ->  "2/9"
  | x when x = 4./.9. ->  "4/9"
  | x when x = 5./.9. ->  "5/9"
  | x when x = 7./.9. ->  "7/9"
  | x when x = 8./.9. ->  "8/9"
  | x when x = 1./.10. ->  "1/10"
  | x when x = 3./.10. ->  "3/10"
  | x when x = 7./.10. ->  "7/10"
  | x when x = 9./.10. ->  "9/10"
  | x when x = 1. ->  "1"
                                   
  | x -> string_of_float x
                        
let rec printH_expr: type a. out_channel -> a expr' -> unit = fun f x -> match eval x with
  | Int(i) -> Printf.fprintf f "%i" i
  | Float(i) -> Printf.fprintf f "%g" i
  | Bool(i) -> Printf.fprintf f "%B" i
  | IntName(s) -> output_string f s
  | FloatName(s) -> output_string f s
  | BoolName(s) -> output_string f s
  | Plus(e1,e2) -> Printf.fprintf f "(%a+%a)" 
    printH_expr e1 
    printH_expr e2
  | Minus(e1,e2) -> Printf.fprintf f "(%a-%a)" 
    printH_expr e1 
    printH_expr e2
  | Mult(e1,e2) -> Printf.fprintf f "(%a*%a)" 
    printH_expr e1 
    printH_expr e2
  | Ceil(e) -> Printf.fprintf f "ceil(%a)" 
    printH_expr e
  | Floor(e) -> Printf.fprintf f "floor(%a)" 
    printH_expr e 
  | Not e-> Printf.fprintf f "(!%a)" printH_expr e
  | And (e1,e2) -> Printf.fprintf f "(%a & %a)" 
    printH_expr e1
    printH_expr e2
  | Or (e1,e2) -> Printf.fprintf f "(%a | %a)" 
    printH_expr e1
    printH_expr e2
  | IntAtom (e1,c,e2) -> Printf.fprintf f "(%a %a %a)" 
    printH_expr e1
    printH_cmp c
    printH_expr e2  
  | FloatAtom (e1,c,e2) -> Printf.fprintf f "(%a %a %a)" 
    printH_expr e1
    printH_cmp c
    printH_expr e2  
 | BoolAtom (e1,c,e2) -> Printf.fprintf f "(%a %a %a)" 
    printH_expr e1
    printH_cmp c
    printH_expr e2  
  | CastInt(x) -> printH_expr f x
  | CastBool(x) -> printH_expr f x
  | Div(e1,e2) -> Printf.fprintf f "(%a/%a)" 
    printH_expr e1 
    printH_expr e2
  | Exp(x) -> Printf.fprintf f "exp(%a)" printH_expr x
  | FunCall(fu,[]) ->  Printf.fprintf f "%s()" fu;
  | FunCall(fu,t::q) ->  Printf.fprintf f "%s(%a" fu printH_expr t;
    List.iter (fun x-> Printf.fprintf f ",%a" printH_expr x) q;
    output_string f ")"
  | If (c,e1,e2) -> Printf.fprintf f "(%a ? %a : %a)" 
    printH_expr c
    printH_expr e1 
    printH_expr e2

let print_full_expr o = function
    BoolExpr b -> printH_expr o b
  | FloatExpr f -> printH_expr o f
  | IntExpr i -> printH_expr o i
    
let print_token f = function 
  | Int 0 -> output_string f " "
  | Int 1 -> output_string f "•" 
  | Int 2 -> output_string f "••"
  | Int 3 -> output_string f "••\n•"
  | Int 4 -> output_string f "••\n••"
  | Int 5 -> output_string f "•••\n••"
  | i -> printH_expr f i

exception FailToEvaluate
                                     
                     
let eval_or_die: type a.
		      ?iname:(string -> int expr' option) ->
		      ?fname:(string -> float expr' option) ->
		      ?bname:(string -> bool expr' option) ->
		      a expr' -> a = fun
    ?iname:(iv=(fun _ -> None))
    ?fname:(fv=(fun _ -> None))
    ?bname:(bv=(fun _ -> None)) x ->
  match eval ~iname:iv ~fname:fv ~bname:bv x with
  | Int(i) -> i
  | Float(f) -> f
  | Bool(b) -> b
  | x -> printH_expr stderr x; output_string stderr "\n";
	 raise FailToEvaluate


(*		       
let x = BoolName "x"
let y = BoolName "y"
let z = BoolName "z"

let conj = [
	       Or(x,y);
	       Or(y,z);
	       Or(x,z)
  ]
	   |> conjunction
	   |> (fun x ->Not x)
	   |> simp_bool
	   |< printH_expr stdout
		
let dij =
  conj
  |> form_dij
  |> disjunction
  |< printH_expr stdout
 *)
