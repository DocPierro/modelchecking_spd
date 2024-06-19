open MarkovChains

type miniedge = {
    target : int;
    prob : float;
  }

type edge = {
    label : string;
    distr : miniedge list;
  }

type t = {
    description : string;
    sccs : int array;
    labels : string list array;
    formulas : (string*Type.full_expr) list array;
    states : string array;
    transitions : edge list array;
    init : int;
  }

(* Constructeur pour t *)
let createMiniedge tar p = { target = tar; prob = p   }
let createEdge l d = {label = l; distr = d }


(** Partie traitement de t en chaine de markov **)

let isMarkovChain t =
  Array.fold_left (fun ret e -> ( List.length e == 1 ) && ret ) true t.transitions


let rec findNomEtat ret = function
  | [] -> ret
  | h::t -> findNomEtat ( ret@[h.nom]) t ;;


let rec addListMiniedge ret matref = function
  |[]-> ret
  |h::t -> addListMiniedge ((createMiniedge h.direct  h.proba )::ret) matref t
;;


let rec findTrans ret matref = function
  |[] -> ret
  |h::t -> findTrans
             (ret @ [[(createEdge " " (addListMiniedge [] matref h.arcs) ) ] ] )
             matref t
;;

let t_of_CM cm =
  let nameEtat = Array.of_list (findNomEtat [] cm)
  and trans = Array.of_list (findTrans [] cm cm) in
  let n = Array.length nameEtat in
  {
    description = "";
    sccs = Array.make n 0;
    labels = Array.make n [];
    formulas = Array.make n [];
    states = nameEtat  ;
    transitions =  trans ;
    init = 0
  }
;;


let cm_of_t t =
  let lCM = Array.of_list (snd (
                               Array.fold_right (fun  e (i,l)  ->
                                   (i+1, (constEtat  e [] ~labels:(t.labels.(i)) )  :: l ))
                                                t.states (0, []))
                          ) in (*créer tt les états *)
  let n = Array.length t.states -1 in
  for i= 0 to n do
    let led = t.transitions.(i) in
    let e = List.fold_left (fun ret e -> e) (createEdge " " []) led in
    let distr = Array.of_list e.distr in
    for j = 0 to (List.length e.distr)-1  do
      let target = distr.(j).target and prob = distr.(j).prob in
      let indice = (findEbyName t.states.(target) 0 (Array.to_list lCM)) in
      addArc lCM.(i)  indice prob
    done;
  done ;
  lCM
;;


