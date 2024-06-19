#use "../testTeamCity.ml";;

test_cosmos "Hospital" "Hospital.grml" "--loop 100" "--max-run 500 --batch 50" ["MeanToken_WaitingRoom",0.46807850579651 ; "Throughput_HospitalArrival",1.34151277897769 ];;
test_cosmos "Hospital" "Hospital.grml" "--loop 100" "--max-run 500 --batch 50 --use-setdomain-impl" ["MeanToken_WaitingRoom",0.46807850579651 ; "Throughput_HospitalArrival",1.34151277897769 ]
