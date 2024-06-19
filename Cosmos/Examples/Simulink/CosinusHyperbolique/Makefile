all: compile play

compile:
	CosmosSimulink CosHyperbolique.slx

play:
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --max-run 1 -v 3 --loop 10 --tmp-status 2

clean:
	rm SKModel* *.hpp *.grml trace.data*
