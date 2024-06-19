all: compile play

compile:
	testSimulink controller.slx

compilehys:
	testSimulink controller_hysteresis.slx

play:
	touch trace.data
	rm trace.data*
	cp SKModel_manuel.grml SKModel.grml
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --max-run 1 -v 3 --tmp-status 2 --loop 50

sampling:
	Cosmos SKModel.grml --output-trace trace.data 0 --gnuplot-driver svg --sampling 50 0.5

interact:
	cp SKModel_manuel.grml SKModel.grml
	Cosmos SKModel.grml -i --loop 50

debug:
	cp SKModel_manuel.grml SKModel.grml
	Cosmos SKModel.grml -v 6

clean:
	rm SKModel* *.hpp *.grml trace.data*
