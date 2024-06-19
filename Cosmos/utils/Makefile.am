bin_SCRIPTS = linecombinator sptToPt ptToSpt sptToSwn

PACKAGES = 
LIBS     = 

linecombinator: linecombinator.ml
	cd $(srcdir); $(OCAMLBUILD) -libs str,unix linecombinator.native
	cp $(srcdir)/linecombinator.native linecombinator

EXTRA_DIST = linecombinator.ml sptToPt ptToSpt mlcall.ml sptToSwn

CLEANFILES = _build/* $(bin_SCRIPTS) *~

