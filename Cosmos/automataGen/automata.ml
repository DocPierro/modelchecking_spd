open Type

type haslformulaAlg = Last of floatExpr
and haslformulaTop = Avg of haslformulaAlg | Prob

let rec print_haslTop = function
  | Prob -> print_string "PROB";
  | Avg(halg) -> print_string "AVG(";
    print_haslAlg halg;
    print_string ")"

and print_haslAlg = function
  | Last(x) -> print_string "Last(";
    print_float_expr x;
    print_string ")"
and print_HASL (n,f) =
  print_string n;
  print_string "=";
  print_haslTop f;
  print_endline ";"


type labels =
  All | AllMinus of string list
| Labels of string list

let rec conjonct_labels l1 l2 = match l1,l2 with
  All,x -> x
| x,All -> x
| AllMinus(ll1),AllMinus(ll2) -> AllMinus(List.filter (fun x -> List.exists (fun y -> x=y) ll2) ll1)
| AllMinus(ll1),Labels(ll2) -> Labels(List.filter (fun x -> not (List.exists (fun y -> x=y) ll1)) ll2)
| Labels(ll1),AllMinus(ll2) -> Labels(List.filter (fun x -> not (List.exists (fun y -> x=y) ll2)) ll1)
| Labels(ll1),Labels(ll2) -> Labels(List.filter (fun x -> (List.exists (fun y -> x=y) ll2)) ll1)



let print_labels = function
  | All -> print_string "ALL";
  | AllMinus l -> print_string "ALL\\{";
    print_list (print_string) (fun () -> print_string ",") l;
    print_string "}";
  | Labels l -> print_string "{";
    print_list (print_string) (fun () -> print_string ",") l;
    print_string "}";

type transType = 
  Synch of labels * stateFormula
| Autonomous of ((int*cmp*floatExpr) list)

type automata = {
  haslForm: (string*haslformulaTop) list;
  nbLoc: int;
  nbContVar: int;
  nbDiscVar:int;
  invariant : (int*stateFormula) list;
  flows : (int*((int*floatExpr) list)) list;
  init : int;
  final : int list;
  trans : (int *transType*(((int*floatExpr) list))*(((int*floatExpr) list) )*int) list;
}



let insertLH (h,c) l s =
  if (Hashtbl.mem h s) then l
  else (
    Hashtbl.add h s !c;
    incr c;
    s::l
  )

let hashfind (h,_) e =
  Hashtbl.find h e


let rec decalvar n1 n2 = function
  | ContinuousVar(x) -> ContinuousVar(n1+x)
  | DiscreteVar(x) -> DiscreteVar(n2+x)
  | MultF(f1,f2) -> MultF(decalvar n1 n2 f1,decalvar n1 n2 f2)
  | PlusF(f1,f2) -> PlusF(decalvar n1 n2 f1,decalvar n1 n2 f2)
  | MinusF(f1,f2) -> MinusF(decalvar n1 n2 f1,decalvar n1 n2 f2)
  | DivF(f1,f2) -> DivF(decalvar n1 n2 f1,decalvar n1 n2 f2)
  | x -> x

let dvup n1 n2 n = List.map (fun (x,y)-> (x+n,decalvar n1 n2 y))
let dvguard n1 n2 n = List.map (fun (x,y,z)-> (x+n,y,decalvar n1 n2 z))

let rec explore_and_synch h a1 a2 qt = function
  | [] -> qt
  | (s1,s2)::q -> 
    let l1 = List.filter (fun (i,_,_,_,_) -> i=s1) a1.trans
    and l2 = List.filter (fun (i,_,_,_,_) -> i=s2) a2.trans in
    let (lp,qt2) = List.fold_left (fun q2 t1 -> List.fold_left (fun q3 t2 -> sychtrans h a1 a2 q3 t1 t2) q2 l2) (q,qt) l1 in 
    let (lp2,qt3) = List.fold_left (fun q2 t -> autotrans1 h a1 a2 q2 t s2) (lp,qt2)  l1 in
    let (lp3,qt4) = List.fold_left (fun q2 t -> autotrans2 h a1 a2 q2 s1 t) (lp2,qt3) l2 in
explore_and_synch h a1 a2 qt4 lp3

and autotrans1 h a1 a2 (lp,lt) (s1,tt1,cu1,du1,sp1) s2 =
  match tt1 with 
      Autonomous(_)->
	let lp2 =  insertLH h lp (sp1,s2) in
	let t =  ((hashfind h (s1,s2)),
		  tt1 ,
		  cu1 ,
		  du1 ,
		  (hashfind h (sp1,s2))) in  
	(lp2,t::lt)
    | _ -> (lp,lt)

and autotrans2 h a1 a2 (lp,lt) s1 (s2,tt2,cu2,du2,sp2) =
  match tt2 with 
      Autonomous(guard2)->
	let lp2 =  insertLH h lp (s1,sp2) in
	let t =  ((hashfind h (s1,s2)),
		  Autonomous(dvguard a1.nbContVar a1.nbDiscVar a1.nbContVar guard2),
		  (dvup a1.nbContVar a1.nbDiscVar a1.nbContVar cu2),
		  (dvup a1.nbContVar a1.nbDiscVar a1.nbDiscVar du2),
		  (hashfind h (s1,sp2))) in  
	(lp2,t::lt)
    | _ -> (lp,lt)


