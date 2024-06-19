dtmc

module E1 
	s: [0..2] init 1 ; 
	[] s= 0 ->  (s'=0); 
	[] s= 1 -> 0.5 : (s'=0) + 0.5 : (s'=2) ; 
	[] s= 2 ->  (s'=2); 
endmodule