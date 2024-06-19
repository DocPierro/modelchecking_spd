bin_SCRIPTS = modelConvert

#modelConvert: generator.ml lexer.mll parser.mly type.ml
#	cd $(srcdir); $(OCAMLBUILD) -libs unix,str prism2SPT.native
#	cp $(srcdir)/prism2SPT.native modelConvert

modelConvert: DataFile.ml PetriNet.ml StochasticPetriNet.ml pnmlparser.ml \
	prism2SPT.ml stateflowType.ml stateflowTrans.ml simulinkparser.ml modelConvert.ml \
	stateflowparser.ml type.ml parserSimulEdge.mly lexerSimulEdge.mll simulinkTrans.ml \
	CosmosSimulink.ml simulinkreader.ml\
	Prism/prismTrans.ml Prism/prismType.ml Prism/parserPrism.mly Prism/lexerPrism.mll \
	Prism/prelexer.mll Prism/preparser.mly Prism/prismTrans.ml prism2SPT.ml \
	ClessParser/cless_ast.ml _tags
	cd $(srcdir); $(OCAMLBUILD) -use-ocamlfind -I ClessParser -I Prism -I Semantics prism2SPT.native modelConvert.native CosmosSimulink.native
	cp $(srcdir)/modelConvert.native modelConvert
	cp $(srcdir)/CosmosSimulink.native CosmosSimulink

EXTRA_DIST = DataFile.ml PetriNet.ml StochasticPetriNet.ml Prism/prismTrans.ml pnmlparser.ml\
	prism2SPT.ml Prism/prismType.ml stateflowType.ml simulinkparser.ml stateflowparser.ml simulinkTrans.ml stateflowTrans.ml\
	CosmosSimulink.ml simulinkreader.ml\
	type.ml Prism/parserPrism.mly parserSimulEdge.mly Prism/lexerPrism.mll lexerSimulEdge.mll\
	Prism/prelexer.mll Prism/preparser.mly StochPTPrinter.ml GrMLParser.ml StochasticSymmetricNet.ml\
	ClessParser/cless_ast.ml  ClessParser/cless.ml ClessParser/cless.mli ClessParser/cless_lexer.mll ClessParser/cless_parser.mly\
	Semantics/Simulation.ml Semantics/SemanticPT.ml Semantics/MDPStrategy.ml Semantics/LTS.ml Semantics/MarkovChains.ml Prism/LoadAdversary.ml\
	PlainStochasticPetriNet.ml cless2PT.ml modelConvert.ml _tags build.sh

CLEANFILES = _build/* $(bin_SCRIPTS) *~

