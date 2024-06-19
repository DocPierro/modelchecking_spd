#use "../testTeamCity.ml"

let joblist2 = [
  ("dataN1P1", "databasecNfP.grml", "--const 'N=1,P=1' --trace-place 'Modify,COLOR'");
  ("unfoldingN1P1", "databasecNfP.grml" , "--unfold databaseunfoldN1P1.grml --const 'N=1,P=1'");
  ("dataunfoldN1P1", "databaseunfoldN1P1.grml", "--trace-place 'Modify_site_IC_1_file_IC_1'");

  ("dataN2P2", "databasecNfP.grml", "--const 'N=2,P=2' --trace-place 'Modify,COLOR'");
  ("unfoldingN2P2", "databasecNfP.grml" , "--unfold databaseunfoldN2P2.grml --const 'N=2,P=2'");
  ("dataunfoldN2P2", "databaseunfoldN2P2.grml", "--trace-place 'Modify_site_IC_1_file_IC_1'");

  ("dataN3P3", "databasecNfP.grml", "--const 'N=3,P=3' --trace-place 'Modify,COLOR'");
  ("unfoldingN3P3", "databasecNfP.grml" , "--unfold databaseunfoldN3P3.grml --const 'N=3,P=3'");
  ("dataunfoldN3P3", "databaseunfoldN3P3.grml", "--trace-place 'Modify_site_IC_1_file_IC_1'");

]

let add_option (x,y,t) =
  execSavedCosmos ~prefix:false (x,y,"",t^" --loop 100 --max-run 100000 --width 0 --njob 1 -v 1")


let _ =
  for i = 1 to 30 do
    (*add_option (
      (Printf.sprintf "dataN%iP%i" i i),
      ("databasecNfP.grml"),
      (Printf.sprintf "--const 'N=%i,P=%i' --trace-place 'Modify,COLOR'" i i));*)
    add_option (
      (Printf.sprintf "unfoldingN%iP%i" i i),
      ("databasecNfP.grml"),
      (Printf.sprintf "--const 'N=%i,P=%i' --unfold databaseunfoldN%iP%i.grml" i i i i));
    (*add_option (
      (Printf.sprintf "dataunfoldN%iP%i" i i),
      (Printf.sprintf "databaseunfoldN%iP%i.grml" i i),
      (Printf.sprintf "--trace-place 'Modify_site_IC_1_file_IC_1'"));*)
  done
