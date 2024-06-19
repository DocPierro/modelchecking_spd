open PetriNet
open StochasticPetriNet
open Type
open StateflowType
open Stateflowparser

let erlangstep = ref 10
let escape_XML_var = ref true

(* Distribution to attribute to Deterministic Delays*)
let detfun s =
  if !modelStoch then if !erlangstep=1 then
      StochasticPetriNet.Exp (Div (Float (float !erlangstep),s))
    else StochasticPetriNet.Erl (Int !erlangstep,Div (Float (float !erlangstep),s))
  else StochasticPetriNet.Det s

(* Generate human readable name*)
let post_name sl s t =
  match List.assoc s sl with
    None -> Printf.sprintf "%i_Pre" t
  | Some(sn) -> Printf.sprintf "%i_Pre_%s" t sn

let exist_delay s l =
  List.exists (fun (ssid,src,label,dst) -> match (src,label.trigger) with 
  Some (src2), Delay(_) -> src2 = s
  |_ -> false) l

(*
  Add intermediate state to some transition to implement simulink semantic
*)
let expand_trans l =
  List.map (fun (ssidmod,name,sl,tl,scrl,p,ss) ->
    let (nsl,ntl) = List.fold_left (fun (sl2,tl2) (ssid,src,label,dst) -> 
      match (label.trigger,label.write,src) with 
      | (Delay(_),(_::_),_) -> 
	(* wait and synch loop 
	   This is necessary to make delay transition independant, therefore they do not synchronize
	   with anything thus cannot be disabled by synchronization.
	   use only when write list is not empty *)
	let news = fresh_ssid ()
	and newsn = post_name sl dst ssid in
	let newt = fresh_ssid () in
	((news,Some newsn)::sl2,
	(ssid,src,{empty_trans_label with trigger=label.trigger; priority =label.priority; nameT=(src |>> (fun x ->List.assoc x sl)); description=label.description ; isFinal=false },news)::
	  (newt,Some(news),{label with trigger=Imm},dst)::tl2)
      | (Delay(_),[],_) -> 
	(* wait loop Check whether an instanenous transition exist in target state
	   change isFinal accordingly *)
	if List.exists (fun (_,s,l,_) -> s= Some dst && l.trigger=Imm) tl then
	  (sl2,(ssid,src,label,dst)::tl2)
	else
	  (sl2,(ssid,src,{label with isFinal=true},dst)::tl2)

      | (RAction(sn),_,Some src2) when src2=dst && exist_delay src2 tl->  
	(* self loop with read 
	   This case is used to reset delay transition waiting on this state,
	   without this delay transition are not disabled by a selfloop read action*)
	let news = fresh_ssid ()
	and newsn = post_name sl dst ssid in
	let newt = fresh_ssid () in
	((news,Some newsn)::sl2,
	(ssid,src,{empty_trans_label with trigger=label.trigger; priority =label.priority; nameT=Some sn },news)::
	  (newt,Some(news),{label with trigger=Imm},dst)::tl2)
      | (_,_,None) when label.update <> [] -> 
	(* Initial transition with update 
	   Initial transition are not real transition thus a new state and a new transition
	   is introduce to implement updates*)
	let news = fresh_ssid ()
	and newsn = post_name sl dst ssid in
	let newt = fresh_ssid () in
	((news,Some newsn)::sl2,
	 (ssid,None,{empty_trans_label with priority =label.priority},news)::
	   (newt,Some(news),{label  with nameT=Some "init"},dst)::tl2)
      | _ -> (sl2,(ssid,src,label,dst)::tl2)) (sl,[]) tl in
    (ssidmod,name,nsl,ntl,scrl,p,ss)) l

(* Add self loop such that if a module may read a signal it can read it from any state*)
let flatten_module (ssid,name,sl,tl,scrl,p,ss) =
  let inaction,_ = interface_of_modu tl in
  let tl2 = sl
    |> List.fold_left (fun nt2 (ss,_) -> (*Itere over state*)
      nt2 |> StringSet.fold (fun sa tl3 ->  (* Iter over signal name*)
	if not (tl |> List.exists (fun (_,src,lab,_) -> (* iter over transition*)
	  match (src,lab.trigger) with 
	    ((Some x),(RAction y)) when x=ss && y = sa -> true 
	  | ((Some x),(Imm)) when x=ss -> true
	  | _-> false
	)) 
	then (* There is no transition from state src that read signal ss or that are immediate. Add selfloop*)
	  ((fresh_ssid ()),Some ss,{empty_trans_label with trigger = RAction(sa) },ss)::tl3 
	else (* Nothing to do *) 
	  tl3
      ) inaction) tl in 
  (ssid,name,sl,tl2,scrl,p,ss)

(* Look for the initial transition to determinate initial state *)
let init_state (ssid,name,sl,tl,scrl) =
  try
    let (_,_,_,init) =  
      List.find (function (_,None,_,_) -> true | _ -> false) tl in
    init
  with Not_found -> ( match sl with
    [init,_] -> init
  | _ -> begin
    match name with Some s -> failwith ("No init state in "^s)
    |None -> failwith ("No init state in "^string_of_int ssid)
  end)

