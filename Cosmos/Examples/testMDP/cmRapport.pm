dtmc

module teste

	s : [0..3] init 0;
	
	[] s=0 -> 0.5 : (s'=1) + 0.5 : (s'=2);
	[] s=1 -> 1: (s'=3) ; 
	[] s=2 -> 1: (s'=3) ; 
	[] s=3 -> 1 : (s'=0);
	
	
endmodule

rewards "steps"
	true : 1;
endrewards

