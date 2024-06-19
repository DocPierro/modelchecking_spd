#use "../testTeamCity.ml";;

test_cosmos "DNA Walker" "DNARingLL.grml" "DNARingLL.lha"  "--max-run 10000 --gppflags ''"
  [("FinishCorrect",0.6681) ; 
  ("Finish", 0.9327) ;
  ("DeadLock", 0.0);
  ("Step", 7.7635)]
