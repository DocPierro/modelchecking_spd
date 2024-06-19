all: compile play

compile:
	CosmosSimulink discrete_test.slx

dbg:
	CosmosSimulink -v 6 discrete_test.slx

play:
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --max-run 1 -v 3 --tmp-status 2 --loop 10

benchmark: compile
	Cosmos SKModel.grml --max-run 10000 --batch 10000 --loop 10

clean:
	rm SKModel* *.hpp *.grml trace.data*
