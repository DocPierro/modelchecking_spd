open Str
open Printf
#load "unix.cma"
#load "str.cma"

let uni f =
  let v = ref None in
  function () -> match !v with
      Some x -> x
    | None -> let v2 = f () in
              v := Some v2;
              v2

exception CmdFail of int

let cosmos_options = ref (
  try
    let _ = Sys.getenv "COV" in
    " --gppflags \"--coverage -Wno-return-type\" --gppcmd clang++"
  with Not_found -> "");;

(*let cosmos_path = (Sys.getcwd ())^"/../../bin/Cosmos"*)
let cosmos_path = ref (try
    let cp = Sys.getenv "COSMOS" in
    (*cosmos_options := !cosmos_options^" --bin-path ../../src/Simulator/bin "; *)
    cp
  with Not_found -> "../../bin/Cosmos");;



(*" --gppflags --coverage --gppcmd clang++"
 let _ = Sys.getenv "COV" in " --gppflags --coverage --gppcmd clang++"
*)

let pe () = print_char (char_of_int 0x1b)
let print_color s c =
  pe ();
  printf "[1;%im%s" c s;
  pe ();
  print_string "[0m"

let dots = regexp ":\\(\t\\)?"
let confintdel = regexp "\\[\\| , \\|\\]"

type haslr =
{
  mutable mean: float;
  mutable stdDev: float;
  mutable confInterval: float*float;
  mutable minmax: float*float;
}

let print_hasl_header f sep =
  Printf.fprintf f "%s%s%s%s%s%s%s%s%s%s%s%s%s%s" "HASLname" sep "MeanValue" sep "StandarDev" sep "MinimalValue" sep "ConfidenceIntervalLow" sep "ConfidenceIntervalUp" sep "MaximalValue" sep
let print_haslr f sep (s,h) =
  Printf.fprintf f "%s%s%g%s%g%s%g%s%g%s%g%s%g%s" s sep h.mean sep h.stdDev sep (fst h.minmax) sep (fst h.confInterval) sep (snd h.confInterval) sep (snd h.minmax) sep

let dummy_haslr = {
  mean =0.;
  stdDev =0.;
  confInterval = 0.,0. ;
  minmax = 0.0 , 0.0
}

let init_log s =
  let fl = s in
  Unix.putenv "COSMOS_TEST_LOG" fl;
  let fs2 = open_out fl in
  output_value fs2 ([],[]);
  close_out fs2;;

let update_log b (s:string) =
  if Array.length Sys.argv >2 then
  try
    let fl = Sys.argv.(2) in(* Unix.getenv "COSMOS_TEST_LOG" in*)
    let fs = open_in fl in
    let (success,failure) = input_value fs in
    close_in fs;
    let sf2 = if b then (s::success,failure)
      else (success,s::failure) in
    let fs2 = open_out fl in
    output_value fs2 sf2;
    close_out fs2
  with
      Not_found -> print_endline "No log Found"

let read_log s =
  let fs = open_in s (*((Unix.getcwd ()) ^"/cosmos_test_log")*) in
  let ((succ:string list),(fail:string list)) = input_value fs in
  close_in fs;
  (succ,fail);;


let rec check_result l = function
  | [] -> true
  | (lab,v)::q -> try
                    let h = List.assoc lab l in
                    if v >= (fst h.confInterval) -. 0.00001 && (snd h.confInterval) +. 0.00001 >= v && ((snd h.confInterval) -. (fst h.confInterval) < 1000.0) then check_result l q
                    else false
    with x -> false

let check_unique_result r1 r2 =
  match (r1,r2) with
    ([lab1,h],[lab2,v]) ->
               if v < fst h.confInterval then (-1)
               else if v > snd h.confInterval then 1
               else 0
  | _ ->  0

let rec print_readable l =  function
  | [] -> ""
  | (lab,v)::q -> try
                    let h = List.assoc lab l in
                    if v >= (fst h.confInterval) -. 0.00001 && (snd h.confInterval) +. 0.00001 >= v then (
                      Printf.sprintf  "%s: Value %e is inside confidence interval [%e,%e]\n%s" lab v (fst h.confInterval) (snd h.confInterval) (print_readable l q)
                    ) else (
                      Printf.sprintf "%s: Value %e is outside confidence interval [%e,%e]\n%s" lab v (fst h.confInterval) (snd h.confInterval) (print_readable l q)
                  )
    with _ -> print_readable l q

type result = {
  mutable haslResult : (string*haslr) list;
  mutable conflevel : float;
  mutable simtime: float;
  mutable systime: float;
  mutable memory: float;
  mutable nbRun: int;
  mutable nbSuccRun: int;
  mutable modelName: string;
  mutable propName: string;
  mutable batch: int;
  mutable nbJob:int;
  mutable date: string;
}

