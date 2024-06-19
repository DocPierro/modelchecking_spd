#use "../testTeamCity.ml";;



test_cosmos "TestEventQueueSet" "moded_CoSimulation.grml" "propCollision.lha" 
  "--max-run 20000 --use-setdomain-impl --batch 0 --const EnvBehaviour=1 --const TypeGeneration=2 --const Ncases=1000 --const Vmax=5 --const VminInit=3 --const Ncases=1000 --const Nveh=20"
  ["collb",0.814241486068111]
