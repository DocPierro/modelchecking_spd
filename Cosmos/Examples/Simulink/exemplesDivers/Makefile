all: compile play

ex12102017:
	CosmosSimulink exple_12102017.slx

ex12102017dbg:
	CosmosSimulink -v 6 exple_12102017.slx

ex12102017str:
	CosmosSimulink exple_12102017_strict.slx

ex12102017strdbg:
	CosmosSimulink -v 6 exple_12102017_strict.slx

ex12102017disc:
	CosmosSimulink exple_12102017_discrete.slx

ex12102017strdisc:
	CosmosSimulink exple_12102017_strict_discrete.slx

play:
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --loop 2 --max-run 1 -v 3 --tmp-status 2

interact:
	Cosmos SKModel.grml -i

clean:
	rm SKModel* *.hpp *.grml trace.data*
