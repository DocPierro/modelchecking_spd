
let open_connection s b =
  let serial = Unix.openfile s [Unix.O_RDWR;Unix.O_NOCTTY; Unix.O_NONBLOCK] 0o666 in
  let term = Unix.tcgetattr serial in 
  Unix.tcflush serial Unix.TCIOFLUSH;
  Unix.tcsetattr
    serial
    Unix.TCSANOW
    {term with 
      Unix.c_ibaud = b ;
      Unix.c_obaud = b ;

      Unix.c_icanon = false;
      Unix.c_echo = false;
      Unix.c_echoe = false;
      Unix.c_echok = false;
      Unix.c_echonl = false;
      Unix.c_isig = false;
      
      Unix.c_inlcr = false;
      Unix.c_igncr = false;
      Unix.c_icrnl = false;
      Unix.c_ignbrk = false;
      Unix.c_parmrk = false;

      Unix.c_ixon = false;
      Unix.c_ixoff = false;

      Unix.c_cread =true;
      Unix.c_clocal=true;

      Unix.c_csize = 8;
      Unix.c_parenb = false ;
      Unix.c_parodd = false ;
      Unix.c_inpck = false;
      Unix.c_istrip = false;

      Unix.c_cstopb = 1;
      Unix.c_opost = false ;
      Unix.c_vmin = 1;
      Unix.c_vtime =0;
      
      
    (*  Unix.c_hupcl = false;*)
      
      };
  serial;;

let get_int s = 
  try 
    let nr = ref '0' in
    Scanf.sscanf s "%i" (fun n -> nr := char_of_int n);
    Some !nr
  with 
    _ -> None;;
  
let splitnb = Str.regexp "[ \n]"

let interpret_str s =
  let s2 = Bytes.create (String.length s) in
  let nchar = ref 0 in
  let lastnb =ref false in
  let spl = Str.full_split splitnb s in
  List.iter (function 
  | Str.Text x -> begin match get_int x with
    | Some n -> 
      Bytes.set s2 !nchar n; 
      incr nchar;
      lastnb:=true;
    | None -> let nl = String.length x in
	      Bytes.blit_string x 0 s2 !nchar nl;
	      nchar := !nchar + nl;
	      lastnb := false

  end
  | Str.Delim x when x = "\n" || !lastnb -> lastnb:= false
  | Str.Delim x -> let nl = String.length x in
	       Bytes.blit_string x 0 s2 !nchar nl;
	       nchar := !nchar + nl;
	       lastnb := false
  ) spl;
  Bytes.sub s2 0 !nchar;;
	 
let _ =
  assert (Array.length Sys.argv > 1);
  let b = (if Array.length Sys.argv>2 then int_of_string Sys.argv.(2) else 115200) in
  print_endline ("Opening:"^Sys.argv.(1)^" at rate:"^(string_of_int b));
  let serial = open_connection Sys.argv.(1) b in
  let closeimm = Array.length Sys.argv <=3 in
  let s = Bytes.create (101) in
  while closeimm do
    let rf,_,_ = (Unix.select [serial;Unix.stdin] [] [] (-1.0)) in
    match rf with
      x::_ when x=serial ->
	let nread =  Unix.read serial s 0 100 in
	if nread = 0 then raise End_of_file;
	print_string (Bytes.sub_string s 0 nread);
	flush stdout;
    | x::_ when x=Unix.stdin -> 
      let nread =  Unix.read Unix.stdin s 0 100 in
      if nread = 0 then raise End_of_file;
      let s2 = interpret_str (Bytes.sub_string s 0 nread ) in
      print_string "Send";
      for i = 0 to Bytes.length s2-1 do
	Printf.printf "[%i:%c]" (int_of_char (Bytes.get s2 i)) (Bytes.get s2 i); 
      done;
      print_newline ();
      let n = Bytes.length s2 in
      let n2 =Unix.write serial s2 0 n in assert (n=n2);
      
  done


(*
begin
	begin try
		Scanf.bscanf Scanf.Scanning.stdin "%i\n" (fun n -> s.[0]<- char_of_int n)
	  with 
	    _ -> Scanf.bscanf Scanf.Scanning.stdin "%c" (fun c -> s.[0]<- c)
	end;
	ignore @@ Unix.write serial s 0 1;
	
      end*)
