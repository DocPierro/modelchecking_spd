open Unix

let _ =
  if Array.length Sys.argv <= 2 then (
    print_endline "usage: split_merge file chunk_size [sep]";
    exit 1;)

let file = open_in Sys.argv.(1)
let chunk_size = int_of_string Sys.argv.(2)
let sep = if Array.length Sys.argv<=3 then "\t"
          else Sys.argv.(3)


let _ =
  let n = ref 0 in
  try while true do
        incr n;
        let l = input_line file in
        if !n mod chunk_size =0 then
          print_endline l
        else print_string (l^sep)
      done with
    End_of_file -> ()
