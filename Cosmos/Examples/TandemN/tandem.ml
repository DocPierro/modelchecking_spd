let automataGenPath = "automataGen"

let print_tr f name id rate =
  Printf.fprintf f "  <node id=\"%i\" nodeType=\"transition\">
    <attribute name=\"name\">%s</attribute>
    <attribute name=\"distribution\">
      <attribute name=\"type\">
        EXPONENTIAL
      </attribute>
      <attribute name=\"param\">
        <attribute name=\"number\">0</attribute>
        <attribute name=\"expr\"><attribute name=\"name\">
          %s
        </attribute></attribute>
      </attribute>
    </attribute>
  </node>\n" !id name rate;
  incr id;;

let print_pl f name id tok =
  Printf.fprintf f "  <node id=\"%i\" nodeType=\"place\">
    <attribute name=\"name\">%s</attribute>
    <attribute name=\"marking\">
      <attribute name=\"expr\"><attribute name=\"numValue\">
        %i
      </attribute></attribute>
    </attribute>
  </node>\n" !id name tok;
  incr id;;

let print_arc f id source target valuation inhib =
  let arctype = if inhib then "inhibitorarc" else "arc" in
  Printf.fprintf f "  <arc id=\"%i\" arcType=\"%s\" source=\"%i\" target=\"%i\">
    <attribute name=\"valuation\">
      <attribute name=\"expr\"><attribute name=\"numValue\">
        %i
      </attribute></attribute>
    </attribute>
  </arc>\n" !id arctype source target valuation; 
  incr id;;

