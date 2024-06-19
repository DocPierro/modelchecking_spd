all: compile play

compile:
	CosmosSimulink SinusAbsolueIntegree.slx

play:
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --max-run 1 -v 3 --tmp-status 2 --loop 8

interact:
	Cosmos SKModel.grml -i --max-run 1 --loop 8

clean:
	rm SKModel* *.hpp *.grml trace.data*