(* 
   - Replace type for state from an id to an array of one id
in order to compose state in transitions. 
   - Remove initial transition
*)
let incr_trans l (ssid,src,lab,dst) =
  match src with
  | None -> l
  | Some src2 ->
    (ssid,[(0,src2)],lab,[(0,dst)])::l
  
let uni_ssid = ref 0

let flatten_state_ssid (ssid,name,sl,tl,scrl,p,ss) =
  let i2,sl2,sl3 = List.fold_left (fun (i,l2,l3) (j,n) ->
    ((i+1),(i,n)::l2,(j,i)::l3)) (0,[],[]) sl in
  uni_ssid := i2;
  let tl2 = List.map (fun (ssid,src,lab,dst) ->
    (ssid,src |>>> (fun x -> List.assoc x sl3),lab,List.assoc dst sl3)) tl in
  (ssid,name,sl2,tl2,scrl,p,ss)

(* 
   Replace type for state from an id to an array of one id
 in order to compose state in state. 
 Transform state from interger to an array of integer to allows composition *)
let incr_state (ssid,name,sl,tl,scrl,p,ss) =
  if sl=[] then { ssid=ssid;
	       name=name;
	       stateL=[];
	       ivect=[||];
	       transL=[];
	       scriptL=scrl;
	       interfaceR=StringSet.empty;
	       interfaceW=StringSet.empty;
	       priority = p;
	       signals=ss;
	     }
  else begin let init = init_state (ssid,name,sl,tl,scrl) in
	     let ivect = Array.make 1 (init,name) in
	     let tl2 = List.fold_left incr_trans [] tl in
	     (*Printf.printf "\t%s\n" (stateasso init sl);*)
	     let (r,w) = interface_of_modu tl2 in
	     { ssid=ssid;
	       name=name;
	       stateL=sl;
	       ivect=ivect;
	       transL=tl2;
	       scriptL=scrl;
	       interfaceR=r;
	       interfaceW=w;
	       priority=p;
	       signals=ss;
	     }
(*	     (ssid,name,sl,ivect,List.fold_left incr_trans [] tl,scrl)*)
  end


let dec_trans n (ssid,srcl,lab,dstl) =
  (ssid, (List.map (fun (x,y) -> (x+n,y)) srcl), lab ,(List.map (fun (x,y) -> (x+n,y)) dstl))

let comb_name_trans n1 n2 = merge_option (Printf.sprintf "%s_%s") n1 n2

(* Combine 2 transitions that read the same signal  Use List.merge to sort all elements*)
let comb_transRR  (ssidt,srcl,lab,dstl) l (ssidt2,srcl2,lab2,dstl2) =
   match (lab2.trigger,lab.trigger) with
    (RAction st,RAction st2) when st=st2 ->
      let lab3= {
	nameT = comb_name_trans lab.nameT lab2.nameT;
	trigger= lab.trigger;
	isFinal= lab.isFinal && lab2.isFinal;
	guard = None;
	priority= min lab.priority lab2.priority;
	write = List.sort_uniq compare (lab.write @ lab2.write);
	update = lab.update @ lab2.update;
	description = merge_option (Printf.sprintf "%s%s") lab.description lab2.description
      } in
      (fresh_ssid (),List.merge compare srcl srcl2,lab3 ,List.merge compare dstl dstl2 ):: l
  |_-> l

(* Combine 2 transitions one reading the other writing Use List.merge to sort all elements*)
let comb_trans (ssidt,srcl,lab,dstl)  (ssidt2,srcl2,lab2,dstl2) =
  match (lab2.trigger,lab.trigger) with
    (RAction st,_) when List.exists (fun x -> x=st) lab.write ->
      let lab3= {
	nameT = comb_name_trans (Some ("S_"^st)) (comb_name_trans lab.nameT lab2.nameT);
	trigger= lab.trigger;
	isFinal= lab2.isFinal;
	guard = lab.guard; (* The guard come from the transition that write *)
	priority= lab.priority;
	write = List.sort_uniq compare (lab.write @ lab2.write)
		    |> List.filter (fun x -> x<>st);
	update = lab.update @ lab2.update;
	description = merge_option (Printf.sprintf "%s%s") lab.description lab2.description
      } in
      Some (fresh_ssid (),List.merge compare srcl srcl2,lab3 ,List.merge compare dstl dstl2 )
  | (_,RAction st) when List.exists (fun x -> x=st) lab2.write ->
    let lab3= { 
      nameT = comb_name_trans (Some ("S_"^st)) (comb_name_trans lab.nameT lab2.nameT);
      trigger= lab2.trigger;
      isFinal= lab.isFinal;
      guard = lab2.guard; (* The guard come from the transition that write *)
      priority= lab2.priority;
      write = List.sort_uniq compare (lab.write @ lab2.write)
		  |> List.filter (fun x -> x<>st )
      ;
      update = lab.update @ lab2.update;
      description = merge_option (Printf.sprintf "%s%s") lab.description lab2.description
    } in
    Some (fresh_ssid (),List.merge compare srcl srcl2,lab3 ,List.merge compare dstl dstl2 )
  |_-> None

