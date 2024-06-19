# Install script for directory: /home/pierre/Documents/Travail/pm4pierro/Cosmos

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Cosmos" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Cosmos")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Cosmos"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/pierre/Documents/Travail/pm4pierro/Cosmos/cmake-build-debug/Cosmos")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Cosmos" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Cosmos")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Cosmos")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/pierre/Documents/Travail/pm4pierro/Cosmos/cmake-build-debug/CMakeFiles/Cosmos.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/pierre/Documents/Travail/pm4pierro/Cosmos/cmake-build-debug/libClientSimBase.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/pierre/Documents/Travail/pm4pierro/Cosmos/cmake-build-debug/CMakeFiles/ClientSimBase.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/pierre/Documents/Travail/pm4pierro/Cosmos/cmake-build-debug/libClientSimMain.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/pierre/Documents/Travail/pm4pierro/Cosmos/cmake-build-debug/CMakeFiles/ClientSimMain.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/pierre/Documents/Travail/pm4pierro/Cosmos/cmake-build-debug/libClientSim.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/pierre/Documents/Travail/pm4pierro/Cosmos/cmake-build-debug/CMakeFiles/ClientSim.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/spn.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/LHA.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/marking.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/Polynome.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/markingTemplate.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/DistributionDef.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/timeGen.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/timeGenBis.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/Event.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/EventsQueue.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/BatchR.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/EventsQueueSet.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/Simulator.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/stateSpace.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/MarkovChain.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/SPNBase.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/LHA_orig.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/NLHA.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/clientsim.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/Simulink/SKTime.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/ModelGenerator/expressionStruct.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/ModelGenerator/parameters.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/ModelGenerator/SimpleSerializer.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/ModelGenerator/HaslFormula.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/RareEvents" TYPE FILE FILES
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/spn.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/LHA.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/marking.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/RareEvents/SimulatorRE.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/RareEvents/SimulatorBoundedRE.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/RareEvents/SimulatorContinuousBounded.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/RareEvents/numericalSolver.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/RareEvents/numSolverBB.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/RareEvents/numSolverSH.hpp"
    "/home/pierre/Documents/Travail/pm4pierro/Cosmos/src/Simulator/RareEvents/foxglynn.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/pierre/Documents/Travail/pm4pierro/Cosmos/cmake-build-debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
