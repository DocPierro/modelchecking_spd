open Type

type format = Simulink | Prism | Pnml | Dot | Marcie | Stateflow | Pdf | GrML | CLess | TikZ

let input = ref stdin
let output = ref "out"
let inname = ref "stdin"
let typeFormat = ref Prism
let outputFormat = ref [Dot;Marcie]
let const_file = ref ""
let verbose = ref 1
let inHibitor = ref true
let traceSize = ref 0
let simule = ref 0
let statespace = ref false
let epsV = ref 0.00001;;
let skBaseStep = ref None;;

let skUseRK4 = ref 0;;
let skUseODE45 = ref 0;;
let skUseEuler = ref 0;;

let suffix_of_filename s =
  let fa = String.rindex s '.'+1 in
  (String.sub s 0  (fa -1)),(
  String.sub s fa (String.length s - fa))

let nbarg = ref 0

let _ =
  Arg.parse [
             "-v",Arg.Set_int verbose,"Set verbose level default 1";
             "--epsv", Arg.Set_float epsV, "Define minimal distinction capacity for integration (ODE45 adaptative value)";
             "--skbasestep", Arg.Float (fun x -> skBaseStep:=Some x), "Define base step for Simulink Simulation";
             "--use-rk4", Arg.Set_int SimulinkTrans.skUseRK4, "Use Runge-Kutta 4 method for Integrators";
             "--use-ode45", Arg.Set_int SimulinkTrans.skUseODE45, "Use Adpatative Runge-Kutta 4/5 method for Integrators";
             "--use-euler", Arg.Set_int SimulinkTrans.skUseEuler, "Use Euler method for Integrators";
             "--tikz", Arg.Unit (fun () -> outputFormat:= TikZ :: !outputFormat), "Output in Tikz";
            ]
    (function s -> incr nbarg; match !nbarg with
      1 -> inname:= s;
        ( let o,suf = suffix_of_filename s in
          output := o;
          match suf with
        | "slx" -> typeFormat := Stateflow
        | _-> failwith "Unsupported file format"
        )
    | _ -> failwith "Do not know what to do with extra arguments.")
    "usage";;

  
let _ =
  let zip = Zip.open_in !inname in
  Simulinkreader.verbose := !verbose;
  Simulinkreader.skBaseStep := !skBaseStep;
  Simulinkreader.epsV := !epsV;
  Simulinkreader.readSimulink zip
  |< (fun x-> if List.exists (function TikZ-> true |_ -> false) !outputFormat then
                Simulinkparser.printLaTeX stdout x)
  |> SimulinkTrans.generateGSPN
  |> StochasticSymmetricNet.print_ssn ~initid:1000 ("SKModel.grml")
