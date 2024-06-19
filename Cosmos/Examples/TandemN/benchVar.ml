#use "tandem.ml";;

for i = 8 to 8 do
  let lambda = 0.1 +.  0.01*.(float i) in
  execSavedCosmos ~prefix:false ((Printf.sprintf "Variancevar-%f" lambda),"tandemVar.grml","tandemHisto.lha",Printf.sprintf " --width 0 --output-PDFCDF pdf_%f.dat  --const \"lambda=%f,rho2=%f\" --max-run 100000000000 --njob 12" lambda lambda (0.55-.lambda));
done;;
