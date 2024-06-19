open Type
open PetriNet
open StochasticPetriNet

let string_of_cmpGrML = function
  | EQ -> "equal"  
  | NEQ-> "notEqual" 
  | LE-> "lessOrEqual"
  | SL-> "stricltyLess"
  | GE-> "greaterOrEqual"
  | SG-> "strictlyGreater"

let print_at f x fy v =
  Printf.fprintf f "<attribute name=\"%s\">%a</attribute>" x fy v

let rec print_expr: type a. out_channel -> a expr' -> unit = fun f x -> 
  let pa x f = print_at f x print_expr in 
  let pa2 x f = print_at f x (fun f (v1,v2) -> Printf.fprintf f "\n\t%a\n\t%a\n" print_expr v1 print_expr v2) in
  match x with
  | IntName(n) -> print_at f "name" output_string n
  | FloatName(n) -> print_at f "name" output_string n
  | BoolName(n) -> print_at f "name" output_string n
  | CastBool(e) -> print_expr f e
  | Int(fl) -> print_at f "numValue" (fun f ->Printf.fprintf f "%i") fl
  | Float(fl) -> print_at f "numValue" (fun f ->Printf.fprintf f "%f") fl
  | Bool(fl) -> print_at f "boolValue" (fun f ->Printf.fprintf f "%B") fl 
  | Not(fe) -> print_at f "function" (pa "not") fe
  | And(fe1,fe2) -> print_at f "function" (pa2 "and") (fe1,fe2)
  | Or(fe1,fe2)  -> print_at f "function" (pa2 "or") (fe1,fe2)
  | Mult(fe1,fe2) -> print_at f "function" (pa2 "*") (fe1,fe2)
  | Plus(fe1,fe2) -> print_at f "function" (pa2 "+") (fe1,fe2)
  | Minus(fe1,fe2) -> print_at f "function" (pa2 "minus") (fe1,fe2)
  | Ceil(fe) -> print_at f "function" (pa "ceil") fe
  | Floor(fe) -> print_at f "function" (pa "floor") fe
  | CastInt(ie) -> print_expr f ie
  | Exp(fe) -> print_at f "function" (pa "exp") fe
  | Div(fe1,fe2) -> print_at f "function" (pa2 "/") (fe1,fe2)
  | FunCall(s,fl) -> print_at f "function" (fun f -> print_at f s (fun f l ->
  List.iter (fun x ->print_expr f x;output_string f "\n  ";) l)) fl
  | IntAtom(fe1,cmp,fe2) -> print_at f "function" (pa2 (string_of_cmpGrML cmp)) (fe1,fe2)
  | FloatAtom(fe1,cmp,fe2) -> print_at f "function" (pa2 (string_of_cmpGrML cmp)) (fe1,fe2)
  | BoolAtom(fe1,cmp,fe2) -> print_at f "function" (pa2 (string_of_cmpGrML cmp)) (fe1,fe2)
  | If(c,e1,e2) -> print_at f "function" (fun f -> print_at f "If" (fun f () ->
    print_expr f c;output_string f "\n  ";
    print_expr f e1;output_string f "\n  ";
    print_expr f e2;output_string f "\n  ";
)) ()

