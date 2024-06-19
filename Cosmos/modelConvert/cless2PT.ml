
open Cless_ast
open Type
       
exception Type_error of location * string
exception NotYetImplemented of bool expr'
       
let rec sanity map ast = match ast.node with 
  | IntdefFull(s,v) ->
     ignore (sanity_int map v);
     StringMap.add s false map
  | IntdefEmpty(s) ->
     StringMap.add s false map
  | BooldefFull(s,v) ->
     sanity_bool map v;
     StringMap.add s true map
 | BooldefEmpty(s) ->
    StringMap.add s true map
 | While(c,l) ->
    sanity_bool map c;
    ignore( List.fold_left sanity map l);
    map
 | VarAssign(s,v) ->
    if not (StringMap.mem s map) then
      raise (Type_error (ast.loc,"Unbind variable " ^s));
    let tv = StringMap.find s map in
    sanity_gen map tv v;
    map
 | If(c,l) ->
    sanity_bool map c;
    ignore( List.fold_left sanity map l);
    map
 | IfElse(c,l1,l2) ->
    sanity_bool map c;
    ignore( List.fold_left sanity map l1);
    ignore( List.fold_left sanity map l2);
    map
    
and sanity_bool map ast = match ast.node with
  | True | False -> ()
  | Neg(v) -> sanity_bool map v
  | BoolVar(s) -> begin try
		      if not (StringMap.find s map) then
			raise (Type_error (ast.loc,"Wrong type variable "^s));
		      ()
		    with Not_found -> raise (Type_error (ast.loc,"Unbind variable " ^s)) end
  | Neq(i1,i2) -> sanity_int map i1; sanity_int map i2; 
  | Leq(i1,i2) -> sanity_int map i1; sanity_int map i2;
  | Geq(i1,i2) -> sanity_int map i1; sanity_int map i2;
  | LL(i1,i2) -> sanity_int map i1; sanity_int map i2; 
  | GG(i1,i2) -> sanity_int map i1; sanity_int map i2; 
  | Eq(i1,i2) -> sanity_int map i1; sanity_int map i2
 					       
and sanity_int map ast =  match ast.node with
  | Const(_) -> ()
  | Var(s) -> begin try
		      if (StringMap.find s map) then
			raise (Type_error (ast.loc,"Wrong type variable "^s));
		      ()
		    with Not_found -> raise (Type_error (ast.loc,"Unbind variable " ^s)) end
  | Par(i) -> sanity_int map i;
  | Plus(i1,i2) -> sanity_int map i1; sanity_int map i2
  | Mult(i1,i2) -> sanity_int map i1; sanity_int map i2
  | Minus(i1,i2) -> sanity_int map i1; sanity_int map i2						 
    
and sanity_gen map is_bool ast = match ast.node with
  | Int(i) when not is_bool -> sanity_int map i
  | Bool(b) when is_bool -> sanity_bool map b
  | _ -> raise (Type_error (ast.loc,"Wrong type variable "))

let once_cmpt = ref (-1)

let new_once s =
  incr once_cmpt;
  s ^ (string_of_int !once_cmpt)

let new_trans net = PetriNet.Net.add_transition net (new_once "T_") (StochasticPetriNet.Imm,Float(0.0),Float(0.0))


let line_of_ast ast =
    ast.loc |> fst |> fun x -> x.Lexing.pos_lnum |> string_of_int
      
open PetriNet.Net	

let rec translate = function
    Var(x) -> IntName("VI_"^x)
  | Const(x) -> Int(x)
  | Cless_ast.Par(x) -> translate x.node
  | Cless_ast.Plus(x,y) -> Type.Plus(translate x.node,translate y.node)
  | Cless_ast.Mult(x,y) -> Type.Mult(translate x.node,translate y.node)
  | Cless_ast.Minus(x,y) -> Type.Minus(translate x.node,translate y.node)
and translate_bool = function
    BoolVar(x) -> IntName("VB_"^x)
  | True -> Int(1)
  | False -> Int(0) 

