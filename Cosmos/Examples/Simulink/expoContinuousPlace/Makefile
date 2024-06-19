all: compile play

compile:
	CosmosSimulink exponential.slx

play:
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --max-run 1 -v 3 --tmp-status 2 --loop 2 --use-domainset

interactif:
	Cosmos SKModel.grml --use-domainset --tmp-status 2 -i

clean:
	rm SKModel* *.hpp *.grml trace.data*
