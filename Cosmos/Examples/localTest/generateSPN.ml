open Printf

let n = int_of_string Sys.argv.(1)

let fo = open_out Sys.argv.(2)

let _ =
	fprintf fo "NbPlaces = %i;\nNbTransitions = %i;\n" (2*n+1) (n+1);

	fprintf fo "PlacesList={end ";
	for i=1 to n do
		fprintf fo ", Plit%i, Plbuff%i" i i
	done;
	fprintf fo "};\nTransitionsList = { endtr ";
	for i=1 to n do
		fprintf fo ", tr%i" i
	done;
	fprintf fo "};\nMarking = {\n(Plit1 , 1) ; (Plbuff1 , 0);\n";
	for i=2 to n do
		fprintf fo "(Plit%i , 0) ; (Plbuff%i , 0) ;\n" i i
	done;
	fprintf fo "};\nTransitions = {\n";
	for i=1 to n do
		fprintf fo "(tr%i , EXPONENTIAL(1.0),1,1,SINGLE);\n" i
	done;
	fprintf fo "(endtr , EXPONENTIAL(1.0),1,1,SINGLE);\n};\nInArcs = {\n";
	for i=1 to n do
		fprintf fo "(Plit%i , tr%i,1);\n" i i;
		fprintf fo "(Plbuff%i , endtr,1);\n" i;
	done;
	fprintf fo "};\nOutArcs = {\n";
	for i=1 to n-1 do
		fprintf fo "(tr%i , Plbuff%i,1);\n" i i;
		fprintf fo "(tr%i , Plit%i,1);\n" i (i+1);
	done;
	fprintf fo "(tr%i , Plbuff%i,1);(endtr , end,1);\n};" n n;


(*
 
};

Transitions={
(Sampx,UNIFORM(0,1),1,1); 

(Sampy,UNIFORM(0,1),1,1);

(fin,IMMEDIATE,1,1);
};

InArcs={
(P1,Sampx,1);
(P2,Sampy,1);
(P3,fin,1);
};

OutArcs={
(Sampx,P2,1);
(Sampy,P3,1);
};

*)