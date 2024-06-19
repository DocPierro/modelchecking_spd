#!../../../modelConvert/petriscript
#directory "../../../modelConvert/_build";;
open Type
open PetriNet
open StochasticPetriNet
open StochasticSymmetricNet
       
let _ =
  let net = Net.create () in
  let ccc =
    "<attribute name=\"classDeclaration\">
      <attribute name=\"name\">ContinuousColorClass</attribute>
      <attribute name=\"continuous\">true</attribute>
     </attribute>
     <attribute name=\"variableDeclaration\">
      <attribute name=\"name\">c1</attribute>
      <attribute name=\"type\">ContinuousColorClass</attribute>
     </attribute>
     <attribute name=\"variableDeclaration\">
      <attribute name=\"name\">c2</attribute>
      <attribute name=\"type\">ContinuousColorClass</attribute>
  </attribute>" in
  net.Net.def <- Some ({ intconst=[]; floatconst=[]; labels=[];formulas=[]; clock=[]; classDef=[ccc]; printer= (fun out () -> ())});

  let pn1 = Net.add_place net "Nm1" (singleTokenInPlace "ContinuousColorClass" 2.0) in
  let pn2 = Net.add_place net "Nm2" (singleTokenInPlace "ContinuousColorClass" 1.0) in
  let tr = Net.add_transition net "tr" (Imm, Float 1.0,Float 1.0) in
  Net.add_inArc net pn1 tr (ColouredVal (singleToken (Var "c1")));
  Net.add_inArc net pn2 tr (ColouredVal (singleToken (Var "c2")));

  Net.add_outArc net tr pn2 (ColouredVal (singleToken (Var"c1")));
  Net.add_outArc net tr pn1 (ColouredVal (singleToken (Var"c2")));

  print_ssn ("fibo.grml") net
