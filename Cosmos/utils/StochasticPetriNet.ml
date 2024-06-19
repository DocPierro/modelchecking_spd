#use "PetriNet.ml"

type distr = Exp of float | Imm of float;;

let print_distr f d = 
  output_string f "    <attribute name=\"distribution\">
      <attribute name=\"type\">\n";
  begin match d with
      Exp r -> Printf.fprintf f "        EXPONENTIAL
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\"><attribute name=\"numValue\">
          %f
        </attribute></attribute>
      </attribute>
    </attribute>" r;
    | Imm p -> Printf.fprintf f "        DETERMINISTIC
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\"><attribute name=\"numValue\">
          0
        </attribute></attribute>
      </attribute>
    </attribute>
    <attribute name=\"weight\">
      <attribute name=\"expr\"><attribute name=\"numValue\">
        %f
      </attribute></attribute>
    </attribute>" p;
  end

let print_tr f name id rate =
  Printf.fprintf f "  <node id=\"%i\" nodeType=\"transition\">
    <attribute name=\"name\">%s</attribute>" id name;
  print_distr f rate;
  output_string f "</node>\n"
  

let print_pl f name id tok =
  Printf.fprintf f "  <node id=\"%i\" nodeType=\"place\">
    <attribute name=\"name\">%s</attribute>
    <attribute name=\"marking\">
      <attribute name=\"expr\"><attribute name=\"numValue\">
        %i
      </attribute></attribute>
    </attribute>
  </node>\n" id name tok


let print_arc f id source target valuation inhib =
  let arctype = if inhib then "inhibitorarc" else "arc" in
  Printf.fprintf f "  <arc id=\"%i\" arcType=\"%s\" source=\"%i\" target=\"%i\">
    <attribute name=\"valuation\">
      <attribute name=\"expr\"><attribute name=\"numValue\">
        %i
      </attribute></attribute>
    </attribute>
  </arc>\n" id arctype source target valuation 