let combname x y = merge_option (Printf.sprintf "( %s | %s )") x y

let is_changed inter (_,_,lab,_) =
  match lab.trigger with 
  | RAction s when StringSet.mem s inter->true 
  |_-> List.exists (fun x -> (StringSet.mem x inter)) lab.write

(* Avoid inserting twice the same transition *)
let rec inser_trans ((_,srcl,lab,dstl) as t1) = function 
  | [] -> [t1]
  | ((_,srcl2,lab2,dstl2) as t2)::q when srcl = srcl2 && dstl=dstl2 && lab.trigger = lab2.trigger ->
    t2::q
  | t2::q -> t2::(inser_trans t1 q)

let rec combine_trans_list inter statel l l2 = 
  Printf.fprintf !logout "\ttransition list: [\n";
  List.iter (print_trans_simulink2 statel !logout) l2;
  Printf.fprintf !logout "\t]\n";
  match l2 with
    | [] -> l
    | [_] -> l
    | t1::lt -> 
      let lchanged2,lunchanged2 =
	lt |> List.fold_left (fun (lchanged,lunchanged) t2 ->
	  match comb_trans t1 t2 with
	  None -> (lchanged,lunchanged)
	| Some t -> if is_changed inter t
	  then (inser_trans t lchanged),lunchanged
	  else lchanged,(inser_trans t lunchanged)) (lt,l) in
    combine_trans_list inter  statel lunchanged2 lchanged2

let split_trans_sync interRR interRW tl =
  let tlchangedRW,tlunchangedRW = List.partition (is_changed interRW) tl in
  let tlchangedRR,tlunchanged = List.partition (fun (_,_,lab,_) -> match lab.trigger with
      RAction st when StringSet.mem st interRR -> true | _-> false) tlunchangedRW in
  (tlchangedRW,tlchangedRR,tlunchanged)    

let combine_modu ?aggrSyn:(aggrSyn=false)  m1 m2 =
(*(ssid,name,sl,ivect,tl,scrl) (ssid2,name2,sl2,ivect2,tl2,scrl2) =*)
  let n = Array.length m1.ivect in
  let ivect3 = Array.append m1.ivect m2.ivect in (* Concatenate state vector*)
  let tl3 = List.map (dec_trans n) m2.transL in  (* Shift state in transition from m2 *)
  (* Compute the set of action on which to synchronize with RW *)
  let interRW = StringSet.union (StringSet.inter m1.interfaceR m2.interfaceW)
    (StringSet.inter m2.interfaceR m1.interfaceW) in 
  (* Compute the set of action on which to synchronize with RR *)
  let interRR = StringSet.inter m1.interfaceR m2.interfaceR in
  (* Transition in m1 unchanged by synchronization RW *)
  let tlchangedRW1,tlchangedRR1,tlunchanged1 = split_trans_sync interRR interRW m1.transL 
  (* Transition in m2 unchanged by synchronization RW *)
  and tlchangedRW2,tlchangedRR2,tlunchanged2 = split_trans_sync interRR interRW tl3 in

  let tlsynchRR= 
    tlchangedRR1 |> List.fold_left (fun l1 t1 -> 
      tlchangedRR2 |> List.fold_left (fun l2 t2 -> comb_transRR t1 l2 t2) l1) [] in
  
  let tl4 = combine_trans_list interRW (m1.stateL @ m2.stateL) (tlunchanged1@tlunchanged2@tlsynchRR) (tlchangedRW1@tlchangedRW2) in
  let rm,wm = interface_of_modu tl4 in
  let newname = combname m1.name m2.name in
  let mod3 = {ssid= fresh_ssid ();
	      name= newname;
	      stateL= m1.stateL @ m2.stateL;
	      ivect=ivect3;
	      transL=tl4;
	      scriptL= m1.scriptL @ m2.scriptL;
	      interfaceR= rm;
	      interfaceW=wm;
	      priority= None;
	      signals= StringMap.merge (fun _ a b ->(match (a,b) with
	      | Some x,None -> Some x
	      | None,Some x-> Some x 
	      | Some x,Some y -> Some x 
	      | _ -> None)) m1.signals m2.signals
	     } in
  Printf.fprintf !logout "Build %a\n" print_option newname;
  Printf.fprintf !logout "Interface combinaisonRW: [ %a ]\n" (print_set ", ") interRW;
  Printf.fprintf !logout "Interface combinaisonRR: [ %a ]\n" (print_set ", ") interRR;
  print_module2 !logout mod3;
  (*print_simulink_dot2 newname mod3;*)
   mod3

