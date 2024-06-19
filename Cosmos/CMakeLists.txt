project(Cosmos)

cmake_minimum_required (VERSION 3.3.1)

find_package(BISON)
find_package(FLEX)
find_package(EXPAT)
find_package(Git)
find_package(Boost 1.55)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

set (CMAKE_CXX_STANDARD 11)

EXECUTE_PROCESS(
COMMAND ${GIT_EXECUTABLE} describe --abbrev=6 --dirty --always --tags
OUTPUT_VARIABLE GIT_REVISION
OUTPUT_STRIP_TRAILING_WHITESPACE
)

message("Compiler used: " ${CMAKE_CXX_COMPILER_ID})

ADD_DEFINITIONS(
  -O3 -Wall -Wno-sign-compare
  -DGIT_REVISION=\"${GIT_REVISION}\"
  -DBOOST_PATH=\"-I${Boost_INCLUDE_DIRS}\"
  -DBOOST_LIB=\"${Boost_LIBRARIES}\"
  -DCPP_COMPILER=\"g++\"
  -DCMAKE_VERSION=\"${CMAKE_VERSION}\"
)


add_library(grml src/libgrml/expatmodelparser.cc
                 src/libgrml/modelwriter.cc)
target_link_libraries(grml ${EXPAT_LIBRARY})

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/src/ModelGenerator/Eval/")
BISON_TARGET(EvalParser src/ModelGenerator/Eval/Eval-parser.yy
    ${CMAKE_CURRENT_BINARY_DIR}/src/ModelGenerator/Eval/Eval-parser.cc COMPILE_FLAGS -peval)
FLEX_TARGET(EvalScanner src/ModelGenerator/Eval/Eval-scanner.ll
    ${CMAKE_CURRENT_BINARY_DIR}/src/ModelGenerator/Eval/Eval-scanner.cc COMPILE_FLAGS --prefix=eval)
ADD_FLEX_BISON_DEPENDENCY(EvalScanner EvalParser)
add_library(libeval
    ${BISON_EvalParser_OUTPUTS}
    ${FLEX_EvalScanner_OUTPUTS}
    src/ModelGenerator/Eval/Eval.cpp)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(libeval PRIVATE -Wno-unneeded-internal-declaration)
endif()

target_include_directories(libeval PRIVATE "src/ModelGenerator/Eval")
target_include_directories(libeval PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/src/ModelGenerator/Eval)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/src/ModelGenerator/GspnParser/")
BISON_TARGET(GrmlParser src/ModelGenerator/GspnParser/Gspn-parser.yy
${CMAKE_CURRENT_BINARY_DIR}/src/ModelGenerator/GspnParser/Gspn-parser.cc COMPILE_FLAGS -pgspn)
FLEX_TARGET(GrmlScanner src/ModelGenerator/GspnParser/Gspn-scanner.ll
${CMAKE_CURRENT_BINARY_DIR}/src/ModelGenerator/GspnParser/Gspn-scanner.cc COMPILE_FLAGS --prefix=gspn)
ADD_FLEX_BISON_DEPENDENCY(GrmlScanner GrmlParser)
add_library(libgspn
${BISON_GrmlParser_OUTPUTS}
${FLEX_GrmlScanner_OUTPUTS}
src/ModelGenerator/GspnParser/Gspn-Grml-Output.cpp
src/ModelGenerator/GspnParser/Gspn-Writer-Color.cpp
src/ModelGenerator/GspnParser/Gspn-model.cpp
src/ModelGenerator/GspnParser/unfolder.cpp
src/ModelGenerator/GspnParser/Gspn-Reader.cpp
src/ModelGenerator/GspnParser/Gspn-Writer.cpp
src/ModelGenerator/GspnParser/Gspn_gmlparser.cpp
)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(libgspn PRIVATE -Wno-format-truncation)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(libgspn PRIVATE -Wno-unneeded-internal-declaration)
endif()
target_link_libraries (libgspn grml libeval)
target_include_directories(libgspn PUBLIC "src/libgrml")
target_include_directories(libgspn PRIVATE "src/ModelGenerator/GspnParser")
target_include_directories(libgspn PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/src/ModelGenerator/GspnParser)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/src/ModelGenerator/LhaParser/")
BISON_TARGET(LHAParser src/ModelGenerator/LhaParser/Lha-parser.yy
    ${CMAKE_CURRENT_BINARY_DIR}/src/ModelGenerator/LhaParser/Lha-parser.cc COMPILE_FLAGS -plha)
