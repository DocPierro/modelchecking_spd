all: compile play

compile:
	CosmosSimulink IntegratorGrowingTooFast.slx

play:
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --max-run 1 -v 3 --tmp-status 2 --loop 0.72108

interact:
	Cosmos SKModel.grml -i

clean:
	rm SKModel* *.hpp *.grml trace.data*
