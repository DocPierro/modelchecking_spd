open Xml
open PetriNet
open StochasticPetriNet
open Type

let tree_of_pnml s =
     parse_file s

let rec find_child s e1 = function
    Element (s,_,[PCData e2])-> Some e2
  | Element (_,_,cl) -> List.fold_left (find_child s) e1 cl
  | PCData _ -> e1 

let find_child_list s xl =
    List.fold_left (find_child s) None xl

let find_at s l = try Some (List.assoc s l) with Not_found -> None
let find_id l = find_at "id" l
let find_type l = find_at "nodeType" l

let rec find_name_rec s e = function
    Element (x,_,cl) when s=x-> find_child_list "name" cl
  | Element (_,_,cl) -> List.fold_left (find_name_rec s) e cl
  | PCData _ -> e

let find_prop s l =
  List.fold_left (fun c n -> match n with 
    | Element ("attribute",atl,cl) ->
      let v = find_at "name" atl in
      if v = Some s then Some cl
      else c
    | _ -> c
  ) None l

let find_simp_prop s l =
  find_prop s l
  |>> (function [PCData x] -> Some (String.trim x) | _-> None)
           
let find_name s t = find_name_rec s None t

let idmap = ref StringMap.empty
  
let rec parse_Grml_expr : type a . a expr' -> Xml.xml -> a expr' option = fun parseType e ->
  begin match e with 
  | Element ("attribute",atl,cl) ->
    let t = find_at "name" atl in
    begin match t with
      | Some "expr" -> opHd cl |>> parse_Grml_expr parseType
      | Some "numValue" | Some "intValue" ->
	let sv = cl |> (function [PCData x] -> Some (String.trim x) | _ -> None) in
	begin match sv,parseType with
	    None,_ -> None
	  | Some bv,Bool _ -> Some (Bool (bool_of_string bv))
	  | Some iv,Int _ -> Some (Int (int_of_string iv))
	  | Some fv,Float _ -> Some (Float (float_of_string fv))
	
	  | _ -> None
	end
      | Some "name" ->
	let sv = cl |> (function [PCData x] -> Some (String.trim x) | _ -> None) in
	begin match sv,parseType with
	    None,_ -> None
	  | Some bv,Bool _ -> Some (BoolName bv)
	  | Some iv,Int _ -> Some (IntName iv)
	  | Some fv,Float _ -> Some (FloatName fv)
	  | _ -> None
	end
	  
      | Some te -> Printf.printf "at type : %s\n" te;
	None
      |_ ->  None
    end
  |  _ -> None
  end

let int_expr_of_atr s at =
  let a =		    
    find_prop s at |>> (function  x::_ -> parse_Grml_expr (Int 0) x; | _-> None ) in
    a
let float_expr_of_atr s at =
  find_prop s at
  |>> (function  x::_ -> parse_Grml_expr (Float 0.0) x | _-> None )
let bool_expr_of_atr s at =
  find_prop s at
  |>> (function  x::_ -> parse_Grml_expr (Bool false) x | _-> None )

let parse_distr at =
  match find_prop "distribution" at with
      Some (cl) -> begin
	let dist = find_simp_prop "type" cl in
	let larg = List.fold_left (fun c child ->
	  match child with
	      Element ("attribute",["name","param"],cl2) ->
		let n= Some (find_simp_prop "number" cl2 |>>> int_of_string |>>| 0 )in
		let ex = float_expr_of_atr "expr" cl2 in
		Some n
	    |>> (fun x -> ex |>>> (fun y -> (x,y)::c))
	    |>>| c
	    | _ -> c
	) [] cl
	  |> List.sort (fun (i,_) (j,_) -> compare i j) in
	match dist,larg with
	    (Some "EXPONENTIAL",[_,f]) -> StochasticPetriNet.Exp(f)
	  | (Some "IMMEDIATE",[]) -> Imm
          | (Some "MASSACTION",[]) -> StochasticPetriNet.Exp(Float 1.0)
          | (Some "MASSACTION",[_,f]) -> StochasticPetriNet.Exp(f)
	  | (Some "IMDT",[]) -> Imm
	  | (Some "DETERMINISTIC",[_,f]) -> Det(f)
          | (Some "UNIFORM",[_,f1;_,f2]) -> Unif(f1,f2)
          | (Some "DISCRETEUNIF",[_,f1]) -> DiscUnif(Floor f1)
	  | (Some "USERDEFINE",_) -> DiscUserDef(0)
	  | (Some "PLAYER1",[]) -> Player1
          | (Some "ERLANG",[_,f1;_,f2]) -> Erl(Floor f1,f2)
	  | (Some "GEOMETRIC",[_,f1;_,f2]) -> Geometric(f1,f2)
          | (Some "SYNC",[]) -> Imm
	  | (Some x,y) -> failwith ("Unknown distribution "^x^"/"^(string_of_int (List.length y)) )
	  | _ -> failwith "ill define distribution"
      end
    | _ -> failwith "ill define distribution"
 


