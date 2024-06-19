#use "../testTeamCity.ml"

let add_option (x,y,t) =
  execSavedCosmos ~prefix:false (x,y,"",t^" --loop 100 --max-run 10000 --width 0 --njob 1 -v 1")


let _ =
  let nbpos = 4 in
  for j = 1 to 20 do
    let i = 10*j in
    add_option (
      (Printf.sprintf "cliserveCli%iPos%i" i 4),
      ("ClientServerStoch3PS.grml"),
      (Printf.sprintf "--const 'NbClient=%i,NbPosition=%i' --trace-place 'ReadyOutput,COLOR'" i 4));
    add_option (
      (Printf.sprintf "unfoldingCli%iPos%i" i 4),
      ("ClientServerStoch3PS.grml"),
      (Printf.sprintf "--const 'NbClient=%i,NbPosition=%i' --unfold clientserverunfoldCli%iPos%i.grml" i 4 i 4));
   (* add_option (
      (Printf.sprintf "dataunfoldCli%iPos%i" i 4),
      (Printf.sprintf "clientserverunfoldCli%iPos%i.grml" i 4),
      (Printf.sprintf "--trace-place 'ReadyOutput_client_IC_1'"));*)
  done
