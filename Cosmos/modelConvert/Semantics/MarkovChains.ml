
type arc = {
    direct: int ;
    proba: float;
  }
             
type etat = {
         label : string list;
         nom: string;
         mutable arcs: arc list;(* permet de modifier la liste des arcs  *)
       }

type t = etat array
type imc = t*t

(* Constructeurs  *)
let constEtat ?labels:( lab = [] ) nom fleches   = { nom = nom; arcs = fleches; label = lab };;
let constArc dir prob = { direct = dir; proba = prob};;

(* Ajouter un arc *)
let addArc cm i prob= cm.arcs <- ((constArc (i) (prob))::cm.arcs) ;;
(* Getter proba *)
let getProba elementTest ensemble  =
  ensemble
  |> List.filter (fun e -> e.direct = elementTest)
  |> List.fold_left (fun x y -> x +. y.proba) 0.

(* Fonctions d'Affichage *)
let rec printArc cm = function          (* affichage d'une liste d'arc*)
  |[]-> ()
  |h :: t -> Printf.printf "  \t direction %s  -> %f\n"  cm.(h.direct).nom h.proba;
             printArc cm t

let printEtat e t =                             (* affichage d'un etat*)
  Printf.printf "nom : %s \n  " e.nom;
  printArc t e.arcs

let rec printColonne cm = function      (* affichage debugage de la multiplication avec vecteur *)
  |[]-> ()
  |(l, h)::t ->
    Printf.printf " Colonne : %s nom : %s  proba: %f \n " l cm.(h.direct).nom h.proba;
    printColonne cm t

(* Print dot *)
let print_dot ?ter:(x =  -1) f cm =
  let open Printf in
  let n = Array.length cm in
  let o = open_out f in
  output_string o "digraph G {\n";

  for i= 0 to n-1 do
    let label = List.fold_left (fun s1 s2 -> if s1="" then s2 else s1^","^s2) "" cm.(i).label in
    let color = if   i=x then " crimson " else "black" in
    let na = List.fold_left (fun ret e -> if e.proba <>0. &&  e.direct <> i then ret+1 else ret ) 0 cm.(i).arcs in
    let abs = if na = 0 then " 2 " else  "1" in
    fprintf o "\"%s\" [ color = %s , peripheries = %s , xlabel = \"%s %d\"] ; " cm.(i).nom color abs label i;
    List.iter (fun a ->
        if a.proba <> 0. then fprintf o "\"%s\" -> \"%s\" [ label = \"%s\" ]; " cm.(i).nom cm.(a.direct).nom
                                      (Type.print_pretty_float a.proba);
      ) cm.(i).arcs;
  done;
  output_string o "}\n";
  close_out o


let print_dot_mat f cm =
  let open Printf in
  let n = Array.length cm in
  let o = open_out f in
  let s = "c [shape=none width=0 height=0 margin=0 label=<<TABLE CELLBORDER=\"1\" BORDER=\"1\" CELLSPACING=\"0\">\n" in
  output_string o " digraph matrice {  concentrate=true; node [shape=record];\n";
  output_string o s ;
  for i= 0 to n-1 do
    fprintf o "<TR>";
    for j=0 to n-1 do
      fprintf o "<TD> %s </TD>" (Type.print_pretty_float (getProba j cm.(i).arcs));
    done;
    fprintf o "</TR>\n ";
  done;
  output_string o "</TABLE>>]\n";
  output_string o "}\n";
  close_out o

(* Fonction de recherches *)

let rec findE e i = function            (* Premiere occurence renvoie la position de celui ci*)
  |[]-> -1
  |h :: t -> if String.equal e.nom h.nom then   i       else            findE e (i+1) t
;;

let rec findEbyName e i = function      (* Premiere élément du même nom renvoie la position de celui ci*)
  |[]-> -1
  |h :: t -> if String.equal e h.nom then       i       else            findEbyName e (i+1) t
;;

let rec findA e i = function            (* Fonction de recherche pour les arc*)
  |[]-> -1
  |h :: t -> if e =  h.direct then      i       else    findA e (i+1) t
;;

let findTer cm lab=   
	let rter = ref 0 in
  for i=0 to (Array.length cm)-1 do
    if  (String.equal cm.(i).nom lab)  then rter := i ;
  done;
  !rter

(** PARTIE Transformation en matrice complete  *)
let rec arcToTab ret  lcm  =function
  |[]-> ret
  |h::t->
    Array.set ret (h.direct) h.proba;
    arcToTab ret lcm t
;;

let rec transMatriceC i lcm ret =
  if i == -1 then  ret
  else let cm= List.nth lcm i and size = List.length lcm in
       transMatriceC (i-1) lcm ( (Array.to_list (arcToTab (Array.make size 0.) lcm cm.arcs))@ ret )
;;

let printMatriceC tab nbEtat=
  let size = (List.length tab)-1 in
  for i = 0 to size do
    if i != 0 && i mod nbEtat == 0 then Printf.printf "\n";
    Printf.printf " %d: %f " i (List.nth tab i);
  done
;;

(* Calcul Matriciel *)
let multMatrice mat1 mat2 nbEtat =
  let size = nbEtat*nbEtat in
  let ret = Array.make size 0. in
  for indice = 0 to size-1 do
    let ligne = indice/ nbEtat and colonne = indice mod nbEtat in
    let acc = Array.make nbEtat 0. in
    for i= 0 to nbEtat-1 do
      let ci = (List.nth mat1 (ligne*nbEtat+i ) )
      and cj = (List.nth mat2 (colonne + i*nbEtat) ) in
      let n = ci *.cj  in
      Array.set acc i n ;
    done;
    let sum = Array.fold_left (fun x y -> x +. y ) 0. acc in
    Array.set ret indice sum;
  done;
  Array.to_list ret
;;

let rec produitMatriceN  n mat ret nbEtat =     (*Produit de matrice N fois *)
  if n == 0 then  ret   else produitMatriceN (n-1) mat  (multMatrice ret mat nbEtat) nbEtat
;;

let goXtoY e1 e2  mat n lcm =           (*Proba arrive de a -> b  en n coup *)
  let nbEtat=  (List.length lcm) in
  let matf= produitMatriceN n mat mat nbEtat
  and x = findE e1 0 lcm and y = findE e2 0 lcm in
  List.nth matf ( x * nbEtat + y )
;;

(** Fin Partie matrice complete *)
let findInTab element tab =
  let n = Array.length tab in
  let ret = ref (-1) in
  for i=0 to n-1 do
    if tab.(i) = element then ret := i
  done;
  !ret
;;

(** Patrie algorithme Prond Pstar et leurs inverses  **)
let algo3 pplus pmoins =                 (* Algorithme 3 IMC  *)
  let n = Array.length pplus in
  let ret = Array.make n (constEtat "" []) in
  for i=0 to n-1 do
    let pre= ref 0. and somplus = ref 0.
        and sommoins = ref ( List.fold_left (fun x y -> x +. y.proba) 0.  pmoins.(i).arcs )  in
    Array.set ret i (constEtat(pplus.(i).nom) []);
    for j=0 to n-1 do
      somplus := !somplus +. (getProba j pplus.(i).arcs )  ;
      sommoins := !sommoins -. ( getProba j pmoins.(i).arcs );
      let calcul = min  (!somplus) (1. -. !sommoins ) in
      if j == 0 then addArc ret.(i) j ( calcul )
      else  addArc ret.(i) j (  calcul -. !pre );
      pre := calcul;
    done;
  done;
  ret
;;


let algo4 pplus pmoins =        (* Algo 4 *)
  let prond = algo3 pplus pmoins in
  let n = Array.length pplus in
  let ret = Array.copy prond in
  for i= n-2 downto 0 do
    let x= ref 0. and   somprond = ref 0. and sompstar = ref 0.   in
    Array.set ret i (constEtat (prond.(i).nom) []);
    for j=0 to n-1 do
      somprond := !somprond +. (getProba j prond.(i).arcs )  ;
      sompstar := !sompstar +. (getProba j ret.(i+1).arcs )  ;
      let calcul = (max (!somprond) (!sompstar )) -. !x in
      addArc ret.(i) j ( calcul );
      x := !x +. calcul;
    done;
  done;
  ret
;;

let algoInverse3 pplus pmoins =
  let n = Array.length pplus in
  let ret = Array.make n (constEtat "" []) in
  for i=0 to n-1 do
    let pre= ref 0. and somplus = ref  (List.fold_left (fun x y -> x +. y.proba) 0.  pplus.(i).arcs )
        and sommoins = ref 0.  in
    Array.set ret i (constEtat (pplus.(i).nom) []);
    for j=0 to n-1 do
      somplus := !somplus -. (getProba j pplus.(i).arcs )  ;
      sommoins := !sommoins +. ( getProba j pmoins.(i).arcs );
      if not (!somplus > 1.) then
        let calcul = max (!sommoins)  (1. -. !somplus ) in
        if j == 0 then addArc ret.(i) j ( calcul )
        else  addArc ret.(i) j (  calcul -. !pre );
        pre := calcul;
    done;
  done;
  ret
;;


let algoInverse4 pplus pmoins =
  let prond = algoInverse3 pplus pmoins in
  let n = Array.length pplus in
  let ret = Array.copy prond in
  for i= 1 to n-1 do
    let x= ref 0. and   somprond = ref 0. and sompstar = ref 0.   in
    Array.set ret i (constEtat (prond.(i).nom) []);
    for j=n-1 downto 0 do
      somprond := !somprond +. (getProba j prond.(i).arcs )  ;
      sompstar := !sompstar +. (getProba j ret.(i-1).arcs )  ;
      let calcul = (max (!somprond) (!sompstar )) -. !x in
      addArc ret.(i) j ( calcul );
      x := !x +. calcul;
    done;
  done;
  ret
;;


let algoItest4 pplus pmoins =        (* Algo 4 *)
  let prond = algoInverse3 pplus pmoins in
  let n = Array.length pplus in
  let ret = Array.copy prond in
  for i= n-2 downto 0 do
    let x= ref 0. and   somprond = ref 0. and sompstar = ref 0.   in
    Array.set ret i (constEtat (prond.(i).nom) []);
    for j=n-1 downto 0 do
      somprond := !somprond +. (getProba j prond.(i).arcs )  ;
      sompstar := !sompstar +. (getProba j ret.(i+1).arcs )  ;
      let calcul = (max (!somprond) (!sompstar )) -. !x in
      addArc ret.(i) j ( calcul );
      x := !x +. calcul;
    done;
  done;
  ret
;;

(*  complexité : N*N *)
let difference c cm  =
  let n  = Array.length  cm in
  let ret = Array.make n (constEtat "" [] )in
  for i = 0 to n-1 do
    let a = ref [] in
    for j = 0 to n-1 do
      let prob = (getProba j c.(i).arcs) -. (getProba j cm.(i).arcs) in
      if prob <>0. then a:= (constArc j prob)::!a
    done;
    ret.(i) <- (constEtat cm.(i).nom (!a) );
  done;
  ret

(*  complexité : N*N *)
let differenceIdentite cm =
  let n  = Array.length  cm in
  let ret = Array.make n (constEtat "" [] )in
  for i = 0 to n-1 do
    let vue = ref false in
    let arc = List.fold_left (fun arcs a ->
                  let p =  if  a.direct=  i then (vue := true;  1.) else 0. in
                  if (p -. a.proba) <> 0. then
                    (constArc a.direct (p -. a.proba)) :: arcs
                  else arcs)
                             [] cm.(i).arcs in
    let a = if (!vue) then arc else ((constArc i (1.))::arc) in
    ret.(i) <- (constEtat cm.(i).nom a );
  done;
  ret

let partitionMat cm = List.fold_left (fun (dd, ll, uu) e ->
                          let nb = findE e 0  cm in
                          let  (d,l,u) = List.fold_left (fun (d, l ,u ) a ->
                                             if nb = a.direct then ( a::d , l ,u )
                                             else if  nb > a.direct then (d ,  a::l ,u )
                                             else  (d , l , a :: u )
                                           ) ([],[],[]) e.arcs   in
                          (dd  @ [(constEtat e.nom d)] , ll @ [(constEtat e.nom l)] , uu@ [(constEtat e.nom u)] )
                        ) ([],[],[]) cm

let eliminationLigne i cm  =
  List.filter (fun  e -> e <> cm.(i)) (Array.to_list cm )

let eliminationColonne i cm  =
  Array.fold_left (fun ret e ->
      ret  @ [(constEtat e.nom  (List.filter (fun a -> a.direct <> i ) e.arcs)) ]) [] cm

let eliminationColonneList i cm  =
  List.fold_left
    (fun ret e ->  ret  @ [(constEtat e.nom  (List.fold_left
                                                (fun ret a -> if a.direct > i then
                                                                (constArc (a.direct-1) a.proba)::ret
                                                              else
                                                                if a.direct = i then ret else a::ret
                                                ) [] e.arcs))]
    ) [] cm

module MySetInt =  Set.Make(struct type t = int let compare = compare end)

let rec determinant cm i cmref=
  let n = Array.length cm in
  if n > 2 then (
    let newcm = Array.of_list (eliminationLigne i cm)  in
    let sum = ref 0. in
    for j = 0 to n-1 do
      let r = (getProba j cm.(i).arcs )
      and det = determinant ( Array.of_list (eliminationColonne j newcm)) i cmref in
      sum := !sum +. (-1.)** (float_of_int (i+j) ) *. r *. (det);
    done;
    !sum
  )
  else (
    let la = ref [] and lb = ref [] in

    let mini = cm.(0) and maxi = cm.(1) in
    for indice =0 to 1 do
      List.iter (fun arc ->
          if ( indice = 0 && mini.nom = cmref.(arc.direct).nom ) || ( indice = 1 && maxi.nom = cmref.(arc.direct).nom )
          then  la := arc.proba :: !la
          else lb := arc.proba :: !lb ;
          ()
        ) cm.(indice).arcs      ;
      if List.length (!lb) < indice+1  then  lb := 0. :: !lb ;
      if List.length (!la) < indice+1  then  la := 0. :: !la ;
    done;
    let a= (List.fold_left ( *. ) 1. (!la)  ) and b =  (List.fold_left ( *.) 1. (!lb) )  in
    a -. b
  )

let newpre cm  =
  let n =Array.length cm in
  let ret = Array.make n (constEtat "" [])    in
  for i = 0 to n-1 do
    ret.(i) <- (constEtat cm.(i).nom [])
  done;
  for i = 0 to n-1 do
    List.iter (fun a -> addArc ret.(a.direct) i a.proba ) cm.(i).arcs
  done;
  ret

let multVec cm vec =
  let pred = newpre cm and ret = ref [] and n =Array.length cm in
  for i = 0 to n-1 do
    let value = List.fold_left (fun r a  -> (getProba a.direct pred.(i).arcs ) *. a.proba +. r ) 0. vec  in
    if value <> 0. then ret :=  (constArc i value ) :: !ret
  done;
  !ret

let multMatCreuse cm cm2 =
  let n  =Array.length cm in
  let ret = Array.make n (constEtat "" [])  in
  for i = 0 to n-1 do
    ret.(i) <- (constEtat cm.(i).nom (multVec cm cm2.(i).arcs ) )
  done;
  ret

(*Multiplie d'une matrice donnée par un vecteur d'etat à atteindre *)
let vecteur_inverse cm ter cmref =
  cm.(ter) <- (constEtat cm.(ter).nom [constArc ter 1.]  );
  let ret = cm  in
  let predeRet = newpre ret in
  let vec = multVec predeRet [(constArc ter 1.)] in
  vec

let rec corpGauss_seidel diff ter xk k  =
  let n = Array.length diff in
  if k = 1000 then xk
  else (
    let xkplus = ref [] in
    for i = 0 to n-1 do
      let sumplus =
        List.fold_left (fun ret a -> if a.direct < i then
                                       (a.proba *. (getProba a.direct xk))+.ret
                                     else ret )
                       0. diff.(i).arcs
      and summoins=
        List.fold_left (fun ret a -> if a.direct > i then
                                       ( a.proba *. (getProba a.direct xk))+.ret
                                     else ret )
                       0. diff.(i).arcs  in
      let v = if getProba i diff.(i).arcs  <> 0. then
                ((getProba i ter) -. sumplus -. summoins) /. (getProba i diff.(i).arcs )
              else 0.  in
      xkplus :=  (constArc i v):: !xkplus;

    (*Printf.printf " %d   v : %f   sumplus : %f summoins : %f  \n " i v sumplus summoins;
                        List.iter (fun a -> Printf.printf "%s -> %f \n " diff.(a.direct).nom a.proba ) (!xkplus)
     *)
    done;
    corpGauss_seidel diff ter (!xkplus) (k+1)
  )

let gauss_Seidel cm ter  =
  cm.(ter) <- (constEtat cm.(ter).nom []  );
  let diff = differenceIdentite cm    and terVec = [(constArc ter 1.)] in
  (*
                print_dot "debugDiff.dot" diff;
                let (fil , s, _ ) = Array.fold_right
                        (fun   e (f , s , num ) -> if List.length e.arcs <> 0  then (e::f , s , num-1 ) else (f , (num)::s , num-1 )  ) diff ([] , [] , (Array.length diff)-1 ) in
   *)
  corpGauss_seidel  diff terVec terVec 0

let algoInverse4v2 pplus pmoins =
  let n = Array.length pplus in
  let ret = Array.make n (constEtat "" [])in
  let prond = ref [] in
  for i=0 to n-1 do
    let pre= ref 0.
    and somplus = ref (List.fold_left (fun x y -> x +. y.proba) 0. pplus.(i).arcs )
    and sommoins = ref 0. in
    let x= ref 0.
    and somprond = ref 0.
    and sompstar = ref 0. in
    prond :=   [];
    for j=0 to n-1 do
      somplus := !somplus -. (getProba j pplus.(i).arcs )  ;
      sommoins := !sommoins +. ( getProba j pmoins.(i).arcs );
      if not (!somplus > 1.) then
        let calcul = max (!sommoins)  (1. -. !somplus ) in
        if j == 0 then prond := (constArc  j  calcul ) :: !prond
        else if (  calcul -. !pre ) <> 0. then prond := (  constArc j (  calcul -. !pre )) :: !prond;
        pre := calcul;
    done;
    if i > 0 then (
      ret.(i) <- (constEtat pplus.(i).nom [] ~labels:(pplus.(i).label) );
      for j = n-1 downto 0 do
        somprond := !somprond +. (getProba j !prond )  ;
        sompstar := !sompstar +. (getProba j ret.(i-1).arcs )  ;
        let calcul = (max (!somprond) (!sompstar )) -. !x in
        if calcul <> 0. then addArc ret.(i) j ( calcul );
        x := !x +. calcul;
      done;
    )else ret.(i) <- (constEtat pplus.(i).nom !prond)
  done;
  ret
;;

let algo4v2 pplus pmoins =
  let n = Array.length pplus in
  let ret = Array.make n (constEtat "" [])in
  for i=n-1 downto 0 do
    let somplus = ref 0.
    and sommoins = ref (List.fold_left (fun x y -> x +. y.proba) 0.  pmoins.(i).arcs )  in
    let x= ref 0.
    and   somprond = ref 0.
    and sompstar = ref 0. in
    let paj = ref 0.
    and pa = ref 0. in
    ret.(i) <- (constEtat pplus.(i).nom [] ~labels:(pplus.(i).label));
    for j=0 to n-1 do
      somplus := !somplus +. (getProba j pplus.(i).arcs )  ;
      sommoins := !sommoins -. ( getProba j pmoins.(i).arcs );
      let calcul = min (!somplus)  (1. -. !sommoins  ) in
      pa := !paj ;
      paj :=   calcul ;
      let prob =  if j = 0 then !paj else !paj -. !pa in
      if i < n-1 then (
        somprond := !somprond +. prob   ;
        sompstar := !sompstar +. (getProba j ret.(i+1).arcs )  ;
        let calcul = (max (!somprond) (!sompstar )) -. !x in
        if calcul <> 0. then addArc ret.(i) j ( calcul );
        x := !x +. calcul;
      )else
        if prob <> 0.  then     addArc ret.(i) j ( prob );
    done;
  done;
  ret
;;

(*Calcul la difference entre deux matrices  ayant le meme nombre d'etat et le meme ordre *)
(* Complexité n X n *)
let diffCM cm1 cm2 =
  let n = Array.length cm1 in
  let ret = Array.make n (constEtat "" []) in
  for i=0 to n-1 do
    ret.(i)<- constEtat cm1.(i).nom [];
    for j=0 to n-1 do
      let p1 = getProba j cm1.(i).arcs
      and p2 = getProba j cm2.(i).arcs in
      let r = ( p1 -. p2 ) in
      addArc ret.(i) j ( r *. r         )
    done;
  done;
  ret

(**  Partie fonction qui test un particularité de la chaine de MARKOV**)

let rec isStochastique  ensembleEtat indice=
  let nbEtat= (Array.length ensembleEtat) in
  if indice == nbEtat then true
  else
    let sum = List.fold_left (fun x y -> x +. (y.proba) ) 0. (ensembleEtat.(indice).arcs) in
    if compare sum 1. <> 0 then
      false
    else isStochastique ensembleEtat (indice+1)

(*Test si c'est un imC *)
(* Complexité n X n *)
let isIMC pm pp =
  let n = Array.length pm in
  let ret =ref true  in
  for i=0 to n-1 do
    for k=0 to n-1 do
      let p1 = getProba k pm.(i).arcs
      and p2 = getProba k pp.(i).arcs in
      if p1 > p2  then ret := false ;
    done;
  done;
  (!ret)

let isSTorder pm pp =
  let n = Array.length pm in
  let ret =ref true
  and acc1 = ref 0. and acc2 = ref 0. in
  for i=0 to n-1 do
    for k=0 to n-1 do
      let p1 = getProba k pm.(i).arcs
      and p2 = getProba k pp.(i).arcs in
      acc1 := p1 +. !acc1;
      acc2 := p2 +. !acc2;
      if acc2 > acc1  then ret := false ;
    done;
    if !ret then (
      acc1 := 0. ;
      acc2 := 0. ;
      for k = n-1 downto 0 do
        let p1 = getProba k pm.(i).arcs
        and p2 = getProba k pp.(i).arcs in
        acc1 := p1 +. !acc1;
        acc2 := p2 +. !acc2;
        if acc1 > acc2  then ret := false ;
      done;
    )
  done;
  !ret

(*Complexité en N²  *)
let changeOrder  cm  order =
  let n = Array.length cm in
  let lCM = Array.make n  (constEtat "" []) in
  for ii = 0 to n-1 do
    let e = cm.(order.(ii)) in
    lCM.(ii)<- (constEtat e.nom [] )
  done;
  for ii = 0 to n-1 do
    let e = cm.(order.(ii)) in
    List.iter ( fun el  ->
                let (_, target) = Array.fold_left
                                    (fun (b ,i ) e ->if e = el.direct then
                                                       (true, i) else
                                                       if b then (b,i) else (b, i+1))
                                    (false, 0) order in
                addArc lCM.(ii)  target el.proba
              ) e.arcs;
  done;
  lCM


let taupologieOrderOfCm cm init =
  let n = Array.length cm in
  let lnum = Array.make n 0 in
  let incr= ref 0 and  pos= ref 1 and   i = ref init in
  while !incr < n do
    let ii = !incr in
    if  !pos-1  <=   ii then (
      lnum.(ii)<- !i
    ) ;
    let e = cm.(!i) in
    let distr =e.arcs in
    List.iter (
        fun e ->
        if Array.exists   (fun el -> el = e.direct ) lnum then ()
        else(
          if(!pos < n ) then lnum.(!pos)<- e.direct ;
          pos := 1 + !pos;
        )
      ) distr;

    let x  = if init > ii then
               ii
             else if init-1 = ii then
               ii +2
             else
               ii +1 in
    i:= x;
    incr := 1+ !incr;
  done;

  let (bo, _) = Array.fold_left
                  (fun  (b, pr)  e -> if  e = pr+1 then
                                        (b && true, e)
                                      else
                                        (false, e)  )(true, -1) lnum in
  if bo then cm
  else changeOrder  cm  lnum



   (* Fin du module  *)