let print_int_expr f (x: int expr') = print_expr f x
let print_float_expr f (x: float expr') = print_expr f x

let print_distr f d = 
  output_string f "    <attribute name=\"distribution\">
      <attribute name=\"type\">\n";
  begin match d with
      Exp r -> Printf.fprintf f "        EXPONENTIAL
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\">%a</attribute>
      </attribute>
                                 </attribute>" print_expr r
    | Synchronisation s ->
       Printf.fprintf f "        SYNC
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\"><attribute name=\"unParsed\">
          %s
        </attribute></attribute>
      </attribute>
    </attribute>" s
    |       Player1 -> Printf.fprintf f "        PLAYER1
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\"><attribute name=\"numValue\">
          0
        </attribute></attribute>
      </attribute>
    </attribute>"
  | Erl (i,r) -> Printf.fprintf f "        ERLANG
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\">%a</attribute>
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">1</attribute>
        <attribute name=\"expr\">%a</attribute>
      </attribute>
      </attribute>" print_expr i print_expr r
  | DiscUnif (i) -> Printf.fprintf f "        DISCRETEUNIF
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\">%a</attribute>
      </attribute>
    </attribute>" print_expr i
  | Unif (l,h) -> Printf.fprintf f "        UNIFORM
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\">%a</attribute>
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">1</attribute>
        <attribute name=\"expr\">%a</attribute>
      </attribute>
      </attribute>" print_expr l print_expr h
  | Geometric (l,h) -> Printf.fprintf f "        GEOMETRIC
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\">%a</attribute>
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">1</attribute>
        <attribute name=\"expr\">%a</attribute>
      </attribute>
    </attribute>" print_expr l print_expr h                                 
  | Imm -> Printf.fprintf f "        DETERMINISTIC
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\"><attribute name=\"numValue\">
          0
        </attribute></attribute>
      </attribute>
    </attribute>"
  | Det p -> Printf.fprintf f "        DETERMINISTIC
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\">
          %a
        </attribute>
      </attribute>
    </attribute>" print_expr p
  | Norm (m,v) -> Printf.fprintf f "        NORMAL
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\">%a</attribute>
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">1</attribute>
        <attribute name=\"expr\">%a</attribute>
      </attribute>
    </attribute>" print_expr m print_expr v
  | DiscUserDef (i) -> Printf.fprintf f "        DISCRETEUSERDEFINE
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\"><attribute name=\"numValue\">%i</attribute></attribute>
      </attribute>
    </attribute>" i
  end

let printH_distr f = function
  | Imm -> Printf.fprintf f "DETERMINISTIC(0.0)"
  | Det r -> Printf.fprintf f "DETERMINISTIC(%a)" printH_expr r
  | Exp r -> Printf.fprintf f "EXPONENTIAL(%a)" printH_expr r
  | Erl (i,r) -> Printf.fprintf f "ERLANG(%a,%a)" printH_expr i
    printH_expr r
  | Unif (i,r) -> Printf.fprintf f "UNIFORM(%a,%a)" printH_expr i
                                 printH_expr r
  | DiscUnif (i) -> Printf.fprintf f "DISCRETEUNI(%a)" printH_expr i
  | Geometric (i,r) -> Printf.fprintf f "GEOMETRIC(%a,%a)" printH_expr i
                                 printH_expr r

  | Norm (m,v) -> Printf.fprintf f "NORMAL(%a,%a)" printH_expr m
    printH_expr v
  | DiscUserDef (i) -> Printf.fprintf f "EXPONENTIAL(%i)" i
  | Player1 -> Printf.fprintf f "PLAYER1()"
  | Synchronisation s -> Printf.fprintf f "SYNC(%s)" s

let printH_distr_CTMC f = function
  | Imm -> Printf.fprintf f "I"
  | Exp r -> Printf.fprintf f "%a" printH_expr r
  | d -> printH_distr f d

let print_tr f name id (rate,weight,prio) =
  Printf.fprintf f "  <node id=\"%i\" nodeType=\"transition\">
    <attribute name=\"name\">%s</attribute>
%a
      <attribute name=\"priority\"><attribute name=\"expr\">
        %a
      </attribute></attribute>
      <attribute name=\"weight\"><attribute name=\"expr\">
        %a
      </attribute></attribute>
  </node>\n" id name print_distr rate print_float_expr prio print_float_expr weight
  
let print_pl f name id (tok,_) =
  Printf.fprintf f "  <node id=\"%i\" nodeType=\"place\">
    <attribute name=\"name\">%s</attribute>
    <attribute name=\"marking\"><attribute name=\"expr\">
      %a
    </attribute></attribute>
  </node>\n" id name print_int_expr tok

let print_arc f id source target valuation inhib =
  let arctype = if inhib then "inhibitorarc" else "arc" in
  Printf.fprintf f "  <arc id=\"%i\" arcType=\"%s\" source=\"%i\" target=\"%i\">
    <attribute name=\"valuation\"><attribute name=\"expr\">
        %a
    </attribute></attribute>
  </arc>\n" id arctype source target print_int_expr valuation
 
let gen_const f li lr le fund classdef =
  Printf.fprintf f "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<model formalismUrl=\"http://formalisms.cosyverif.org/sptgd-net.fml\" xmlns=\"http://cosyverif.org/ns/model\">
%a
  <attribute name=\"declaration\"><attribute name=\"constants\">
    <attribute name=\"intConsts\">\n" fund ();
  List.iter (fun (n,v) -> 
     Printf.fprintf f "      <attribute name=\"intConst\">
        <attribute name=\"name\">%s</attribute>
        <attribute name=\"expr\"><attribute name=\"numValue\">
            %a
        </attribute></attribute>
      </attribute>\n" n printH_expr v) li;
  
  Printf.fprintf f "    </attribute>
    <attribute name=\"realConsts\">\n";
  List.iter (fun (n,v) -> 
    Printf.fprintf f "      <attribute name=\"realConst\">
        <attribute name=\"name\">%s</attribute>
        <attribute name=\"expr\"><attribute name=\"numValue\">
            %a
        </attribute></attribute>
      </attribute>\n" n printH_expr v) lr;
    Printf.fprintf f
      "     </attribute>
    <attribute name=\"extConsts\">\n";
  List.iter (fun (n) -> 
     Printf.fprintf f "      <attribute name=\"extConst\">
        <attribute name=\"name\">%s</attribute>
      </attribute>\n" n) le;
  
  Printf.fprintf f "    </attribute>
  </attribute>\n";
  List.iter (fun s-> Printf.fprintf f "%s\n" s) classdef;
  Printf.fprintf f "</attribute>\n"
		 
let print_general_pt ?initid:(initid=0) fpath placePrinter transitionPrinter valuationPrinter net =
  let f = open_out fpath in
  let (lci,lcd,lce,fund,classDef) = begin match net.Net.def with None -> [],[],[],(fun _ ()->()),[]
						      | Some a -> a.intconst,a.floatconst,a.clock,a.printer,a.classDef end in
  gen_const f 
    (List.map (fun (s,ao) ->
      match ao with None -> s,(Int 1) | Some fl -> s,fl) lci) 
    (List.map (fun (s,ao) ->
      match ao with None -> s,(Float 1.0) | Some fl -> s,fl) lcd) lce fund classDef;
  let np = Data.fold (fun i (s,m) ->placePrinter f s i m; i+1) initid net.Net.place in
  let nt = Data.fold (fun i (s,r) ->transitionPrinter f s i r; i+1) np net.Net.transition in
  let nia = Data.fold (fun i (_,(v,p,t)) ->valuationPrinter f i ((Obj.magic p)+initid) ((Obj.magic t)+np) v false; i+1) nt net.Net.inArc in
  let nio = Data.fold (fun i (_,(v,p,t)) ->valuationPrinter f i ((Obj.magic t)+np) ((Obj.magic p)+initid) v false; i+1) nia net.Net.outArc in
  let _ = Data.fold (fun i (_,(v,p,t)) ->valuationPrinter f i ((Obj.magic p)+initid) ((Obj.magic t)+np) v true; i+1) nio net.Net.inhibArc in
  ();
  Printf.fprintf f "</model>";
  close_out f;;

let print_spt fpath (net:spt)  =
  print_general_pt fpath print_pl print_tr print_arc net;;

let print_pnml_arc f id source target valuation =
  Printf.fprintf f "      <arc id=\"A_%i\" source=\"PT_%i\" target=\"PT_%i\">
        <inscription><text>%a</text></inscription>
      </arc>\n" id source target printH_expr valuation

let print_pnml fpath (net:spt)  =
  let f = open_out fpath in
  Printf.fprintf f "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<pnml xmlns=\"http://www.pnml.org/version-2009/grammar/pnml\">
  <net id=\"Generated-By-Cosmos-0001\" type=\"http://www.pnml.org/version-2009/grammar/ptnet\">
    <page id=\"page0\">
      <name><text>DefaultPage</text></name>\n";
  let np = Data.fold (fun i (s,(m,_)) ->
    Printf.fprintf f "      <place id=\"PT_%i\">
        <name><text>%s</text></name>
        <initialMarking><text>%a</text></initialMarking>
      </place>\n" i s printH_expr m;
    i+1) 0 net.Net.place in
  let nt = Data.fold (fun i (s,_) ->
    Printf.fprintf f "      <transition id=\"PT_%i\">
        <name><text>%s</text></name>
      </transition>\n" i s;
    i+1) np net.Net.transition in
  let nia = Data.fold (fun i (_,(v,p,t)) ->print_pnml_arc f i (Obj.magic p) ((Obj.magic t)+np) v; i+1) nt net.Net.inArc in
  let nio = Data.fold (fun i (_,(v,t,p)) ->print_pnml_arc f i ((Obj.magic t)+np) (Obj.magic p) v; i+1) nia net.Net.outArc in
  let _ = Data.fold (fun _ _ ->failwith "inhibitor Arc not supported in PNML";) nio net.Net.inhibArc in
  ();
  Printf.fprintf f "    </page>
  </net>
</pnml>";
  close_out f;;

let rec inline_ls d = function
  | [] -> ""
  | [s] -> s
  | t::q -> t^d^(inline_ls d q);;

let print_arc_marcie net f t =
  let fq = Data.fold (fun first (_,(v,t2,p)) ->
    if t<>t2 then first else begin 
      if not first then Printf.fprintf f " & ";
      Printf.fprintf f "[%s + %a]" (fst (Data.acca net.Net.place p)) printH_expr v;
      false
    end)
    true net.Net.outArc in
  let fq2 = Data.fold (fun first (_,(v,p,t2)) ->
    if t<>t2 then first else begin
      if not first then Printf.fprintf f " & ";
      Printf.fprintf f "[%s - %a]" (fst (Data.acca net.Net.place p)) printH_expr v;
      false
    end)
    fq net.Net.inArc in
  ignore fq2

let print_condition_arc_marcie net f t =
  let fq3 = Data.fold (fun first (_,(v,p,t2)) ->
    if t<>t2 then first else begin
      if not first then Printf.fprintf f " & ";
      Printf.fprintf f "[%s < %a]" (fst (Data.acca net.Net.place p)) printH_expr v;
      false
    end)
    true net.Net.inhibArc in ignore fq3


let print_spt_marcie fpath net =
  let f = open_out fpath in
  output_string f "gspn [generated_cosmos] {\n";
  
  Printf.fprintf f "constants:\n";
  let (lci,lcd,lce,fund) =
    begin match net.Net.def with None -> [],[],[],(fun _ ()->())
			       | Some a -> a.intconst,a.floatconst,a.clock,a.printer end in
  List.iter (fun (s,ao) -> match ao with 
    None -> Printf.fprintf f "\tint %s;\n" s;
  | Some iv -> Printf.fprintf f "\tint %s=%a;\n" s printH_expr iv) lci;
  List.iter (fun (s,ao) -> match ao with 
    None -> Printf.fprintf f "\tint %s;\n" s;
  | Some fv -> Printf.fprintf f "\tdouble %s=%a;\n" s printH_expr fv) lcd;

  output_string f "places:\n";
  Data.iter (fun (s,(m,bo)) ->
	     Printf.fprintf f "\t%s = %a;\n" s printH_expr m
	    ) net.Net.place;

  output_string f "\ntransitions:\n";
  output_string f "\tstochastic:\n";
  Data.iter (fun (s,(distr,_,_)) -> match distr with
    Exp r -> Printf.fprintf f "\t%s : %a : %a : %a ;\n" s 
      (print_condition_arc_marcie net) (Data.index net.Net.transition s) (print_arc_marcie net) (Data.index net.Net.transition s) printH_expr r
  | Det r -> Printf.fprintf f "\t%s : %a : %a : %a ;\n" s 
    (print_condition_arc_marcie net) (Data.index net.Net.transition s) (print_arc_marcie net) (Data.index net.Net.transition s) printH_expr r    
  | Erl (n,r) -> Printf.fprintf f "\t%s : %a : %a : %a ;\n" s 
      (print_condition_arc_marcie net) (Data.index net.Net.transition s) (print_arc_marcie net) (Data.index net.Net.transition s) printH_expr (Div (r,CastInt (n))) 
    | _ -> ()   
  ) net.Net.transition;
  output_string f "\timmediate:\n";
  Data.iter (fun (s,(distr,r,_)) -> match distr with
      Imm -> Printf.fprintf f "\t%s : %a : %a : %a ;\n" s 
                            (print_condition_arc_marcie net) (Data.index net.Net.transition s) (print_arc_marcie net) (Data.index net.Net.transition s) printH_expr r
    | Player1 -> Printf.fprintf f "\t%s : %a : %a : %a ;\n" s 
                            (print_condition_arc_marcie net) (Data.index net.Net.transition s) (print_arc_marcie net) (Data.index net.Net.transition s) printH_expr r
    | _ -> ()   
  ) net.Net.transition;


  output_string f "}\n";
  close_out f;;

let print_arc_dot f s1 s2 v =
  if v<>Int 1 then
    Printf.fprintf f "\t%s -> %s [xlabel=\"%a\"];\n" s1 s2 printH_expr v
  else Printf.fprintf f "\t%s -> %s;\n" s1 s2;;

let print_inhib_arc_dot f s1 s2 v =
  if v<>Int 1 then
    Printf.fprintf f "\t%s -> %s [arrowhead=odot,xlabel=\"  %a  \"];\n" s1 s2 printH_expr v
  else Printf.fprintf f "\t%s -> %s [arrowhead=odot];\n" s1 s2;;


let colorOfTrans = function
  Exp (Float r) -> begin match r/. 0.009 with 
    x when x >=1. -> "\"#505050\""
  | x when x>= 1./.50. -> "\"#919191\""
  | _ -> "\"#D0D0D0\""
  end
  |_ -> "black";;
  

let print_spt_dot ?(showlabel=true) fpath net cl p =
  let f = open_out fpath in
  output_string f "digraph G {\n\n\tnode[fontsize=18];\n\tedge[fontsize=18];\n";
  
(*  output_string f "\tsubgraph place {\n";
  output_string f "\t\tgraph [shape=circle];\n";
  output_string f "\t\tnode [shape=circle,fixedsize=true];\n";*)
  Data.iter (fun (s,(m,_)) ->
    let pos = (try let x,y = List.assoc s p in
		   Printf.sprintf ",pos=\"%f,%f!\"" (0.75*.x) (0.75*.y)
      with Not_found -> "") in
	
    Printf.fprintf f "\t%s [shape=circle,xlabel=\"%s\",label=\"%a\"%s];\n" s (if showlabel then s else "") 
      print_token m
      pos
  ) net.Net.place;
  (*output_string f "\t}\n\tsubgraph transition {\n";
  output_string f "\t\tnode [shape=rect,fixedsize=true,height=0.2,style=filled,fillcolor=black];\n";*)
  Data.iter (fun (s,(d,_,_)) ->
    let pos = (try let x,y = List.assoc s p in
		   Printf.sprintf ",pos=\"%f,%f!\"" (0.75*.x) (0.75*.y)
               with Not_found -> "") in
    Printf.fprintf f "\t%s [shape=rect,fixedsize=true,height=0.12,width=0.5,style=filled,fillcolor=%s,xlabel=\"%s%a\",label=\"\"%s];\n" s (colorOfTrans d) (if showlabel then s^":" else "") (fun o v -> if showlabel then printH_distr_CTMC o v else ()) d pos) net.Net.transition;
  (*output_string f "\t}\n";*)
  Data.iter (fun (_,(v,p,t)) ->
    print_arc_dot f (fst (Data.acca net.Net.place p)) 
      (fst (Data.acca net.Net.transition t)) v) net.Net.inArc;
  Data.iter (fun (_,(v,p,t)) ->
    print_inhib_arc_dot f (fst (Data.acca net.Net.place p)) 
      (fst (Data.acca net.Net.transition t)) v) net.Net.inhibArc;
  Data.iter (fun (_,(v,t,p)) ->
    print_arc_dot f (fst (Data.acca net.Net.transition t))
      (fst (Data.acca net.Net.place p)) v) net.Net.outArc;

  List.iter (fun (s,l) ->
    Printf.fprintf f "\tsubgraph \"test%i\" {\n\trank=%i; " s s;
    List.iter (fun s2 -> Printf.fprintf f " %s;" s2) l; 
    output_string f "}\n";) cl;
  output_string f "}\n";
  close_out f;;

let display_dot net =
  let file = "tmpgraph" in
  print_spt_dot (file^".dot") net [] [];
  ignore @@ Sys.command (Printf.sprintf "dot -Tpdf %s.dot -o %s.pdf" file file );
  ignore @@ Sys.command (Printf.sprintf "open  %s.pdf" file )
