#include "BatchR.hpp"
#include "clientsim.hpp"
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include "MultiModel.hpp"

#include "../ModelGenerator/SimpleSerializer.hpp"
#include "../ModelGenerator/parameters.hpp"
#include "SPNBase.cpp"
#include "Simulator.cpp"
// #include "MarkovChain.hpp"
#include "LHA_orig.cpp"
template class LHA<abstractMarking>;
// template class LHA_orig<abstractMarking>;

int main(int argc, char **argv) {
  const string tmppath(argv[1]);
  std::ifstream ifs(tmppath + "/parameters.txt");
  readParameters(ifs);

  signal(SIGINT, signalHandler);
  signal(SIGHUP, signalHandler);
  SPN_orig<EventsQueue> N1(0);
  // SPN_orig<EventsQueue> N2(N1.tr);
  // MarkovChain<EventsQueue> MC;
  // MultiModel<EventsQueue,SPN_orig<EventsQueue>,SPN_orig<EventsQueue>>
  // mm(N1,N2);
  LHA_orig<typename decltype(N1)::stateType> A;
  Simulator<EventsQueue, typeof N1> sim(N1, A);
  bool singleBatch = false;
  sim.SetBatchSize(P.Batch);
  setSimulator(sim, argc, argv, P);
  if ((P.verbose >= 4) | singleBatch)
    sim.RunBatch();
  else
    while (!cin.eof() && P.graceFullEnding <= 1) {
      BatchR batchResult = sim.RunBatch(); // simulate a batch of trajectory
      batchResult.outputR(cout); // output the result on the standart output
    }
  return (EXIT_SUCCESS);
}