let rec net_of_tree (n:StochasticPetriNet.spt) = function
  | Element (name,alist,clist) -> begin
    (*Printf.printf "new xmlnode: %s\n" name;*)
    begin match name with
      "xml" | "model" ->
	List.iter (net_of_tree n) clist
      | "attribute" ->
	 begin match find_at "name" alist |>>| "" with
	       | "declaration" | "constants" | "intConsts" | "realConsts"->
		  List.iter (net_of_tree n) clist
	       | "intConst" ->
		  let nameopt = find_simp_prop "name" clist in
		  let value = (int_expr_of_atr "expr" clist) in
		  begin match nameopt,n.Net.def with
			  Some(name),Some(def) ->
			  Printf.fprintf !logout "intconst: %s:%a\n" name printH_expr (value |>>| Int(0));
			  def.intconst <- ((name,value)::def.intconst)
			| _ ,None -> failwith "undefined preembule"
			| _ -> failwith "fail to read intConst"
		  end
	       | "realConst" ->
		  let nameopt = find_simp_prop "name" clist in
		  let value = (float_expr_of_atr "expr" clist) in
		  begin match nameopt,n.Net.def with
			  Some(name),Some(def) ->
			  Printf.fprintf !logout "floatconst: %s:%a\n" name printH_expr (value |>>| Float(0.0));
			  def.floatconst <- ((name,value)::def.floatconst)
			| _ ,None -> failwith "undefined preembule"
			| _ -> failwith "fail to read intConst"
		  end
	       | x-> Printf.fprintf stderr "Dont know what to do with attibute \"%s\", ignore\n" x;
	 end
		      
      |  "node" when find_at "nodeType" alist = Some "transition" ->
	 (match ((find_simp_prop "name" clist),(find_id alist)) with
	     (Some name,Some id) -> begin
	       let d = parse_distr clist in
	       Printf.fprintf !logout "new transition: %s\n\tdistribution: %a\n" name StochPTPrinter.printH_distr d;
	       idmap := StringMap.add id name !idmap;
	       Data.add (name,(d,(Float 1.0),(Float 1.0))) n.Net.transition
	     end
           | _-> ()
	 )

       |  "node" when find_at "nodeType" alist = Some "place" ->
	   let bo = (int_expr_of_atr "bound" clist) in
	   ( match ((find_simp_prop "name" clist),(int_expr_of_atr "marking" clist),(find_id alist)) with
           | (Some name,Some im,Some id) ->  begin
	   idmap := StringMap.add id name !idmap;
      	   Printf.fprintf !logout "new place: %s\n\tmarking: %a\n" name printH_expr im;
	   Data.add (name,(im,bo)) n.Net.place;
	 end
         | (Some name,None,Some id) -> begin
	   Printf.fprintf !logout "new place: %s\n" name;
	   idmap := StringMap.add id name !idmap;
	   Data.add (name,(Int 0,bo)) n.Net.place;
	 end  
         | (None,None,Some _) ->  Printf.fprintf stderr "Unknown node\n";
	 | _ ->()
                 
       )
       |  "arc" -> (
	 let sid = find_at "source" alist |>>> (fun x -> StringMap.find x !idmap)
	 and tid = find_at "target" alist |>>> (fun x -> StringMap.find x !idmap)
	 and arctype = find_at "arcType" alist |>>| "arc" in
	 begin match ((find_id alist),(sid),(tid),(int_expr_of_atr "valuation" clist),arctype ="inhibitorarc" ) with
           | (_,Some source,Some target,Some v, false) ->
	     Printf.fprintf !logout " (%s)-(%a)->(%s)\n" source printH_expr v target ;
	     Net.add_arc n source target v
	   | (_,Some source,Some target,Some v, true) ->
	     Printf.fprintf !logout " (%s)-(%a)-o(%s)\n" source printH_expr v target ;
	     Net.add_inhibArcS n source target v
	   | (_,Some source,Some target, None ,false) -> Printf.printf " (%s)-(1)->(%s)\n" source target ;
	     StochPTPrinter.print_spt_marcie "test.andl" n ;
	     Net.add_arc n source target (Int 1)
	   | (_,Some source,Some target,None, true) -> Printf.printf " (%s)-(1)-o(%s)\n" source target ;
	     StochPTPrinter.print_spt_marcie "test.andl" n ;
	     Net.add_inhibArcS n source target (Int 1)
	   | _-> ()
	 end
    )

       | x-> Printf.printf "Dont know what to do with %s, ignore\n" x;
    end
end
    | _ ->()

