#use "../testTeamCity.ml"

(*
let joblist = [
  ("N3P2_L2L11", "MAPK_P2Reduit.grml" "RareEventUntil.lha" "--const 'L=2,L2=11' --prism");
  ("N3P2_L2L11", "MAPK.grml" "RareEventUntil2.lha" "--const 'L=2,L2=11' -r");
  ("N3P2_L3L11", "MAPK_P2Reduit.grml" "RareEventUntil.lha" "--const 'L=3,L2=11' --prism");
  ("N3P2_L3L11", "MAPK.grml" "RareEventUntil2.lha" "--const 'L=3,L2=11' -r");
  ("N3P2_L4L11", "MAPK_P2Reduit.grml" "RareEventUntil.lha" "--const 'L=4,L2=11' --prism");
  ("N3P2_L4L11", "MAPK.grml" "RareEventUntil2.lha" "--const 'L=4,L2=11' -r");
  ("N3P2_L5L11", "MAPK_P2Reduit.grml" "RareEventUntil.lha" "--const 'L=5,L2=11' --prism");
  ("N3P2_L5L11", "MAPK.grml" "RareEventUntil2.lha" "--const 'L=5,L2=11' -r");
  ("N3P2_L6L11", "MAPK_P2Reduit.grml" "RareEventUntil.lha" "--const 'L=6,L2=11' --prism");
  ("N3P2_L6L11", "MAPK.grml" "RareEventUntil2.lha" "--const 'L=6,L2=11' -r");
  ("N3P2_L7L11", "MAPK_P2Reduit.grml" "RareEventUntil.lha" "--const 'L=7,L2=11' --prism");
  ("N3P2_L7L11", "MAPK.grml" "RareEventUntil2.lha" "--const 'L=7,L2=11' -r");
  ("N3P2_L8L11", "MAPK_P2Reduit.grml" "RareEventUntil.lha" "--const 'L=8,L2=11' --prism");
  ("N3P2_L8L11", "MAPK.grml" "RareEventUntil2.lha" "--const 'L=8,L2=11' -r");
  ("N3P2_L9L11", "MAPK_P2Reduit.grml" "RareEventUntil.lha" "--const 'L=9,L2=11' --prism");
  ("N3P2_L9L11", "MAPK.grml" "RareEventUntil2.lha" "--const 'L=9,L2=11' -r");
  ("N3P2_L10L11", "MAPK_P2Reduit.grml" "RareEventUntil.lha" "--const 'L=10,L2=11' --prism");
  ("N3P2_L10L11", "MAPK.grml" "RareEventUntil2.lha" "--const 'L=10,L2=11' -r");
]*)

let joblist2 = [
  ("N2P3_L2", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=2,N=2'");
  ("N2P3_L3", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=3,N=2'");
  ("N2P3_L4", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=4,N=2'");
  ("N2P3_L5", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=5,N=2'");
  ("N2P3_L6", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=6,N=2'");
  ("N2P3_L7", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=7,N=2'");

  ("N3P3_L6", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=6,N=3'");
  ("N3P3_L7", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=7,N=3'");
  ("N3P3_L8", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=8,N=3'");
  ("N3P3_L9", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=9,N=3'");
  ("N3P3_L10", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=10,N=3'");
  ("N3P3_L11", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=11,N=3'");

  ("N4P3_L8", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=8,N=4'");
  ("N4P3_L9", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=9,N=4'");
  ("N4P3_L10", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=10,N=4'");
  ("N4P3_L11", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=11,N=4'");
  ("N4P3_L12", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=12,N=4'");
  ("N4P3_L13", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=13,N=4'");
  ("N4P3_L14", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=14,N=4'");
  ("N4P3_L15", "MAPK.grml", "Prop3/prop3.lha", "--const 'L=15,N=4'");
]


let add_option (x,y,z,t) =
  execSavedCosmos ~prefix:false (x,y,z,t^" --max-run 1000000 --njob 6")


let _ =
  List.iter add_option joblist2
