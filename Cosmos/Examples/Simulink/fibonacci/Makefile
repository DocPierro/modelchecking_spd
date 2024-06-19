all: compile play

compile:
	CosmosSimulink fibonacci.slx

play:
	touch trace.data
	rm trace.data*
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --max-run 1 -v 3 --tmp-status 2 --loop 9

interact:
	Cosmos SKModel.grml --loop 9 -i

clean:
	rm SKModel* *.hpp *.grml trace.data*
