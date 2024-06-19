dtmc

const int QMAX = 10;

module Serveur

	sr : [0..1] init 0;
	
	[tick] 	sr=0 ->0.5:(sr'=0) + 0.5 : (sr'=1);
	[tick] 		sr=1 -> 0.5: (sr'=0) + 0.5: (sr'=1);

endmodule

module FileAttente

	q : [0..QMAX] init 0;
	
	[tick] sr=1 & q= 0 -> 0.5: (q'=q)  + 0.5 : (q' = 0);
	[tick] sr=1 & q>0  -> 0.5 : (q' = q-1) + 0.5 : (q'= q);
	[tick] sr= 0 & q<QMAX -> 0.5: (q'=q) + 0.5: (q'=q+1) ;
	[tick ] q=QMAX ->  (q'=q) ;
endmodule

