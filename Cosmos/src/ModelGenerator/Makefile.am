
bin_PROGRAMS = Cosmos


Cosmos_SOURCES = \
	../Simulator/BatchR.cpp ../Simulator/BatchR.hpp \
	SimpleSerializer.cpp SimpleSerializer.hpp \
	casesWriter.cpp casesWriter.hpp \
	parameters.cpp parameters.hpp \
	result.cpp result.hpp \
	server.cpp server.hpp \
	HaslFormula.cpp HaslFormula.hpp \
	Generator.cpp Generator.hpp \
    	expressionStruct.cpp expressionStruct.hpp \
	Cosmos.cpp

Cosmos_LDADD = ../libgrml/libgrml.la Eval/libeval.la GspnParser/libgspn.la LhaParser/liblha.la

AM_CPPFLAGS = -I$(srcdir)/../libgrml -I$(srcdir)/Eval -I../libgrml -IEval -IGspnParser -ILhaParser -I$(srcdir)/../Simulator $(BOOST_CPPFLAGS) $(PTHREAD_LIBS) -DGIT_REVISION=\"`git describe --abbrev=6 --dirty --always --tags`\"  -DCPP_COMPILER=\"$(CXX)\" -DBOOST_PATH=\"$(BOOST_CPPFLAGS)\" -DBOOST_LIB="\"\"" -Wall -Wextra -Wno-unused-local-typedef -Wno-unused-parameter

#-DCXXFLAGS=\"$(CXXFLAGS)\"

AUTOMAKE_OPTIONS = subdir-objects

includesdir = $(includedir)
includes_HEADERS = parameters.hpp HaslFormula.hpp expressionStruct.hpp SimpleSerializer.hpp

AM_LDFLAGS = $(PTHREAD_LIBS)

parameters.o: $(wildcard $(srcdir)/../**/*pp)

FORCE:
