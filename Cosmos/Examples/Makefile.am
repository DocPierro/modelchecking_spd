

all:
	(cd TandemQN && ocaml test.ml $(INTC))
	(cd pi && ocaml test.ml $(INTC))
	(cd testDistribution && ocaml test.ml $(INTC))
	(cd Workflow-Net && ocaml test.ml $(INTC))
	(cd QueueMM1 && ocaml test.ml $(INTC))
	(cd signallingCascade && ocaml test.ml $(INTC))
	(cd DataBase && ocaml test.ml $(INTC))
	(cd RareEventUnbounded && ocaml test.ml $(INTC))
#	(cd RareEventUnbounded && ocaml test2.ml $(INTC))
#	(cd TandemCosy && ocaml testPrism.ml $(INTC))
	(cd SharedMemory && ocaml test.ml $(INTC))
	(cd RareEventU && ocaml test.ml $(INTC))
	(cd DNAWalker && ocaml test.ml $(INTC))
	(cd Philosophers && ocaml test.ml $(INTC))


bash:
	ocaml bench.ml

coverage:
	(cd TandemQN && ocaml testcoverage.ml bash cosmos_test_log)