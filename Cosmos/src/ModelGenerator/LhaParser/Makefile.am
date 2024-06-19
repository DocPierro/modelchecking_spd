
noinst_LTLIBRARIES = liblha.la

liblha_la_SOURCES = \
	Lha-Reader.cpp Lha-Reader.hpp \
	Lha-scanner.ll Lha-parser.yy \
	Lha_gmlparser.cpp Lha_gmlparser.hpp

BUILT_SOURCES = Lha-parser.cc

liblha_la_LIBADD = ../../libgrml/libgrml.la ../Eval/libeval.la

EXTRA_DIST = Lha-parser.hh location.hh position.hh stack.hh
 
AM_YFLAGS = -y -p lha
AM_CPPFLAGS = -I$(srcdir)/../../libgrml -I../Eval -I$(srcdir)/../Eval -I../GspnParser -I$(srcdir)/../GspnParser -I$(srcdir)/../../Simulator $(BOOST_CPPFLAGS) -Wall -Wextra -Wno-unused -Wno-deprecated -Wno-write-strings -Wno-unneeded-internal-declaration
AM_LFLAGS = -o$(LEX_OUTPUT_ROOT).c --prefix=lha
