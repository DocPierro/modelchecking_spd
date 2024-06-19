bin_SCRIPTS = automataGen

PACKAGES = 
LIBS     = 

automataGen: automata.ml generator.ml lexer.mll parser.mly type.ml writer.ml
	cd $(srcdir); $(OCAMLBUILD) generator.native
	cp $(srcdir)/generator.native automataGen

EXTRA_DIST = automata.ml generator.ml lexer.mll parser.mly type.ml writer.ml

CLEANFILES = _build/* $(bin_SCRIPTS) *~

