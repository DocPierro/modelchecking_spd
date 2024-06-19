
module type DATA =
sig
  exception Empty
  type ('a, 'b) t
  val create : unit -> ('a, 'b) t
  val acca : ('a, 'b) t -> int -> 'a * 'b
  val index : ('a, 'b) t -> 'a -> int
  val acc : ('a, 'b) t -> 'a -> 'b
  val add : 'a * 'b -> ('a, 'b) t -> unit
  val fold : ('a -> 'b * 'c -> 'a) -> 'a -> ('b, 'c) t -> 'a
  val iter : ('b * 'c -> unit) -> ('b, 'c) t -> unit
  val size : ('a, 'b) t -> int
  val sample : ('a, 'b) t -> 'a * 'b
end

module Data:DATA = 
struct
  exception Empty
  type ('a,'b) t = { 
    mutable table:('a * 'b) array option;
    mutable hash: ('a,int) Hashtbl.t;
    mutable size:int
  }

  let create () = {table = None; hash = Hashtbl.create 10; size=0}
  let acca t i = match t.table with
      None -> raise Empty
    | Some(t) -> t.(i)
  let index t s = Hashtbl.find t.hash s
  let acc t s = 
    let i = index t s in
    snd (acca t i) 
      
  let rec add (a,b) t = match t.table with
      None -> t.table <- Some(Array.make 10 (a,b));
        t.size <- 1;
	Hashtbl.add t.hash a 0;
    | Some(t1) when Array.length t1 > t.size ->
      t1.(t.size) <- (a,b);
      Hashtbl.add t.hash a t.size;
      t.size <- t.size +1
    | Some(t1) -> let t2 = Array.make (2*t.size) t1.(0) in
                  Array.blit t1 0 t2 0 t.size;
                  t.table <- Some(t2);
                  add (a,b) t

  let fold f e t = match t.table with
      None -> e
    | Some t1 -> let buff = ref e in
                 for i = 0 to t.size-1 do
                   buff := f !buff t1.(i);
                 done;
                 !buff
  let size t = t.size
  let sample d = match d.table with
      Some(t) -> t.(0)
    | None -> raise Empty
  let iter f t = fold (fun _ a -> f a) () t
end

module Net =
struct
  type ('a,'b) t = { 
    place: (string,'a) Data.t;
    transition: (string,'b) Data.t;
    inArc: (unit,('a*int*int)) Data.t;
    outArc: (unit,('a*int*int)) Data.t;
  }
  let create () = {
    place = Data.create ();
    transition = Data.create ();
    inArc = Data.create ();
    outArc = Data.create ();
  }
    
  let add_inArc net s1 s2 v =
    let i1 = Data.index net.place s1 
    and i2 = Data.index net.transition s2 in
    Data.add ((),(v,i1,i2)) net.inArc
      
  let add_outArc net s1 s2 v =
    let i1 = Data.index net.transition s1 
    and i2 = Data.index net.place s2 in
    Data.add ((),(v,i1,i2)) net.outArc
      
  let add_arc net s1 s2 v =
    try add_inArc net s1 s2 v with
	Not_found -> add_outArc net s1 s2 v
end

(*
type pt = (int,unit) Net.t;;
type spt = (int,float) Net.t;;

let (testpt:spt) = Net.create ();;

Data.add ("p1",1) testpt.Net.place;;
Data.add ("t1",1.0) testpt.Net.transition;;

Net.add_arc testpt "t1" "t1" 1;;

testpt.Net.place;;

Data.acc testpt.Net.place "p1";;
Data.size testpt.Net.outArc;;
*)
