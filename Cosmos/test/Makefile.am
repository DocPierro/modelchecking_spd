
noinst_PROGRAMS = ClientSim2

# ClientSim1_SOURCES = \
	../src/Simulator/BatchR.cpp \
	../src/Simulator/EventsQueue.hpp ../src/Simulator/macro.h ../src/Simulator/numSolverBB.cpp ../src/Simulator/SimulatorContinuousBounded.cpp ../src/Simulator/sparse_io.hpp ../src/Simulator/BatchR.hpp ../src/Simulator/foxglynn.cpp ../src/Simulator/numSolverBB.hpp ../src/Simulator/SimulatorContinuousBounded.hpp ../src/Simulator/spn.hpp ../src/Simulator/clientsim.cpp ../src/Simulator/foxglynn.hpp ../src/Simulator/numSolverSH.cpp ../src/Simulator/Simulator.cpp ../src/Simulator/stateSpace.cpp ../src/Simulator/Event.cpp ../src/Simulator/LHA.hpp ../src/Simulator/marking.hpp ../src/Simulator/numSolverSH.hpp ../src/Simulator/Simulator.hpp ../src/Simulator/stateSpace.hpp ../src/Simulator/Event.hpp ../src/Simulator/LHA_orig.cpp ../src/Simulator/numericalSolver.cpp ../src/Simulator/SimulatorBoundedRE.cpp ../src/Simulator/SimulatorRE.cpp ../src/Simulator/EventsQueue.cpp ../src/Simulator/numericalSolver.hpp ../src/Simulator/SimulatorBoundedRE.hpp ../src/Simulator/SimulatorRE.hpp LHA.cpp markingImpl.hpp spn.cpp


spn.cpp: ../src/ModelGenerator/Cosmos $(srcdir)/../Examples/Hospital/Hospital.grml
	../src/ModelGenerator/Cosmos $(srcdir)/../Examples/Hospital/Hospital.grml --loop 100000 --trace-place "" --tmp-status only-gen
	cp tmp/spn.cpp ./
	cp tmp/LHA.cpp ./
	cp tmp/main.cpp ./
	cp tmp/markingImpl.hpp ./

LHA.cpp: spn.cpp
markingImpl.hpp: spn.cpp
main.cpp: spn.cpp

BUILT_SOURCES = LHA.cpp markingImpl.hpp spn.cpp main.cpp

ClientSim2_SOURCES = LHA.cpp markingImpl.hpp spn.cpp main.cpp

ClientSim2_LDADD = ../src/Simulator/libClientSim.la ../src/Simulator/libClientSimBase.la

ClientSim2_CPPFLAGS = -Wall -Wextra -Wno-unused-const-variable -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -I$(srcdir)/../src/Simulator -I$(srcdir)/../src/ModelGenerator $(BOOST_CPPFLAGS)

AM_CPPFLAGS =  -I$(srcdir)/include -isystem $(BOOST_CPPFLAGS) $(PTHREAD_CFLAGS)

AM_LDFLAGS = $(PTHREAD_CFLAGS)

AUTOMAKE_OPTIONS = subdir-objects
