#use "../testTeamCity.ml";;

test_cosmos "generateProbVect" "tandemReduit.grml" "tandemRElha.grml" "--prism" ["HASLFormula",4.2234e-31];;

test_cosmos_grml "tandemRE" ["HASLFormula",3.79e-31;"Likelyhood_HASLFormula",4.22341118e-31]  "-r --max-run 5000"
