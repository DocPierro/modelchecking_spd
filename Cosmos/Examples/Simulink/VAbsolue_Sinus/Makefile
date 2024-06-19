all: compile play

compile:
	CosmosSimulink SinusAbsolue.slx

play:
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --max-run 1 -v 3 --tmp-status 2 --loop 10

clean:
	rm SKModel* *.hpp *.grml trace.data*
