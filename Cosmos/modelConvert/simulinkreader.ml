open Xml
open Type

let verbose = ref 0;;
let epsV = ref 0.00001;;
let skBaseStep = ref None;;


let rec parseSimulinkVersion liste = function
	| Element ("cp:version",[],[PCData(ver)]) -> ver::liste
	| Element (name,alist,clist) -> List.fold_left parseSimulinkVersion liste clist
	| PCData(autre) -> liste;;

let getSimulinkVersion zip =
	let aux = function
	[] -> (Printf.eprintf "[FATAL:] No version specified in the SLX file or wrong format.\n";
	        exit 1;)
	| [ver] -> ver
	| a::b -> (Printf.eprintf "[FATAL:] Found several Matlab versions in the SLX file. Wrong format?\n";
	            exit 1;)
	in
	try
	zip
	|> (fun z -> (Zip.find_entry z "metadata/coreProperties.xml")
	    |> Zip.read_entry z)
	|> Xml.parse_string
	|> parseSimulinkVersion []
	|> aux
	with Not_found ->
	(Printf.eprintf "[FATAL:] No metadata/coreProperties.xml found in the SLX archive.";
	exit 1;)
	;;

let rec getDefaults2017b liste = function
	| Element ("BlockDiagramDefaults",alist,clist) -> List.fold_left getDefaults2017b liste clist
	| Element ("BlockParameterDefaults",alist,clist) as element ->
		let (_,def,_) = Simulinkparser.blockparams_of_simulink ([],[],[]) element in
			def
	| Element (name,alist,clist) -> List.fold_left getDefaults2017b liste clist
	| PCData(autre) -> liste;;

let readSimulink zip =
	let skVersion = getSimulinkVersion zip
	in
	if (!verbose > 5) then Printf.eprintf "[INFO:] Parsing a Simulink file from MATLAB %s\n" skVersion;
	if (skVersion > "R2016b") then Printf.eprintf "[WARNING:] MATLAB %s Simulink files are possibly not compatible with CosmosSimulink. Several errors may occur.\n" skVersion;

    let skSettings = match skVersion with
    ver -> (* OLDER VERSIONS *) begin
		zip
		|> (fun z -> (
			try Zip.find_entry z "simulink/configSet0.xml"
			with Not_found -> Zip.find_entry z "simulink/configSetInfo.xml")
			|> Zip.read_entry z)
		|> Xml.parse_string
		|> Simulinkparser.getSettings []
		end
	in
	Simulinkparser.verbose := !verbose;
	SimulinkTrans.verbose := !verbose;
	SimulinkTrans.epsV := !epsV;
	SimulinkTrans.skBaseStep := !skBaseStep;

	(** Getting default parameters **)
	let listeDefaults = match skVersion with
	ver when ver > "R2016z" -> begin
		zip
		|> (fun z -> (Zip.find_entry z "simulink/bddefaults.xml"
					|> Zip.read_entry z))
		|> Xml.parse_string
		|> getDefaults2017b []
	end
	| ver -> []
	in

	zip
	|> (fun z -> (
		try Zip.find_entry z "simulink/blockdiagram.xml"
		with Not_found -> Zip.find_entry z "simulink/stateflow.xml")
		|> Zip.read_entry z)
	|> Xml.parse_string
	|> Simulinkparser.blocklist_of_tree ([], listeDefaults, [])
	|> Simulinkparser.pushDefaults
	|> Simulinkparser.computeLatencies skSettings
	|> SimulinkTrans.topologicSort
	|> SimulinkTrans.tagContinuous
	|< SimulinkTrans.generateCode skSettings