and translate_bool2 = function
    BoolVar(x) -> BoolName("VB_"^x)
  | True -> Bool(true)
  | False -> Bool(false)
  | Neg (x) -> Not (translate_bool2 x.node)
  | GG(i1,i2) -> IntAtom(translate i1.node,SG,translate i2.node) 
  | LL(i1,i2) -> IntAtom(translate i1.node,SL,translate i2.node)
  | Eq(i1,i2) -> IntAtom(translate i1.node,EQ,translate i2.node)
  | Neq(i1,i2) -> IntAtom(translate i1.node,NEQ,translate i2.node)
  | Leq(i1,i2) -> IntAtom(translate i1.node,LE,translate i2.node)
  | Geq(i1,i2) -> IntAtom(translate i1.node,GE,translate i2.node)
			 
		     
			
let rec build_spt (net:StochasticPetriNet.spt) map pin pout ast =
  (* Cless.print_expr_list false Format.std_formatter [ast];*)
  let line = line_of_ast ast in
  match ast.node with
  | IntdefFull(s,v) ->
     let pv = add_place net ("VI_"^s) (Int(0),None) in
     let t = new_trans net in
     PetriNet.Data.add ((),(Int(1),pin,t)) net.inArc;
     PetriNet.Data.add ((),(Int(1),t,pout)) net.outArc;

     PetriNet.Data.add ((),(IntName("VI_"^s),pv,t)) net.inArc;
     begin match translate v.node |> eval with
	   | Int(0) -> ()
	   | x -> PetriNet.Data.add ((),(x,t,pv)) net.outArc; 
     end;
		  
     let map2 = StringMap.add s (false,pv) map in map2
  | IntdefEmpty(s) ->
     let pv = add_place net ("VI_"^s) (Int(0),None) in
     let t = new_trans net in
     PetriNet.Data.add ((),(Int(1),pin,t)) net.inArc;
     PetriNet.Data.add ((),(Int(1),t,pout)) net.outArc;
     PetriNet.Data.add ((),(IntName("VI_"^s),pv,t)) net.inArc;
     let map2 = StringMap.add s (false,pv) map in map2
  | BooldefFull(s,v) ->
     let pv = add_place net ("VB_"^s) (Int(0),Some (Int 1)) in
     let t = new_trans net in
     PetriNet.Data.add ((),(Int(1),pin,t)) net.inArc;
     PetriNet.Data.add ((),(Int(1),t,pout)) net.outArc;
     PetriNet.Data.add ((),(IntName("VB_"^s),pv,t)) net.inArc;
     begin match translate_bool v.node |> eval with
	   | Int(0) -> ()
	   | x -> PetriNet.Data.add ((),(x,t,pv)) net.outArc; 
     end;

     let map2 = StringMap.add s (false,pv) map in map2
  | BooldefEmpty(s) ->
     let pv = add_place net ("V_"^s) (Int(0),Some (Int 1)) in
     let t = new_trans net in
     PetriNet.Data.add ((),(Int(1),pin,t)) net.inArc;
     PetriNet.Data.add ((),(Int(1),t,pout)) net.outArc;
     let map2 = StringMap.add s (false,pv) map in map2
  | VarAssign(s,v) ->
     let tmp_var_name = (new_once "TV_") in
     let b,var = StringMap.find s map in
     let var_name = if b then "VB_"^s else "VI_"^s in
     let tmpvar = add_place net tmp_var_name (Int(0),None) in
     let tmpplace = add_place net (new_once ("PI"^line^"_")) (Int(0),Some (Int(1))) in
     let t1 = new_trans net in
     let t2 = new_trans net in
     PetriNet.Data.add ((),(Int(1),pin,t1)) net.inArc;
     PetriNet.Data.add ((),(Int(1),t1,tmpplace)) net.outArc;
     PetriNet.Data.add ((),(IntName(tmp_var_name),tmpvar,t1)) net.inArc;
     let v2 = match v.node with Int(x) -> translate x.node |> eval
			 | Bool(x) -> translate_bool x.node |> eval in
     begin match v2 with
	   | Int(0) -> ()
	   | x ->
	      let dep = dependency StringSet.empty x in 
	      StringSet.iter (
		  fun y ->
		  let varindex = PetriNet.Data.index net.place y in
		  PetriNet.Data.add ((),(Int(0),varindex,t1)) net.inArc) dep;
	      PetriNet.Data.add ((),(x,t1,tmpvar)) net.outArc; 
     end;
          
     PetriNet.Data.add ((),(Int(1),tmpplace,t2)) net.inArc;
     PetriNet.Data.add ((),(Int(1),t2,pout)) net.outArc;

     PetriNet.Data.add ((),(IntName(tmp_var_name),tmpvar,t2)) net.inArc;
     PetriNet.Data.add ((),(IntName(var_name),var,t2)) net.inArc;
     PetriNet.Data.add ((),(IntName(tmp_var_name),t2,var)) net.outArc; 


     map
  | If(c,v) ->
     let ptrue = add_place net (new_once ("P"^line^"_")) (Int(0),Some (Int(1))) in
     build_conditionnal net pin ptrue pout c;
     ignore @@ build_spt_list net map ptrue pout v;
     map
  | IfElse(c,v1,v2) ->
     let ptrue = add_place net (new_once "P_") (Int(0),Some (Int(1))) in
     let pfalse = add_place net (new_once "P_") (Int(0),Some (Int(1))) in
     build_conditionnal net pin ptrue pfalse c;     
     ignore @@ build_spt_list net map ptrue pout v1;
     ignore @@ build_spt_list net map pfalse pout v2;
     map      
  | While(c,v) ->
     let ptrue = add_place net (new_once ("P"^line^"_")) (Int(0),Some (Int(1))) in
     build_conditionnal net pin ptrue pout c;
     ignore @@ build_spt_list net map ptrue pin v;
     map
    
