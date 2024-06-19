const int mrep = 10;
const bool sleep = false;

formula swap  = (c=2 |c=4|c=6 |c=8 | c=10 |c=12| c=14 |c=16); // receiver swaps trains at end of sequence only when c is even
formula swap2  = ((c=2 |c=4|c=6 |c=8 | c=10 |c=12| c=14 ) & freq=c+1) | (c=16 & freq=1) | ((c=1 |c=3|c=5 |c=7 | c=9 |c=11| c=13 |c=15) & freq!=c+1); // receiver swaps trains when changing frequency set (when receiver sleeps)
formula sleep = (receiver=0 & y1=1); // state where reciever's next time step corresponds to the whole of scan and sleep (scan interval)  
formula hear  = (freq1=freq & train1=train & send=1); // when the receiver hears something

module sender
	
	// still should try some different orderings?
	
	send : [1..3];   // 1 sending and 2,3 receiving
	freq  : [1..16]; // current frequency mod 16 freq+train*16
	train : [0..1];  // used to work out the frequency (actual frequency equals freq+train*16)
	c : [1..16]; // used to work out the trains
	rep : [1..mrep]; // no of repetitions of a train
	
	// sending
	[time] !sleep & send=1 & (freq=1 | freq=3| freq=5| freq=7| freq=9| freq=11| freq=13| freq=15) & freq!=c -> (freq'=freq+1);
	[time] !sleep & send=1 & (freq=1| freq=3| freq=5| freq=7| freq=9| freq=11| freq=13| freq=15) & freq=c  -> (freq'=freq+1) & (train'=1-train);
	[time] !sleep & send=1 & (freq=2| freq=4| freq=6| freq=8| freq=10| freq=12| freq=14| freq=16) -> (send'=2);
	// receiving
	[time] !sleep & send=2 -> (send'=3);
	[time] !sleep & send=3 & freq<16 & freq!=c          -> (send'=1) & (freq'=freq+1);
	[time] !sleep & send=3 & freq<16 & freq=c           -> (send'=1) & (freq'=freq+1) & (train'=1-train);
	[time] !sleep & send=3 & freq=16 & rep<mrep & c!=16 -> (send'=1) & (freq'=1) & (train'=1-train) & (rep'=rep+1); 
	[time] !sleep & send=3 & freq=16 & rep<mrep & c=16  -> (send'=1) & (freq'=1) & (rep'=rep+1);
	[time] !sleep & send=3 & freq=16 & rep=mrep         -> (send'=1) & (freq'=1) & (train'=swap?1-train:train) & (c'=c=16?1:c+1) & (rep'=1);
	// big time step (2048 slots = 64 repetitions)
	[time] sleep & rep<=64 -> (rep'=rep+64); // sleeping does not change frequency set
	[time] sleep & rep>64  -> (rep'=rep-64) & (c'=c=16?1:c+1) & (train'=swap2?1-train:train); // sleeping changes current frequency set
	
endmodule
