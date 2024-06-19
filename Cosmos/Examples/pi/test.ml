#use "../testTeamCity.ml";;

test_cosmos_grml "pi" ["",3.1415] "--max-run 5000 --HASL-formula \"(0+4*AVG(Last(OK))/4);\" --outputdata test.dat --output-raw testraw.dat"

(*
let _ =
	printf "##teamcity[testStarted name='Test Interactive' captureStandardOutput='<true>']\n";
  let cmd =  "echo \"help\nfkdhfkds\nfire toto\nfire Sampy\nfire Sampx\nstep\n\nstep\nstop\n\" |  "^ cosmos_path  ^" pi.gspn pi.lha -i "^cosmos_options^" 2>&1 | grep Isucc -q" in
  print_endline cmd;
	let b = Sys.command cmd in
	if b=0 then printf "##teamcity[testFinished name='Test Interactive' message='Test sucessfull']\n"
  else printf "##teamcity[testFailed name='Test Interactive' message='Test fail']\n##teamcity[testFinished name='Test Interactive' message='Test failed']\n"
*)
