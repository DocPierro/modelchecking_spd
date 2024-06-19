#use "../testTeamCity.ml";;

test_cosmos "shared memory" "sms_Unif.gspn" "--transient 100 --loop 100" "--max-run 1000" ["MeanToken_Request1",150.0 ; "Throughput_Arrive1",2.0 ; "MeanToken_Free",0.0]