(*
let comb_trans2 l (ssidt,srcl,lab,dstl) (ssidt2,srcl2,lab2,dstl2) =
  match (lab2.trigger,lab.trigger) with
    (RAction st,_) when List.exists (fun x -> x=st) lab.write ->
      let lab3= {
	nameT = Some st;
	trigger = lab.trigger;
	priority = lab.priority; 
	write = List.sort_uniq compare (lab.write @ lab2.write);
	update = lab.update @ lab2.update} in
      (fresh_ssid (),srcl@srcl2,lab3 ,dstl@dstl2 ) ::l
  | (_,RAction st) when List.exists (fun x -> x=st) lab2.write ->
    let lab3= { 
      nameT = Some st;
      trigger=lab2.trigger;
      priority = lab2.priority;
      write = List.sort_uniq compare (lab.write @ lab2.write);
      update = lab.update @ lab2.update} in
    (fresh_ssid (),srcl@srcl2,lab3 ,dstl@dstl2 ) ::l
  | (RAction st,RAction st2) when st=st2 ->
    let lab3= { 
      nameT = Some st;
      trigger=lab2.trigger;
      priority=lab2.priority;
      write = List.sort_uniq compare (lab.write @ lab2.write);
      update = lab.update @ lab2.update} in
    (fresh_ssid (),srcl@srcl2,lab3 ,dstl@dstl2 ) ::l
  |_-> l
*)


let find_read x ml =
  List.find (fun m ->
    StringSet.mem x m.interfaceR) ml
let find_write x ml =
  List.find (fun m ->
    StringSet.mem x m.interfaceW) ml

let markmode m = m.transL |>
  List.fold_left (fun (x,y) (_,_,lab,_) -> match (lab.trigger,lab.write) with
    RAction(_),(_::_) -> (x+1,y+1) |_ -> (x,y+1)) (0,0)
    |> fun (x,y) -> (float x)/.(float y) 

(* Find module that compose well together, that is they only
   sychronize throught action shared by no other module
*)
let rec find_combinaison l =
  output_string !logout "Enter find_combinaison\n";
  Printf.fprintf !logout "Module list: [ %a ]\n" (print_list (fun x->x.name |>>| "()" ) ", ") l;
  match l with
  | [] -> []
  | [t] -> [t]
  | ml when List.exists (fun x -> StringSet.is_empty x.interfaceR && StringSet.is_empty x.interfaceW) ml ->
    let m,ol = selectL (fun x -> StringSet.is_empty x.interfaceR && StringSet.is_empty x.interfaceW) ml in
    (combine_modu m (List.hd ol)) :: (List.tl ol)
  |> find_combinaison
  | ml -> 
    let (in1,out1) = List.fold_left (fun (ii,oi) m ->
      ((add_set m.interfaceR ii),(add_set m.interfaceW oi))) (StringMap.empty,StringMap.empty) ml in 
    Printf.fprintf !logout "Read Set: [ %a ]\nWrite Set [ %a ]\n" (print_multi ", ") in1 (print_multi ", ") out1;
    try 
      let fstread,(frm,fwm) =
	StringMap.filter (fun x xn->try xn=1 && StringMap.find x out1 =1 with Not_found -> false) in1 
  |> StringMap.mapi (fun x _ -> (find_read x ml,find_write x ml))
  |> StringMap.filter (
    fun x (rm,wm) -> StringSet.union (StringSet.inter rm.interfaceR wm.interfaceW) (StringSet.inter rm.interfaceW wm.interfaceR) 
			  |> StringSet.for_all (fun x -> (StringMap.find x in1) =1 && (StringMap.find x out1)=1))
  |> StringMap.choose in
	      (combine_modu frm fwm) :: (List.filter (fun x -> x<> frm && x<>fwm) ml)
  |> find_combinaison
    with Not_found -> 
      if false then ( match ml with 
      | [] -> []
      | [t] -> [t]
      | m1::m2::q -> output_string !logout "Aggressive combinaison\n";
	(combine_modu ~aggrSyn:true m1 m2)::q |> find_combinaison
      ) else (output_string !logout "No combinaison\n"; ml)	

let prune_unread m =
  let tl2 = List.filter (fun (_,_,lab,_) -> match lab.trigger with
    RAction _-> false
  | _ -> true) m.transL in
  {m with transL=tl2}

 (* Remove selfloop on read *)
let prune_unread2 m =
  let tl2 = List.filter (fun (_,src,lab,dst) -> match lab.trigger with
    RAction _-> not (src = dst && lab.write=[] && lab.update=[])
  | _ -> true) m.transL in
  {m with transL=tl2}