let print_header f sep =
  Printf.fprintf f "%s%s%s%s%s%s%s%s%s%s%s%s%s%s" "ExpName" sep "ModelName" sep "PropName" sep "NumberOfRun" sep "NumberOfSuccessfullRun" sep "NumberOfThread" sep "BatchSize" sep;
  Printf.fprintf f "%s%s%s%s%s%s%s%s" "simulationTime" sep "SystemTime" sep "Memory" sep "Date" sep ;
  print_hasl_header f sep;
  output_string f "\n"

let print_result f sep name rf =
  let ps ()= output_string f sep in
  let pf fl = output_string f (string_of_float fl) in
  let pi i = output_string f (string_of_int i) in
  Printf.fprintf f "%s%s%s%s%s%s" name sep rf.modelName sep rf.propName sep;
  pi rf.nbRun; ps ();
  pi rf.nbSuccRun; ps ();
  pi rf.nbJob; ps ();
  pi rf.batch; ps ();
  pf rf.simtime; ps ();
  pf rf.systime; ps ();
  pf rf.memory; ps();
  output_string f rf.date; ps ();
  List.iter (print_haslr f sep) rf.haslResult;
  output_string f "\n";;

let dummyresult = {
  haslResult=[];
  conflevel= 0.;
  simtime = 0.;
  systime = 0. ;
  memory =0. ;
  nbRun = 0;
  nbSuccRun = 0;
  modelName = "dummyModel.grml";
  propName ="dummyProp.grml";
  batch = 0;
  nbJob = 0;
  date = "dummyDate";
}

let parse_result f =
  let fs = open_in f in
  let result = {dummyresult with systime = dummyresult.systime} in
  (try while true do
      let str = input_line fs in
      let ls = split_delim dots str in
      match ls with
        | "Prism Result" :: v :: [] -> let x = float_of_string v in
                                       (snd (List.hd result.haslResult)).mean <- x;
                                       (snd (List.hd result.haslResult)).confInterval <- (x-.0.0001 , x+. 0.0001)
        | "Estimated value" :: v :: [] -> (snd (List.hd result.haslResult)).mean <- (float_of_string v)
        | "Standard deviation" :: v :: [] -> (snd (List.hd result.haslResult)).stdDev <- (float_of_string v)
        | "Confidence level" :: v :: [] -> result.conflevel <- (float_of_string v)
        | "Confidence interval" :: v :: [] ->
          (match split confintdel v with
            | a::b::_ -> (snd (List.hd result.haslResult)).confInterval <- (float_of_string a , float_of_string b)
            | _ -> printf "Fail to parse confidence interval %s\n" v)
        | "Minimal and maximal value" :: v :: [] ->
          (match split confintdel v with
            | a::b::_ -> (snd (List.hd result.haslResult)).minmax <- (float_of_string a , float_of_string b)
            | _ -> printf "Fail to parse minmax %s\n" v)
        | "Binomiale confidence interval" :: v :: [] ->
          (match split confintdel v with
            | a::b::_ -> (snd (List.hd result.haslResult)).confInterval <- (float_of_string a , float_of_string b)
            | _ -> printf "Fail to parse confidence interval %s\n" v)

        | "Time for simulation" :: v :: [] ->
          let v2 = String.sub v 0 (String.length v -1) in
          result.simtime <- (float_of_string v2)
        | "Seed" :: v:: [] -> ()
        | "Command line" :: v :: [] -> ()
        | "Total CPU time" :: v :: [] -> result.systime <- (float_of_string v)
        | "Total Memory used" :: v :: [] ->
          result.memory <- (float_of_string (String.sub v 0 ((String.length v) -3)))
        | "Total paths" :: v :: [] -> result.nbRun <- (int_of_string v)
        | "Accepted paths" :: v :: [] -> result.nbSuccRun <- (int_of_string v)
        | "Number of jobs" :: v :: [] -> result.nbJob <- (int_of_string v)
        | "Batch size" :: v :: [] -> result.batch <- (int_of_string v)
        | "Width" :: v :: [] -> ()
        | "Model path" :: v :: [] -> result.modelName <- v;
        | "LHA path" :: v :: [] -> result.propName <- v;
        | "Formula" :: v :: [] -> result.propName <- v;
        | "LHA loop" :: v :: "transient" :: w :: [] -> result.propName <- ("loop"^v^"transient"^w);
        | s1 :: [""] -> result.haslResult <- (s1,{dummy_haslr with mean=0.0}):: result.haslResult
        | "Level" :: v :: [] -> ()
        | "Method" :: _ ->()
        | "LHA loop" :: _ -> ()
        | "Command line" :: _ -> ()
        | "Seed" :: _ -> ()
        | _ -> print_endline ("Fail to parse'"^str^"'")
    done
   with
     | End_of_file -> close_in fs
     |  x -> print_endline "fail to parse"; raise x
  );
  result;;

