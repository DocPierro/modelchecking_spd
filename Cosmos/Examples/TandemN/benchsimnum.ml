#use "tandem.ml";;

level:= 0.99;;

for i = 5 to 7 do
  for j= 0 to 0 do 
    generate (1+i) 13 13 (0.4 -. 0.015*.(float j) -. 0.01*.(float j)) (fun _ -> 0.75); 
      Sys.command "prism tandem.sm prismtandem.csl >> prismout";
  done
done;;
  
(*grep -o "States:[ ]*[0-9]*\|Result:[ ]*[0-9.E-]*\|Time for model checking:[ ]*[0-9.]*" prismout*)

(*grep -o "States:[ ]*[0-9]*\|Result:[ ]*[0-9.E-]*\|Time for model checking:[ ]*[0-9.]*" prismout  |  tr '\n' ' ' | sed -e 's/States//g' -e 's/Time for model checking://g' -e 's/Result://g' | tr ':' '\n'*)

