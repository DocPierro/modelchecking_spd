open Xml
open PetriNet
open StochasticPetriNet
open Type

let tree_of_pnml s =
     parse_file s

let rec find_child s e1 = function
    Element (s,_,[PCData e2])-> Some e2
  | Element (_,_,cl) -> List.fold_left (find_child s) e1 cl
  | PCData _ -> e1 

let find_child_list s xl =
    List.fold_left (find_child s) None xl

let find_at s l = try Some (List.assoc s l) with Not_found -> None
let find_id l= find_at "id" l

let rec find_name_rec s e = function
    Element (x,_,cl) when s=x-> find_child_list "text" cl
  | Element (_,_,cl) -> List.fold_left (find_name_rec s) e cl
  | PCData _ -> e

let find_name s t = find_name_rec s None t

let rec net_of_tree n = function
    | Element (name,alist,clist) as t->
       begin match name with
	       "xml" | "pnml" | "net" | "page" ->
					 print_endline name;
					 List.iter (net_of_tree n) clist
|  "transition" -> (match ((find_name "name" t),(find_id alist)) with
        (Some name,Some id) -> Data.add (id,(StochasticPetriNet.Exp (Float 1.0),(Float 1.0),(Float 1.0))) n.Net.transition
        | _-> ()
       )
    |  "place" -> ( match ((find_name "name" t),(find_name "initialMarking" t),(find_id alist)) with
        | (_,Some im,Some id) -> Data.add (id,(Int (int_of_string im),None)) n.Net.place
        | (_,None,Some id) -> Data.add (id,(Int 0,None)) n.Net.place
        | _-> ()
    )
    |  "arc" -> ( match ((find_id alist),(find_at "source" alist),(find_at "target" alist),(find_name "inscription" t)) with
        (_,Some source,Some target,Some v) -> Net.add_arc n source target (Int (int_of_string v))
      | (_,Some source,Some target,_) -> Net.add_arc n source target (Int 1)
      | _-> ()
    )
    | x-> Printf.printf "Dont know wat to do with %s, ignore\n" x;
       end
    | _ ->()
