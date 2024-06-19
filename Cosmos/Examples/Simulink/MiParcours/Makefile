all: compile play

compile:
	CosmosSimulink exemple.slx

compiledbg:
	CosmosSimulink exemple.slx -v 4

play:
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver png --max-run 1 -v 3 --tmp-status 2 --loop 4

clean:
	rm SKModel* *.hpp *.grml trace.data*