let gen_const f li lr =
  Printf.fprintf f "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<model formalismUrl=\"http://formalisms.cosyverif.org/sptgd-net.fml\" xmlns=\"http://cosyverif.org/ns/model\">
  <attribute name=\"declaration\"><attribute name=\"constants\">
    <attribute name=\"intConsts\">\n";
  List.iter (fun (n,v) -> 
     Printf.fprintf f "      <attribute name=\"intConst\">
        <attribute name=\"name\">%s</attribute>
        <attribute name=\"expr\"><attribute name=\"numValue\">
            %i
        </attribute></attribute>
      </attribute>\n" n v) li;
  
  Printf.fprintf f "    </attribute>
    <attribute name=\"realConsts\">\n";
  List.iter (fun (n,v) -> 
    Printf.fprintf f "      <attribute name=\"realConst\">
        <attribute name=\"name\">%s</attribute>
        <attribute name=\"expr\"><attribute name=\"numValue\">
            %f
        </attribute></attribute>
      </attribute>\n" n v) lr;
    Printf.fprintf f
      "     </attribute>
    </attribute>
  </attribute>\n"

type spt = (int,distr) Net.t;;

let print_spt fpath net =
  let f = open_out fpath in
  
  gen_const f [] [];
  let np = Data.fold (fun i (s,m) ->print_pl f s i m; i+1) 0 net.Net.place in
  let nt = Data.fold (fun i (s,r) ->print_tr f s i r; i+1) np net.Net.transition in
  let nia = Data.fold (fun i (_,(v,p,t)) ->print_arc f i p (t+np) v false; i+1) nt net.Net.inArc in
  let _ = Data.fold (fun i (_,(v,t,p)) ->print_arc f i (t+np) p v false; i+1) nia net.Net.outArc in
  ();
(*
  for i =1 to n do
    print_pl f ("RE_Queue"^(string_of_int i)) countid (if i=1 then 1 else 0);
  done;

  print_tr f "Arrive" countid "lambda";

  for i =1 to n do
    print_tr f ("Serve"^(string_of_int i)) countid ("rho"^(string_of_int i));
  done;
  
  if genDTMC then for i =1 to n do
      print_tr f ("AServe"^(string_of_int i)) countid ("rho"^(string_of_int i));
    done;

  for i =0 to n-1 do
    print_arc f countid (n+i) i 1 false;
    print_arc f countid i (n+i+1) 1 false;
  done;
  
  if genDTMC then for i =0 to n-1 do
      print_arc f countid i (2*n+i+1) 1 true;
    done;*)
  Printf.fprintf f "</model>";
  close_out f;;

let rec inline_ls d = function
  | [] -> ""
  | [s] -> s
  | t::q -> t^d^(inline_ls d q);;

let print_arc_marcie net t =
  let l1 = Data.fold (fun l (_,(v,t2,p)) ->
    if t<>t2 then l else 
      (Printf.sprintf "[%s + %i]" (fst (Data.acca net.Net.place p)) v) ::l)
    [] net.Net.outArc in
  let l2 = Data.fold (fun l (_,(v,p,t2)) ->
    if t<>t2 then l else 
      (Printf.sprintf "[%s - %i]" (fst (Data.acca net.Net.place p)) v) ::l)
    l1 net.Net.inArc in
  
  inline_ls " & " l2;;
  

let print_spt_marcie fpath net =
  let f = open_out fpath in
  output_string f "gspn [generated_cosmos] {\n";
  
  output_string f "places:\n";
  Data.iter (fun (s,m) ->Printf.fprintf f "\t%s = %i;\n" s m) net.Net.place;

  output_string f "\ntransitions:\n";
  output_string f "\tstochastic:\n";
  Data.iter (fun (s,distr) -> match distr with
      Exp r -> Printf.fprintf f "\t%s : : %s : %f ;\n" s 
    (print_arc_marcie net (Data.index net.Net.transition s)) r
    | _ -> ()   
  ) net.Net.transition;
  output_string f "\timmediate:\n";
  Data.iter (fun (s,distr) -> match distr with
      Imm r -> Printf.fprintf f "\t%s : : %s : %f ;\n" s 
    (print_arc_marcie net (Data.index net.Net.transition s)) r
    | _ -> ()   
  ) net.Net.transition;


 
  output_string f "}\n";
  close_out f;;

let print_arc_dot f s1 s2 v =
  if v<>1 then
    Printf.fprintf f "\t%s -> %s [label=\"%i\"];\n" s1 s2 v
  else Printf.fprintf f "\t%s -> %s;\n" s1 s2;;

let print_spt_dot fpath net cl p =
  let f = open_out fpath in
  output_string f "digraph G {\n";
  
(*  output_string f "\tsubgraph place {\n";
  output_string f "\t\tgraph [shape=circle];\n";
  output_string f "\t\tnode [shape=circle,fixedsize=true];\n";*)
  Data.iter (fun (s,m) ->
    let label = (match m with 
	0 -> "" | 1 -> "•" | 2 -> "••" | i -> string_of_int i) in
    let pos = (try let x,y = List.assoc s p in
		   Printf.sprintf ",pos=\"%f,%f!\"" (2.0*.x) (2.0*.y)
      with Not_found -> "") in
	
    Printf.fprintf f "\t%s [shape=circle,xlabel=\"%s\",label=\"%s\"%s];\n" s s label pos
  ) net.Net.place;
  (*output_string f "\t}\n\tsubgraph transition {\n";
  output_string f "\t\tnode [shape=rect,fixedsize=true,height=0.2,style=filled,fillcolor=black];\n";*)
  Data.iter (fun (s,m) -> let color = (match m with Exp _->"" | _->",fillcolor=black") in
			  Printf.fprintf f "\t%s [shape=rect,fixedsize=true,height=0.2,style=filled%s,xlabel=\"%s\",label=\"\"];\n" s color s) net.Net.transition;
  (*output_string f "\t}\n";*)
  Data.iter (fun (_,(v,p,t)) ->
    print_arc_dot f (fst (Data.acca net.Net.place p)) 
      (fst (Data.acca net.Net.transition t)) v) net.Net.inArc;
  Data.iter (fun (_,(v,t,p)) ->
    print_arc_dot f (fst (Data.acca net.Net.transition t))
      (fst (Data.acca net.Net.place p)) v) net.Net.outArc;

  List.iter (fun (s,l) ->
    Printf.fprintf f "\tsubgraph \"test%i\" {\n\trank=%i; " s s;
    List.iter (fun s2 -> Printf.fprintf f " %s;" s2) l; 
    output_string f "}\n";) cl;

  output_string f "}\n"; 
  close_out f;;


