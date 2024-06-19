# CMake generated Testfile for 
# Source directory: /home/pierre/Documents/Travail/pm4pierro/Cosmos
# Build directory: /home/pierre/Documents/Travail/pm4pierro/Cosmos/cmake-build-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(benchTest "ocaml" "bench.ml")
set_tests_properties(benchTest PROPERTIES  PASS_REGULAR_EXPRESSION "25 is 5" WORKING_DIRECTORY "/home/pierre/Documents/Travail/pm4pierro/Cosmos/Examples/" _BACKTRACE_TRIPLES "/home/pierre/Documents/Travail/pm4pierro/Cosmos/CMakeLists.txt;239;add_test;/home/pierre/Documents/Travail/pm4pierro/Cosmos/CMakeLists.txt;0;")