let gen_const f li lr =
  Printf.fprintf f "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<model formalismUrl=\"http://formalisms.cosyverif.org/sptgd-net.fml\" xmlns=\"http://cosyverif.org/ns/model\">
  <attribute name=\"declaration\"><attribute name=\"constants\">
    <attribute name=\"intConsts\">\n";
  List.iter (fun (n,v) -> 
     Printf.fprintf f "      <attribute name=\"intConst\">
        <attribute name=\"name\">%s</attribute>
        <attribute name=\"expr\"><attribute name=\"numValue\">
            %i
        </attribute></attribute>
      </attribute>\n" n v) li;
  
  Printf.fprintf f "    </attribute>
    <attribute name=\"realConsts\">\n";
  List.iter (fun (n,v) -> 
    Printf.fprintf f "      <attribute name=\"realConst\">
        <attribute name=\"name\">%s</attribute>
        <attribute name=\"expr\"><attribute name=\"numValue\">
            %f
        </attribute></attribute>
      </attribute>\n" n v) lr;
    Printf.fprintf f
      "     </attribute>
    </attribute>
  </attribute>\n"

let generate_spn fpath n n2 lambda rho r genDTMC=
  let f = open_out fpath in
  let countid = ref 0 in
  let li = [("r",r); ("Nmax",n2)]
  and lr = ref ["lambda",lambda] in
  for i = 1 to n do
    lr := (("rho"^string_of_int i),rho i):: !lr;
  done;
  gen_const f li !lr;

  for i =1 to n do
    print_pl f ("RE_Queue"^(string_of_int i)) countid (if i=1 then 1 else 0);
  done;

  print_tr f "Arrive" countid "lambda";

  for i =1 to n do
    print_tr f ("Serve"^(string_of_int i)) countid ("rho"^(string_of_int i));
  done;
  
  if genDTMC then for i =1 to n do
      print_tr f ("AServe"^(string_of_int i)) countid ("rho"^(string_of_int i));
    done;

  for i =0 to n-1 do
    print_arc f countid (n+i) i 1 false;
    print_arc f countid i (n+i+1) 1 false;
  done;
  
  if genDTMC then for i =0 to n-1 do
      print_arc f countid i (2*n+i+1) 1 true;
    done;
  Printf.fprintf f "</model>";
  close_out f;;

let generate_reduce_spn fpath n lambda rho r =
  let f = open_out fpath in
  let countid = ref 0 in
  let li = [("r",r)]
  and lr = ref [("lambda"),lambda] in
  for i = 1 to n do
    lr := (("rho"^string_of_int i),rho i):: !lr;
  done;
  gen_const f li !lr;

  for i =1 to n do
    print_pl f ("RE_Queue"^(string_of_int i)) countid (if i=1 then 1 else 0);
  done;

  print_tr f "Arrive" countid "lambda";

  for i =1 to n do
    print_tr f ("Serve"^(string_of_int i)) countid ("rho"^(string_of_int i));
  done;

  for i =2 to n do
    print_pl f ("AQueue"^(string_of_int i)) countid r;
  done;

  for i =0 to n-1 do
    print_arc f countid (n+i) i 1 false ;(*post*)
    print_arc f countid i (n+i+1) 1 false ;(*pre*)
    if i>=1 then (
    print_arc f countid (2*n+i) (n+i) 1 false ; (*pre*)
    print_arc f countid (n+i+1) (2*n+i) 1 false; (*post*)
    )
  done;
  Printf.fprintf f "</model>";
  close_out f;;


let rec gen_queue n =
  if n = 1 then "RE_Queue1"
  else (gen_queue (n-1))^"+RE_Queue"^(string_of_int n);;

let generate_marcie marciepath n lambda rho r n2 =
  let f = open_out marciepath in
  Printf.fprintf f "spn [Tandem%i]\n{\nconstants:
  int N = %i;
  int N2 =%i;
  double lambda = %f;
  double rho = %f;\n" n n n2 lambda (rho 1);
  Printf.fprintf f "places:\n  RE_Queue1 = 1;
  AQueues = %i;\n" (n2-1);
  for i = 2 to n do
    Printf.fprintf f "  RE_Queue%i = 0;\n" i;
  done;
  Printf.fprintf f "transitions:\n  Arrive : : [AQueues -1] & [RE_Queue1 +1] : lambda;\n";
  for i = 1 to (n-1) do
    Printf.fprintf f "  Serve%i : : [RE_Queue%i -1] & [RE_Queue%i +1] : %f;\n" i i (i+1) (rho i);
  done;
  Printf.fprintf f "  Serve%i : : [RE_Queue%i -1] & [AQueues+1] : %f;\n}\n" n n (rho n);
  close_out f;;
  
let generate_csl cslpath n n2 =
  let f = open_out cslpath in
  let queues = gen_queue n in
  (*Printf.fprintf f "const int n=%i;\n" n2;*)
  Printf.fprintf f "P=?[[ %s >0 ] U [ %s = %i ]]\n" queues queues n2;
  close_out f;;

let generate_lha lhapath n n2=
  let queues = gen_queue n in
  let f = open_out lhapath in
  Printf.fprintf f "VariablesList = {vc0};
LocationsList = {l0,l1,l2};
Probability=PROB;
LikelyhoodProb = AVG(Last(vc0));
InitialLocations = { l0 };
FinalLocations = {l1};
Locations = {
(l0,((%s)>0 & (%s)<%i));
(l1,(%s)=%i);
(l2,(%s)=0);
};
Edges = {
((l0,l0),ALL,#,#);
((l0,l1),ALL,#,{vc0=1});
((l0,l2),ALL,#,{vc0=0});
};" queues queues n2 queues n2 queues;
  close_out f;;

let generate_lumping_fun lumppath n =
  let f = open_out lumppath in
  Printf.fprintf f "#include \"markingImpl.hpp\"
void SPN::print_state(const vector<int> &vect){
}
bool SPN::precondition(const abstractMarking &Marking){return true;}

void SPN::lumpingFun(const abstractMarking &Marking,vector<int> &vect){\n";
  for i =1 to n do 
    Printf.fprintf f 
      "\tvect[%i] = Marking.P->_PL_RE_Queue%i;\n" (i-1) i;
  done;
  Printf.fprintf f
    "\tint reste =0;
    for (int i=vect.size()-1; i>0; i--) {
        if(vect[i]>r){
            reste += vect[i]-r ;
            vect[i]=r;
        } else if(vect[i]+reste>r){
            reste -= (r - vect[i]);
            vect[i]=r;
        } else {
            vect[i] += reste;
            reste =0;
        }
    }
    vect[0] += reste;
}";
  close_out f;;

let generate_prism prismpath n lambda rho r n2 = 
  let f = open_out prismpath in
  Printf.fprintf f "ctmc
const int n = %i;
const int n2= %i;
const int r = %i;

const double lambda = %f;
const double rho = %f;\n" n n2 r lambda (rho 1);

  Printf.fprintf f "module Tandem
\tRE_Queue1 : [0..n2] init 1;\n";
  for i =2 to n do 
    Printf.fprintf f "\tRE_Queue%i : [0..r] init 0;\n" i;
  done;
  Printf.fprintf f "\t[] %s<n2 -> lambda : (RE_Queue1'=RE_Queue1+1);\n" (gen_queue n);
  for i =2 to n do 
    Printf.fprintf f "\t[] RE_Queue%i>0 & RE_Queue%i<r -> %f : (RE_Queue%i'=RE_Queue%i-1) & (RE_Queue%i'=RE_Queue%i+1);\n" (i-1) i (rho i) (i-1) (i-1) i i;
  done;
  Printf.fprintf f "\t[] RE_Queue%i>0 -> %f : (RE_Queue%i'=RE_Queue%i-1);\n" n (rho n) n n;
  Printf.fprintf f "endmodule

formula win = %s = n2;
formula loose = %s = 0;" (gen_queue n) (gen_queue n) ;
close_out f;;



#use "../testTeamCity.ml";;

let generate n n2 r mu rho = 
  Printf.printf "Generate n=%i n2=%i r=%i mu=%f" n n2 r mu;
  for i =1 to n do Printf.printf " rho%i=%f" i (rho i) done;
  print_newline ();
  generate_spn "tandem.grml" n n2 mu rho r false;
  generate_spn "tandemDTMC.grml" n n2 mu rho r true;
  generate_marcie "tandem.andl" n mu rho r n2;
  generate_csl "tandem.csl" n n2;
  generate_reduce_spn "tandemRE.grml" n mu rho r;
  generate_lha "tandem.lha" n n2;
  generate_lumping_fun "lumpingfun.cpp" n;
  generate_prism "tandem.sm" n mu rho r n2;;

let level = ref 0.99;;

let simule n n2 r mu rho batch =
  generate n n2 r mu rho;
  Printf.printf "Simulate n=%i n2=%i r=%i" n n2 r;
  print_newline ();
  execSavedCosmos ~prefix:false ("ReduceModel","tandemRE.grml","tandem.lha"," --prism --prism-path /Applications/Prism.app/Contents/MacOs/prism");
  Sys.command "/usr/bin/time -lp prism -v tandem.sm prismtandem.csl > prismout";
  Sys.command ("ocaml traduittable.ml prismout muFile "^(string_of_int n));
  execSavedCosmos ~prefix:false ("ImportantSampling","tandem.grml","tandem.lha",Printf.sprintf " -r --width 0 --max-run %i --level %f" batch !level );;




(*simule 1 50 5;;
simule 2 50 5;;
simule 3 50 5;;
simule 4 50 5;;
simule 5 50 5;;
simule 6 50 5;;
simule 7 50 5;;
simule 8 50 5;;
simule 9 50 5;;
simule 10 50 5;;
*)

assert (Array.length Sys.argv>3);;
let n = int_of_string Sys.argv.(1);;
let n2 = int_of_string Sys.argv.(2);;
let r = int_of_string Sys.argv.(3);;
assert(n>=1);;

generate n n2 r 0.05 (fun _-> 0.45);;
simule n n2 r 0.01 (fun _-> 0.11);;

