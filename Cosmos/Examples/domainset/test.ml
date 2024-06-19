#use "../testTeamCity.ml";;

test_cosmos "SimpleConsumer" "simpleConsumer/SimpleConsumer.grml" "--loop 100" "" ["Throughput_RollDice", 0.06; "MeanToken_RolledDices",0.06 ];;
test_cosmos "SimpleConsumer" "simpleConsumer/SimpleConsumer.grml" "--loop 100" "--use-setdomain-impl" ["Throughput_RollDice", 0.06; "MeanToken_RolledDices",0.06 ];;
test_cosmos "UniformDice" "UniformDice/UniformDice.grml" "--loop 100" "" ["Throughput_RollDice", 5.94000000; "MeanToken_RolledDices", 297.00000000];;
test_cosmos "UniformDice" "UniformDice/UniformDice.grml" "--loop 100" "--use-setdomain-impl" ["Throughput_RollDice", 5.94000000; "MeanToken_RolledDices", 297.00000000];;
