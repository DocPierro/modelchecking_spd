#use "tandem.ml";;

level:= 0.99;;

for i =3 to 3 do 
  simule i 50 3 0.01 (fun _-> 0.11) 1000000;
done;;
  
