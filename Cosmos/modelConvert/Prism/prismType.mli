type expType = IntT | BoolT | DoubleT | IntVar | BoolVar | FunT | Clock
val allInt : bool ref
val allReal : bool ref
val mapType : expType Type.StringMap.t ref
val add_int : Type.StringMap.key -> unit
val add_bool : Type.StringMap.key -> unit
val add_double : Type.StringMap.key -> unit
val add_var : Type.StringMap.key -> expType -> unit
val add_copy : Type.StringMap.key -> Type.StringMap.key -> unit
val find_action :
  (Type.StringSet.elt option * 'a * 'b) list -> Type.StringSet.t
type constdef =
    (string * int Type.expr' option) list *
    (string * float Type.expr' option) list
type update = IntUp of int Type.expr' | BoolUp of bool Type.expr'
val print_update : out_channel -> update -> unit
type varKind =
    IntK of string * (int Type.expr' * int Type.expr') * int Type.expr'
  | BoolK of string * (int Type.expr' * int Type.expr') * int Type.expr'
  | ClockK of string
module Guard :
sig
  type sing = string * Type.cmp * int Type.expr'
  type conj (*= (Type.cmp * int Type.expr') list Type.StringMap.t*)
  type t = conj list
  val fold : ('a -> sing -> 'a) -> 'a -> conj -> 'a
    val map : (sing -> sing) -> conj -> conj
    val to_list : conj -> sing list
    val iter : (sing -> unit) -> conj -> unit
    val print : out_channel -> t -> unit
    val print_conj : out_channel -> conj -> unit
    val conj : t -> t -> t
    val flatten : bool Type.expr' -> t
  end
type prism_module = {
  name : string;
  varlist : varKind list;
  actionlist :
    (string option * Guard.t * ((float Type.expr' * (string * update) list)) list)
    list;
  actionset : Type.StringSet.t;
}
type moduledef =
    Full of prism_module
  | Renaming of string * string * (string * string) list

type modelKind = DTMC | MDP | CTMC |PTA
type labels = (string*(bool Type.expr')) list
                                      
type prism_file = {
    consts: constdef;
    modlist: moduledef list;
    kind: modelKind;
    labels: labels;
    formulas: (string*Type.full_expr) list;
  }

val print_prism_mod : out_channel -> prism_module -> unit
val print_prism : out_channel -> prism_file -> unit
