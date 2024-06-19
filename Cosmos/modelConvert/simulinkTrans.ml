open Simulinkparser
open Printf
open PetriNet
open StochasticPetriNet
open Type

module OrderedBlocks =
  struct
    type t = Simulinkparser.block
    let compare x y = compare x.blockid y.blockid
end
module BlockSet = Set.Make(OrderedBlocks)

module OrderedLinks =
  struct
    type t = Simulinkparser.simulinkLink
    let compare x y = compare x y
end
module LinksSet = Set.Make(OrderedLinks)

let linksSetMap f s =
  LinksSet.fold (fun a s2 -> LinksSet.add (f a) s2) s LinksSet.empty
                          
let skInfinitesimalLatency = ["Integrator"];;
let skNoInputs = ["Constant";"Inport"];;
let skConditional = ["Switch";"Relay"];;
let skContinuous = ["Switch";"Integrator";"TransportDelay";"Reference"];; (* ;"Sum";"Sin";"Gain";"Product"];; *)
let skDiscrete = ["UnitDelay";"Delay";"DiscreteIntegrator"];;
let skInports = ["Inport"];;
let skOutports = ["Outport";"Scope";"Display"];;
let skLatencies = ["UnitDelay";"Delay";"TransportDelay"];;

let verbose = ref 0;;
let epsV = ref 0.00001;;
let epsT = ref 0.00001;;
let maxSimZC = ref 0;;
let skBaseStep = ref None;;
let skUseRK4 = ref 0;;
let skUseODE45 = ref 0;;
let skUseEuler = ref 0;;
  
let topologicSort (lB,lL) =
  let e = ref (LinksSet.of_list lL) in
  let l = ref [] and s = ref BlockSet.empty in
    let c = Hashtbl.create (List.length lB) in
     let rec initiaC = function
       [] -> ()
       | t::q -> Hashtbl.add c t.blockid 0; initiaC q
     and countEdges = function
       [] -> ()
       | t::q -> Hashtbl.replace c t.toblock (1 + Hashtbl.find c t.toblock); countEdges q;
     and initiaS = function
       [] -> ()
       | t::q -> let incoming = Hashtbl.find c t.blockid in
                   if (incoming = 0) then s := BlockSet.add t !s;
                   initiaS q;
     and findblock id = function
       [] -> failwith "Un lien fait n'importe quoi"
       | t::q when t.blockid = id -> t
       | t::q -> findblock id q
     and processblock block =
       l := block::!l;
       s := BlockSet.remove block !s;
       let outgoingedges = ref (LinksSet.filter (fun x -> x.fromblock = block.blockid) !e) and
           incomingedges = ref (LinksSet.filter (fun x -> x.toblock = block.blockid) !e) and
           dealwithedgeout edge = begin
             e := LinksSet.remove edge !e;
             Hashtbl.replace c edge.toblock ( (Hashtbl.find c edge.toblock - 1) );
             if (Hashtbl.find c edge.toblock = 0) then
               if (not (List.exists (fun x -> x.blockid=edge.toblock) !l)) then
                 s := BlockSet.add (findblock edge.toblock lB) !s;
             edge
           end and 
           dealwithedgeinc edge = begin (* Nettoyage pour les blocs infinitésimaux/… *)
             e := LinksSet.remove edge !e;
             edge
           end in
             outgoingedges := linksSetMap dealwithedgeout !outgoingedges;
             incomingedges := linksSetMap dealwithedgeinc !incomingedges;
     and infinitesimalLatencies = function
       [] -> ()
       | t::q when List.exists (fun x -> x=t.blocktype) skInfinitesimalLatency -> processblock t; infinitesimalLatencies q;
       | t::q when t.blocktype="DiscreteIntegrator" -> begin
           let intMethod = List.assoc "IntegratorMethod" t.values in
              if intMethod = "Integration: Forward Euler" then processblock t;
           end;
           infinitesimalLatencies q
       | t::q -> infinitesimalLatencies q;
     and noInputs = function
       [] -> ()
       | t::q when List.exists (fun x -> x=t.blocktype) skNoInputs -> processblock t; noInputs q;
       | t::q -> noInputs q;
     and findLatencies = function
       [] -> []
       | t::q when List.exists (fun (x,y) -> x="LATENCY") t.values -> (float_of_string (List.assoc "LATENCY" t.values),t)::(findLatencies q)
(*       | t::q when List.exists (fun (x,y) -> x="DelayLength") t.values -> (float_of_string (List.assoc "DelayLength" t.values),t)::(findLatencies q)
       | t::q when List.exists (fun (x,y) -> x="DelayTime") t.values -> (float_of_string (List.assoc "DelayTime" t.values),t)::(findLatencies q) *)
       | t::q when t.blocktype="UnitDelay" -> (1.0,t)::(findLatencies q)
       | t::q -> findLatencies q
     and processLatencied = function
       [] -> ()
       | (l,t)::q when l = 0.0 -> processLatencied q
       | (l,t)::q -> processblock t; processLatencied q
     in
       initiaC lB;
       countEdges lL;
       initiaS lB;
       noInputs lB;
       let latenciedBlocks = ref (findLatencies lB) in
         latenciedBlocks := List.sort (compare) !latenciedBlocks;
         processLatencied !latenciedBlocks;
         infinitesimalLatencies lB;
         while (not (BlockSet.is_empty !s)) do
           let vertex = BlockSet.choose !s in
             processblock vertex
         done;
         if (LinksSet.is_empty !e) then (List.rev !l, lL) else failwith "Cannot simulate this Simulink model";;

let rec findSrc (b,p) = function
  [] -> Printf.eprintf "[WARNING:] Couldn't find a source port linked to port %i of block %i" p b; (0,0)
  | l::q when l.toblock=b && l.toport=p -> (l.fromblock,l.fromport)
  | l::q -> findSrc (b,p) q;;

let rec getBlockById bid = function
  [] -> Printf.eprintf "[FATAL:] Couldn't find block number %i" bid; exit 0;
  | b::q when b.blockid=bid -> b
  | b::q -> getBlockById bid q;;

let rec isCondSignal (b,p) lB = function
  [] -> false
  | l::q when l.fromblock=b && l.fromport=p ->
    let toblock = getBlockById l.toblock lB in
    begin match (toblock.blocktype,l.toport) with
    | ("Switch",2) -> true
    | ("Relay",1) -> true
    | _ -> isCondSignal (b,p) lB q end
  | l::q -> isCondSignal (b,p) lB q;;

(** ToDo : fusionner les deux fonctions en une seule fonction avec Some/None **)
let rec getCondBlock (b,p) lB = function
  [] -> Printf.eprintf "[FATAL:] Block number %i output signal from port %i is not linked to a conditional block." b p; failwith "Encountered a fatal error.";
  | l::q when l.fromblock=b && l.fromport=p ->
    let toblock = getBlockById l.toblock lB in
    begin match (toblock.blocktype,l.toport) with
    | ("Switch",2) -> l.toblock
    | ("Relay",1) -> l.toblock
    | _ -> getCondBlock (b,p) lB q end
  | l::q -> getCondBlock (b,p) lB q

let rec getCondThreshold (b,p) lB = function
  [] -> Printf.eprintf "[FATAL:] Block number %i output signal from port %i is not linked to a conditional block." b p; failwith "Encountered a fatal error.";
  | l::q when l.fromblock=b && l.fromport=p ->
    let toblock = getBlockById l.toblock lB in
    begin match (toblock.blocktype,l.toport) with
    | ("Switch",2) -> List.assoc "Threshold" toblock.values
    | ("Relay",1) -> begin
        let onswval = List.assoc "OnSwitchValue" toblock.values
        and offswval = List.assoc "OffSwitchValue" toblock.values in
        Printf.sprintf "(idx > 0 ? ( (Marking.P->_STATE%i[Marking.P->lastEntry-1] == 0) ? %s : %s ) : %s)" l.toblock onswval offswval onswval
        end
    | _ -> getCondThreshold (b,p) lB q end
  | l::q -> getCondThreshold (b,p) lB q;;

let rec getConditional b = match b.blocktype with
   | "Switch" -> List.assoc "Threshold" b.values
   | "Relay" -> (begin
       let onswval =  List.assoc "OnSwitchValue" b.values
       and offswval = List.assoc "OffSwitchValue" b.values in
       Printf.sprintf "(idx > 0 ? ( (Marking.P->_STATE%i[Marking.P->lastEntry-1] == 0) ? %s : %s ) : %s)" b.blockid onswval offswval onswval
       end)
   | _ -> Printf.eprintf "[WARNING:] Conditional block %s is poorly implemented." b.blocktype; "0.0";;


let tagContinuous (lB,lL) = (** Cette fonction doit être améliorée. **)
  let continus = ref [] in
  let discret = ref [] in
    let rec obvContinus = function
      | [] -> ()
      | t::q when (List.exists (fun x -> x=t.blocktype) skContinuous) -> continus := (t.blockid)::!continus; obvContinus q
      | t::q when (List.exists (fun x -> x=t.blocktype) skDiscrete) -> discret := (t.blockid)::!discret; obvContinus q
      | t::q -> obvContinus q
    in
      let rec parcoursB = function
        | [] -> ()
        | b::q when (List.exists (fun x -> x=b.blocktype) skDiscrete) -> parcoursB q
        | b::q when (List.exists (fun x -> x=b.blocktype) skContinuous) -> parcoursB q
        | b::q -> parcoursL b.blockid lL; parcoursB q;
      and parcoursL bid = function
        | [] -> ()
        | l::q when l.toblock=bid -> let entree = l.fromblock in
          if (List.exists (fun x -> x=entree) !continus) then continus := (bid)::!continus;
          if (List.exists (fun x -> x=entree) !discret) then discret := (bid)::!discret;
          parcoursL bid q
        | l::q -> parcoursL bid q
      in
      let rec tag = function
        | [] -> []
        | t::q when (List.exists (fun x -> x=t.blockid) !continus) -> ({ blocktype = t.blocktype; blockid = t.blockid; name = t.name; values = ("COSMOSCONT","1")::t.values })::(tag q)
        | t::q -> t::(tag q)
      in
      obvContinus lB;
      parcoursB lB;
      (tag lB,lL);;

let bkwdGraph (lB,lL) b =
  let kB = ref [] and kL = ref [] in
    let addB = function
    | b when List.exists (fun x -> x=b) !kB -> false
    | b -> kB := b::!kB; true
    and addL = function
    | l when List.exists (fun x -> x=l) !kL -> false
    | l -> kL := l::!kL; true
    and noOutput = function _ -> ()
    in
      let rec auxBkwd = function
      | t when List.exists (fun x -> x=t.blocktype) skNoInputs -> noOutput(addB t)
      | t when List.exists (fun (x,y) -> x="LATENCY") t.values -> noOutput(addB t)
      | t when List.exists (fun x -> x=t.blocktype) skInfinitesimalLatency -> noOutput(addB t) 
      | t when t.blocktype="DiscreteIntegrator" -> begin
           let intMethod = List.assoc "IntegratorMethod" t.values in
              if intMethod = "Integration: Forward Euler" then noOutput(addB t)
              else let isNew = addB t in if isNew then defileLinks t.blockid lL
           end;
      | t -> let isNew = addB t in if isNew then defileLinks t.blockid lL
      and defileLinks bid = function
      | [] -> ()
      | l::q when l.toblock=bid -> let isNew = addL l in if isNew then begin auxBkwd (getBlockById l.fromblock lB); defileLinks bid q end;
      | l::q -> defileLinks bid q
      in
        (* defileLinks b.blockid lL; *)
        auxBkwd b;
        topologicSort (!kB,!kL)

