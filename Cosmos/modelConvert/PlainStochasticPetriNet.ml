open Type
open PetriNet

type distr =
  Exp of float expr'
| Imm

let sample_distr ev = function
  | Imm -> 0.0
  | Exp(x) -> -. log(Random.float 1.0)/. (ev x)

type declarationGSPN = {
    mutable printer:out_channel->unit->unit;
    mutable intconst:(string*int) list;
    mutable floatconst:(string*float) list;
  }
  
type gspn =
  (int, distr, int , declarationGSPN) Net.t

open Net

let of_spn (spn:StochasticPetriNet.spt) =
  let net = create () in
  net.def <- spn.def |>>> (fun spndef -> {
                 intconst= List.map (fun (s,x) -> s, (
                                       match x with
                                         Some v -> eval_or_die v
                                        |None -> raise FailToEvaluate))
                                    spndef.StochasticPetriNet.intconst;
                 floatconst= List.map (fun (s,x) -> s, (
                                       match x with
                                         Some v -> eval_or_die v
                                        |None -> raise FailToEvaluate))
                                    spndef.StochasticPetriNet.floatconst;
                 printer= fun _ () -> ();
               });
    
  Data.iter (fun (n,(x,_)) ->
      let inmark = StochasticPetriNet.eval_or_die_spt spn x in
      ignore @@ add_place net n inmark
    ) spn.place;
  Data.iter (fun (n,(x,_,_)) ->
      match x with
      | StochasticPetriNet.Player1
      | StochasticPetriNet.Det (Float 0.0)
      | StochasticPetriNet.Imm ->
         ignore @@ add_transition net n Imm
      | StochasticPetriNet.Exp x ->
         let y = StochasticPetriNet.eval_spt spn x in
         ignore @@ add_transition net n (Exp y)
      | d -> StochPTPrinter.printH_distr stderr d; 
         failwith "Fail to reduce to Plain GSPN"
    ) spn.transition;
  Data.iter (fun ((),(v,p,t)) ->
      let v2 = StochasticPetriNet.eval_or_die_spt spn v in
      let pn,_ = Data.acca spn.place p in
      let tn,_ = Data.acca spn.transition t in
      add_inArcS net pn tn v2;  
    ) spn.inArc;
  Data.iter (fun ((),(v,t,p)) ->
      let v2 = StochasticPetriNet.eval_or_die_spt spn v in
      let pn,_ = Data.acca spn.place p in
      let tn,_ = Data.acca spn.transition t in
      add_outArcS net tn pn v2;  
    ) spn.outArc;
  Data.iter (fun ((),(v,p,t)) ->
      let v2 = StochasticPetriNet.eval_or_die_spt spn v in
      let pn,_ = Data.acca spn.place p in
      let tn,_ = Data.acca spn.transition t in
      add_inhibArcS net pn tn v2;  
    ) spn.inhibArc;
  
  net

let to_spn net =
  let spn = (create ():StochasticPetriNet.spt) in
  Data.iter (fun (n,x) ->
      ignore @@ add_place spn n (Int x,None)
    ) net.place;
  Data.iter (fun (n,x) ->
      match x with
        Imm -> ignore @@ add_transition spn n (StochasticPetriNet.Imm,Float 1.0,Float 1.0)
      | Exp x -> ignore @@ add_transition spn n (StochasticPetriNet.Exp (x),Float 1.0,Float 1.0)
    ) net.transition;
  Data.iter (fun ((),(v,p,t)) ->
      let pn,_ = Data.acca net.place p in
      let tn,_ = Data.acca net.transition t in
      add_inArcS spn pn tn (Int v);  
    ) net.inArc;
  Data.iter (fun ((),(v,t,p)) ->
      let pn,_ = Data.acca net.place p in
      let tn,_ = Data.acca net.transition t in
      add_outArcS spn tn pn (Int v);  
    ) net.outArc;
  spn

  
module GspnOp = struct
  type placetype = int
  type valuationtype = int
  type transitiontype = distr
  type declarationtype = declarationGSPN
  type result = bool
                  
  let eval net (m:placetype array) e =
    let intrpl = fun s -> try
                         let index = Data.index net.place s in
		         Some (Int (m.(Data.unsafe_rev index)))
                       with _ -> (
                         net.def
                         |>> (fun s -> Some (s.intconst))
                         |>> (fun s2 -> try Some (Int (List.assoc s s2)) with |_ -> None)
                       ) in
    let floatrpl = fun s -> net.def
                            |>> (fun s -> Some (s.floatconst))
                            |>> (fun s2 -> try Some (Float (List.assoc s s2)) with |_ -> None) in
    Type.eval ~fname:floatrpl ~iname:intrpl e
                                         
  let eval_or_die net m e = 
    Type.eval_or_die (eval net m e)
         
  let eval_marking net m p = p

  let print_distr net m f d = match d with
      Exp r -> Printf.fprintf f "%a" (printH_expr) r
    | Imm -> Printf.fprintf f "I" 
                               
  let eval_marking_bool net m p =
    eval net m p

  let eval_marking_full_expr net m e = match e with
      IntExpr i -> IntExpr (eval net m i)
    | FloatExpr f -> FloatExpr (eval net m f)
    | BoolExpr b -> BoolExpr (eval net m b)
                               
  let print_marking f p =
    Type.printH_expr f (Int p)
		     
  let compare net m pt v = pt -v
                
  let compare_marking p1 p2 = p1 -p2
       
  let add net m pt v = pt+v

  let minus net m pt v = pt -v

  let sample net m t =
    let eval x = eval_or_die net m x in
    
    let (d,p,w) = snd @@ Data.acca net.Net.transition t in
    let time = sample_distr eval d in
    let weight = sample_distr eval (Exp (w)) in
    match p with
      Type.Float(f) -> time,f,weight,t
     |_ -> failwith "Cannot evaluate"
	            
  let choose net m en =
      List.hd en
              
  let get_prob net m t =
    match t with
      (Exp f) -> eval_or_die net m f 
    | _ -> -1.0
  let get_int net m p = p
                  
  let finalResult net m = None

  end

                    
module SemanticGSPN = SemanticPT.NetWithMarking(GspnOp)
