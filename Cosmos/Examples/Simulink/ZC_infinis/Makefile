all: compile play

compile:
	CosmosSimulink ZC_infinis.slx

compiledbg:
	CosmosSimulink ZC_infinis.slx -v 4

play:
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver png --max-run 1 -v 3 --tmp-status 2 --loop 2

interact:
	Cosmos SKModel.grml -i

clean:
	rm SKModel* *.hpp *.grml trace.data*