let genSwitchStatment out fgen blockList =
  fprintf out "\n\tswitch(bid){\n";
  List.iter (fun b -> begin
                 fprintf out "\tcase %i:  //%s \n" b.blockid b.blocktype;
                 fprintf out "\t\t{\n";
                 fgen out b;
                 fprintf out "\t\t} break;\n";
               end) blockList;
  fprintf out "\n\t}\n"

let genSwitchStatmentNames out fgen blockList =
  fprintf out "\n\t\n";
  List.iter (fun b -> begin
                fprintf out "if (bname == \"%s\") //%s \n" b.name b.blocktype;
                fprintf out "\t\t{\n";
                fgen out b;
                fprintf out "\t\t}\n";
            end) blockList;
  fprintf out "\n\t\n"

let pob ?port:(blockport=1) ?time:(timeval="idx") () out blockid =
  Printf.fprintf out "Marking.P->_BLOCK%i_OUT%i[%s]" blockid blockport timeval
          
(* Génération des fonctions de traitement des blocs à comparaison *)
let genCondFunctions lL skCpp b = 
  begin match b.blocktype with                    
    "Switch" ->
    let (ba,ia) = findSrc(b.blockid,2) lL in
    begin
      begin match List.assoc "Criteria" b.values with 
      | "u2 > Threshold" ->
         let threshold = float_of_string (List.assoc "Threshold" b.values) in
         Printf.fprintf skCpp "\n\t\tMarking.P->_STATE%i[idx] = (Marking.P->_BLOCK%i_OUT%i[idx] > %f) ? 1 : 0;" b.blockid ba ia (threshold);
      | "u2 >= Threshold" -> let threshold = float_of_string (List.assoc "Threshold" b.values) in
                             Printf.fprintf skCpp "\n\tMarking.P->_STATE%i[idx] = (Marking.P->_BLOCK%i_OUT%i[idx] >= %f) ? 1 : 0;" b.blockid ba ia (threshold);
      | x -> Printf.eprintf "[WARNING:] Unknown Criteria for block Switch : %s\n" x
      end;
    end;
  | "Relay" ->
    let (ba,ia) = findSrc(b.blockid,1) lL in
    let onswval = List.assoc "OnSwitchValue" b.values and
        offswval = List.assoc "OffSwitchValue" b.values in
        Printf.fprintf skCpp "\n\tif (Marking.P->_STATE%i[Marking.P->lastCondEntry]) {" b.blockid;
        Printf.fprintf skCpp "\n\t\tMarking.P->_STATE%i[idx] = (Marking.P->_BLOCK%i_OUT%i[idx] > %s) ? 1 : 0;" b.blockid ba ia offswval;
        Printf.fprintf skCpp "\n\t} else {";
        Printf.fprintf skCpp "\n\t\tMarking.P->_STATE%i[idx] = (Marking.P->_BLOCK%i_OUT%i[idx] > %s) ? 1 : 0;" b.blockid ba ia onswval;
        Printf.fprintf skCpp "\n}";
  | _ -> ()
  end

let generateBindingImpl out =
      (* abstractBindingImpl *)
    Printf.fprintf out "#ifndef ABSTRACT_BINDING_h\n";
    Printf.fprintf out "#define ABSTRACT_BINDING_h\n";

    Printf.fprintf out "\nclass abstractBindingImpl {\npublic:\n};\n";
    (* abstractBindingIteratorImpl *)
    Printf.fprintf out "\nclass abstractBindingIteratorImpl {\npublic:\n";
    Printf.fprintf out "\tvoid reset(stateImpl& m){};\n";
    Printf.fprintf out "\tbool next(size_t& t, stateImpl& m){ return false; };\n";
    Printf.fprintf out "\tsize_t getIndex(){ return 0; };\n";
    Printf.fprintf out "\tabstractBinding getBinding(){ return abstractBinding(); };\n};\n";
    Printf.fprintf out "#endif"
    
    
let genLinAppCrossing skCpp b =
  begin match b.blocktype with                    
    "Switch" -> begin
      let threshold = float_of_string (List.assoc "Threshold" b.values) in
      Printf.fprintf skCpp "\treturn tU.getDouble() + (%f - vU)*(tA.getDouble() - tU.getDouble())/(vA - vU);" threshold;
    end;
  | "Relay" -> begin
               let onswval = List.assoc "OnSwitchValue" b.values
               and offswval = List.assoc "OffSwitchValue" b.values in
               Printf.fprintf skCpp "\tif (Marking.P->_STATE%i[Marking.P->lastPrintEntry]) {" b.blockid;
               Printf.fprintf skCpp "\n\t\treturn tU.getDouble() + (%s - vU)*(tA.getDouble() - tU.getDouble())/(vA - vU);"offswval;
               Printf.fprintf skCpp "\n\t} else {";
               Printf.fprintf skCpp "\n\t\treturn tU.getDouble() + (%s - vU)*(tA.getDouble() - tU.getDouble())/(vA - vU);"onswval;
               Printf.fprintf skCpp "\n\t}";
               end;
  | _ -> ()
  end

let genZCFunctions lL lB skCpp b =
  if List.exists (fun x -> x=b.blocktype) skConditional then begin
      let portCond = match b.blocktype with
        | "Switch" -> 2
        | "Relay" -> 1
        | _ -> Printf.eprintf "[WARNING:] Couldn't find conditional port for block %s, defaulting to 1.\n" b.blocktype; 1
      in
      Printf.fprintf skCpp "\n\tSKTime step = Marking.P->_TIME[idx] - Marking.P->_TIME[Marking.P->lastEntry-1];";
      let rec aux i b2 =
        if b2.blocktype="Integrator" then
          (if i=0 then Printf.fprintf skCpp "\n\testimateIntegrators(idx,Marking.P->lastEntry-1,step);";)
        else if List.exists (fun x -> x=b2.blocktype) skConditional then (Printf.fprintf skCpp "\n\tcheckStates(%i,idx); executeBlocks(%i,idx);" b2.blockid b2.blockid;)
        else Printf.fprintf skCpp "\n\texecuteBlocks(%i,idx);" b2.blockid;
      in let (ba,ia) = findSrc(b.blockid,portCond) lL in
         let inb = getBlockById ba lB in
         let (bB,bL) = bkwdGraph (lB,lL) inb in
         List.iter (aux 0) bB;
         (*Printf.fprintf skCpp "\n};";*)
    end

let genfindStateChange lL skCpp b =
  if List.exists (fun x -> x=b.blocktype) skConditional then begin
      let portCond = match b.blocktype with
        | "Switch" -> 2
        | "Relay" -> 1
        | _ -> Printf.eprintf "[WARNING:] Couldn't find conditional port for block %s, defaulting to 1.\n" b.blocktype; 1
      in
      let (ba,ia) = findSrc(b.blockid,portCond) lL in
      
      Printf.fprintf skCpp "\n\tint idxtampon = Marking.P->lastEntry+8;"; (* ÉTAIT +7 *)
      Printf.fprintf skCpp "\n\tSKTime step = stepSK;";
      Printf.fprintf skCpp "\n\tbool signChange = true;";
      Printf.fprintf skCpp "\n\tMarking.P->_TIME[idxtampon+0] = Marking.P->_TIME[idx-1];";
      Printf.fprintf skCpp "\n\tMarking.P->_TIME[idxtampon+1] = Marking.P->_TIME[idx-1] + stepSK;";
      Printf.fprintf skCpp "\n\tSKTime candidate = Marking.P->_TIME[idxtampon+1];";
      Printf.fprintf skCpp "\n\t\t\tif (Marking.P->_BLOCK%i_OUT%i[Marking.P->lastPrintEntry] == %s) { candidate = Marking.P->_TIME[Marking.P->lastPrintEntry]; signChange = false; }" ba ia (getConditional b);
      Printf.fprintf skCpp "\n\twhile (signChange) {";
      Printf.fprintf skCpp "\n\t\tstep = Marking.P->_TIME[idxtampon+1] - Marking.P->_TIME[idxtampon+0];";
      Printf.fprintf skCpp "\n\t\tif (Marking.P->_TIME[idxtampon+0] == Marking.P->_TIME[Marking.P->lastPrintEntry]) {";
      Printf.fprintf skCpp "\n\t\t\tcomputeBkwds(%i,idxtampon+0);" b.blockid;
      Printf.fprintf skCpp "\n\t\t\tMarking.P->_STATE%i[idxtampon+0] = Marking.P->_STATE%i[Marking.P->lastPrintEntry];" b.blockid b.blockid;
      Printf.fprintf skCpp "\n\t\t} else {";
      Printf.fprintf skCpp "\n\t\t\tcomputeBkwds(%i,idxtampon+0); checkStates(%i,idxtampon+0);" b.blockid b.blockid;
      Printf.fprintf skCpp "\n\t\t}";
      Printf.fprintf skCpp "\n\t\tcomputeBkwds(%i,idxtampon+1); checkStates(%i,idxtampon+1);" b.blockid b.blockid;
      Printf.fprintf skCpp "\n\t\tsignChange = not (Marking.P->_STATE%i[idxtampon+0] == Marking.P->_STATE%i[idxtampon+1]);" b.blockid b.blockid;
      Printf.fprintf skCpp "\n\t\tif (signChange) {";
      Printf.fprintf skCpp "\n\t\tMarking.P->_ZC%i[Marking.P->lastEntry] = 1;" b.blockid;
      Printf.fprintf skCpp "\n\t\tif ((Marking.P->_TIME[idxtampon+1].getBareTime() - Marking.P->_TIME[idxtampon+0].getBareTime()) == 1) { candidate = Marking.P->_TIME[idxtampon+0]; break; }";
      Printf.fprintf skCpp "\n\t\t\tcandidate = linAppCrossings(%i,Marking.P->_TIME[idxtampon+0], Marking.P->_TIME[idxtampon+1], Marking.P->_BLOCK%i_OUT%i[idxtampon+0], Marking.P->_BLOCK%i_OUT%i[idxtampon+1]);" b.blockid ba ia ba ia;
      Printf.fprintf skCpp "\n\t\t\tif (candidate > Marking.P->_TIME[idxtampon+1] || Marking.P->_TIME[idxtampon+0] > candidate) {";
      Printf.fprintf skCpp "\n\t\t\t\tcerr << \"The candidate for threshold crossing for block %i is \" << candidate.getDouble() << \" which is not between \" << Marking.P->_TIME[idxtampon+0].getDouble() << \" and \" << Marking.P->_TIME[idxtampon+1].getDouble() << \". Abort.\" << endl;" b.blockid;
      Printf.fprintf skCpp "\n\t\t\t\tcerr << \"The algorithm was attempting to find a correct next simulation time after time \" << Marking.P->_TIME[Marking.P->lastPrintEntry].getDouble() << \", for which the state was \" << Marking.P->_STATE%i[Marking.P->lastPrintEntry] << \" and the value was \" << Marking.P->_BLOCK%i_OUT%i[Marking.P->lastPrintEntry] << endl;" b.blockid ba ia;
      Printf.fprintf skCpp "\n\t\t\t\tcerr << \"The value of entry (conditional) signal was, at time \" << Marking.P->_TIME[idxtampon+0].getDouble() << \" : \" << Marking.P->_BLOCK%i_OUT%i[idxtampon+0] << \" (state \" << Marking.P->_STATE%i[idxtampon+0] << \"). At time \" << Marking.P->_TIME[idxtampon+1].getDouble() << \" it was \" << Marking.P->_BLOCK%i_OUT%i[idxtampon+1] << \" (state \" << Marking.P->_STATE%i[idxtampon+1] << \").\" << endl << endl;" ba ia b.blockid ba ia b.blockid;
      Printf.fprintf skCpp "\n\t\t\t\texit(50);";
      Printf.fprintf skCpp "\n\t\t\t}";
      Printf.fprintf skCpp "\n\t\t\tMarking.P->_TIME[idxtampon+2] = candidate; computeBkwds(%i,idxtampon+2); checkStates(%i,idxtampon+2);" b.blockid b.blockid;
      Printf.fprintf skCpp "\n\t\t\tif (Marking.P->_BLOCK%i_OUT%i[idxtampon+2] == %s) { break; }" ba ia (getConditional b);
      Printf.fprintf skCpp "\n\t\t\tif (Marking.P->_TIME[idxtampon+0] == candidate) { break; }";
      Printf.fprintf skCpp "\n\t\t\tif (Marking.P->_TIME[idxtampon+1] == candidate) { break; }";
      Printf.fprintf skCpp "\n\t\t\t(Marking.P->_STATE%i[idxtampon+0] == Marking.P->_STATE%i[idxtampon+2]) ? (Marking.P->_TIME[idxtampon+0]=candidate) : (Marking.P->_TIME[idxtampon+1]=candidate);" b.blockid b.blockid;
      Printf.fprintf skCpp "\n\t\t}";
      Printf.fprintf skCpp "\n\t}";
      Printf.fprintf skCpp "\n\treturn candidate - Marking.P->_TIME[idx-1];";
    end