and sychtrans h a1 a2 (lp,lt) (s1,tt1,cu1,du1,sp1) (s2,tt2,cu2,du2,sp2) =
  match tt1,tt2 with
  | Synch(label1,guard1),Synch(label2,guard2) ->
    let labels = conjonct_labels label1 label2 in
    if labels <> Labels([]) then 
      let lp2 = insertLH h lp (sp1,sp2) in
      let t =  ((hashfind h (s1,s2)),
		Synch(labels,And(guard1,guard2)),
		cu1 @  (dvup a1.nbContVar a1.nbDiscVar a1.nbContVar cu2),
		du1 @  (dvup a1.nbContVar a1.nbDiscVar a1.nbDiscVar du2),
		(hashfind h (sp1,sp2))) in
      (lp2,t::lt)
    else (lp,lt)
  | _ -> (lp,lt) 

let and_automaton a1 a2 =
  let hash_state = (Hashtbl.create a1.nbLoc,ref 0) in 
  let trans = explore_and_synch hash_state a1 a2 [] [a1.init,a2.init] in
  {
    haslForm = a1.haslForm @ a2.haslForm;
    nbLoc = Hashtbl.length (fst hash_state);
    nbContVar = a1.nbContVar + a2.nbContVar ;
    nbDiscVar = a1.nbDiscVar + a2.nbDiscVar ;
    invariant = a1.invariant @ (List.map (fun (x,y) -> (a1.nbLoc+x,y)) a2.invariant);
    flows = [];
    init =0;
    final=[1];
    trans=trans;
  }
					   



let rec automata_of_formula = function
  | Until(s1,s2) ->
    {
      haslForm = ["Probability",Prob];
      nbLoc = 2;
      nbContVar = 1;
      nbDiscVar = 0;
      invariant = [(0,And(s1,Not(s2)));(1,s2)];
      flows = [];
      init =0;
      final=[1];
      trans=[(0,Synch(All,True),[],[],0);
	     (0,Synch(All,True),[],[],1)];
    }
  | BoundedUntil(s1,(fexpr1,fexpr2),s2) when fexpr1 <> fexpr2 ->
    {
      haslForm = ["Probability",Prob];
      nbLoc = 4;
      nbContVar = 1;
      nbDiscVar = 0;
      invariant = [(0,And(s1,Not(s2)));(1,And(s1,Not(s2)));(2,s2)];
      flows = [(0,[(0,Float(1.0))]) ; (1,[(0,Float(1.0))])];
      init =0;
      final=[2];
      trans=[(0,Synch(All,True),[],[],0);
	     (0,Autonomous([(0,EQ,fexpr1)]),[],[],1);
	     (1,Synch(All,True),[],[],1);
	     (1,Synch(All,True),[],[],2);
	     (1,Autonomous([(0,EQ,fexpr2)]),[],[],3);
	    ];
    }
  | BoundedUntil(s1,(fexpr,_),s2) ->
    {
      haslForm = ["Probability",Prob];
      nbLoc = 3;
      nbContVar = 1;
      nbDiscVar = 0;
      invariant = [(0,s1);(1,s2);(2,Not(s2))];
      flows = [(0,[(0,Float(1.0))])];
      init =0;
      final=[1];
      trans=[(0,Synch(All,True),[],[],0);
	     (0,Autonomous([(0,EQ,fexpr)]),[],[],1);
	     (0,Autonomous([(0,EQ,fexpr)]),[],[],2);
	    ];
    }
  | Next(sf) ->
     {
       haslForm = ["Probability",Prob];
      nbLoc = 2;
      nbContVar = 1;
      nbDiscVar = 0;
      invariant = [(1,sf)];
      flows = [];
      init =0;
      final=[1];
      trans=[(0,Synch(All,True),[],[],1) ];
    }
  | AndF(f1,f2) -> and_automaton (automata_of_formula f1) (automata_of_formula f2)
    


  | _ -> failwith "Not yet implemented"

  
let rec flowlist = function
  | -1 -> []
  | i -> (i,[(0,Float(1.0))]) :: flowlist (i-1)

let rec savelist i = function
  | [] -> []
  | t::q -> (i,CastInt(IntName(t))) :: (savelist (i+1) q)

let rec translist pllist timeint j = function
  | -1 -> []
  | i -> (0, Autonomous([0,EQ,Float((float i) *. (1.0+.timeint))]),[(0,PlusF(ContinuousVar(0),Float(1.0)))],(savelist (i*List.length pllist) pllist),j)
    :: (translist pllist timeint 0 (i-1))

let rec hasllist l time i = function
  | [] -> l
  | t::q -> ((t^"_"^(string_of_int (i+1))),Avg(Last(DiscreteVar(i)))) ::
    (hasllist l time (i+1) q)

let rec hasllistTop l stept pllist = function
  | -1 -> l
  | i ->  let time = string_of_float (stept*.(float (i+1))) in
	  let callrec = hasllist l time ((i)*List.length pllist) pllist  in
	  hasllistTop callrec stept pllist (i-1) 
	  


let automata_of_pltrlist pllist trlist trans loop stepn =
    let invT = loop /. (float stepn) in
    {
      haslForm = hasllistTop [] invT pllist stepn;
      nbLoc = 2;
      nbContVar = 1;
      nbDiscVar = (1+stepn)*((List.length pllist)+(List.length trlist));
      invariant = [];
      flows = flowlist stepn;
      init =0;
      final=[1];
      trans=  (0,Synch(All,True),[],[],0) ::(translist pllist invT 1 stepn);
    }
