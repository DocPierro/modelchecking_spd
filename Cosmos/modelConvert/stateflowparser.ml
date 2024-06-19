open Xml
open Type
open StateflowType
open Lexing

(* Handling of SSID*)
let ssid_count = ref (-1)
let getSSID al = 
  let i = int_of_string (List.assoc "SSID" al)  in
  ssid_count := max !ssid_count (i+1); 
  i

let fresh_ssid () =
  let f= !ssid_count in
  incr ssid_count; f

(* Function to fetch various attribute in XML tree*)
let rec findprop f = function
  | [] -> None
  | te::q -> match te with
    | Element ("props",_,cl2) -> begin match findprop f cl2 with
      None -> (match f te with None -> findprop f q | x-> x)
      | x -> x
    end
    | t -> (match f t with None -> findprop f q | x-> x)

let findpropName s cl = 
  findprop (function 
  | Element ("P",["Name",x],[PCData(l)]) when x=s -> Some l 
  | _ -> None) cl

let getName = findpropName "labelString"
let getScope = findpropName "scope"
let getDescription = findpropName "description"
let getType = findpropName "type"

let getPriority cl = findpropName "executionOrder" cl
  |>> (fun x -> Some (int_of_string x))

let getdst cl = findprop (function 
  | Element ("dst",_,cl2) -> 
    findpropName "SSID" cl2 |>>> int_of_string
  | _ -> None) cl

let getsrc cl = findprop (function 
  | Element ("src",_,cl2) -> 
    findpropName "SSID" cl2 |>>> int_of_string
  | _ -> None) cl

let getScript cl = findprop (function 
  | Element ("eml",_,cl2) -> findpropName "script" cl2
  | _ -> None) cl

let rec exp_data = function
   | Element ("data",alist,clist) when getScope clist = Some "LOCAL_DATA" ->
     let name = List.assoc "name" alist
     and initValue = findpropName "initialValue" clist |>>| "0"
     and datatype = findpropName "dataType" clist |>>| "double" in
     Some ((getSSID alist),Var(datatype,name,initValue))
    | _-> None

(* position for error reporting in the parser of edge*)
let print_position outx lexbuf =
  let pos = lexbuf.Lexing.lex_curr_p in
  Printf.fprintf outx "%s:%d:%d" pos.pos_fname
    pos.pos_lnum (pos.pos_cnum - pos.pos_bol + 1)

let regexp = "f[^̂†]*\\)\nend"
let parse_regexp = Str.regexp 
  "function[ ]*\\([A-Za-z0-9_]+\\)[ ]*=[ ]*\\([A-Za-z0-9_]+\\)\\([(][A-Za-z0-9_, ]*[)]\\)[ ]*\n\\([^§]*\\)\nend";;

let reg_comm = Str.regexp "%"
let reg_type1 = Str.regexp "[(]"
let reg_type2 = Str.regexp "[,]"

(* transform matlab function into C function *)
let parse_fun comm scr =
  if not (Str.string_match parse_regexp scr 0) then failwith ("Fail to parse:"^scr);
  let var = Str.matched_group 1 scr
  and name = Str.matched_group 2 scr
  and arg = Str.matched_group 3 scr
  and body = Str.matched_group 4 scr in
  let arg2 = if arg= "()" then "()" else arg
    |> Str.global_replace reg_type1 "(double "
    |> Str.global_replace reg_type2 ", double " in
  let body2 = Str.global_replace reg_comm "//" body in
  match comm with 
    Some s when String.sub s 0 6 = "Cosmos" -> 
      let s2 = String.sub s 7 (String.length s -7) in
      (name,s2)
  | Some s when String.sub s 0 9 = "Prototype" -> 
    let s2 = String.sub s 9 (String.length s -9) in
    (name,s2)
  | Some s when String.sub s 0 9 = "Cfunction" -> 
    let s2 = String.sub s 9 (String.length s -9) in
    (name,s2)
  | _ ->  (name,Printf.sprintf "double %s%s{\n//%s\n\tdouble %s;\n%s\n\treturn %s;\n}" name arg2 (string_of_option "" comm) var body2 var)
	    
(* Parse a simulink automaton recursively *)				   
let rec exp_mod (sl,tl,scriptl) = function
    | Element (name,alist,clist) ->
       begin match name with
       | "Children" -> List.fold_left exp_mod (sl,tl,scriptl) clist
       | "state" -> begin match getScript clist with
			    None -> (* It is a real state add it to list of state *)
			    (((getSSID alist),(getName clist))::sl,tl,scriptl)
			  | Some scr -> (* It is a Matlab function definition *)
			     let comm = getDescription clist in
			     let fn,fb = parse_fun comm scr in (sl,tl,(Funct(fn,fb))::scriptl)
       end 
       | "transition" -> (match getdst clist with Some dst ->
	 let desc = getDescription clist
	 |>> (fun x -> let n = String.length x in if n<=7 then None 
	   else if String.sub x 0 7 = "Cosmos\n" then Some (let s2 = String.sub x 7 (n-7) in s2) else None) in
	 (sl,((getSSID alist),(getName clist),(getsrc clist),dst,desc)::tl,scriptl) 
	 | _ -> failwith "Ill formed transition")
       | "data" -> begin try 
			   let name = List.assoc "name" alist
			   and initValue = findpropName "initialValue" clist |>>| "0"
			   and datatype = findpropName "dataType" clist |>>| "double" in
			   Printf.fprintf stdout "new var data:%s with type: %s\n" name datatype;
			   (sl,tl,(Var(datatype,name,initValue))::scriptl); 
	 with
	 | Not_found -> (sl,tl,scriptl)
       end

       | x -> Printf.fprintf stderr "Dont know what to do with %s, ignore\n" x; (sl,tl,scriptl)
       end
    | PCData (s) -> output_string stderr s; (sl,tl,scriptl)

