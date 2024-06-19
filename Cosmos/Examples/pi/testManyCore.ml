#use "../testTeamCity.ml";;

let benchfile = open_out "bench.dat";;

for i = 1 to 32 do
  call_cosmos ("pi.grml pi.lha --max-run 1000000000 --batch 1000000 --width 0 --njob "^(string_of_int i));
  let result = parse_result "Result.res" in
  Printf.fprintf benchfile "%i\t%f\t%f\t%f\n" i result.simtime (1000. /. result.simtime) (result.systime);
  flush benchfile;
done;;

close_out benchfile;;

Sys.command "gnuplot ../../bin/gnuplotScriptbenchmanycore.gnu";;
