open Type
open PetriNet

type colouredMarking = Var of string
		     | SingleColor of string
		     | Real of float
		     | All of string
       
type placetypeSSN = Ordinary of int Type.expr'
                  | Coloured of string*string 
type valuationtypeSSN = OrdinaryVal of int Type.expr'
                      | ColouredVal of string
					 
type sspt = 
  ( placetypeSSN,  (* Initial Marking type and bound on place*)
    StochasticPetriNet.transitiontypeSPT , (* Label of transitions type *)
   valuationtypeSSN , (* valuation of arcs *)
   StochasticPetriNet.declarationSPT (* Initialization *)
  ) Net.t;;

let singleToken d =
  let tokProf =
    match d with
      Var v -> "<attribute name=\"name\">"^v^"</attribute>"
    | SingleColor sc -> sc
    | Real r -> Printf.sprintf "<attribute name=\"realConst\">%f</attribute>" r
    | All v -> "<attribute name=\"function\">
              <attribute name=\"all\"><attribute name=\"type\">"^v^"</attribute></attribute>
            </attribute>"
    in
    Printf.sprintf "<attribute name=\"token\">
            <attribute name=\"occurs\">
                <attribute name=\"intValue\">
                    1
                </attribute>
            </attribute>
            <attribute name=\"tokenProfile\">
                <attribute name=\"expr\">
                   %s
                </attribute>
            </attribute>
		  </attribute>" tokProf

let singleTokenInPlace dom d =
  Coloured(dom,singleToken (Real d))
		 
let print_pl f name id tok =
    Printf.fprintf f "  <node id=\"%i\" nodeType=\"place\">
    <attribute name=\"name\">%s</attribute>" id name;
    begin match tok with
      Ordinary(i) ->
      Printf.fprintf f "<attribute name=\"marking\"><attribute name=\"expr\">
      %a
            </attribute></attribute>\n" StochPTPrinter.print_int_expr i
    | Coloured(domain,mark) ->
       Printf.fprintf f "<attribute name=\"domain\"><attribute name=\"type\">
        %s
                             </attribute></attribute>\n" domain;
       Printf.fprintf f "<attribute name=\"marking\">%s</attribute>\n" mark
    end;
    output_string f "</node>\n"

let print_arc f id source target valuation inhib =
  let arctype = if inhib then "inhibitorarc" else "arc" in
  Printf.fprintf f "  <arc id=\"%i\" arcType=\"%s\" source=\"%i\" target=\"%i\">
                    <attribute name=\"valuation\">" id arctype source target;
  begin match valuation with
    OrdinaryVal(i) -> Printf.fprintf f "<attribute name=\"expr\">
        %a
    </attribute>" StochPTPrinter.print_int_expr i
  | ColouredVal(s) -> output_string f s
  end;
  output_string f "</attribute>\n</arc>\n"
  
let print_ssn ?initid:(initid=0) fpath (net:sspt) =
  StochPTPrinter.print_general_pt ~initid fpath print_pl StochPTPrinter.print_tr print_arc net
  