let string_date () =
  let tm = Unix.localtime (Unix.gettimeofday ()) in
  Printf.sprintf "%i/%i/%i %i:%i:%i" tm.Unix.tm_mday tm.Unix.tm_mon
    (tm.Unix.tm_year+1900) tm.Unix.tm_hour tm.Unix.tm_min tm.Unix.tm_sec

let raw_cosmos opt printcmd =
  let cmd = sprintf "%s %s" !cosmos_path opt in
  let wd = Sys.getcwd () in
  if printcmd then (
    print_string (wd^":\t");
    print_endline cmd;
  );
  let retcode =  Sys.command cmd in
  if retcode <> 0 then raise (CmdFail(retcode));
  let result = parse_result "Result.res" in
  result.date <- string_date ();
  result

let exec_cosmos model prop opt printcmd =
  let cmd = sprintf "%s %s %s %s" model prop !cosmos_options opt in
  raw_cosmos cmd printcmd

let test_cosmosBash testname model prop opt v =
  print_color (sprintf "testStarted: %s \n" testname) 33;
  flush stdout;
  try let result = exec_cosmos model prop opt true in
       if check_result result.haslResult v
       then (
         update_log true testname;
         print_color (sprintf "testFinished: %s\n%s" testname (print_readable result.haslResult v)) 32
       ) else (
         update_log false testname;
         print_color (sprintf "testFailed: %s\n%s" testname (print_readable result.haslResult v)) 31
       )
  with CmdFail(ret) ->
    update_log false testname;
    print_color (sprintf "testFailed: %s Test %s fail: Cosmos return value:%i\n" testname testname ret) 31
    | x ->
      update_log false testname;
      print_color (sprintf "testFailed: %s Test %s fail due to uncaught exception: %s\n" testname testname (Printexc.to_string x)) 31


let test_coverage_full name model prop level v o n =
  let _ = exec_cosmos model prop ("--batch 10 -v 0 --tmp-status 2 --max-run 10 "^o) false  in
  let cmd = Printf.sprintf "Cosmos %s -v 0 --tmp-status 3 %s %s --level %f" o model prop level in
  print_endline cmd;
  let succ = ref 0 in
  for i = 1 to n do
    let result = exec_cosmos model prop (o^" --tmp-status 3 --HASL-expression='Overflow=PROB;' --level "^(string_of_float level)) false in
    let r = check_unique_result result.haslResult v in
    if r=0 then (
      incr succ;
      print_string "+";
    ) else if r<0 then (
      print_string "<";
    ) else print_string ">";
    flush stdout;
  done;
  let cov = ((float !succ)/.(float n)) in
  printf "\nCoverage:\t%f\n" cov;
  if cov <= level*.0.9 || cov >= level*.1.1 then
    update_log false ("Coverage_"^name^"_"^(string_of_float level))
  else update_log true ("Coverage_"^name^"_"^(string_of_float level))

let test_coverage name level v o n =
  test_coverage_full name (name^".gspn") (name^".lha") v o n


let execCosmosLog_free resultFile csvFile (name,model,prop,option,prefix) =
   try
    let r =
      (if prefix then (
        let cwd = (Sys.getcwd ())^"/" in
        ignore (Sys.command ("mkdir -p "^name));
        ignore (Sys.command ("lumpingfun.cpp "^name^"/lumpingfun.cpp"));
        Sys.chdir name;
        let r2 = try exec_cosmos (cwd^model) (cwd^prop) option true
          with x -> Sys.chdir cwd; raise x in
        Sys.chdir cwd; r2
      ) else (
        let r2 = exec_cosmos model prop option true in
        ignore (Sys.command (sprintf "mv Result.res %s.res" name)); r2
       )) in
    output_value resultFile (name,r);
    print_result csvFile "," name r;
    flush resultFile;
    flush csvFile;
  with x ->print_endline (Printexc.to_string x);;

let execSavedCosmos_free prefix resultFile csvfile (name,model,prop,option)  =
  execCosmosLog_free resultFile csvfile (name,model,prop,option,prefix)

let openCreate path fu =
  let e = Sys.file_exists path in
  let f = open_out_gen [Open_wronly; Open_creat; Open_append] 0o644 path in
  if not e then fu f;
  f

let rf = uni (fun () -> openCreate "CamlResultFile" (fun _->() ));;
let csv = uni (fun () -> openCreate "csvResultFile" (fun f->(
  print_header f ",\t")));;

let execSavedCosmos ?(prefix=false) x = execSavedCosmos_free prefix (rf ()) (csv ()) x

let printHeadCsv () =
  print_header (csv ()) ",";;

let producelog resultFile =
  let rf = open_in resultFile in (
    try while true do
        let (n,v) = input_value rf in
        print_result stdout ",\t" n v;
      done with
          End_of_file -> ()
  );
  close_in rf
