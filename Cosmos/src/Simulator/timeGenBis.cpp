#include "timeGenBis.hpp"
#include "EventsQueue.hpp"
#include "SPNBase.cpp"
#include <float.h>

using namespace std;

/**
 * Generate an event based on the type of his distribution
 * @param E the event to update
 * @param Id the number of the transition to of the SPN
 * @param b is the binding of the variable of the SPN for the transition.
 */

template <class DEDS>
void generateEvent(double ctime, Event &E, size_t Id, const abstractBinding &b,
                   timeGen &TG, DEDS &N) {
  double t = ctime;
  if (N.Transition[Id].DistTypeIndex != IMMEDIATE) {
    N.GetDistParameters(Id, b);
    t += fmax(TG.GenerateTime(N.Transition[Id].DistTypeIndex, Id, N.ParamDistr,
                              N.customDistr),
              0.0);
    if (P.verbose > 4) {
      cerr << "Sample " << N.Transition[Id].label << ": ";
      cerr << TG.string_of_dist(N.Transition[Id].DistTypeIndex, N.ParamDistr,
                                N.customDistr);
      cerr << endl;
    }
  }

  // The weight of a transition is always distributed exponentially
  // It is used to solved conflict of two transitions with same time
  // and same priority.
  double w = 0.0;
  switch (N.Transition[Id].DistTypeIndex) {
  case DETERMINISTIC:
  case DISCRETEUNIF:
  case IMMEDIATE:
  case IMDT:
  case SYNC:
  case DISCRETEUSERDEFINE:
    N.ParamDistr[0] = N.GetWeight(Id, b);
    w = TG.current_weight +
        TG.GenerateTime(EXPONENTIAL, Id, N.ParamDistr, N.customDistr);
    if (P.verbose > 4) {
      cerr << "weight : " << N.ParamDistr[0] << endl;
      cerr << TG.string_of_dist(EXPONENTIAL, N.ParamDistr, N.customDistr);
      cerr << endl;
    }
    break;
  case PARETO:
  case NORMAL:
  case TRUNKNORMAL:
  case GAMMA:
  case GEOMETRIC:
  case UNIFORM:
  case ERLANG:
  case WEIBULL:
  case EXPONENTIAL:
  case LOGNORMAL:
  case TRIANGLE:
  case USERDEFINE:
  case USERDEFINEPOLYNOMIAL:
  case MASSACTION:
  case PLAYER1:;
  }

  E.transition = N.shift + Id;
  E.time = t;
  E.priority = N.GetPriority(Id, b);
  E.weight = w;
  E.binding = b;
}

template void generateEvent<SPN_orig<EventsQueue>>(double, Event &,
                                                   unsigned long,
                                                   abstractBinding const &,
                                                   timeGen &,
                                                   SPN_orig<EventsQueue> &);
template void generateEvent(double ctime, Event &E, size_t Id,
                            const abstractBinding &b, timeGen &,
                            SPN_orig<EventsQueueSet> &);

/*#include "MarkovChain.hpp"
template void generateEvent(double ctime,Event& E,size_t Id,const
abstractBinding& b,timeGen &,MarkovChain<EventsQueue<vector<Edge>>> &);
*/
