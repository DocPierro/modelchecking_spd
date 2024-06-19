all: compile play

compile:
	CosmosSimulink DoubleThermostat.slx

play:
	cp SKModel_edite.grml SKModel.grml
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --max-run 1 -v 3 --tmp-status 2 --loop 200

playsanspanne:
	cp SKModel_sanspanne.grml SKModel.grml
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --max-run 1 -v 3 --tmp-status 2 --loop 200

playwo:
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --max-run 1 -v 3 --tmp-status 2 --loop 200

playmin:
	cp SKModel_edite.grml SKModel.grml
	Cosmos SKModel.grml --loop 20 --HASL-expression "AVG(Min(Temp));" --trace-place "" --tmp-status 2 --batch 1

playmax:
	cp SKModel_edite.grml SKModel.grml
	Cosmos SKModel.grml --loop 20 --HASL-expression "AVG(Max(Temp));" --trace-place "" --tmp-status 2 --batch 1

basic: compile
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --max-run 1 -v 3 --tmp-status 2 --loop 200

compilediff:
	CosmosSimulink DoubleThermostat_diff.slx

playdiff:
	cp SKModel_edite.grml SKModel.grml
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --max-run 1 -v 3 --tmp-status 2 --loop 200

diff: compilediff playdiff