(* Génération des fonctions d'éxécution des blocs simples *)

let genLatencyFunction lL skCpp b =
       let latency = float_of_string (List.assoc "LATENCY" b.values) and (ba,ia) = findSrc (b.blockid,1) lL and initValue = float_of_string (List.assoc "InitialCondition" b.values) in
       Printf.fprintf skCpp "\t\t{\n";
       Printf.fprintf skCpp "\t\tint latidx = findLatencyIndex(%f);" latency;
       Printf.fprintf skCpp "\n\t\tif (latidx > -1) {";
       Printf.fprintf skCpp "\n\t\t\tMarking.P->_BLOCK%i_OUT%i[idx] = Marking.P->_BLOCK%i_OUT%i[latidx];" b.blockid 1 ba ia;
       Printf.fprintf skCpp "\n\t\t} else {";
       Printf.fprintf skCpp "\n\t\t\tMarking.P->_BLOCK%i_OUT%i[idx] = %f;" b.blockid 1 initValue;
       Printf.fprintf skCpp "\n\t\t}}"
                      
     
let keepInportValues skCpp tabs b =
  begin match b.blocktype with
  | "Inport" -> Printf.fprintf skCpp "%sMarking.P->_BLOCK%i_OUT%i[Marking.P->lastEntry+1] = Marking.P->_BLOCK%i_OUT%i[Marking.P->lastInport];\n" tabs b.blockid 1 b.blockid 1
  | _ -> ()
  end;;

let genBlockFunctions lL lB skCpp b =
  let isCS = isCondSignal (b.blockid,1) lB lL in
  if isCS then begin
    let zcBlock = getCondBlock (b.blockid,1) lB lL in
      Printf.fprintf skCpp "\nif (Marking.P->lastPrintEntry == idx && Marking.P->_ZC%i[idx]) {" zcBlock;
      Printf.fprintf skCpp "\n\tMarking.P->_BLOCK%i_OUT%i[idx] = %s;" b.blockid 1 (getCondThreshold (b.blockid,1) lB lL);
      Printf.fprintf skCpp "\n} else {";
  end;
  begin match b.blocktype with                    
  | "Integrator" -> ()
  | "Terminator" -> ()
  | "DiscreteIntegrator" -> let (ba,ia)=findSrc(b.blockid,1) lL
                            and gainval = List.assoc "gainval" b.values
                            and initCond = List.assoc "InitialCondition" b.values
                            and intMethod = List.assoc "IntegratorMethod" b.values in
                            Printf.fprintf skCpp "\n\tif (idx == 0) {";
                            Printf.fprintf skCpp "\n\t\tMarking.P->_BLOCK%i_OUT1[idx] = %s;" b.blockid initCond;
                            Printf.fprintf skCpp "\n\t} else {";
                            begin match intMethod with
                            "Integration: Backward Euler" ->
                            Printf.fprintf skCpp "\n\t\t%a = %s * (Marking.P->_TIME[idx] - Marking.P->_TIME[Marking.P->lastEntry-1]).getDouble() * %a + %a;"
                                           (pob ()) b.blockid gainval (pob ~port:ba ()) ia (pob ~time:"Marking.P->lastEntry-1" ()) b.blockid;
                            | "Integration: Forward Euler" ->
                               Printf.fprintf skCpp "\n\t\t%a = %s * (Marking.P->_TIME[idx] - Marking.P->_TIME[Marking.P->lastEntry-1]).getDouble() * %a + %a;"
                                              (pob ()) b.blockid gainval (pob ~time:"Marking.P->lastEntry-1" ~port:ia ()) ba (pob ~time:"Marking.P->lastEntry-1" ()) b.blockid;
                            | other -> Printf.eprintf "Could not interprete %s for block DiscreteIntegrator." other end;
                            Printf.fprintf skCpp "\n\t}";
  | "Display" -> let (ba,ia)=findSrc(b.blockid,1) lL in
                 Printf.fprintf skCpp "\n\tMarking.P->_BLOCK%i_OUT1[idx] = Marking.P->_BLOCK%i_OUT%i[idx];" b.blockid ba ia;
                 Printf.fprintf skCpp "\n\tMarking.P->_%s_OUT1[idx] = Marking.P->_BLOCK%i_OUT%i[idx];" b.name ba ia;
  | "Scope" -> let outputs_parse_regexp = Str.regexp "\\[\\([0-9]+\\), \\([0-9]+\\)\\]" in
               let numOfPorts = List.assoc "Ports" b.values in begin
                 try let didmatch = Str.string_match outputs_parse_regexp numOfPorts 0 in
                   if didmatch then begin
                     try let nb = int_of_string@@ Str.matched_group 2 numOfPorts in
                       for i = 1 to nb do
                         let (ba,ia)=findSrc(b.blockid,i) lL in
                           Printf.fprintf skCpp "\n\tMarking.P->_BLOCK%i_OUT%i[idx] = Marking.P->_BLOCK%i_OUT%i[idx];" b.blockid i ba ia;
                           Printf.fprintf skCpp "\n\tMarking.P->_%s_OUT%i[idx] = Marking.P->_BLOCK%i_OUT%i[idx];" b.name i ba ia;
                       done
                     with Not_found -> Printf.eprintf "Scope?\n"
                   end
                 with Not_found -> Printf.eprintf "Scope?\n"
               end
  | "Inport" -> Printf.fprintf skCpp "\t\tif (idx > Marking.P->lastInport) { Marking.P->_BLOCK%i_OUT1[idx] = Marking.P->_BLOCK%i_OUT1[Marking.P->lastInport]; }" b.blockid b.blockid
  | "Outport" -> let (ba,ia)=findSrc(b.blockid,1) lL in
                 Printf.fprintf skCpp "\n\tMarking.P->_BLOCK%i_OUT1[idx] = Marking.P->_BLOCK%i_OUT%i[idx];" b.blockid ba ia;
                 Printf.fprintf skCpp "\n\tMarking.P->_%s_OUT1[idx] = Marking.P->_BLOCK%i_OUT%i[idx];" b.name ba ia
  | "Sum" -> let currInpt = ref 0 and signs = ref "" in (* List.assoc "Inputs" b.values in *)
             begin try
                 signs := List.assoc "Inputs" b.values
             with Not_found ->
                 Printf.eprintf "[WARNING:] No Inputs parameter found for block %s (type %s); defaulting to ++ (adding both signals).\n" b.name b.blocktype;
                 signs := "++"
             end;
             let signSize = String.length !signs in
             Printf.fprintf skCpp "\t\tMarking.P->_BLOCK%i_OUT%i[idx] =" b.blockid 1;
             let rec buildSum = function
               | i when i > signSize -> ()
               | i when (i = signSize) -> ()
               | i -> begin match !signs.[i] with
                      | '+' -> currInpt:=!currInpt+1; let (ba,ia)=findSrc(b.blockid,!currInpt) lL in
                                                      Printf.fprintf skCpp " + Marking.P->_BLOCK%i_OUT%i[idx]" ba ia
                      | '-' -> currInpt:=!currInpt+1; let (ba,ia)=findSrc(b.blockid,!currInpt) lL in
                                                      Printf.fprintf skCpp " - Marking.P->_BLOCK%i_OUT%i[idx]" ba ia
                      | _ -> ()
                      end; buildSum (i+1)
             in buildSum 0;
                Printf.fprintf skCpp ";\n"
  (* let (ba,ia) = findSrc (b.blockid,1) lL and (bb,ib) = findSrc (b.blockid,2) lL in
            Printf.fprintf skCpp "\n\tMarking.P->_BLOCK%i_OUT%i[idx] = Marking.P->_BLOCK%i_OUT%i[idx] + Marking.P->_BLOCK%i_OUT%i[idx];" b.blockid 1 ba ia bb ib; *)
  | "Product" -> let (ba,ia)=findSrc(b.blockid,1) lL
                 and (bb,ib)=findSrc(b.blockid,2) lL in
                 Printf.fprintf skCpp "\n\tMarking.P->_BLOCK%i_OUT%i[idx] = Marking.P->_BLOCK%i_OUT%i[idx] * Marking.P->_BLOCK%i_OUT%i[idx];" b.blockid 1 ba ia bb ib;
  | "Fcn" -> let expr = List.assoc "Expr" b.values
             and (ba,ia) = findSrc(b.blockid,1) lL in
             begin match expr with
             | "2^u" -> Printf.fprintf skCpp "\n\tMarking.P->_BLOCK%i_OUT%i[idx] = pow(2,Marking.P->_BLOCK%i_OUT%i[idx]);" b.blockid 1 ba ia
             | "sin(u)" -> Printf.fprintf skCpp "\n\tMarking.P->_BLOCK%i_OUT%i[idx] = sin(Marking.P->_BLOCK%i_OUT%i[idx]);" b.blockid 1 ba ia;
             | _ -> begin
                 Printf.fprintf skCpp "\nfprintf(stderr,\"Could not execute block %i of type %s and expression %s !\");" b.blockid b.blocktype expr;
                 Printf.eprintf "[WARNING:] Found unimplemented expression %s.\n" expr;
               end
             end
  | "Constant" -> begin try
                      let cstValue = float_of_string (List.assoc "Value" b.values) in
                          Printf.fprintf skCpp "\t\tMarking.P->_BLOCK%i_OUT%i[idx] = %f;\n" b.blockid 1 cstValue;
                  with Not_found ->
                      Printf.eprintf "[WARNING:] No Value parameter found for block %s (type %s); defaulting to 1.\n" b.name b.blocktype;
                      Printf.fprintf skCpp "\t\tMarking.P->_BLOCK%i_OUT%i[idx] = 1;\n" b.blockid 1;
                  end
  | "Delay" -> genLatencyFunction lL skCpp b
  | "UnitDelay" -> genLatencyFunction lL skCpp b
  | "Sin" -> begin
      let amplitude = List.assoc "Amplitude" b.values
      and frequency = List.assoc "Frequency" b.values
      and phase = List.assoc "Phase" b.values
      and bias = List.assoc "Bias" b.values in
      Printf.fprintf skCpp "\t\tMarking.P->_BLOCK%i_OUT%i[idx] = %s * sin(%s * Marking.P->_TIME[idx].getDouble() + %s) + %s;\n" b.blockid 1 amplitude frequency phase bias;
    end
  | "TransportDelay" -> begin
      let latency = float_of_string (List.assoc "LATENCY" b.values)
      and (ba,ia) = findSrc(b.blockid,1) lL
      and initValue = float_of_string (List.assoc "InitialOutput" b.values) in
      Printf.fprintf skCpp "\t\t{\n";
      Printf.fprintf skCpp "\t\tint latidx = findLatencyIndex(%f);" latency;
      Printf.fprintf skCpp "\n\t\tif (latidx > -1) {";
      Printf.fprintf skCpp "\n\t\t\tMarking.P->_BLOCK%i_OUT%i[idx] = linearInterpolation( Marking.P->_TIME[latidx], Marking.P->_TIME[latidx+1], Marking.P->_BLOCK%i_OUT%i[latidx], Marking.P->_BLOCK%i_OUT%i[latidx+1], Marking.P->_TIME[idx] - %f);" b.blockid 1 ba ia ba ia latency;
      Printf.fprintf skCpp "\n\t\t} else {";
      Printf.fprintf skCpp "\n\t\t\tMarking.P->_BLOCK%i_OUT%i[idx] = %f;" b.blockid 1 initValue;
      Printf.fprintf skCpp "\n\t\t}}\n";
    end
  | "Gain" -> let gain = float_of_string (List.assoc "Gain" b.values) and (ba,ia) = findSrc(b.blockid,1) lL in
              Printf.fprintf skCpp "\t\tMarking.P->_BLOCK%i_OUT%i[idx] = %f * Marking.P->_BLOCK%i_OUT%i[idx];\n" b.blockid 1 gain ba ia;
  | "Switch" -> let (ba,ia) = findSrc(b.blockid,1) lL and (bb,ib) = findSrc(b.blockid,3) lL and (bc,ic) = findSrc(b.blockid,2) lL in
                Printf.fprintf skCpp "\n\t\tif (Marking.P->_STATE%i[idx] == 0) {" b.blockid;
                Printf.fprintf skCpp "\n\t\t\tMarking.P->_BLOCK%i_OUT%i[idx] = Marking.P->_BLOCK%i_OUT%i[idx];" b.blockid 1 bb ib;
                Printf.fprintf skCpp "\n\t\t} else {";
                Printf.fprintf skCpp "\n\t\t\tMarking.P->_BLOCK%i_OUT%i[idx] = Marking.P->_BLOCK%i_OUT%i[idx];" b.blockid 1 ba ia;
                Printf.fprintf skCpp "\n\t\t}";
                Printf.fprintf skCpp "\n\t\t/* if (Marking.P->_ZC%i[idx] == 1) {" b.blockid;
                let threshold = float_of_string (List.assoc "Threshold" b.values) in
                Printf.fprintf skCpp "\n\t\t* \tMarking.P->_BLOCK%i_OUT%i[idx] = %f;" bc ic threshold;
                Printf.fprintf skCpp "\n\t\t} */"
  | "Relay" -> begin
               let onval = List.assoc "OnOutputValue" b.values
               and offval = List.assoc "OffOutputValue" b.values in
               Printf.fprintf skCpp "\n\t\tif (Marking.P->_STATE%i[idx] == 0) {" b.blockid;
               Printf.fprintf skCpp "\n\t\t\t%a = %s;" (pob ()) b.blockid offval;
               Printf.fprintf skCpp "\n\t\t} else {";
               Printf.fprintf skCpp "\n\t\t\t%a = %s;" (pob ()) b.blockid onval;
               Printf.fprintf skCpp "\n\t\t}"
               end;
  | "Reference" -> begin try
                   begin let sourcetype = List.assoc "SourceType" b.values in
                   match sourcetype with
                   | "Ramp" -> begin 
                               try begin let slope = List.assoc "_ID_slope" b.values and start = List.assoc "_ID_start" b.values in
                               Printf.fprintf skCpp "\n\t\t%a = %s + Marking.P->_TIME[idx].getDouble() * %s;" (pob ()) b.blockid start slope;
                               end with Not_found -> Printf.eprintf "[WARNING:] Couldn't find slope or start for a Reference->Ramp block."
                               end
                   | _ -> Printf.eprintf "[WARNING:] Found unimplemented source type %s in a Reference block." sourcetype;
                   end with Not_found -> begin
                   Printf.eprintf "[WARNING:] Could not find SourceType for block Reference.\n";
                       end end
  | "RelationalOperator" ->
     let (ba,ia)=findSrc (b.blockid,1) lL
     and (bb,ib)=findSrc (b.blockid,2) lL in
     let operator = Some b.values |>>>  List.assoc "Operator" |>>| ">" in
     Printf.fprintf skCpp "\n\t%a = %a %s %a;" (pob ()) b.blockid (pob ~port:ia ()) ba operator (pob ~port:ib ()) bb;
  | "Rounding" ->
     let (ba,ia)=findSrc (b.blockid,1) lL in
     Printf.fprintf skCpp "\n\t%a = round(%a) ;" (pob ()) b.blockid (pob ~port:ia ()) ba;
  | "Logic" ->
     let op2 = begin
       let operator = Some b.values |>>>  List.assoc "Operator" |>>| "OR" in
       match operator with
         "AND" -> "&&" | "OR" -> "||" | x -> Printf.eprintf "[WARNING:]Unsupported Logical Operator: %s replace by OR" x; "||" 
       end in
     let nbinport = Some b.values |>>>  List.assoc "Inputs" |>>> int_of_string |>>| 2  in
     Printf.fprintf skCpp "\n\t%a = " (pob ()) b.blockid;
     for i = 1 to nbinport do
       let (bi,ii) = findSrc (b.blockid,i) lL in
       Printf.fprintf skCpp "%s ( %a != 0.0 )" (if i=1 then "" else op2) (pob ~port:ii ()) bi;
     done;
     Printf.fprintf skCpp ";";
       
  | _ -> begin
      Printf.fprintf skCpp "\nfprintf(stderr,\"Could not execute block %i of type %s !\");" b.blockid b.blocktype;
      Printf.eprintf "[WARNING:] Found unimplemented block %s.\n" b.blocktype;
    end;
  end;
  if isCS then begin
    Printf.fprintf skCpp "\n}";
  end;
  begin match b.blocktype with
  | "Display" -> ()
  | "Scope" -> ()
  | "Terminator" -> ()
  | _ -> Printf.fprintf skCpp "\n\t\tMarking.P->_PL_B%iO%i = %a;" b.blockid 1 (pob ()) b.blockid;
         Printf.fprintf skCpp "\n\t\tif (std::isinf(Marking.P->_PL_B%iO%i) || std::isnan(Marking.P->_PL_B%iO%i)) { cerr << \"The value of output %i block %i is either Inifinite or NaN\" << endl; exit(1); }" b.blockid 1 b.blockid 1 1 b.blockid;
  end;;

let rec genSetInputs lL skCpp b =
  match b.blocktype with
  | "Inport" -> Printf.fprintf skCpp "\n\t\tMarking.P->_BLOCK%i_OUT1[idx+1] = value;" b.blockid;
  | _ -> ();;

(* Génère les lignes de code mettant à jour les valeurs des blocs; currMode=1 (3) s'il faut s'occuper de calculer le nouveau step *)
(* currMode=4 (5) dans le cas de la vérification à t+\epsT *)
(* currMode=6 (7) dans le cas d'un updateInput *)
let rec genSignalChanges skCpp tabs currMode b =
  match b.blocktype with
  | "Integrator" -> 
     begin match currMode with
       0 -> begin
         Printf.fprintf skCpp "\n%sif (Marking.P->lastEntry==0) { initialiseIntegrators(0); }" tabs; 
         Printf.fprintf skCpp "\n%selse { executeIntegrators(Marking.P->lastEntry); }" tabs;
       end;
            2 
     | 1 -> Printf.fprintf skCpp "\n%sstep = estimateIntegrators(Marking.P->lastEntry,Marking.P->lastEntry-1,step);" tabs;
            Printf.fprintf skCpp "\n%sif (step.getBareTime() == 0) { step.setBareTime(1); }" tabs;
            Printf.fprintf skCpp "\n%sif (oldStep != step) { continue; }" tabs;
            3
     | 4 -> Printf.fprintf skCpp "\n%sSKTime step = %f;" tabs !epsT;
            Printf.fprintf skCpp "\n%sstep = estimateIntegrators(Marking.P->lastEntry+1,Marking.P->lastEntry,step);" tabs;
            5
     | 6 -> Printf.fprintf skCpp "\n%sexecuteIntegrators(Marking.P->lastEntry);" tabs;
            7
     | _ -> currMode;
     end;
  | y when List.exists (fun x -> x=y) skConditional ->
     begin match currMode with
     | 0 -> Printf.fprintf skCpp "\n%scheckStates(%i,Marking.P->lastEntry);\n%sexecuteBlocks(%i,Marking.P->lastEntry);" tabs b.blockid tabs b.blockid;
     | 2 -> Printf.fprintf skCpp "\n%scheckStates(%i,Marking.P->lastEntry);\n%sexecuteBlocks(%i,Marking.P->lastEntry);" tabs b.blockid tabs b.blockid;
     | 1 -> Printf.fprintf skCpp "\n%sstep = findStateChanges(%i,Marking.P->lastEntry,step);\n%scheckStates(%i,Marking.P->lastEntry);\n%sexecuteBlocks(%i,Marking.P->lastEntry);" tabs b.blockid tabs b.blockid tabs b.blockid;
     | 3 -> Printf.fprintf skCpp "\n%sstep = findStateChanges(%i,Marking.P->lastEntry,step);\n%scheckStates(%i,Marking.P->lastEntry);\n%sexecuteBlocks(%i,Marking.P->lastEntry);" tabs b.blockid tabs b.blockid tabs b.blockid;
     | 4 -> Printf.fprintf skCpp "\n%scheckStates(%i,Marking.P->lastEntry+1);\n%sexecuteBlocks(%i,Marking.P->lastEntry+1);" tabs b.blockid tabs b.blockid;
     | 5 -> Printf.fprintf skCpp "\n%scheckStates(%i,Marking.P->lastEntry+1);\n%sexecuteBlocks(%i,Marking.P->lastEntry+1);" tabs b.blockid tabs b.blockid;
     | 6 -> Printf.fprintf skCpp "\n%scheckStates(%i,Marking.P->lastEntry);\n%sexecuteBlocks(%i,Marking.P->lastEntry);" tabs b.blockid tabs b.blockid;
     | 7 -> Printf.fprintf skCpp "\n%scheckStates(%i,Marking.P->lastEntry);\n%sexecuteBlocks(%i,Marking.P->lastEntry);" tabs b.blockid tabs b.blockid;
     | _ -> ()
     end;
     currMode
  | y when ((currMode = 1 || currMode = 3) && List.exists (fun x -> x=y) skLatencies) ->
     Printf.fprintf skCpp "\n%sexecuteBlocks(%i,Marking.P->lastEntry);" tabs b.blockid;
     Printf.fprintf skCpp "\n%slatnb = ( ((currentUpdTime-startTime).getDouble()) / %f ) + 1;" tabs (float_of_string (List.assoc "LATENCY" b.values));
     Printf.fprintf skCpp "\n%slatstep = %f;" tabs (float_of_string (List.assoc "LATENCY" b.values));
     Printf.fprintf skCpp "\n%slatstep = latstep * latnb; latstep = latstep - currentUpdTime;" tabs;
     Printf.fprintf skCpp "\n%sif (latstep < step && latstep.getBareTime() > 0) { step = latstep; }" tabs;
     currMode
  | _ -> begin match currMode with
     | 4 -> Printf.fprintf skCpp "\n%sexecuteBlocks(%i,Marking.P->lastEntry+1);" tabs b.blockid;
     | 5 -> Printf.fprintf skCpp "\n%sexecuteBlocks(%i,Marking.P->lastEntry+1);" tabs b.blockid;
     | _ -> Printf.fprintf skCpp "\n%sexecuteBlocks(%i,Marking.P->lastEntry);" tabs b.blockid
     end;
     currMode

let rec checkStateChanges skCpp tabs b =
  match b.blocktype with
  | y when List.exists (fun x -> x=y) skConditional ->
    Printf.fprintf skCpp "\n%sif (Marking.P->_STATE%i[Marking.P->lastEntry] != Marking.P->_STATE%i[Marking.P->lastEntry+1]) {" tabs b.blockid b.blockid;
    Printf.fprintf skCpp "\n%s\tMarking.P->_STATE%i[Marking.P->lastEntry] = Marking.P->_STATE%i[Marking.P->lastEntry+1];" tabs b.blockid b.blockid;
    Printf.fprintf skCpp "\n%s\tstateChanges = true;" tabs;
    Printf.fprintf skCpp "\n%s}" tabs
  | _ -> ();;

let rec getPreviousStates skCpp tabs b =
  match b.blocktype with
  | y when List.exists (fun x -> x=y) skConditional ->
    Printf.fprintf skCpp "\n%sMarking.P->_STATE%i[Marking.P->lastEntry+1] = Marking.P->_STATE%i[Marking.P->lastEntry];" tabs b.blockid b.blockid;
  | _ -> ();;

let rec genEulerIntegrators skCpp lL lB = function
    [] -> Printf.fprintf skCpp "\treturn stepSK;\n";
  | b::q when b.blocktype="Integrator" ->
     let (ba,ia) = findSrc (b.blockid,1) lL in
        let isCS = isCondSignal (b.blockid,1) lB lL in
          if isCS then begin
            let zcBlock = getCondBlock (b.blockid,1) lB lL in
            Printf.fprintf skCpp "\tif (Marking.P->lastPrintEntry == idx && Marking.P->_ZC%i[idx]) {" zcBlock;
            Printf.fprintf skCpp "\n\t\tMarking.P->_BLOCK%i_OUT%i[idx] = %s;" b.blockid 1 (getCondThreshold (b.blockid,1)
lB lL);
            Printf.fprintf skCpp "\n} else { \n"
          end;
     Printf.fprintf skCpp "\tMarking.P->_BLOCK%i_OUT%i[idx] = Marking.P->_BLOCK%i_OUT%i[previdx] + step * Marking.P->_BLOCK%i_OUT%i[idx];\n" b.blockid 1 b.blockid 1 ba ia;
          if isCS then begin
            Printf.fprintf skCpp "\n}"
          end;
     genEulerIntegrators  skCpp lL lB q;
  | b::q -> genEulerIntegrators  skCpp lL lB q;;
           
let rec genRK4Integrators skCpp lL lB step lI = function
    | [] when step<3 -> genRK4Entries  skCpp lL lB step lI;
                        Printf.fprintf skCpp "\tSKTime t%i = " (step+1);
      begin match step with
      | 0 -> Printf.fprintf skCpp "t0.getDouble() * step/2.;";
      | 1 -> Printf.fprintf skCpp "t0.getDouble() * step/2.;";
      | 2 -> Printf.fprintf skCpp "t0.getDouble() * step;"
      | _ -> () end;
      Printf.fprintf skCpp "\n\tMarking.P->currentLookup = t%i;" (step+1);
      Printf.fprintf skCpp "\n\tMarking.P->countDown = Marking.P->lastEntry;";
      Printf.fprintf skCpp "\n\tMarking.P->_TIME[idxtampon+%i] = t%i;\n" (step+1) (step+1);
      genRK4Integrators skCpp lL lB (step+1) [] lB
    | [] when step=3 -> genRK4Entries skCpp lL lB step lI;
                        genRK4Values skCpp lL lB lI;
    | [] -> () (* Terminé *)
    | b::q when b.blocktype="Integrator" -> begin
      let (ba,ia) = findSrc (b.blockid,1) lL in
      Printf.fprintf skCpp "\tdouble k%i_b%i = " step b.blockid;
      begin match step with
      0 -> Printf.fprintf skCpp "Marking.P->_BLOCK%i_OUT%i[previdx];" b.blockid 1; (* ba ia; *)
      | step -> Printf.fprintf skCpp "Marking.P->_BLOCK%i_OUT%i[idxtampon+%i];" ba ia (step-1);
      end; Printf.fprintf skCpp "\n\tdouble y%i_b%i = " step b.blockid;
      begin match step with
      0 -> Printf.fprintf skCpp "k%i_b%i;" step b.blockid
      | 1 -> Printf.fprintf skCpp "y0_b%i + step/2. * k1_b%i;" b.blockid b.blockid
      | 2 -> Printf.fprintf skCpp "y0_b%i + step/2. * k2_b%i;" b.blockid b.blockid
      | 3 -> Printf.fprintf skCpp "y0_b%i + step * k3_b%i;" b.blockid b.blockid
      | _ -> () (* ToDo *)
      end;
      Printf.fprintf skCpp "\n";
      genRK4Integrators skCpp lL lB step (b::lI) q
      end
    | b::q -> genRK4Integrators skCpp lL lB step lI q

  and genRK4Entries skCpp lL lB step = function
    | [] -> ()
    | b::q -> let (ba,ia) = findSrc (b.blockid,1) lL in
                let inb = getBlockById ba lB in
                let (bB,bL) = bkwdGraph (lB,lL) inb in genRK4Bkwd skCpp lL lB step bB;
                                                       genRK4Entries skCpp lL lB step q;

  and genRK4Bkwd skCpp lL lB step = function
    | [] -> ()
    | b::q when List.exists (fun x -> x=b.blocktype) skConditional ->
       Printf.fprintf skCpp "\tMarking.P->_STATE%i[idxtampon+%i] = Marking.P->_STATE%i[previdx];\n" b.blockid step b.blockid;
       Printf.fprintf skCpp "\texecuteBlocks(%i,idxtampon+%i);\n" b.blockid step;
       genRK4Bkwd skCpp lL lB step q;
    | b::q when b.blocktype="Integrator" ->
       Printf.fprintf skCpp "\tMarking.P->_BLOCK%i_OUT%i[idxtampon+%i] = y%i_b%i;\n" b.blockid 1 step step b.blockid;
       genRK4Bkwd skCpp lL lB step q;
    | b::q -> Printf.fprintf skCpp "\texecuteBlocks(%i,idxtampon+%i);\n" b.blockid step;
              genRK4Bkwd  skCpp lL lB step q;

  and genRK4Values  skCpp lL lB = function
    | [] -> Printf.fprintf skCpp "\treturn stepSK;\n";
    | b::q -> begin
        let (ba,ia) = findSrc (b.blockid,1) lL in
        let isCS = isCondSignal (b.blockid,1) lB lL in
          if isCS then begin
            let zcBlock = getCondBlock (b.blockid,1) lB lL in
            Printf.fprintf skCpp "\tif (Marking.P->lastPrintEntry == idx && Marking.P->_ZC%i[idx]) {" zcBlock;
            Printf.fprintf skCpp "\n\t\tMarking.P->_BLOCK%i_OUT%i[idx] = %s;" b.blockid 1 (getCondThreshold (b.blockid,1)
lB lL);
            Printf.fprintf skCpp "\n} else { \n"
          end;
        Printf.fprintf skCpp "\tdouble k4_b%i = Marking.P->_BLOCK%i_OUT%i[idxtampon+3];\n" b.blockid ba ia;
        Printf.fprintf skCpp "\tMarking.P->_BLOCK%i_OUT%i[idx] = y0_b%i + step/6. * (k1_b%i + 2.*k2_b%i + 2.*k3_b%i + k4_b%i);\n" b.blockid 1 b.blockid b.blockid b.blockid b.blockid b.blockid;
        if isCS then begin
          Printf.fprintf skCpp "\n}"
        end
      end;
              genRK4Values skCpp lL lB q           

  let rec genODE45Integrators skCpp lL lB step lI = function
    | [] when step<5 -> genODE45Entries skCpp lL lB step lI; Printf.fprintf skCpp "\tSKTime t%i = " (step+1);
                        begin match step with
                        | 0 -> Printf.fprintf skCpp "t0.getDouble() + step/4.;";
                        | 1 -> Printf.fprintf skCpp "t0.getDouble() + step*3/8.;";
                        | 2 -> Printf.fprintf skCpp "t0.getDouble() + step*12/13.;";
                        | 3 -> Printf.fprintf skCpp "t0.getDouble() + step;";
                        | 4 -> Printf.fprintf skCpp "t0.getDouble() + step*1/2.;";
                        | _ -> ()
                        end;
                        Printf.fprintf skCpp "\n\tMarking.P->currentLookup = t%i;" (step+1);
                        Printf.fprintf skCpp "\n\tMarking.P->countDown = Marking.P->lastEntry;";
                        Printf.fprintf skCpp "\n\tMarking.P->_TIME[idxtampon+%i] = t%i;\n" (step+1) (step+1);
                        genODE45Integrators skCpp lL lB (step+1) [] lB
    | [] when step=5 -> genODE45Entries  skCpp lL lB step lI;
                        genODE45Values  skCpp lL lB lI;
    | [] -> ()
    | b::q when b.blocktype="Integrator" -> begin
        let (ba,ia) = findSrc (b.blockid,1) lL in
        Printf.fprintf skCpp "\n\tdouble k%i_b%i = " step b.blockid;
        begin match step with
          0 -> Printf.fprintf skCpp "Marking.P->_BLOCK%i_OUT%i[previdx];" b.blockid 1;
        | step -> Printf.fprintf skCpp "Marking.P->_BLOCK%i_OUT%i[idxtampon+%i];" ba ia (step-1);
        end; Printf.fprintf skCpp "\n\tdouble y%i_b%i = " step b.blockid;
        begin match step with
          0 -> Printf.fprintf skCpp "k%i_b%i" step b.blockid
        | 1 -> Printf.fprintf skCpp "y0_b%i + step/4. * k1_b%i" b.blockid b.blockid
        | 2 -> Printf.fprintf skCpp "y0_b%i + step*3/32. * k1_b%i + step*9/32. * k2_b%i" b.blockid b.blockid b.blockid
        | 3 -> Printf.fprintf skCpp "y0_b%i + step*1932/2197. * k1_b%i - step*7200/2197. * k2_b%i + step*7296/2197 * k3_b%i" b.blockid b.blockid b.blockid b.blockid
        | 4 -> Printf.fprintf skCpp "y0_b%i + step*439/216. * k1_b%i - step*8. * k2_b%i + step*3680/513. * k3_b%i - step*845/4104. * k4_b%i" b.blockid b.blockid b.blockid b.blockid b.blockid
        | 5 -> Printf.fprintf skCpp "y0_b%i - step*8/27. * k1_b%i + step*2. * k2_b%i - step*3544/2565. * k3_b%i + step*1859/4104. * k4_b%i - step*11/40. * k5_b%i" b.blockid b.blockid b.blockid b.blockid b.blockid b.blockid
        | _ -> ()
        end;
        Printf.fprintf skCpp ";\n";
        Printf.fprintf skCpp "\n\tif (std::isinf(y%i_b%i) || std::isnan(y%i_b%i)) { cerr << \"An intermediate value of an Integrator block is either Infinite or NaN.\" << endl; exit(1); }" step b.blockid step b.blockid;
        genODE45Integrators skCpp lL lB step (b::lI) q;
      end
    | b::q -> genODE45Integrators  skCpp lL lB step lI q
  and genODE45Entries  skCpp lL lB step = function
    | [] -> ()
    | b::q -> let (ba,ia) = findSrc (b.blockid,1) lL in
              let inb = getBlockById ba lB in
              let (bB,bL) = bkwdGraph (lB,lL) inb in
              genODE45Bkwd  skCpp lL lB step bB;
              genODE45Entries  skCpp lL lB step q;
  and genODE45Bkwd  skCpp lL lB step = function
    | [] -> ()
    | b::q when List.exists (fun x -> x=b.blocktype) skConditional ->
       Printf.fprintf skCpp "\tMarking.P->_STATE%i[idxtampon+%i] = Marking.P->_STATE%i[previdx];\n" b.blockid step b.blockid;
       Printf.fprintf skCpp "\texecuteBlocks(%i,idxtampon+%i);\n" b.blockid step;
       genODE45Bkwd skCpp lL lB step q;
    | b::q when b.blocktype="Integrator" ->
       Printf.fprintf skCpp "\tMarking.P->_BLOCK%i_OUT%i[idxtampon+%i] = y%i_b%i;\n" b.blockid 1 step step b.blockid;
       genODE45Bkwd  skCpp lL lB step q;
    | b::q -> Printf.fprintf skCpp "\texecuteBlocks(%i,idxtampon+%i);\n" b.blockid step;
              genODE45Bkwd  skCpp lL lB step q;
  and genODE45Values  skCpp lL lB = function
    | [] -> Printf.fprintf skCpp "\treturn stepSK;\n";
    | b::q -> begin
        let (ba,ia) = findSrc (b.blockid,1) lL in
        Printf.fprintf skCpp "\tdouble k6_b%i = Marking.P->_BLOCK%i_OUT%i[idxtampon+5];\n" b.blockid ba ia;
        Printf.fprintf skCpp "\tdouble rk4_%i = y0_b%i + 25/216.*step*k1_b%i + 1408/2565.*step*k3_b%i + 2197/4104.*step*k4_b%i - 1/5.*step*k5_b%i;\n" b.blockid b.blockid b.blockid b.blockid b.blockid b.blockid;
        Printf.fprintf skCpp "\tdouble rk5_%i = y0_b%i + 16/135.*step*k1_b%i + 6656/12825.*step*k3_b%i + 28561/56430.*step*k4_b%i - 9/50.*step*k5_b%i + 2/55.*step*k6_b%i;\n" b.blockid b.blockid b.blockid b.blockid b.blockid b.blockid b.blockid;
        let isCS = isCondSignal (b.blockid,1) lB lL in
          if isCS then begin
            let zcBlock = getCondBlock (b.blockid,1) lB lL in
            Printf.fprintf skCpp "\tif (Marking.P->lastPrintEntry == idx && Marking.P->_ZC%i[idx]) {" zcBlock;
            Printf.fprintf skCpp "\n\t\tMarking.P->_BLOCK%i_OUT%i[idx] = %s;" b.blockid 1 (getCondThreshold (b.blockid,1) lB lL);
            Printf.fprintf skCpp "\n} else { \n"
          end;
        Printf.fprintf skCpp "\tMarking.P->_BLOCK%i_OUT%i[idx] = rk4_%i;\n" b.blockid 1 b.blockid;
        Printf.fprintf skCpp "\tif (std::isinf(rk4_%i) || std::isinf(rk5_%i) || std::isnan(rk4_%i) || std::isnan(rk5_%i)) { cerr << \"The final value of an Integrator block is either Infinite or NaN\" << endl; exit(1); }\n" b.blockid b.blockid b.blockid b.blockid;
        Printf.fprintf skCpp "\tif (abs(rk4_%i - rk5_%i) > %f) { return stepSK.getDouble() * (pow(%f/abs(rk4_%i - rk5_%i),0.2)); }\n" b.blockid b.blockid !epsV !epsV b.blockid b.blockid;
        if isCS then begin
          Printf.fprintf skCpp "\t}\n"
        end;
      end;
              genODE45Values skCpp lL lB q               
                             
let generateCode lS (lB,lL) =
  let skCpp = open_out "SKModel.cpp" and
      mkImp = open_out "stateImpl.hpp" in
  
  let startTime = Simulinkparser.extfloat_of_string (List.assoc "StartTime" lS) and stopTime = Simulinkparser.extfloat_of_string (List.assoc "StopTime" lS) and fixedStep = Simulinkparser.extfloat_of_string (List.assoc "FixedStep" lS) in
  let fstartTime = match startTime with
    | Finite start -> start
    | _ -> failwith "startTime devrait être une valeur donnée"
  in let baseStep = match !skBaseStep with
    | None -> begin match fixedStep with
    | Auto -> begin match stopTime with
              | Auto -> failwith "stopTime = auto : pas de sens"
              | Infty -> failwith "stopTime = infty et fixedStep = auto ? Ah, le cas est arrivé. Il faut une valeur par défaut."
              | Finite stop -> (stop -. fstartTime) /. 50.
      end
    | Infty -> failwith "fixedStep = infty : cela n'a pas de sens"
    | Finite step -> step end
    | Some step -> step
  in
  let pC = Printf.fprintf skCpp in
  let pM = Printf.fprintf mkImp in

  (* Impression des headers *)
  pM "#ifndef _StateImpl_HPP\n#define _StateImpl_HPP\n";
  pM "#include <string.h>\n";
  (*pM "#include \"marking.hpp\"\n#include \"markingTemplate.hpp\"\n";*)
  pC "#include <iomanip>\n";
  pC "#include <cmath>\n";
  pC "#include <string>\n";
  (*pC "#include \"marking.hpp\"\n#include \"markingImpl.hpp\"\n";*)
  pC "#include \"SKTime.hpp\"\n";
  pC "#include \"SKModel.hpp\"\n";
  pC "#include \"stateImpl.hpp\"\n";
  pM "#include \"SKTime.hpp\"\n";
  pM "#include \"SKModel.hpp\"\n";
  pC "#include \"EventsQueue.hpp\"\n";
  pC "#include \"EventsQueueSet.hpp\"\n";

  generateBindingImpl mkImp;

  (* abstractState def dans le Cpp *)
  pC "\nabstractState::abstractState() {\n";
  pC "\tP = new stateImpl;\n";
  pC "\tresetToInitMarking();\n";
  pC "}\n";

  pC "\nabstractState::abstractState(const abstractState& m) {\n";
  pC "\tP = new stateImpl;\n";
  pC "\t*this = m;\n";
  pC "}\n";
  pC "\nabstractState& abstractState::operator = (const abstractState& m) {\n";
  pC "\t*P = *(m.P);\n";
  pC "\treturn *this;\n";
  pC "}\n";
  pC "\nabstractState::~abstractState() {\n";
  pC "\tdelete(P);\n";
  pC "}\n";

  pC "\nvoid abstractState::swap(abstractState& m) {\n";
  pC "\tstateImpl* tmp = m.P;\n";
  pC "\tm.P = P;\n";
  pC "\tP = tmp;\n";
  pC "}\n";

  (* Création des chaînes de caractères pour les fonctions d'impression *)
  let printHeaderTmp = Buffer.create 16 in
  let printTmp = Buffer.create 16 in
  let printSedCmdTmp = Buffer.create 16 in
  let generateNewEntries = Buffer.create 16 in
  let generateVectors = Buffer.create 16 in

  (* stateImpl + SKMarking.P->*)
  (*Printf.fprintf skHpp "class SKMarking {\n";
  Printf.fprintf skHpp "public:\n";
  Printf.fprintf skHpp "\tvoid printHeader(ostream&);\n";
  Printf.fprintf skHpp "\tvoid print(ostream&, float);\n";
  Printf.fprintf skHpp "\tvoid printSedCmd(ostream&);\n";
  Printf.fprintf skHpp "\n";*)
  
  pM "\nclass stateImpl {\npublic:";
  pM "\n\tsize_t lastPrintEntry;";
  pM "\n\tsize_t lastCondEntry;";
  pM "\n\tsize_t lastEntry;";
  pM "\n\tsize_t lastInport;";
  pM "\n\tsize_t totalEntries;";
  pM "\n\tsize_t countDown;";
  pM "\n\tSKTime currentLookup;";
  pM "\n\tstd::vector<SKTime> _TIME;";
  (*pH "\n\tvector<SKTime> _TIME;";*)
  let outputs_parse_regexp = Str.regexp "\\[\\([0-9]+\\), \\([0-9]+\\)\\]" in
  let listAllParams l = List.iter (fun (a,b) -> Printf.eprintf "%s => %s\n" a b) l in
  let genSignalNames t =
    Printf.fprintf mkImp "\n\t// Block %i - type %s (named %s) :" t.blockid t.blocktype t.name;
    try
      let numOfPorts = List.assoc "Ports" t.values in
      try let didmatch = Str.string_match outputs_parse_regexp numOfPorts 0 in
          if didmatch then begin
              try let nb = int_of_string@@ Str.matched_group 2 numOfPorts in
                  for i = 1 to nb do
                    Printf.fprintf mkImp "\n\tstd::vector<double> _BLOCK%i_OUT%i;" t.blockid i;
                    Printf.fprintf mkImp "\n\tdouble _PL_B%iO%i;" t.blockid i;
                    (*pH "\n\tvector<double> _BLOCK%i_OUT%i;" t.blockid i;*)
                    if !verbose > 3 || List.exists (fun x -> x=t.blocktype) skOutports then begin
                        Printf.bprintf printHeaderTmp "\n\ts << setw(5) << \"%s(%i).%i \";" t.name t.blockid i;
                        if List.exists (fun x -> x=t.blocktype) skContinuous || List.exists (fun (x,y) -> x="COSMOSCONT") t.values then
                          Printf.bprintf printTmp "\n\ts << setw(1) << (eTime > 0 ? P->_BLOCK%i_OUT%i[(P->lastEntry)] : P->_BLOCK%i_OUT%i[(P->lastPrintEntry)]) << \" \";" t.blockid i t.blockid i
                        else Printf.bprintf printTmp "\n\ts << setw(1) << P->_BLOCK%i_OUT%i[(P->lastPrintEntry)] << \" \";" t.blockid i;
                        Printf.bprintf printSedCmdTmp "\n\ts << \"-e 's/\\\\$B%iO%i\\\\$/\" << P->_BLOCK%i_OUT%i[(P->lastPrintEntry)] << \"/g' \";" t.blockid i t.blockid i;
                    end;
                    Printf.bprintf generateNewEntries "\n\tMarking.P->_BLOCK%i_OUT%i.push_back(0.0);" t.blockid i;
                    Printf.bprintf generateVectors "\n\tP->_BLOCK%i_OUT%i = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};" t.blockid i;
                    Printf.bprintf generateVectors "\n\tP->_PL_B%iO%i = 0.0;" t.blockid i;
                    if (List.exists (fun x -> x=t.blocktype) skOutports) then begin
                      Printf.fprintf mkImp "\n\tstd::vector<double> _%s_OUT%i;" t.name i;
                      Printf.bprintf generateNewEntries "\n\tMarking.P->_%s_OUT%i.push_back(0.0);" t.name i;
                      Printf.bprintf generateVectors "\n\tP->_%s_OUT%i = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};" t.name i;
                    end
                  done
              with Not_found ->  begin Printf.eprintf "[WARNING:] Wrong port format for block %i (type %s) : %s\n" t.blockid t.blocktype numOfPorts; end;
            end
          else Printf.fprintf mkImp "No Output."
      with Not_found -> begin Printf.eprintf "[WARNING:] Wrong port format for block %i (type %s) : %s\n" t.blockid t.blocktype numOfPorts; end;
    with Not_found -> begin Printf.eprintf "[WARNING:] Couldn't find port numbers for block %i (type %s)\n" t.blockid t.blocktype; listAllParams t.values end;
  in List.iter genSignalNames lB;
  Printf.fprintf mkImp "\n\tdouble _PL_testPlace;";
     
     let genCBStateNames b =
       if List.exists (fun x -> x=b.blocktype) skConditional then begin
           Printf.fprintf mkImp "\n\tstd::vector<size_t> _STATE%i;" b.blockid;
           Printf.fprintf mkImp "\n\tstd::vector<size_t> _ZC%i;" b.blockid;
           (*pH "\n\tvector<size_t> _STATE%i;" b.blockid;*)
           Printf.bprintf generateNewEntries "\n\tMarking.P->_STATE%i.push_back(0);" b.blockid;
           Printf.bprintf generateNewEntries "\n\tMarking.P->_ZC%i.push_back(0);" b.blockid;
           Printf.bprintf generateVectors "\n\tP->_STATE%i = {0,0,0,0,0,0,0,0,0,0,0};" b.blockid;
           Printf.bprintf generateVectors "\n\tP->_ZC%i = {0,0,0,0,0,0,0,0,0,0};" b.blockid;
         end in
     List.iter genCBStateNames lB; (* Conditional Block StateNames *)
     Printf.fprintf mkImp "\n};\n";
     (*pH "\n};\n";*)
     (* Footers *)
     Printf.fprintf mkImp "\n#endif";
  
     (*List.iter (fun t ->
    if t.blocktype = "Integrator" then ()
    else if List.exists (fun x -> x=t.blocktype) skConditional then begin
        Printf.fprintf skHpp "\n\tvoid executeBlock%i(int);" t.blockid;
        Printf.fprintf skHpp "\n\tvoid checkState%i(int);" t.blockid;
        Printf.fprintf skHpp "\n\tSKTime findStateChange%i(int,SKTime);" t.blockid;
        Printf.fprintf skHpp "\n\tSKTime linAppCrossing%i(SKTime,SKTime,double,double);" t.blockid;
        Printf.fprintf skHpp "\n\tvoid computeBkwd%i(int);" t.blockid;
      end else begin
        Printf.fprintf skHpp "\n\tvoid executeBlock%i(int);" t.blockid
      end)
            lB;*)

  (* Printing functions *)
  
  Printf.fprintf skCpp "\nvoid abstractState::printHeader(ostream& s) const{";
  Buffer.output_buffer skCpp printHeaderTmp;
  Printf.fprintf skCpp "\n}\n";
  Printf.fprintf skCpp "\nvoid abstractState::print(ostream& s,double eTime) const{";
  Buffer.output_buffer skCpp printTmp;
  Printf.fprintf skCpp "\n}\n";

  Printf.fprintf skCpp "\nvoid abstractState::printSedCmd(ostream& s) const{";
  Buffer.output_buffer skCpp printSedCmdTmp;
  Printf.fprintf skCpp "\n}\n";
  
  (* Reset abstractMarking *)
  Printf.fprintf skCpp "\nvoid abstractState::resetToInitMarking() {\n";
  Printf.fprintf skCpp "\tP->lastEntry = 0;\n";
  Printf.fprintf skCpp "\tP->lastPrintEntry = 0;\n";
  Printf.fprintf skCpp "\tP->lastCondEntry = 0;\n";
  Printf.fprintf skCpp "\tP->countDown = 0;\n";
  Printf.fprintf skCpp "\tP->currentLookup = 0.0;\n";
  Printf.fprintf skCpp "\tP->_TIME = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};";
  Buffer.output_buffer skCpp generateVectors;
  Printf.fprintf skCpp "\n}\n";

  (* Commandes liées à l'abstractBinding Now Useless *)
  (*
  Printf.fprintf skCpp "\nbool abstractBinding::next() {\n";
  Printf.fprintf skCpp "\tidcount++;\n";
  Printf.fprintf skCpp "\treturn false;\n";
  Printf.fprintf skCpp "}\n";
  Printf.fprintf skCpp "\nabstractBinding::abstractBinding() {\n";
  Printf.fprintf skCpp "\tidcount=0;\n";
  Printf.fprintf skCpp "}\n";
  Printf.fprintf skCpp "\nabstractBinding::abstractBinding(const abstractBinding& m) {\n";
  Printf.fprintf skCpp "\tidcount=m.idcount;\n";
  Printf.fprintf skCpp "}\n";
  Printf.fprintf skCpp "abstractBinding::~abstractBinding() { }\n";
  Printf.fprintf skCpp "abstractBinding& abstractBinding::operator = (const abstractBinding& m) {\n";
  Printf.fprintf skCpp "\tidcount = m.idcount;\n";
  Printf.fprintf skCpp "\treturn *this;\n";
  Printf.fprintf skCpp "}\n";
  Printf.fprintf skCpp "void abstractBinding::print() const{ }\n";
  Printf.fprintf skCpp "int abstractBinding::id() const { return idcount; }\n";
  Printf.fprintf skCpp "int abstractBinding::idTotal() const { return 0; }\n";*)

  (* Gestion des latences *)
  Printf.fprintf skCpp "\ntemplate <class EQT>\nint SKModel<EQT>::findLatencyIndex(double latency) {\n";
  Printf.fprintf skCpp "\tSKTime currTime = Marking.P->currentLookup;\n";
  Printf.fprintf skCpp "\twhile (Marking.P->countDown > 0 && Marking.P->_TIME[Marking.P->countDown] > (currTime - latency)) {\n";
  Printf.fprintf skCpp "\t\tMarking.P->countDown--;\n";
  Printf.fprintf skCpp "\t}\n";
  Printf.fprintf skCpp "\tif (Marking.P->_TIME[Marking.P->countDown] > (currTime - latency)) { return -1;\n";
  Printf.fprintf skCpp "\t} else { return Marking.P->countDown; }\n";
  Printf.fprintf skCpp "}\n";

  (* Initialisation des blocs Intégrateurs *)
  Printf.fprintf skCpp "\ntemplate<class EQT>\nvoid SKModel<EQT>::initialiseIntegrators(int idx) {";
  let rec genInitIntegrators = function
    [] -> ()
    | b::q when b.blocktype="Integrator" || b.blocktype="DiscreteIntegrator" ->
      begin try
         let cstValue = List.assoc "InitialCondition" b.values in
              Printf.fprintf skCpp "\n\tMarking.P->_BLOCK%i_OUT1[idx] = %s;" b.blockid cstValue;
      with Not_found -> begin
          Printf.eprintf "[WARNING:] No initial condition found for block %s (type %s); defaulting to 0.\n" b.name b.blocktype;
          Printf.fprintf skCpp "\n\tMarking.P->_BLOCK%i_OUT1[idx] = 0;" b.blockid;
      end; end;
      genInitIntegrators q;
    | b::q -> genInitIntegrators q
  in genInitIntegrators lB;
  Printf.fprintf skCpp "\n};\n";

  (* Implémentation des intégrales : Euler *) (* INTEGRATORS *)
  Printf.fprintf skCpp "\ntemplate<class EQT>\nvoid SKModel<EQT>::executeIntegrators(int idx) {\n";
  Printf.fprintf skCpp "\testimateIntegrators(idx,Marking.P->lastEntry-1,(Marking.P->_TIME[idx] - Marking.P->_TIME[Marking.P->lastEntry-1]).getDouble());\n";
  Printf.fprintf skCpp "};\n";
  Printf.fprintf skCpp "\ntemplate<class EQT>\nSKTime SKModel<EQT>::estimateIntegrators(int idx,int previdx,SKTime stepSK) {\n";
  Printf.fprintf skCpp "\tdouble step = stepSK.getDouble();\n";
  Printf.fprintf skCpp "\tint idxtampon = Marking.P->lastEntry+2;\n"; (* ÉTAIT +1 *)
  Printf.fprintf skCpp "\tSKTime t0 = Marking.P->_TIME[previdx];\n";
  Printf.fprintf skCpp "\tMarking.P->_TIME[idxtampon+0] = Marking.P->_TIME[previdx];\n";
  Printf.fprintf skCpp "\tMarking.P->countDown = Marking.P->lastEntry;\n";
  Printf.fprintf skCpp "\tMarking.P->currentLookup = t0;\n";
  let diffMethods = !skUseRK4 + !skUseEuler + !skUseODE45 in
  if diffMethods > 1 then Printf.eprintf "[WARNING:] Specified multiple solvers for Integrators.\n";
  if diffMethods = 0 then begin
    Printf.eprintf "[INFO:] No solver specified for Integrators, defaulting to ODE45.\n";
    skUseODE45 := 1;
  end;

  if !skUseODE45>0 then genODE45Integrators skCpp lL lB 0 [] lB;
  if !skUseEuler>0 then genEulerIntegrators skCpp lL lB lB;
  if !skUseRK4>0 then genRK4Integrators skCpp lL lB 0 [] lB;

  Printf.fprintf skCpp "};\n";       
  
  pC "\ntemplate<class EQT>\nvoid SKModel<EQT>::checkStates(size_t bid,int idx) {\n";
  genSwitchStatment skCpp (genCondFunctions lL) lB;
  pC "}\n";

  pC "template<class EQT>\nSKTime SKModel<EQT>::linAppCrossings(size_t bid,SKTime tU,SKTime tA,double vU,double vA) {\n";
  genSwitchStatment skCpp genLinAppCrossing lB;
  pC "}\n";
     
  pC "\ntemplate<class EQT>\nvoid SKModel<EQT>::computeBkwds(size_t bid,int idx) {\n";
  genSwitchStatment skCpp (genZCFunctions lL lB) lB;
  pC "}\n";
        
  pC "\ntemplate<class EQT>\nSKTime SKModel<EQT>::findStateChanges(size_t bid,int idx,SKTime stepSK) {";
  genSwitchStatment skCpp (genfindStateChange lL) lB;
  pC "}\n";

  pC "\ntemplate<class EQT>\nvoid SKModel<EQT>::executeBlocks(size_t bid,int idx) {\n";
  genSwitchStatment skCpp (genBlockFunctions lL lB) lB;
  pC "}\n";

  pC "\ntemplate<class EQT>\nvoid SKModel<EQT>::setInput(size_t bid,double value) {";
  pC "\n\tsize_t idx = Marking.P->lastEntry;";
  pC "\n\tMarking.P->lastInport = Marking.P->lastEntry + 1;";
  genSwitchStatment skCpp (genSetInputs lL) lB;
  pC "}";

  pC "\ntemplate<class EQT>\nvoid SKModel<EQT>::setInput(std::string bname,double value) {";
  pC "\n\tsize_t idx = Marking.P->lastEntry;";
  pC "\n\tMarking.P->lastInport = Marking.P->lastEntry + 1;";
  genSwitchStatmentNames skCpp (genSetInputs lL) lB;
  pC "}";

  pC "\ntemplate<class EQT>\nvoid SKModel<EQT>::updateInput(double ctime, size_t tr, const abstractBinding& b, EQT &EQ, timeGen &TG) {";
  pC "\n\tif (Marking.P->lastEntry == 0) { initialiseIntegrators(0); }";
  pC "\n\tMarking.P->lastEntry = Marking.P->lastPrintEntry + 1;";
  Printf.fprintf skCpp "\tMarking.P->_TIME.push_back(0.0);";
  Buffer.output_buffer skCpp generateNewEntries;
  pC "\n\tSKTime t = ctime;";
  pC "\n\tMarking.P->_TIME[Marking.P->lastEntry] = t;";
  ignore @@ List.fold_left (genSignalChanges skCpp "\t") 6 lB;
  pC "\n\tgenerateEvent(ctime, E, 0, TG);";
  pC "\n\tEQ.replace(E);";
  ignore @@ List.iter (keepInportValues skCpp "") lB;
  pC "\n}";

  (* Génération de la fonction fire *)
  Printf.fprintf skCpp "\ntemplate<class EQT>\nvoid SKModel<EQT>::fire(size_t tr,const abstractBinding&, double ctime) {\n";
  Printf.fprintf skCpp "\tMarking.P->countDown = Marking.P->lastEntry;\n";
  Printf.fprintf skCpp "\tMarking.P->lastPrintEntry = Marking.P->lastEntry;\n";
  Printf.fprintf skCpp "\tMarking.P->_TIME[Marking.P->lastEntry] = ctime;\n";
  Printf.fprintf skCpp "\tMarking.P->currentLookup = Marking.P->_TIME[Marking.P->lastEntry];";
  ignore @@ List.fold_left (genSignalChanges skCpp "\t\t") 0 lB;
  Printf.fprintf skCpp "\n\tMarking.P->lastCondEntry = Marking.P->lastEntry;\n";
  Printf.fprintf skCpp "\n\tMarking.P->_TIME[Marking.P->lastEntry+1] = ctime + %f;" !epsT;
  ignore @@ List.iter (getPreviousStates skCpp "\t") lB;
  ignore @@ List.fold_left (genSignalChanges skCpp "\t") 4 lB;
  Printf.fprintf skCpp "\n\tbool stateChanges = false;";
  Printf.fprintf skCpp "\n\tint simZCs = 0;";
  ignore @@ List.iter (checkStateChanges skCpp "\t") lB;
  Printf.fprintf skCpp "\n\twhile (stateChanges) {";
  Printf.fprintf skCpp "\n\t\tsimZCs = simZCs + 1;";
  Printf.fprintf skCpp "\n\t\tstateChanges = false;";
  ignore @@ List.iter (getPreviousStates skCpp "\t\t") lB;
  ignore @@ List.fold_left (genSignalChanges skCpp "\t\t") 4 lB;
  ignore @@ List.iter (checkStateChanges skCpp "\t\t") lB;
  Printf.fprintf skCpp "\n\t\tif (stateChanges && simZCs > %i) {" !maxSimZC;
  Printf.fprintf skCpp "\n\t\t\tcerr << \"Could not find a reliable state after \" << simZCs << \" attempts.\" << endl;";
  Printf.fprintf skCpp "\n\t\t\texit(1);";
  Printf.fprintf skCpp "\n\t\t}";
  Printf.fprintf skCpp "\n\t}";
  Printf.fprintf skCpp "\n};\n";

  (* Mise à jour de la queue d'évènements *)
(*  let endTcondB = match stopTime with
  | Infty -> "true"
  | Auto -> "true"
  | Finite value -> "t < "^(string_of_float value) and
  endTcond = match stopTime with
  | Infty -> "false"
  | Auto -> "false"
  | Finite value -> "t < "^(string_of_float value) and 
  endTvalue = match stopTime with 
  | Finite value -> string_of_float value
  | _ -> "NaN" in *)

  Printf.fprintf skCpp "\ntemplate<class EQT>\nvoid SKModel<EQT>::update(double ctime, size_t tr, const abstractBinding& b, EQT &EQ, timeGen &TG) {\n";
  Printf.fprintf skCpp "\tMarking.P->lastEntry = Marking.P->lastEntry + 1;\n";
  Printf.fprintf skCpp "\tMarking.P->_TIME.push_back(0.0);";
  Buffer.output_buffer skCpp generateNewEntries;
  Printf.fprintf skCpp "\n\tSKTime t = ctime;\n";
  Printf.fprintf skCpp "\n\tSKTime currentUpdTime = ctime;\n";
  Printf.fprintf skCpp "\tSKTime startTime = %f;\n" fstartTime;
  Printf.fprintf skCpp "\tSKTime step = %f;\n" baseStep;
  Printf.fprintf skCpp "\tSKTime latstep = %f;\n" baseStep;
  Printf.fprintf skCpp "\tint latnb = 0;\n";
  Printf.fprintf skCpp "\tSKTime oldStep = 0.0;\n";
(*  Printf.fprintf skCpp "\tif (%s) {\n" endTcondB; *)
  Printf.fprintf skCpp "\t\twhile (oldStep != step) {\n";
  Printf.fprintf skCpp "\t\t\toldStep = step;\n";
  Printf.fprintf skCpp "\t\t\tt = step + ctime;\n";
(*  Printf.fprintf skCpp "\t\t\tif (%s) {" endTcondB; *)
  Printf.fprintf skCpp "\n\t\t\t\tMarking.P->countDown = Marking.P->lastEntry;";
  Printf.fprintf skCpp "\n\t\t\t\tMarking.P->_TIME[Marking.P->lastEntry] = t;";
  ignore @@ List.fold_left (genSignalChanges skCpp "\t\t\t\t") 1 lB;
  Printf.fprintf skCpp "\n\t\t\t\tif (step.getBareTime() == 0) { step.setBareTime(1); }";
(*  Printf.fprintf skCpp "\n\t\t\t} else {\n";
  Printf.fprintf skCpp "\t\t\t\tstep = %s - ctime;\n" endTvalue;
  Printf.fprintf skCpp "\n\t\t\t}\n"; *)
  Printf.fprintf skCpp "\t\t}\n";
  Printf.fprintf skCpp "\tgenerateEvent(t.getDouble(), E, 0, TG);\n";
  Printf.fprintf skCpp "\tEQ.replace(E);\n";
  ignore @@ List.iter (keepInportValues skCpp "") lB;
(*  Printf.fprintf skCpp "\t} else { EQ.reset(); }\n"; *)
  Printf.fprintf skCpp "};\n";
  
  (* Dernière ligne : *)
  Printf.fprintf skCpp "template class SKModel<EventsQueue>;\n";
  Printf.fprintf skCpp "template class SKModel<EventsQueueSet>;\n"

let generateGSPN (lB, lL) =
  let net = (Net.create ():StochasticSymmetricNet.sspt) in
  let ccc =
    "<attribute name=\"classDeclaration\">
      <attribute name=\"name\">ContinuousColorClass</attribute>
      <attribute name=\"continuous\">true</attribute>
     </attribute>
     <attribute name=\"variableDeclaration\">
      <attribute name=\"name\">continuous</attribute>
      <attribute name=\"type\">ContinuousColorClass</attribute>
  </attribute>" in
  net.Net.def <- Some ({ intconst=[]; floatconst=[]; labels=[];formulas=[]; clock=[]; classDef=[ccc]; printer= (fun out () ->
      output_string out "  <attribute name=\"Hybrid_GSPN_Simulink\"></attribute>"
                      (*output_string out "  <attribute name=\"externalMainFile\">main.cpp</attribute>"*))});
  let outputs_parse_regexp = Str.regexp "\\[\\([0-9]+\\), \\([0-9]+\\)\\]" in
let outports_parse_regexp = Str.regexp "\\[\\([0-9]+\\).*\\]" in
    let generatePlaces = function
      b when List.exists (fun x -> x=b.blocktype) skOutports -> begin
      try
        let numOfPorts = List.assoc "Ports" b.values in
        try let didmatch = Str.string_match outports_parse_regexp numOfPorts 0 in
            if didmatch then
              try let nb = int_of_string@@ Str.matched_group 1 numOfPorts in
                  for i = 1 to nb do
                    let p = Net.add_place net (sprintf "B%iO%i" b.blockid i) (StochasticSymmetricNet.singleTokenInPlace "ContinuousColorClass" 0.0) in
                    let t =  Net.add_transition net (sprintf "Synch%iO%i" b.blockid i) (Synchronisation (sprintf "_%s_OUT%i" b.name i), Float 1.0,Float 1.0) in
                    Net.add_outArc net t p (StochasticSymmetricNet.OrdinaryVal (Int 1)) 
                  done
              with Not_found -> begin Printf.eprintf "[WARNING:] Wrong port format for block %i (type %s) : %s\n" b.blockid b.blocktype numOfPorts
              end
        with Not_found -> begin Printf.eprintf "[WARNING:] Wrong port format for block %i (type %s) : %s\n" b.blockid b.blocktype numOfPorts; end
      with Not_found -> begin Printf.eprintf "[WARNING:] Couldn't find port numbers for block %i (type %s)\n" b.blockid b.blocktype; end
    end
    | b when List.exists (fun x -> x=b.blocktype) skInports -> begin
      try
        let numOfPorts = List.assoc "Ports" b.values in
        try let didmatch = Str.string_match outputs_parse_regexp numOfPorts 0 in
            if didmatch then
              try let nb = int_of_string@@ Str.matched_group 2 numOfPorts in
                 for i = 1 to nb do
                    let p = Net.add_place net (sprintf "IN%i" b.blockid) (StochasticSymmetricNet.Ordinary(Int 0)) in
                    let t = Net.add_transition net (sprintf "SynchIN%i" b.blockid) (Synchronisation (sprintf "%i" b.blockid), Float 1.0, Float 1.0) in
                    Net.add_inArc net p t (StochasticSymmetricNet.OrdinaryVal (Int 1))
                 done
              with Not_found -> begin Printf.eprintf "[WARNING:] Wrong port format for block %i (type %s) : %s\n" b.blockid b.blocktype numOfPorts end
        with Not_found -> begin Printf.eprintf "[WARNING:] Wrong port format for block %i (type %s) : %s\n" b.blockid b.blocktype numOfPorts end
      with Not_found -> begin Printf.eprintf "[WARNING:] Couldn't find port numbers for block %i (type %s)\n" b.blockid b.blocktype; end
      end
    | b -> ()
    in List.iter generatePlaces lB; 
       (*let p = Net.add_place net "testPlace" (StochasticSymmetricNet.Ordinary(Int 0)) in
       let distr = ((StochasticPetriNet.Exp (Float 1.0)) ,Float 1.0, Float 1.0) in
       let t = Net.add_transition net "testTransition" distr in
       Net.add_outArc net t p (StochasticSymmetricNet.OrdinaryVal (Int 1));*) 
       net