FLEX_TARGET(LHAScanner src/ModelGenerator/LhaParser/Lha-scanner.ll
    ${CMAKE_CURRENT_BINARY_DIR}/src/ModelGenerator/LhaParser/Lha-scanner.cc COMPILE_FLAGS --prefix=lha)
ADD_FLEX_BISON_DEPENDENCY(LHAScanner LHAParser)
add_library(liblha
    ${BISON_LHAParser_OUTPUTS}
    ${FLEX_LHAScanner_OUTPUTS}
    src/ModelGenerator/LhaParser/Lha-Reader.cpp
    src/ModelGenerator/LhaParser/Lha_gmlparser.cpp
    )
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(liblha PRIVATE -Wno-format-truncation)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(liblha PRIVATE -Wno-unneeded-internal-declaration)
endif()
target_include_directories(liblha PRIVATE "src/ModelGenerator/LhaParser")
target_include_directories(liblha PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src/ModelGenerator/LhaParser)
target_link_libraries(liblha libeval libgspn)

add_executable(Cosmos
        src/Simulator/BatchR.cpp

        src/ModelGenerator/Cosmos.cpp
        src/ModelGenerator/HaslFormula.cpp
        src/ModelGenerator/expressionStruct.cpp
        src/ModelGenerator/result.cpp
        src/ModelGenerator/Generator.cpp
        src/ModelGenerator/casesWriter.cpp
        src/ModelGenerator/parameters.cpp
        src/ModelGenerator/SimpleSerializer.cpp
        src/ModelGenerator/server.cpp)

