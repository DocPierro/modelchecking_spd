open Xml
open Type
open Lexing

type block = { blocktype: string; blockid: int; name: string; values: (string * string) list }
type blockDefault = (string * (string * string) list) (* C'est blocktype et values seulement *)
type blockPort = int * int
type simulinkLink = { fromblock: int; fromport: int; toblock: int; toport: int }
type simulinkModel = block list * blockDefault list * simulinkLink list
type simulinkPModel = block list * simulinkLink list

let rec printValues f = function
        (c,v)::q -> Printf.fprintf f "\t%s -> %s\n" c v; printValues f q
        | [] -> ();;
let printBlock f b = Printf.fprintf f "%s: %s (%i)\n%a" b.name b.blocktype b.blockid printValues b.values;;
let printBlocklist f lB = List.iter (printBlock f) lB;;
let printParDef f (t,v) = Printf.fprintf f "%s:\n%a" t printValues v;;
let printBlockParDeflist f lP = List.iter (printParDef f) lP;;
let printLink f link = Printf.fprintf f "[ %i (port %i) -> %i (port %i) ]\n" link.fromblock link.fromport link.toblock link.toport;;
let printLinks f lL = List.iter (printLink f) lL;;

let verbose = ref 0;;

let printModel f ((lB, lP, lL):simulinkModel) =
  printBlocklist f lB;
  printBlockParDeflist f lP;
  printLinks f lL

let printPModel f ((lB,lL):simulinkPModel) =
  printBlocklist f lB;
  printLinks f lL

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

(* Build block parameters defaults *)

let completeDefaults params = function
  | "UnitDelay" -> ("Ports","[1, 1]")::params
  | "Gain" -> ("Ports","[1, 1]")::params
  | "Rounding" -> ("Ports","[1, 1]")::params
  | "Constant" -> ("Ports","[0, 1]")::params
  | "Switch" -> ("Ports","[3, 1]")::params
  | "Fcn" -> ("Ports","[1, 1]")::params
  | "Inport" -> ("Ports","[0, 1]")::params
  | "Outport" -> ("Ports","[1, 1]")::params
  | "Terminator" -> ("Ports", "[1, 0]")::params
  | "Relay" -> ("Ports", "[1, 1]")::params
  | blockType -> params;;

let completeOutputs lB =
  let rec completeOutports = function
    | ("Ports",valeur)::q ->
      let inport_regexp = Str.regexp "\\[\\([0-9]+\\)\\]" and nbp=ref "0" in
      begin
        try let didmatch = Str.string_match inport_regexp valeur 0 in
          if didmatch then begin
            try let nb = Str.matched_group 1 valeur in
               nbp:=nb;
               Printf.eprintf "[INFO:] Changing ports from %s to [%s,%s]\n" valeur nb nb;
            with Not_found -> begin
                Printf.eprintf "[WARNING:] Could not fix ports for value %s\n" valeur;
            end
          end
        with Not_found -> begin
                Printf.eprintf "[WARNING:] Could not fix ports for value %s\n" valeur;
        end;
        end;
        ("Ports","["^(!nbp)^", "^(!nbp)^"]")::q
    | t::q -> t::(completeOutports q)
    | [] -> []
  and completeBlock b = ({ blocktype = b.blocktype;
            blockid = b.blockid;
            name = b.name;
            values = (completeOutports b.values)})
  and aux = function
    | b::q when b.blocktype="Scope" -> (completeBlock b)::(aux q)
    | b::q when b.blocktype="Display" -> (completeBlock b)::(aux q)
    | b::q -> b::aux q
    | [] -> []
  in aux lB;;

let parseInstantData prefix liste = function
    | Element ("P",["Name",x],[PCData(l)]) -> (prefix^x,l)::liste
    | _ -> failwith "Could not parse a InstanceData instance.";;

let parseblockParams liste = function
    | Element  ("P",["Name",x],[PCData(l)]) -> (x,l)::liste;
    | Element  ("InstanceData",[],clist) -> (List.fold_left (parseInstantData "_ID_") [] clist)@liste;
    | Element  ("List",_,_) -> Printf.fprintf stderr "Do not not what to do with List:ignore\n"; liste
    | Element  ("P",_,_) -> failwith "Unexpected format for a P XML block."
    | Element  ("InstanceData",_,_) -> failwith "Unexpected format for a InstanceData XML block"
    | Element  (blockname,_,_) -> failwith ("Unexpected "^blockname^" in block parameters.")
    | _ -> failwith "Could not parse the parameters of a block";;

let rec blockparams_of_simulink ((lB, lP, lL) as ml)  = function
    | Element (name,alist,clist) (* as t *) ->
        begin match name with
        | "BlockParameterDefaults" -> List.fold_left blockparams_of_simulink ml clist
        | "Block" -> let blockType = List.assoc "BlockType" alist
                     and blockParams = List.fold_left parseblockParams [] clist in
                (lB, (blockType, completeDefaults blockParams blockType)::lP, lL)
        | _ -> Printf.fprintf stderr "In BlockParameterDefaults, %s should not exist\n" name; ml
        end
    | PCData (s) -> output_string stderr s; ml
;;

let extract_src s = s;;
let extract_dst s = s;; (* Vérifier si c'est le bon type de in/out *)

let extractLink (s,d) =
  let regexpIn = Str.regexp "\\([0-9]+\\)#out:\\([0-9]+\\)" and
      regexpOut = Str.regexp "\\([0-9]+\\)#in:\\([0-9]+\\)" in
        assert (Str.string_match regexpIn s 0);
        let inId = int_of_string (Str.matched_group 1 s) and
            inPort = int_of_string (Str.matched_group 2 s) in
        assert (Str.string_match regexpOut d 0);
        let outId = int_of_string (Str.matched_group 1 d) and
            outPort = int_of_string (Str.matched_group 2 d) in
        ({ fromblock = inId;
           fromport = inPort;
           toblock = outId;
           toport = outPort });;

let rec flattenLinkBranches s lL = function
  | [] -> lL
  | (Element ("Branch",alist,clist))::q -> flattenLinkBranches s lL (clist@q);
  | (Element ("P",["Name","Dst"],[PCData(d)]))::q -> extractLink (s,d)::(flattenLinkBranches s lL q);
  | (Element ("P",["Name","ZOrder"],_))::q -> flattenLinkBranches s lL q;
  | (Element ("P",["Name","Points"],_))::q -> flattenLinkBranches s lL q;
  | (Element ("P",["Name","Src"],_))::q -> flattenLinkBranches s lL q; (* déjà parsé *)
  | (Element ("P",["Name",name],_))::q -> print_string name; failwith "Unexpected element.";
  | (PCData (s))::q -> print_string s; failwith "PCData ?";
  | (Element (e,_,_))::q -> print_string e; failwith "Element ?";;
(*  | _ -> failwith "Unknown element in a Branch of a Link.";; *)

let line_of_simulink lL clist =
    let src = extract_src (findpropName "Src" clist) and dst = extract_dst (findpropName "Dst" clist) in
        match (src,dst) with
        | Some s, Some d -> (extractLink (s,d))::lL
        | Some s, None -> flattenLinkBranches s lL clist
        | _ -> failwith "A link without source or destination shouldn't exist";;

let flattenBlockNames name =
  let nameb = Bytes.of_string name in
  let caract = ref 'a' in
    for i=0 to ((Bytes.length nameb)-1) do
      caract := Bytes.get nameb i;
      if (!caract=' ') then Bytes.set nameb i '_';
    done;
    Bytes.to_string nameb

(* Build block lists *)
let rec blocklist_of_simulink ((lB, lP, lL):simulinkModel) = function
    | Element (name,alist,clist) (* as t *) ->
        begin match name with
        | "System" -> List.fold_left blocklist_of_simulink (lB, lP, lL) clist
        | "Line" -> (lB, lP, line_of_simulink lL clist)
        | "Block" -> let blockType = List.assoc "BlockType" alist and blockName = List.assoc "Name" alist
                     and blockID = List.assoc "SID" alist and blockParams = List.fold_left parseblockParams [] clist in
                     ({ blocktype = blockType;
                        blockid = int_of_string blockID;
                        name = flattenBlockNames blockName;
                        values = blockParams
                      }::lB, lP, lL);
        | _ when !verbose>4 -> Printf.fprintf stderr "Couldn't find meaning of %s in System\n" name; (lB, lP, lL)
        | _ -> (lB, lP, lL)
        end
    | PCData (s) -> output_string stderr s; (lB, lP, lL)
;;

let rec blocklist_of_tree ml = function (* ml = (lB, lP, lL) *)
  | Element (name,alist,clist) as t->
    begin match name with
      | "xml" | "ModelInformation" | "Model" | "P" -> List.fold_left blocklist_of_tree ml clist
      | "BlockParameterDefaults" -> blockparams_of_simulink ml t
      | "System" -> blocklist_of_simulink ml t
      | _ when !verbose>4 -> Printf.fprintf stderr "I don't know how to process %s : ignored\n" name; ml
      | _ -> ml
    end
    | PCData (s) -> ml

(* *)
let pushDefaults ((lB, lP, lL):simulinkModel) =
  if !verbose>5 then Printf.fprintf stderr "Pushing default parameters...\n";
  let blockDef bloc =
    try let listDefaults = List.assoc bloc.blocktype lP in
    let pred (paramName,_) =
      List.for_all (function (x,_) when x=paramName ->
                                        false | _ -> true) bloc.values in
    { blocktype = bloc.blocktype;
      blockid = bloc.blockid;
      name = bloc.name;
      values = ((List.filter pred listDefaults)@bloc.values)
     }
     with Not_found -> begin
       Printf.fprintf stderr "[WARNING:] No Simulink default parameters for block %s.\n" bloc.blocktype;
     let pred (paramName,_) =
       List.for_all (function (x,_) when x=paramName ->
                     false | _ -> true) bloc.values in
     { blocktype = bloc.blocktype;
       blockid = bloc.blockid;
       name = bloc.name;
       values = ((List.filter pred (completeDefaults [] bloc.blocktype))@bloc.values)
     }
     end
  in
  ((List.map blockDef lB), lL);;

let position_parse_regexp = Str.regexp "\\[\\(-?[0-9]+\\)[ ,]*\\(-?[0-9]+\\)[ ,]*\\(-?[0-9]+\\)[ ,]*\\(-?[0-9]+\\)\\]"

let getchar i = char_of_int ((int_of_char('A') + i - 1))
let printLaTeX f ((lB,lL):simulinkPModel) =
  let printBlock bloc =
    let btype = bloc.blocktype and bid = bloc.blockid and bval = bloc.values in
      let position = List.assoc "Position" bval in
        assert (Str.string_match position_parse_regexp position 0);
        let (x,y,w,h) = (int_of_string@@ Str.matched_group 1 position,
                         int_of_string@@ Str.matched_group 2 position,
                         int_of_string@@ Str.matched_group 3 position,
                         int_of_string@@ Str.matched_group 4 position) in
          Printf.fprintf f "\\node[sk%s,draw] at (%imm,%imm) (b%i) {};\n" btype x y bid
  and printLink link =
    Printf.fprintf f "\\path[->] (b%i.out%c) edge (b%i.in%c);\n"
                   link.fromblock (getchar link.fromport)
                   link.toblock (getchar link.toport)
  in
  List.iter printBlock lB;
  List.iter printLink lL;;

(* Récupération de la configuration de la simulation *)
let rec getSettings lS = function
  | Element ("P",["Name",x],[PCData(l)]) -> (x,l)::lS
  | Element (name,alist,clist) ->
  begin match name with
    | "P" -> lS
    | _ -> List.fold_left getSettings lS clist
  end
  | PCData (s) -> lS
;;

let rec dispSettings lS = function
  [] -> lS
  | (x,l)::q -> print_string x; print_string " = "; print_string l; print_newline(); dispSettings ((x,l)::lS) q;;

type extFloat = Infty | Auto | Finite of float

let extfloat_of_string = function
  | "auto" -> Auto
  | "inf" -> Infty
  | value -> Finite (float_of_string value);;
let extfloat_of_sampletime = function
  | "-1" -> Auto
  | value -> Finite (float_of_string value);;

let computeLatencies lS ((lB, lL):simulinkPModel) =
  if !verbose>5 then Printf.fprintf stderr "Computing exact latencies for blocks…\n";
  let startTime = extfloat_of_string (List.assoc "StartTime" lS) and stopTime = extfloat_of_string (List.assoc "StopTime" lS) and fixedStep = extfloat_of_string (List.assoc "FixedStep" lS) in
    let baseStep = match fixedStep with
    | Auto -> begin match stopTime with
      | Auto -> failwith "stopTime = auto : pas de sens"
      | Infty -> failwith "stopTime = infty et fixedStep = auto ? Ah, le cas est arrivé. Il faut une valeur par défaut."
      | Finite stop -> begin match startTime with
        | Finite start -> (stop -. start) /. 50.
        | _ -> failwith "startTime a une tête étrange." end; end;
    | Infty -> failwith "fixedStep = infty : cela n'a pas de sens"
    | Finite step -> step
    in
      let rec aux = function
        [] -> []
        | b::q when b.blocktype="TransportDelay" ->
           { blocktype = b.blocktype; blockid = b.blockid; name = b.name; values = ("LATENCY", List.assoc "DelayTime" b.values)::(b.values) }::(aux q);
        | b::q when b.blocktype="UnitDelay" -> let sampleTime = extfloat_of_sampletime (List.assoc "SampleTime" b.values) in
            let realLatency = match sampleTime with
            | Auto -> baseStep
            | Finite latency -> latency
            | _ -> failwith "Error in realLatency"
            in { blocktype = b.blocktype;
                 blockid = b.blockid;
                 name = b.name;
                 values = ("LATENCY", string_of_float realLatency)::(b.values) }::(aux q)
        | b::q when b.blocktype="Delay" -> let sampleTime = extfloat_of_sampletime (List.assoc "SampleTime" b.values) in
            let realLatency = (float_of_string (List.assoc "DelayLength" b.values)) *. match sampleTime with
            | Auto -> baseStep
            | Finite latency -> latency
            | _ -> failwith "Error in realLatency"
            in { blocktype = b.blocktype;
                 blockid = b.blockid;
                 name = b.name;
                 values = ("LATENCY", string_of_float realLatency)::(b.values) }::(aux q)
        | b::q -> (b)::(aux q)
      in (completeOutputs (aux lB), lL);;