(*fonction qui trouve l'état initial *)
let findInit arg =
  Array.fold_left (fun (b, i) e ->
      if b then (b, i)
      else (
        let bb =  List.exists (fun  a ->  (a = "initial" ) )  e  in
        (bb, i+1)
      )
    ) (false, -1) arg.labels

(*Complexité en 2 N²  *)
let taupologiqueOrderOfT  t  init =
  let n = Array.length t.states in
  let lCM = Array.make n (constEtat "" []) in
  let lnum = Array.make n 0 in
  let incr= ref 0 and  pos= ref 1 and   i = ref init in
  while !incr < n do
    let ii = !incr in
    lnum.(ii)<- !i;
    let led = t.transitions.(!i) in
    let e = List.fold_left (fun ret e -> e) (createEdge " " []) led in
    let distr =e.distr in
    List.iter (
        fun e ->
        if Array.exists   (fun el -> el = e.target ) lnum then ()
        else(
          lnum.(!pos)<- e.target ;
          pos := 1 + !pos;
        )
      ) distr;
    i:= ii +1;
    incr := 1+ !incr;
  done;
  for ii = 0 to n-1 do
    lCM.(ii)<- constEtat (t.states.(lnum.(ii))) [];
    let e = List.fold_left (fun ret e -> e) (createEdge " " []) t.transitions.(lnum.(ii)) in
    let distr = Array.of_list e.distr in
    for j = 0 to (Array.length distr)-1  do
      let (_, target) = Array.fold_left (fun (b ,i ) e ->
                            if e = distr.(j).target then
                              (true, i)
                            else if b then
                              (b,i)
                            else
                              (b, i+1) ) (false, 0) lnum
      and prob = distr.(j).prob in
      addArc lCM.(ii)  target prob
    done;
  done;
  lCM
;;




let printT t =
  Array.iter (fun el->    List.iter (
                              fun e -> Printf.printf "sate label : %s \n " e.label ;
                                       List.iter (fun ee ->
                                           Printf.printf "target : %d  prob:%f \n " ee.target ee.prob
                                         ) e.distr
                            ) el
             ) t.transitions;
  Array.iter (fun e -> Printf.printf "sates : %s \n " e ) t.states


let getElementFormula = function
  | Type.IntExpr i -> (match i with
                       | Type.Int   (ii) -> ii
                       | _ ->  0
                      )
  | _ ->  0
(*
  | Float(i) -> [i]
  | Bool(i) ->[i]
 *)

let  getIndiceElementTabInt e tab  =
  let ret = ref 0 in
  for i= 0 to (Array.length tab)-1 do
    if tab.(i) = e      then ret := i
  done;
  (!ret)
  
let rec getIndiceElementList ret e   = function 
	|[]-> ret
	|h :: t -> if h = e then ret else getIndiceElementList (ret+1) e t

module MyMapString =  Map.Make(struct type t = string  let compare = compare end)

(* Fussion des etats avec les mêmes nom de labels *)
let rec simplifieLabel cm2 t =
  let lab = t.labels in
  let n= Array.length cm2 in
  let map = ref MyMapString.empty in
  (* construction d'une map avec pour chaque label la liste des etats associés *)
  for i= n-1 downto 0 do
    let l  = lab.(i) in
    List.iter (fun la -> if (MyMapString.exists (fun k v ->  k = la ) (!map))
                         then  map := MyMapString.add la ( i :: MyMapString.find la (!map)  ) (!map)
                         else  map := MyMapString.add la [i] (!map);
                         ()) l;
  done;
  let nb = ref 0 in

  (** Change ordre avec calcul de l'ordre avec la formule **)

  let tmpform= Array.fold_right (fun  e ret ->
                   List.fold_left (fun ret (s, f) ->
                       if String.equal s "order" then
                         (getElementFormula f)  :: ret
                       else ret
                     ) ret e ;
                 ) t.formulas [] in
  let tmporder = List.sort  compare (tmpform) in           
  let listorder = List.fold_right (fun e ret -> (getIndiceElementList 0 e tmpform ):: ret ) tmporder [] in
  let order = Array.of_list (listorder) in

  let cm = if Array.length order > 0 then (
             MyMapString.iter (fun k v  ->
                 let mini= ref n and maxi = ref 0 in
                 List.iter (fun e ->
                     mini:= min (getIndiceElementTabInt  e order) (!mini);
                     maxi:= max (getIndiceElementTabInt  e order) (!maxi);
                     ()
                   ) v;
                 let size = List.length v in
                 if (size - 1) <> (!maxi - !mini)  then
                   failwith "Erreur dans l'ordre donné dans la formule \" order\"  " ;
                 ()
               ) (!map);
             
             map := MyMapString.empty ;
             (* construction d'une map avec pour chaque label la liste des etats associés *)
             for i= n-1 downto 0 do
               let l  = lab.(i) in
               List.iter (fun la ->
                   if (MyMapString.exists (fun k v ->  k = la ) (!map))
                   then
                     map := MyMapString.add la ( (getIndiceElementTabInt  i order) :: MyMapString.find la (!map)  ) (!map)
                   else
                     map := MyMapString.add la [(getIndiceElementTabInt  i order)] (!map);
                   ()) l;
             done;
             (changeOrder cm2 order )
           ) else
             cm2 in

  (*  Calcule de la taille du nouveau tableau*)
  MyMapString.iter (fun k v  ->
      let size = List.length v in
      if size >1 then nb := size-1 + !nb ;
      ()
    ) (!map);
  let ret = Array.make (n - !nb ) (constEtat "" []) and ii = ref 0 in

  (* Construction des nouveaux etats*)
  for i=0 to  (n-1)do
    let (bo, l) = MyMapString.fold (fun k v (b, l) ->
                      let bb =  (List.exists (fun e ->  e = i)  v ) in
                      if bb then (bb , k) else (b  , l )
                    ) (!map) (false, "") in
    if bo then (
      let b = Array.exists (fun e ->String.equal e.nom  l ) ret in
      if  not b then (
        ret.( !ii)<- constEtat l   [] ;
        ii := 1 + !ii
      )
    )else (ret.(!ii)<- constEtat cm.(i).nom   [] ; ii := 1 + !ii  ) ;
  done;
  (* Construction des arcs*)
  for i=0 to  (n -1) do
    let (b, lb) = MyMapString.fold (fun k v (b, l) ->
                      let bb =  (List.exists (fun e ->  e = i)  v ) in
                      if bb then (bb , k) else (b  , l )
                    ) (!map) (false, "") in
    let x = if b then
              findEbyName lb 0 (Array.to_list ret)
            else
              findEbyName (cm.(i).nom) 0 (Array.to_list ret) in
    List.iter (fun  a  ->
        let (bo, l) = MyMapString.fold (fun k v (b, l) ->
                          let bb =      (List.exists (fun e ->  e = a.direct)  v ) in
                          if bb then (bb , k) else (b  , l )
                        ) (!map) (false, "") in
        if bo then (
          let j =  (findEbyName l 0 (Array.to_list ret) ) in
          addArc  ret.(x) j  (a.proba)
        )else (
          let j =  (findE cm.(a.direct) 0  (Array.to_list ret) )  in
          if (j <> -1 ) then addArc ret.(x) j (a.proba)
        )
      )  cm.(i).arcs
  done;

  Array.iter (fun e ->
      let arc = List.fold_left (fun ret a ->
                    let p = e.arcs
                            |> List.filter (fun aa -> aa.direct = a.direct)
                            |> List.fold_left (fun ret ee ->
                                   ret +. ee.proba
                                 ) 0.  in
                    if (List.exists (fun e ->  e.direct = a.direct)  ret ) then ret
                    else (
                      let pro =  if p > 1. then 1. else p in
                      (constArc a.direct pro ):: ret
                    )
                  ) []  e.arcs in
      e.arcs <- arc;

    ) ret  ;
  ret
;;

module MyMap =  Map.Make(struct type t = int let compare = compare end)
module MySet =  Set.Make(struct type t = int let compare = compare end)

let getPmoinsPplus t =
  let n = Array.length t.states -1 in
  let retplus = Array.make (n+1) (constEtat "" [] )
  and retmoin = Array.make (n+1) (constEtat "" []) in
  for i= 0 to n do
    let led = t.transitions.(i) in
    let targetSet = List.fold_left (fun set e ->
                        List.fold_left (fun set minied ->
                            MySet.add minied.target set ) set e.distr
                      ) MySet.empty  led in
    let mapret = List.fold_left (fun map e ->
                     let li = ref e.distr in
                     if MySet.cardinal targetSet <> List.length e.distr then
                       li := (MySet.fold (fun  target ret  ->
                                  if List.exists (fun e-> e.target == target  ) e.distr then ret
                                  else (createMiniedge target 0.)::ret
                                ) targetSet  []  )
                             @ !li ;
                     List.fold_left (fun map minied ->
                         if MyMap.exists (fun k _ -> k == minied.target  ) map then
                           let (pm, pp) = MyMap.find minied.target  map in
                           MyMap.add minied.target (min pm minied.prob , max pp minied.prob ) map
                         else   MyMap.add minied.target ( minied.prob , minied.prob ) map
                       ) map !li
                   ) MyMap.empty  led in
    let (etatmoin , etatplus) =
      MyMap.fold (fun k (pm, pp) (retmoin , retplus) ->
          addArc retmoin  k pm ;
          addArc retplus k pp ;
          ( retmoin, retplus)
        ) mapret
                 (constEtat t.states.(i) [] ~labels:(t.labels.(i)), constEtat t.states.(i) [] ~labels:(t.labels.(i)))
    in
    retplus.(i) <- etatplus;
    retmoin.(i) <- etatmoin;
  done;
  let pm = simplifieLabel retmoin t
  and pp = simplifieLabel retplus t in
  (pm, pp)

let getPmoinsPplusV2 t =
  let n = Array.length t.states -1 in
  let retplus = Array.make (n+1) (constEtat "" [] )
  and retmoin = Array.make (n+1) (constEtat "" []) in
  for i= 0 to n do
    let led = t.transitions.(i) in
    let size = float_of_int  (List.length led) in
    let mapret = List.fold_left (fun map e ->
                     let li = ref e.distr in
                     List.fold_left (fun map minied ->

                         if MyMap.exists (fun k _ -> k == minied.target  ) map then(
                           let (pm, pp) = MyMap.find minied.target  map in
                           MyMap.add minied.target (min pm (minied.prob /. size) , max pp minied.prob ) map
                         )else(
                           MyMap.add minied.target ( (minied.prob /. size) , minied.prob  ) map)
                       ) map !li
                   ) MyMap.empty  led in
    let (etatmoin , etatplus) =
      MyMap.fold (fun k (pm, pp) (retmoin , retplus) ->
          addArc retmoin  k pm ;
          addArc retplus k pp ;
          ( retmoin, retplus)
        ) mapret
                 (constEtat t.states.(i) [] ~labels:(t.labels.(i)), constEtat t.states.(i) [] ~labels:(t.labels.(i)))
    in
    retplus.(i) <- etatplus;
    retmoin.(i) <- etatmoin;
  done;
  let pm = simplifieLabel retmoin t     and pp = simplifieLabel retplus t in
  (pm, pp)




let rec affichagePDF ?ter:( x = -1) ?mat:( b = false)= function
  |[] ->
    ignore (Sys.command ("dot -Tpdf result/DEBUG.dot -o result/debug.pdf" ));
    ignore (Sys.command ("open result/debug.pdf"));
    ()
  |(s, cm) :: t ->
    print_dot ~ter:(x) (Printf.sprintf "result/%s.dot" s) cm;
    ignore (Sys.command (Printf.sprintf "dot -Tpdf result/%s.dot -o result/%s.pdf" s s ));
    ignore (Sys.command (Printf.sprintf "open result/%s.pdf" s));

    if b then (
      print_dot_mat (Printf.sprintf "result/tab%s.dot" s) cm;
      ignore (Sys.command (Printf.sprintf "dot -Tpdf result/tab%s.dot -o result/tab%s.pdf" s s ));
      ignore (Sys.command (Printf.sprintf "open result/tab%s.pdf" s));
    );
    affichagePDF ~ter:(x)  t


(** Appel de Gauss Seidel **)
let applyGaussSeidel mdp  nom input =
  Printf.printf "Sommet que l'on souhaite atteindre %s" nom ;
  if (isMarkovChain mdp) then Printf.printf "Cet MDP est une chaine de Markov" ; 
  let (pm, pp) = getPmoinsPplus   mdp in
	let ter =  findTer pm nom  in
  let psup = algoInverse4v2 pp pm and pinf = algo4v2 pp pm in

    (* Test si IMC *)
   (* Printf.printf "P- P+ IMC ? %b \n " (isIMC pm pp);
    Printf.printf "Pinf Psup IMC ? %b \n " (isIMC pinf psup);
    *)

  affichagePDF ~ter:(ter)  [("pm", pm); ("pp", pp); ("pinf", pinf);  ("psup", psup) ];
  Printf.printf "\nsup -> \n";
  List.iter (fun e -> Printf.printf "%s -> %f \n " psup.(e.direct).nom e.proba ) (gauss_Seidel psup ter);

  Printf.printf "\ninf -> \n";
  List.iter (fun e -> Printf.printf "%s -> %f \n " pinf.(e.direct).nom e.proba ) (gauss_Seidel pinf ter) ;
  ()
;;  
  
let testPmm mdp lab coups =
  let (pm, pp) = getPmoinsPplusV2   mdp  in
  let ter = findTer pm lab in
  let psup = algoInverse4v2 pp pm and pinf = algo4v2 pp pm in
  let rpsup = ref psup and rpinf= ref pinf in
  for i=1 to coups do
    rpinf:=  ( multMatCreuse pinf !rpinf) ;
    rpsup := ( multMatCreuse psup !rpsup) ;
  done;
  Printf.printf "V2 sup -> \n";
  List.iter (fun e -> Printf.printf "%s -> %f \n " psup.(e.direct).nom e.proba ) (vecteur_inverse !rpsup ter !rpsup);
  Printf.printf "V2 inf -> \n";
  List.iter (fun e -> Printf.printf "%s -> %f \n " pinf.(e.direct).nom e.proba ) (vecteur_inverse !rpinf ter !rpinf) ;
  ()

let applyVecInv mdp lab coups =
  testPmm mdp lab coups ;
  let (pm, pp) = getPmoinsPplus   mdp  in
  let ter = findTer pm lab in
  Printf.printf "%s \n" pm.(ter).nom ;
  let psup = algoInverse4v2 pp pm and pinf = algo4v2 pp pm in
  let rpsup = ref psup and rpinf= ref pinf in
  for i=1 to coups do
    rpinf:=  ( multMatCreuse pinf !rpinf) ;
    rpsup := ( multMatCreuse psup !rpsup) ;
  done;
  Printf.printf "sup -> \n";
  List.iter (fun e -> Printf.printf "%s -> %f \n " psup.(e.direct).nom e.proba ) (vecteur_inverse !rpsup ter !rpsup);
  Printf.printf "inf -> \n";
  List.iter (fun e -> Printf.printf "%s -> %f \n " pinf.(e.direct).nom e.proba ) (vecteur_inverse !rpinf ter !rpinf) ;
  ()


let printAppli ?arg:(a=[|0|])  mdp nb  =
  match nb with
  | 1 -> let (pm, pp) = getPmoinsPplus   mdp in
        let psup = algoInverse4v2 pp pm and pinf = algo4v2 pp pm in
        affichagePDF  [("pm", pm); ("pp", pp); ("pinf", pinf);  ("psup", psup) ];
  | 3 -> let (pm, pp) = getPmoinsPplus mdp in
         let psup = algoInverse4v2 pp pm
         and pinf = algo4v2 pp pm in
         Printf.printf "Borne superieur monotone \n ";
         Array.iter (fun e -> printEtat e pm) psup ;
         Printf.printf "Borne inferieur monotone \n ";
         Array.iter (fun e -> printEtat e pp) pinf
  |4 -> let (pm, pp) = getPmoinsPplus mdp in
        Printf.printf "P- \n ";
        Array.iter (fun e -> printEtat e pm) pm ;
        print_dot "pm.dot" pm;
        Printf.printf "P+ \n ";
        Array.iter (fun e -> printEtat e pp) pp;
        print_dot "pp.dot" pp;
        Printf.printf "diff \n ";
        Array.iter (fun e -> printEtat e pp) (difference  pp pm );
        Printf.printf "Fin Diff \n ";
  |_-> ()

let computeBound m = if false then () else(

	(*Partie teste *)
	
	()
)

let print_dot_miniedge o g i do_print_trans =
  ignore @@
    List.fold_left (fun j e ->
        let st,lab = if List.length e.distr > 1 then (
                       let miniedgelabel = (g.states.(i)^"L"^(string_of_int j)^e.label) in
                       Printf.fprintf o "\t\"%s\" [shape=point];\n" miniedgelabel;
                       if do_print_trans then Printf.fprintf o "\t\"%s\" -> \"%s\" [ shape=point,label = \"%s\" ];\n"
                                                             g.states.(i) miniedgelabel e.label;
                       miniedgelabel,"_prob_")
                     else g.states.(i),e.label in
        List.iter (fun me ->
            if do_print_trans then
              Printf.fprintf o "\t\"%s\" -> \"%s\" [ label = \"%s\" ];\n"
                             st g.states.(me.target)
                             (if lab <> "_prob_" then lab else Type.print_pretty_float me.prob) ) e.distr;
        j+1) 0
                   g.transitions.(i)

let print_labels_formulas g f i =
  let first = List.fold_left (fun b s2 ->
                  if b then (output_string f s2; false) else (Printf.fprintf f ",%s" s2; false)) true g.labels.(i) in
  ignore @@ List.fold_left (fun b (s,v) ->
                if b then (Printf.fprintf f "%s(%a)" s Type.print_full_expr v; false) else (Printf.fprintf f ",%s(%a)" s Type.print_full_expr v; false)) first g.formulas.(i)


let print_dot f g =
  let open Printf in
  let n = Array.length g.states in
  let sccLists = Hashtbl.create 8 in
  for i=0 to n-1 do
    let scc = g.sccs.(i) in
    let ol = if Hashtbl.mem sccLists scc then Hashtbl.find sccLists scc else [] in
    Hashtbl.replace sccLists scc (i::ol)
  done;

  let o = open_out f in
  output_string o "digraph G {\n";
  output_string o ("\tlabel=\"state=("^g.description);
  (*Array.iteri (fun i s -> if i<>0 then output_string o ","; output_string o s;) g.states; *)
  output_string o ")\"\n\tlabelloc=top;\n\tlabeljust=left;\n";
  sccLists
  |> Hashtbl.iter (fun scc sccList ->
         fprintf o "\tsubgraph cluster_%i {\n" scc;
         fprintf o "\tstyle=filled;\n\tcolor=lightgrey;\n";
         fprintf o "\tlabel=\"scc%i\"\n" scc;
         sccList
         |> List.iter (fun i ->
                let shape = match List.length g.transitions.(i) with
                    0 -> "doublecircle"
                  | 1 -> "ellipse"
                  | _ -> "box" in
                fprintf o "\t\"%s\" [xlabel = \"%a\",shape=%s];\n"
                        g.states.(i) (print_labels_formulas g) i shape;
                if List.length g.transitions.(i) > 1 then
                  print_dot_miniedge o g i false
              );
         fprintf o "\t}\n");
  for i =0 to n-1 do
    if List.length g.transitions.(i) > 1 then
      print_dot_miniedge o g i true
    else match g.transitions.(i) with
           [e] -> List.iter (fun me ->
                      fprintf o "\t\"%s\" -> \"%s\" [ label = \"%s\" ];\n"
                              g.states.(i) g.states.(me.target) ( Type.print_pretty_float me.prob) ) e.distr

         | _ -> ();
  done;
  output_string o "}\n";
  close_out o


let compact_mdp mdp =
  let n2 = Array.fold_left (fun i tr -> if List.length tr != 1 then i+1 else i) 0 mdp.transitions in
(*  let map = Array.make (Array.length mdp.transitions) (-1) in
  let states = Array.make n2 "" in
  let labels = Array.make n2 [] in
  let formulas = Array.make n2 [] in
 *)

  Printf.printf "real state:%i\n" n2
