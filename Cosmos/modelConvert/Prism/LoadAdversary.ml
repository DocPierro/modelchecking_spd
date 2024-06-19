let strat_name = ref ""

let inter = Str.regexp "^(\\(.*\\))$"
let commasep = Str.regexp ","
let spacesep = Str.regexp " "
let stateline = Str.regexp "^\\([0-9]+\\):(\\(.*\\))$"
                          
exception FailToParse

module StateMap = Map.Make(struct
			    type t = int array
			    let compare (x:t) (y:t) =
			      compare x y 
			  end)
            
type strategy ={
    shape: string list;
    strat: string StateMap.t
  }
            

let print_int_array sep f v =
  Array.iteri (fun i j -> if i>0 then output_string f sep; Printf.fprintf f "%i" j) v
            
let rec read_state_line n fin =
  try
    let line = input_line fin in
    if not @@ Str.string_match stateline line 0 then raise FailToParse;
    let staten = int_of_string (Str.matched_group 1 line) in
    let statedesc = Str.matched_group 2 line in
    let mark = Array.make n 0 in
    ignore @@ List.fold_left (fun i v -> mark.(i) <- int_of_string v; i+1) 0 (Str.split commasep statedesc);
    (staten,mark) :: (read_state_line n fin)
  with End_of_file -> []
            
let loadState f =
  let fin = open_in f in
  let shape = input_line fin in
  if not @@ Str.string_match inter shape 0 then raise FailToParse;
  let interval = Str.matched_group 1 shape in
  let vals = Str.split commasep interval in
  let n = List.length vals in
  Printf.printf "shape: %s\n" shape;
  let l = read_state_line n fin in
  (*List.iter (fun (n,v) ->Printf.printf "%i:(%a)\n" n (print_int_array ",") v) l;*)
  close_in fin;
  l,vals

let rec read_strat_line fin=
  try
    let line = input_line fin in
    let vals = Str.split spacesep line in
    match vals, read_strat_line fin with
      (st::_::_::e::_),(st2,e2)::q when st=st2 && e=e2 -> (st2,e2)::q
    | (st::_::_::e::_),q -> (st,e)::q
    | _-> raise FailToParse;
  with
    End_of_file -> []
    
let loadStrat f =
  let fin = open_in f in
  let _ = input_line fin in
  let strat_list = read_strat_line fin
                   |> List.map (fun (x,y) -> (int_of_string x),y)
  in
  
  (*List.iter (fun (n,v) ->Printf.printf "%i %s\n" n v) strat_list;*)
  close_in fin;
  strat_list


    
let loadFullStrat f =
  let states,shape = loadState (f^".sta") in
  let strat = loadStrat (f^".tra") in
  let maxnb = List.fold_left (fun x (y,_) -> max x y) 0 states in
  let strat_array = Array.make (maxnb+1) "" in
  (*Printf.printf "max: %i" maxnb;*)
  List.iter (fun (n,v) -> strat_array.(n) <- v) strat;

  
  let fs = List.fold_left (fun m (n,v) -> StateMap.add v strat_array.(n) m) StateMap.empty states in
  { strat= fs ;shape}
