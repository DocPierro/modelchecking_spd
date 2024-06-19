(** Cless parser entry points *)

open Cless_ast

(** {2 Entry Points} *)

val expr_list_of_channel: in_channel -> expr_list
val expr_list_of_file: string -> expr_list
val expr_list_of_function: (bytes -> int -> int) -> expr_list
val expr_list_of_string: string -> expr_list

val file_of_channel: in_channel -> file
val file_of_file: string -> file
val file_of_function: (bytes -> int -> int) -> file
val file_of_string: string -> file

(** {2 Location Tools} *)

val print_location: Format.formatter -> location -> unit
val dummy_location: location

(** {2 Printers} *)

(** Those functions print Abstract Syntax Trees (ASTs) in the OCaml syntax,
    not in the syntax of your language. This is handy for debugging, to check
    that your AST is the AST you expect.
    Please note that location nodes are ignored, and if you use
    polymorphic variants, the backquote is omitted.

    Put [true] in the boolean argument if you need parentheses around the
    expression if the expression is not simple. *)

val print_expr_list: bool -> Format.formatter -> expr_list -> unit

val print_file: bool -> Format.formatter -> file -> unit