and build_conditionnal net pin ptrue pfalse c =
  let simpc = simp_bool (translate_bool2 c.node) in
  let ctrue = form_dij simpc in
  let cfalse = form_dij (simp_bool (Not simpc)) in
  List.iter (fun co -> build_conditionnal2 net pin ptrue co) ctrue;
  List.iter (fun co -> build_conditionnal2 net pin pfalse co) cfalse;
and build_conditionnal2 net pin pout c =
  let t = new_trans net in
  (*print_string "new conditionnal";
  printH_expr stdout c;*)
  begin match c with
	| IntAtom( lhs, op,rhs) ->
	   let p = (
	     match lhs with
	     | IntName(x) -> PetriNet.Data.index net.place x
	     | _ -> raise (NotYetImplemented c)) in
	   (match op with
	    | GE ->
	         PetriNet.Data.add ((),(rhs,p,t)) net.inArc;
		 PetriNet.Data.add ((),(rhs,t,p)) net.outArc;
	    | SG ->
	       PetriNet.Data.add ((),(incr_int rhs,p,t)) net.inArc;
	       PetriNet.Data.add ((),(incr_int rhs,t,p)) net.outArc;
	    | SL ->
	       PetriNet.Data.add ((),(rhs,p,t)) net.inhibArc;
	    | LE ->
	       PetriNet.Data.add ((),(incr_int rhs,p,t)) net.inhibArc;
	    | EQ ->
	       PetriNet.Data.add ((),(incr_int rhs,p,t)) net.inhibArc;
	       PetriNet.Data.add ((),(rhs,p,t)) net.inArc;
	       PetriNet.Data.add ((),(rhs,t,p)) net.outArc;
	    | NEQ ->
	       (* SG*)
	       PetriNet.Data.add ((),(incr_int rhs,p,t)) net.inArc;
	       PetriNet.Data.add ((),(incr_int rhs,t,p)) net.outArc;
	   (* SL *)
	       let t2 = new_trans net in
	       PetriNet.Data.add ((),(rhs,p,t2)) net.inhibArc;
	       PetriNet.Data.add ((),(Int(1),pin,t2)) net.inArc;
	       PetriNet.Data.add ((),(Int(1),t2,pout)) net.outArc;
	   ) 
	| _-> raise (NotYetImplemented c)
  end;
  PetriNet.Data.add ((),(Int(1),pin,t)) net.inArc;
  PetriNet.Data.add ((),(Int(1),t,pout)) net.outArc;
		    
and build_spt_list net map pin pout = function
    [] -> map
  | [t] -> build_spt net map pin pout t
  | t::q -> let tmpplace = add_place net (new_once ("P"^(line_of_ast t)^"_")) (Int(0),Some (Int(1))) in
	    let map2 = build_spt net map pin tmpplace t in
	    build_spt_list net map2 tmpplace pout q
