all: compile play

compile:
	CosmosSimulink Sinus_yt.slx

dbg:
	CosmosSimulink Sinus_yt.slx -v 4

play:
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --loop 2 --max-run 1 -v 3 --tmp-status 2

interact:
	Cosmos SKModel.grml -i

clean:
	rm SKModel* *.hpp *.grml trace.data*