let combine_modu2 m1 m2 =
(*(ssid,name,sl,ivect,tl,scrl) (ssid2,name2,sl2,ivect2,tl2,scrl2) =*)
  let n = Array.length m1.ivect in
  let ivect3 = Array.append m1.ivect m2.ivect in
  let tl3 = List.map (dec_trans n) m2.transL in
  let tl4 = m1.transL @ tl3 in
  let rm,wm = interface_of_modu tl4 in
  let newname = combname m1.name m2.name in
  let mod3 = {ssid= fresh_ssid ();
	      name= newname;
	      stateL= m1.stateL @ m2.stateL;
	      ivect=ivect3;
	      transL=tl4;
	      scriptL= m1.scriptL @ m2.scriptL;
	      interfaceR=rm;
	      interfaceW=wm;
	      priority=None;
	      signals= StringMap.merge (fun _ a b ->(match (a,b) with
	      | Some x,None -> Some x
	      | None,Some x-> Some x 
	      | Some x,Some y -> Some x 
	      | _ -> None)) m1.signals m2.signals
	     } in
  Printf.fprintf !logout "Build %a\n" print_option newname;
  let inter = StringSet.union (StringSet.inter m1.interfaceR m2.interfaceW)
    (StringSet.inter m2.interfaceR m1.interfaceW) in 
  Printf.fprintf !logout "Interface combinaison: [ %a ]\n" (print_set ", ") inter;
  print_module2 !logout mod3;
  (*let newname = Printf.sprintf "co%i_%i.dot" m1.ssid m2.ssid in
  Printf.fprintf stdout "Combine %s -> %a,%a\n" newname print_option m1.name print_option m2.name;
  print_simulink_dot2 newname mod3;*)
   mod3

let expand_trans2 m =
  let (nsl,ntl) = List.fold_left (fun (sl2,tl2) (ssid,src,label,dst) -> 
    match (label.trigger,src) with 
    | RAction s , [(x,y)]->  (* read *)
      let news = fresh_ssid ()
      and newsn = Printf.sprintf "PostRead_%s" s in
      let newt = fresh_ssid () in
      ((news,Some newsn)::sl2,
       (ssid,src,{empty_trans_label with trigger=label.trigger},[x,news])::
	 (newt,[x,news],{label with trigger=Imm},dst)::tl2)
    | _ -> (sl2,(ssid,src,label,dst)::tl2)) (m.stateL,[]) m.transL in
  {
    m with stateL=nsl;
      transL =ntl
  }

let prune m =
  let open IntSQMulti in 
  m.transL
	  |> List.fold_left (fun tab (trid,read,label,write) -> 
	    read |> List.fold_left (fun tab2 st -> add_multi st tab2) tab) empty
	  |> filter (fun _ i -> i=1)
	  |< iter (fun (x,y) _->Printf.printf "%i_%s\n" x @@ stateasso y m.stateL)
	  |> (fun sl -> List.filter (fun (trid,read,label,write) -> 
	    match (read,label.trigger) with
	      ([x],Imm) when mem x sl -> true
	    | _ -> false) m.transL)
	  |> List.iter (fun tr -> print_trans_simulink2 m.stateL stdout tr) 


let trans_of_int i lab =
  let label = (match lab.nameT with 
      None -> string_of_list "_" (fun x->x) lab.write
    | Some n -> n) in 
  let lab2 = (match lab.trigger with
      Imm -> "I" | Delay _ -> "D" | RAction s -> "R"^s | ImmWC _ -> "C") in
  Printf.sprintf "%s%i_%s" lab2 i label

let place_of_int ivect i =
  match ivect.(i) with
    _,None -> Printf.sprintf "pl%i" i
  | _,Some(n) -> Printf.sprintf "%s" n

let escape_XML s =
  let reg = Str.regexp"[&<>]" in
    Str.global_substitute reg (function x -> match Str.matched_string x with 
    "&" -> "&amp;" 
  | "<" -> "&lt;" 
  | ">" -> "&gt;" 
  | x-> x) s

