
lib_LTLIBRARIES = libClientSim.la libClientSimMain.la libClientSimBase.la libClientSimSK.la

libClientSim_la_SOURCES = \
	timeGenBis.cpp timeGenBis.hpp \
	spn.hpp \
	EventsQueueSet.cpp EventsQueueSet.hpp \
	NLHA.cpp NLHA.hpp \
	sparse_io.hpp \
	stateSpace.cpp stateSpace.hpp \
	RareEvents/numericalSolver.cpp RareEvents/numericalSolver.hpp \
	RareEvents/numSolverBB.cpp RareEvents/numSolverBB.hpp \
	RareEvents/numSolverSH.cpp  RareEvents/numSolverSH.hpp \
	RareEvents/foxglynn.cpp RareEvents/foxglynn.hpp \
	Simulink/SKTime.cpp Simulink/SKTime.hpp \
	InstanceSPNSim.cpp RareEvents/InstanceSPNSimRE.cpp
#    SPNBase.cpp SPNBase.hpp
#    RareEvents/SimulatorRE.cpp RareEvents/SimulatorRE.hpp
#    RareEvents/SimulatorContinuousBounded.cpp RareEvents/SimulatorContinuousBounded.hpp
#    RareEvents/SimulatorBoundedRE.cpp RareEvents/SimulatorBoundedRE.hpp

libClientSimSK_la_SOURCES = \
	Simulink/SKTime.cpp Simulink/SKTime.hpp \
	Simulink/SKModel.hpp Simulink/InstanceSKSim.cpp
#	Simulink/SKModelBase.cpp

libClientSimMain_la_SOURCES = \
	timeGenBis.cpp timeGenBis.hpp \
	spn.hpp \
	SPNBase.hpp \
	main.cpp
#	MarkovChain.cpp MarkovChain.hpp
	
libClientSimBase_la_SOURCES = \
	BatchR.cpp BatchR.hpp \
	../ModelGenerator/parameters.cpp ../ModelGenerator/parameters.hpp\
    ../ModelGenerator/expressionStruct.cpp ../ModelGenerator/expressionStruct.hpp\
    ../ModelGenerator/SimpleSerializer.cpp ../ModelGenerator/SimpleSerializer.hpp\
	Event.cpp  Event.hpp \
	EventsQueue.cpp EventsQueue.hpp \
	timeGen.cpp timeGen.hpp \
	marking.hpp \
	LHA.hpp \
    clientsim.cpp
#   Simulator.cpp Simulator.hpp
#    LHA_orig.cpp LHA_orig.hpp

includesdir = $(includedir)
includes_HEADERS = LHA.hpp marking.hpp spn.hpp Polynome.hpp markingTemplate.hpp DistributionDef.hpp \
	timeGen.hpp timeGenBis.hpp Event.hpp EventsQueue.hpp BatchR.hpp EventsQueueSet.hpp \
	Simulator.hpp stateSpace.hpp MarkovChain.hpp SPNBase.hpp MultiModel.hpp\
	LHA_orig.hpp NLHA.hpp clientsim.hpp \
        Simulink/SKTime.hpp Simulink/SKModel.hpp

otherincludedir = $(includedir)/RareEvents
otherinclude_HEADERS = RareEvents/numericalSolver.hpp \
	RareEvents/foxglynn.hpp RareEvents/numSolverBB.hpp RareEvents/numSolverSH.hpp\
	RareEvents/SimulatorRE.hpp RareEvents/SimulatorBoundedRE.hpp RareEvents/SimulatorContinuousBounded.hpp

EXTRA_DIST = SPNBase.cpp RareEvents/SimulatorRE.cpp RareEvents/SimulatorContinuousBounded.cpp RareEvents/SimulatorBoundedRE.cpp Simulink/SKModelBase.cpp Simulator.cpp LHA_orig.cpp

AM_CPPFLAGS = $(BOOST_CPPFLAGS) $(PTHREAD_LIBS) -I$(srcdir)/../ModelGenerator/ -DGIT_REVISION=\"`git describe --abbrev=6 --dirty --always --tags`\"  -DCPP_COMPILER=\"$(CXX)\" -DBOOST_PATH=\"$(BOOST_CPPFLAGS)\" -DBOOST_LIB="\"\"" -Wall -Wextra -Wno-unused-local-typedef  -Wno-unused-parameter

AUTOMAKE_OPTIONS = subdir-objects

AM_LDFLAGS = $(PTHREAD_LIBS)