target_include_directories(Cosmos PRIVATE "src/ModelGenerator/GspnParser")
target_include_directories(Cosmos PRIVATE "src/ModelGenerator/LhaParser")
target_include_directories(Cosmos PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/src/ModelGenerator/LhaParser")
target_link_libraries (Cosmos grml liblha ${EXPAT_LIBRARY})
install(TARGETS Cosmos DESTINATION bin)


add_library(ClientSimBase
        src/ModelGenerator/parameters.cpp
        src/ModelGenerator/expressionStruct.cpp
        src/ModelGenerator/SimpleSerializer.cpp
        src/Simulator/BatchR.cpp
        src/Simulator/Event.cpp
        src/Simulator/EventsQueue.cpp
        src/Simulator/timeGen.cpp
        src/Simulator/clientsim.cpp
)
target_include_directories(ClientSimBase PRIVATE "src/ModelGenerator")

install(TARGETS ClientSimBase DESTINATION lib)
add_library(ClientSimMain
        src/Simulator/timeGenBis.cpp
        src/Simulator/main.cpp
)
target_include_directories(ClientSimMain PRIVATE "src/ModelGenerator")
install(TARGETS ClientSimMain DESTINATION lib)

add_library(ClientSim
        src/Simulator/timeGenBis.cpp
        src/Simulator/EventsQueueSet.cpp
        src/Simulator/NLHA.cpp
        src/Simulator/stateSpace.cpp
        src/Simulator/RareEvents/numericalSolver.cpp
        src/Simulator/RareEvents/numSolverBB.cpp
        src/Simulator/RareEvents/numSolverSH.cpp
        src/Simulator/RareEvents/foxglynn.cpp
        src/Simulator/RareEvents/SimulatorContinuousBounded.cpp
        src/Simulator/RareEvents/InstanceSPNSimRE.cpp
        src/Simulator/Simulink/SKTime.cpp
        src/Simulator/InstanceSPNSim.cpp
        src/Simulator/RareEvents/InstanceSPNSimRE.cpp
)
target_include_directories(ClientSim PRIVATE "src/ModelGenerator")
install(TARGETS ClientSim DESTINATION lib)


install(FILES src/Simulator/spn.hpp src/Simulator/LHA.hpp src/Simulator/marking.hpp
        src/Simulator/Polynome.hpp src/Simulator/markingTemplate.hpp
        src/Simulator/DistributionDef.hpp
        src/Simulator/timeGen.hpp
        src/Simulator/timeGenBis.hpp
        src/Simulator/Event.hpp
        src/Simulator/EventsQueue.hpp
        src/Simulator/BatchR.hpp
        src/Simulator/EventsQueueSet.hpp
        src/Simulator/Simulator.hpp
        src/Simulator/stateSpace.hpp
        src/Simulator/MarkovChain.hpp
        src/Simulator/SPNBase.hpp
        src/Simulator/LHA_orig.hpp
        src/Simulator/NLHA.hpp
        src/Simulator/clientsim.hpp
        src/Simulator/Simulink/SKTime.hpp
        src/ModelGenerator/expressionStruct.hpp
        src/ModelGenerator/parameters.hpp
	src/ModelGenerator/SimpleSerializer.hpp
        src/ModelGenerator/HaslFormula.hpp
       DESTINATION include)
       
install(FILES src/Simulator/spn.hpp src/Simulator/LHA.hpp src/Simulator/marking.hpp
        src/Simulator/RareEvents/SimulatorRE.hpp
        src/Simulator/RareEvents/SimulatorBoundedRE.hpp
        src/Simulator/RareEvents/SimulatorContinuousBounded.hpp
                src/Simulator/RareEvents/numericalSolver.hpp
                src/Simulator/RareEvents/numSolverBB.hpp
                src/Simulator/RareEvents/numSolverSH.hpp
                src/Simulator/RareEvents/foxglynn.hpp
       DESTINATION include/RareEvents)

add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tmp/spn.cpp ${CMAKE_CURRENT_BINARY_DIR}/tmp/LHA.cpp
DEPENDS Cosmos
#TARGET GenerateFile POST_BUILD
COMMAND Cosmos ${CMAKE_SOURCE_DIR}/Examples/Hospital/Hospital.grml --loop 100000 --tmp-status only-gen
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

file(GLOB_RECURSE ml_SOURCES "modelConvert/*.ml*")

add_custom_target(modelConvert ALL
${CMAKE_SOURCE_DIR}/modelConvert/build.sh
#OUTPUT modelConvert
BYPRODUCTS ${CMAKE_SOURCE_DIR}/modelConvert/modelConvert.native
#DEPENDS Cosmos
#TARGET modelConvert
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/modelConvert/
SOURCES  ${ml_SOURCES}
)

file(GLOB_RECURSE ml_automata_SOURCES "automataGen/*.ml*")

add_custom_target(automataGen ALL
${CMAKE_SOURCE_DIR}/automataGen/build.sh
#OUTPUT automataGen
BYPRODUCTS ${CMAKE_SOURCE_DIR}/automataGen/generator.native
#DEPENDS Cosmos
#TARGET automataGen
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/automataGen/
SOURCES  ${ml_automata_SOURCES}
)


add_executable(testClientSim ${CMAKE_CURRENT_BINARY_DIR}/tmp/spn.cpp ${CMAKE_CURRENT_BINARY_DIR}/tmp/LHA.cpp)
target_compile_options(testClientSim PRIVATE -Wno-return-type -Wno-unused-variable)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(testClientSim PRIVATE -Wno-unused-but-set-variable)
endif()
include_directories("src/Simulator")
add_dependencies(testClientSim Cosmos)
target_link_libraries (testClientSim ClientSimMain ClientSimBase)

enable_testing()

add_test(NAME benchTest
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Examples/
            COMMAND ocaml bench.ml)
set_tests_properties (benchTest
  PROPERTIES PASS_REGULAR_EXPRESSION "25 is 5")