(*Parse edge of automaton use the grammar define in ParserSimulEdge.mly  *)
let eval_trans sl priority (ssid,name,src,dst,com) = match name with
    None -> (ssid,src,{empty_trans_label with description=com} ,dst)
  | Some s -> let lexbuf = Lexing.from_string s in
	      try     
		let label = ParserSimulEdge.main LexerSimulEdge.token lexbuf in
		let label2 = {label with write = List.sort_uniq compare label.write;
			     nameT = src |>> (fun x -> List.assoc x sl) |>>> (fun x -> "L_"^x) ;
			     priority = priority |>>> float |>>| 1.0 ;
			     description=com
			     } in
		(ssid,src,label2,dst)
    with 
      Parsing.Parse_error ->
	Printf.fprintf stderr "%a: Parsing error: unexpected token:'%s' in %s\n"
	  print_position lexbuf (Lexing.lexeme lexbuf) s;
	(ssid,src,{empty_trans_label with description=com},dst)

(* Parse a simulink automaton*) 
let module_of_simul al cl = 
  let ssid = getSSID al in
  let name = getName cl in
  let priority = getPriority cl in
  let childs = List.find (function Element ("Children",_,_)->true | _-> false) cl in
  let sl,tl,scriptl = exp_mod ([],[],[]) childs in
  let modu = ssid,name,sl,(List.map (eval_trans sl priority) tl),scriptl,priority,StringMap.empty in 
  (*print_module modu;*)
  modu

(* Print a simulink automaton for dot *)
let print_modu f (ssid,name,sl,tl,scrl,p,_) =
  Printf.fprintf f "\tsubgraph cluster%i {\n" ssid;
  match name with None -> Printf.fprintf f "\t%i[shape=rectangle]\n" ssid
  | Some n2 -> Printf.fprintf f "\t%s[shape=rectangle]\n" n2;
  List.iter (fun (x,y) ->
    Printf.fprintf f "\t%i [shape=circle,label=\"%a\"];\n" x print_option y) sl; 
  List.iter (fun (ssid,src,label,dst) ->
    match src with None -> () | Some src2 ->
    Printf.fprintf f "\t%i -> %i[shape=circle,label=\"%a\"];\n" src2 dst print_label_simulink label) tl;
  output_string f "}\n"
    

let print_simulink_dot fpath ml =
  let f = open_out fpath in
  output_string f "digraph G {\n";
(*  output_string f "\tsubgraph place {\n";
  output_string f "\t\tgraph [shape=circle];\n";
  output_string f "\t\tnode [shape=circle,fixedsize=true];\n";*)
  List.iter (print_modu f) ml;
  output_string f "}\n"; 
  close_out f;;

(* Walk the tree and build a list of module*)
let rec modulist_of_stateflow ml = function
    | Element (name,alist,clist) as t ->
       begin match name with
       | "xml" | "ModelInformation" | "Model" | "Stateflow" | "machine" | "chart" | "Children" ->
	 List.fold_left modulist_of_stateflow ml clist
       | "P" -> ml
       | "state" -> begin match getType clist with
	 None -> ml
	 | Some t when t= "OR_STATE" -> List.fold_left modulist_of_stateflow ml clist
	 | Some t when t= "AND_STATE" -> (module_of_simul alist clist)::ml
	 | Some t when t= "FUNC_STATE" -> begin
	   match getScript clist with
	     None -> ml
	   | Some scr -> 
	     let comm = getDescription clist in
	     let fn,fb = parse_fun comm scr in
	     let modu = (getSSID alist),(Some fn),[],[],[Funct(fn,fb)],None,StringMap.empty in
	     modu::ml
	 end
	 | Some t -> Printf.fprintf stderr "Dont know what to do with %s, ignore\n" t; ml
       end
       | "transition" -> ml
       | "target" -> ml
       | "subviewS" -> ml
       | "instance" -> ml
       | "data" -> begin match exp_data t with None -> ml 
	 | Some (ssid,var) ->
	   (ssid,None,[],[],[var],None,StringMap.empty)::ml
	 end
       | "event" -> begin
	 let name = findpropName "name" clist |>>| "__NO_NAME"
	 and ssid = getSSID alist 
	 and scope = findpropName "scope" clist in 
	 let scope2 = scope |>>| "No type" |> (function 
	     ("OUTPUT_EVENT") -> Out
	   | ("LOCAL_EVENT") -> Local
	   | ("INPUT_EVENT") -> In
	   | x -> failwith ("Unknowm event type:"^x))  in
	 Printf.fprintf !logout "New event: %s : %i : %a\n" name ssid print_option scope;
	 (ssid,None,[],[],[],None,(StringMap.singleton name (ssid,scope2)))::ml
	 (*match scope with
	   Some "OUTPUT_EVENT" -> 
	     (ssid,Some ("Output_sig_"^name),[ssid,Some "q0"],
	      [ssid,Some ssid,{empty_trans_label with trigger = RAction name},ssid],[],None)::ml
	 |_ ->*)
       end
       | x -> Printf.fprintf stderr "Dont know wat to do with %s, ignore\n" x; ml
       end
    | PCData (s) -> output_string stderr s; ml

let rec modulist_of_tree ml = function
  | Element (name,alist,clist) as t->
    begin match name with
    | "xml" | "ModelInformation" | "Model" | "P" ->
      List.fold_left modulist_of_tree ml clist
       | "Stateflow" -> modulist_of_stateflow ml t
       | _ -> ml
       end
    | PCData (s) -> ml