(* Print magic file for Cosmos*) 
let print_magic f sl tl scrl=
  output_string f "  <attribute name=\"externalDeclaration\">";
  output_string f "#define temporalCount(msec) 0\n";
  (*List.iter (fun (x,y) -> match y with None ->()
  | Some s ->  Printf.fprintf f "#define %s %i\n" s x) sl;*)
  if !add_reward then output_string f "#include \"../exbat.cpp\"\n";
  output_string f "#ifndef uint8\n#define uint8 uint8_t\n#define uint16 uint16_t\n#define uint32 uint32_t\n#endif\n";
  output_string f "#include \"markingImpl.hpp\"\n";
  (*
    Now in exbat.cpp files
    if List.exists (fun (_,_,t,_) -> t.description <>None) tl then
     output_string f "#include \"../distr.cpp\"\n";*) 

  if not !lightSim then
    output_string f (escape_XML "template <typename T>
std::string to_string2(T value){
	std::ostringstream os ;
	os << value ;
	return os.str(); }\n");
  List.iter (fun (x,y) -> match y with
   Some s when s="DataAvailable" -> Printf.fprintf f "#define DATA_AVAILABLE %i\n"  x | _->() ) sl;
  output_string f "const string print_magic(int v){\n";
  output_string f "\tswitch(v){\n";
  List.iter (fun (ssid,n) -> match n with 
    Some n2 -> Printf.fprintf f "\t\tcase %i: return \"%s\";\n" ssid n2
  | None -> ()) sl;
  Printf.fprintf f "\t\tdefault: return to_string2(v);\n\t}\n}\n";
  List.iter (function Var(ty,x,initv) when x<>"ctime" -> Printf.fprintf f "%s %s=%s;\n" ty x initv | _-> ()) scrl;
  List.iter (fun x -> Printf.fprintf f "%s\n" x) DataFile.func;
  List.iter (function Funct(a,x) -> Printf.fprintf f "%s\n" (escape_XML x) | _->() ) scrl;
  Printf.fprintf f "void magicUpdate(int t,double ctime){
  switch(t){\n";
  List.iter (fun (ss,_,lab,_) ->
    if lab.description <> None ||  lab.update <> [] then begin
      Printf.fprintf f "\tcase TR_%s_RT:\n" (trans_of_int ss lab);
      List.iter (fun x -> Printf.fprintf f "\t\t%s;\n" x) lab.update; 
      if !add_reward then begin
	match lab.description with
	  None ->()
	| Some sn2 ->
	  let sn = String.sub sn2 1 ((String.length sn2) -1) in
	  Printf.fprintf f "\t\tenterActive(%s,ctime);\n" sn;
	  output_string f "\tbreak;\n";
	  
	  Printf.fprintf f "\tcase TR_TR_%s_RewardStr_%i_RT:\n" sn ss;
	  Printf.fprintf f "\t\tenterIdle(%s,ctime);\n" sn;
	  
      end;
      
      output_string f "\tbreak;\n"
    end
  ) tl;
  output_string f "\tdefault: break; \n\t}\n}\n";
  Printf.fprintf f "bool magicConditional(int t){
  switch(t){\n";
  List.iter (fun (ss,_,lab,_) -> begin match lab.trigger,lab.guard with
    ((ImmWC c),Some g) -> 
      Printf.fprintf f "\tcase TR_%s_RT:\n" (trans_of_int ss lab);
      Printf.fprintf f "\t\treturn (%s && %s);\n" (if !escape_XML_var then escape_XML c else c) (if !escape_XML_var then escape_XML g else g); 
  | ((ImmWC c),None) -> 
      Printf.fprintf f "\tcase TR_%s_RT:\n" (trans_of_int ss lab);
      Printf.fprintf f "\t\treturn %s;\n" (if !escape_XML_var then escape_XML c else c);
  | (_,Some g) -> 
      Printf.fprintf f "\tcase TR_%s_RT:\n" (trans_of_int ss lab);
      Printf.fprintf f "\t\treturn %s;\n" (if !escape_XML_var then escape_XML g else g);
  | _ -> ();
  end) tl;

  if !add_reward then begin
    Printf.fprintf f "\tcase TR_STOP_RT:\n";
    Printf.fprintf f "\t\treturn isBatterieDepleted();\n";
  end;

  output_string f "\tdefault: return true; \n\t}\n}\n";
  (*if !lightSim then output_string f "void abstractMarking::moveSerialState(){ P->_PL_SerialPort = DATA_AVAILABLE;};\n";*)
  output_string f "  </attribute>"


let print_prism_mod f m =
  Printf.fprintf f "module m%i\n" m.ssid;

  let st_name = Array.init (Array.length m.ivect) (fun i -> 
    IntSet.singleton (fst m.ivect.(i))
  ) in

  List.iter (fun (ssidt,src,lab,dst) -> 
    List.iter (fun (i,s) -> st_name.(i) <- IntSet.add s st_name.(i)) src;
    List.iter (fun (i,s) -> st_name.(i) <- IntSet.add s st_name.(i)) dst;
  ) m.transL;

  Array.iteri (fun n (x,n2) -> begin
    let minelem = IntSet.min_elt st_name.(n)
    and maxelem = IntSet.max_elt st_name.(n) in
    Printf.fprintf f "\t%s: [%i..%i] init %i;\n" (place_of_int m.ivect n) minelem (max (minelem+1) maxelem) x;
    IntSet.iter (fun x -> Printf.fprintf f "\t//%i -> %s;\n" x (stateasso x m.stateL)) st_name.(n)
  end) m.ivect;

  List.iter (fun (ssidt,src,lab,dst) -> 
    Printf.fprintf f "\t[%s] " (trans_of_int ssidt lab); 
    ignore @@ List.fold_left (fun b (i,s) ->
      begin
	if b then Printf.fprintf f " & ";
	Printf.fprintf f "(%s=%i)" (place_of_int m.ivect i) s;
	true
      end
    ) false src;
    
    begin match lab.trigger with
      Imm -> Printf.fprintf f " -> imm : ";
    | Delay s-> Printf.fprintf f " -> 1/(%a) : " printH_expr s;
    | RAction s  -> Printf.fprintf f " -> imm : ";
    | ImmWC s  -> Printf.fprintf f " -> imm : ";
    end;

    ignore @@ List.fold_left (fun b (i,s) ->
      if b then Printf.fprintf f " & ";
      Printf.fprintf f "(%s'=%i)" (place_of_int m.ivect i) s;
      true
    ) false dst;
    Printf.fprintf f ";\n";
  ) m.transL;
  Printf.fprintf f "endmodule\n"


(* Check wether two transition can be merged toghether. in O(n^2) should be O(ln(n)) *)
let compat_trans (_,_,_,t1) (_,s2,_,_) =
  t1 |> List.for_all 
      (fun (x,y) -> List.exists (fun (z,t) -> if z=x then t=y else false) s2)
    
let merge_trans (ssid1,s1,l1,t1) (ssid2,s2,l2,t2) =
  Printf.printf "new merge %a,%a" print_option l1.nameT print_option l2.nameT;
  print_newline ();
  (fresh_ssid (),
   s1 @ (s2 |> List.filter (fun (x,y) -> not @@ List.exists (fun (z,_)->x=z) t1)),
   l1,
   t2 @ (t1 |> List.filter (fun (x,y) -> not @@ List.exists (fun (z,_)->x=z) s2)))
 
(*remove instantaneous transition form simulink module*)
let removeImm m = 
  let limm2, lstoch = List.partition (fun (_,_,l,_) -> match l.trigger with Delay _ -> false |_ -> true) m.transL in
  let limm = List.filter (fun t -> not  @@ compat_trans t t) limm2 in (* filter loop*)
  let it l = 
    List.fold_left (fun a ((_,_,l,_) as b) -> 
      if l.isFinal then b::a
      else let cmp =ref 0 in
	   let l2 = List.fold_left (fun c d ->
	     if compat_trans b d then (incr cmp; (merge_trans b d)::c) else c
	   ) a limm in
	   if !cmp=0 then b::a  else l2
    ) [] l in
  let stoch2 = ref lstoch in
  let ssidref= ref (-1) in
  while !ssid_count <> !ssidref do
    print_endline "test";
    ssidref := !ssid_count;
    stoch2 := it !stoch2;
  done;
  {m with transL=(*m.transL*) !stoch2}
    

let distr_of_trans lab =
  match (lab.trigger,lab.description) with
    (ImmWC _),_ | (Imm,_) -> (StochasticPetriNet.Imm,Float 1.0,Float (2.0-.0.01*.lab.priority)) 
  | (Delay s),None -> (detfun s,Float 1.0,Float (1.0-.0.01*.lab.priority))
  | (Delay s),(Some desc) ->
    if String.length desc = 0 then (detfun s,Float 1.0,Float (1.0-.0.01*.lab.priority))
    else if desc.[0] = 'T' then
      desc		     
  |> (fun s -> Some (String.sub s 1 (String.length s -1))) 
  |>>> int_of_string 
  |>>| 0 
  |> (fun x -> StochasticPetriNet.DiscUserDef x,Float 1.0,Float (1.0-.0.01*.lab.priority))
    else desc
  |> (fun s -> Some (String.sub s 1 (String.length s -1))) 
  |>>> float_of_string 
  |>>| 1.0
  |> (fun x -> StochasticPetriNet.Exp (Float x),Float 1.0,Float (1.0-.0.01*.lab.priority))

 | (RAction s),_ -> (StochasticPetriNet.Imm,Float 1.0,Float (4.0-.0.01*.lab.priority))
   

(* Print as a prism CTMC model*)
let print_prism_module fpath cf ml =
  let f = open_out fpath in
  
  Printf.fprintf f "ctmc\nconst double imm=100;\n";
  List.iter (fun (x,y) -> Printf.fprintf f "const double %s=%f;\n" x (y/.1000.)) (DataFile.data_of_file cf);
  (*List.iter (fun (x,y) -> match y with None -> () | Some s -> Printf.fprintf f "const int S_%s=%i;\n" s x) (m.stateL);*)
  List.iter (print_prism_mod f) ml;
  
  close_out f

let strip_type s=
  let i = String.rindex s ' ' in
  String.sub s (i+1) (String.length s-i-1) 

(* Convertion to SPT *)
let stochNet_of_modu cf m =
  let fund = fun f () -> print_magic f m.stateL m.transL m.scriptL in
  let net = (Net.create ():StochasticPetriNet.spt) in 

  m.signals
  |> StringMap.filter (fun x (y,z) -> z=In) 
  |< (fun x -> x
	 |> (fun s -> StringMap.fold (fun x _ y -> StringSet.add x y) s StringSet.empty)
	     
	 |> StringSet.union m.interfaceW
	 |> StringSet.union m.interfaceR 
	     
	 |< Printf.fprintf !logout "Interface [ %a ]\n" (print_set ",")
	 |< StringSet.iter (fun x -> Data.add (("SIG_"^x),(Int 0,Some (Int 1))) net.Net.place)
	 |< StringSet.iter (fun x -> Data.add (("EMPTY_"^x),(StochasticPetriNet.Imm,Float 1.0,Float 3.0)) net.Net.transition)
	 |> StringSet.iter (fun x -> Net.add_inArcS net ("SIG_"^x) ("EMPTY_"^x) (Int 1)))
      
  |< StringMap.iter (fun x _ -> Data.add (("INCOMING_"^x),(StochasticPetriNet.Imm,Float 1.0,Float 5.0)) net.Net.transition)
  |< StringMap.iter (fun x _ -> Net.add_outArcS net ("INCOMING_"^x) ("SIG_"^x) (Int 1))
  |> StringMap.iter (fun x _ -> Net.add_inhibArcS net ("SIG_"^x) ("INCOMING_"^x) (Int 0));
   
  let varlist = 
    m.scriptL |>
    List.fold_left (fun tl -> 
    (function (Var(ty,x,init)) when x<>"ctime" -> x::tl | _-> tl)
  ) [] in
  net.Net.def <- Some { intconst=[];
			floatconst=List.map (fun (x,y) -> (x,Some (Float(y)))) (DataFile.data_of_file cf);
			clock=varlist;
			printer=fund;
                        labels=[];
                        formulas=[];
                        classDef=[];
                      };
  Array.iteri (fun n (x,n2) -> Data.add ((place_of_int m.ivect n),(Int x,Some (Int !ssid_count) )) net.Net.place) m.ivect;
  if !add_reward then begin
    Data.add ("STOP_PL",(Int 0,Some (Int 1))) net.Net.place;
    Data.add (("STOP"),(Det (Float 0.0),Float 1.0,Float 2.0)) net.Net.transition;
    Data.add (("CONTINUE"),(Det (Float 0.0),Float 1.0,Float 1.0)) net.Net.transition;
    Net.add_inArcS net "STOP_PL" "STOP" (Int 1);
    Net.add_inArcS net "STOP_PL" "CONTINUE" (Int 1);
    Data.add (("CHECK_ST"),(Det (Float 10.0),Float 1.0,Float 1.0)) net.Net.transition;
    Net.add_outArcS net "CHECK_ST" "STOP_PL" (Int 1);
  end;
  List.iter (fun (ssidt,src,lab,dst) -> 
    try 
      let d= distr_of_trans lab in
      begin match lab.trigger with
      ImmWC _ | Imm | Delay _-> Data.add ((trans_of_int ssidt lab),d) net.Net.transition
      | RAction s -> Data.add ((trans_of_int ssidt lab),d)
	net.Net.transition;
	Net.add_inArcS net ("SIG_"^s) (trans_of_int ssidt lab) (Int 1);
	Net.add_outArcS net (trans_of_int ssidt lab) ("SIG_"^s) (Int 1);
      end;
    List.iter (fun x -> 
      Net.add_inhibArcS net ("SIG_"^x) (trans_of_int ssidt lab) (Int 1);
      Net.add_outArcS net (trans_of_int ssidt lab) ("SIG_"^x) (Int 1)
    ) lab.write;
    List.iter (fun (i,s) ->
       begin
	Net.add_inArcS net (place_of_int m.ivect i) (trans_of_int ssidt lab) (Int s);
	Net.add_inhibArcS net (place_of_int m.ivect i) (trans_of_int ssidt lab) (Int (s+1));
      end
    ) src;
    List.iter (fun (i,s) ->
      Net.add_outArcS net (trans_of_int ssidt lab) (place_of_int m.ivect i) (Int s)) dst;
    
    (*Add reward struct*)
    if !add_reward then begin
      match lab.description with
	None ->()
      | Some sn2 -> begin
	let sn = String.sub sn2 1 (String.length sn2 -1) in
	Data.add ((Printf.sprintf "P_%s_RewardStr_%i" sn ssidt),(Int 0,Some (Int 1))) net.Net.place;
	Data.add ((Printf.sprintf "TR_%s_RewardStr_%i" sn ssidt),((DiscUserDef(int_of_string sn)),Float 1.0,Float 1.0)) net.Net.transition;
	Net.add_outArcS net (trans_of_int ssidt lab) (Printf.sprintf "P_%s_RewardStr_%i" sn ssidt) (Int 1);
	Net.add_inArcS net (Printf.sprintf "P_%s_RewardStr_%i" sn ssidt) (Printf.sprintf "TR_%s_RewardStr_%i" sn ssidt) (Int 1);
	(*Net.add_outArc net (trans_of_int ssidt lab) ("STOP_PL") (Int 1);
	Net.add_outArc net (Printf.sprintf "TR_%s_RewardStr_%i" sn ssidt) ("STOP_PL") (Int 1);*)
      end
    end

    with Not_found -> print_endline "Not_found uncatch";      
  ) m.transL;
  net
