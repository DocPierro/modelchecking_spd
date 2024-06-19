/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2012 LSV & LACL                                          *
 * Authors: Paolo Ballarini Benoît Barbot & Hilal Djafri                       *
 * Website: http://www.lsv.ens-cachan.fr/Software/cosmos                       *
 *                                                                             *
 * This program is free software; you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by        *
 * the Free Software Foundation; either version 3 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * This program is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU General Public License for more details.                                *
 *                                                                             *
 * You should have received a copy of the GNU General Public License along     *
 * with this program; if not, write to the Free Software Foundation, Inc.,     *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
 * file MarkovChain.cpp created by Benoit Barbot on 13/12/2016.                *
 *******************************************************************************
 */

#include <vector>

#include "EventsQueue.hpp"
#include "MarkovChain.hpp"

#include "Simulator.cpp"
template class SimulatorBase<Simulator<EventsQueue, MarkovChain<EventsQueue>>,
                             EventsQueue, MarkovChain<EventsQueue>>;
template class Simulator<EventsQueue, MarkovChain<EventsQueue>>;

/* AbstractMarking -------------------------------*/

struct abstractMarkingImpl {
  int state;
};

/*abstractMarking::abstractMarking() {
    P= new abstractMarkingImpl;
    resetToInitMarking();
}

abstractMarking::abstractMarking(const std::vector<int>& m) {
    P = new abstractMarkingImpl;
    setVector(m);
}
abstractMarking::abstractMarking(const abstractMarking& m) {
    P= new abstractMarkingImpl;
    *this = m;
}
abstractMarking& abstractMarking::operator = (const abstractMarking& m) {
    *P = *(m.P);
    return *this;
}
abstractMarking::~abstractMarking() {
    delete(P);
}
void abstractMarking::resetToInitMarking(){P->state = 0; }
int abstractMarking::getNbOfTokens(int)const {return P->state;}
std::vector<int> abstractMarking::getVector()const {
    std::vector<int> v({P->state});
    return v;
}
void abstractMarking::setVector(const std::vector<int>&v) {
    P->state = v[0];
}*/

/* AbstractMarking -------------------------------*/

template <class EQT>
const std::vector<size_t> MarkovChain<EQT>::getNbTransition() const {
  auto nbtrans = vector<size_t>(Transition.size());
  for (let t : Transition)
    nbtrans.push_back(t.bindingList.size());
  return nbtrans;
}

template <class EQT>
const std::vector<string> MarkovChain<EQT>::getTransitionLabels() const {
  auto nbtrans = vector<string>(Transition.size());
  for (let t : Transition)
    nbtrans.push_back(t.label);
  return nbtrans;
}

template <class EQT>
const std::string MarkovChain<EQT>::getTransitionLabel(size_t i) const {
  return Transition[i].label;
}

template <class EQT>
MarkovChain<EQT>::MarkovChain()
    : ParamDistr(*(new std::array<double, PARAM_TBL_SIZE>())),
      customDistr(*(new CustomDistr())) {
  for (int i = 0; i < 10; i++) {
    Transition.push_back(Edge(i));
  }
};

template <class EQT>
void MarkovChain<EQT>::initialEventsQueue(EQT &EQ, timeGen &tg) {
  Event E;
  generateEvent(0.0, E, 0, abstractBinding(), tg);
  EQ.insert(E);
}

template <class EQT>
void MarkovChain<EQT>::fire(size_t tr, const abstractBinding &, double) {
  Marking.P->state = (int)tr;
}

template <class EQT> void MarkovChain<EQT>::reset() { Marking.P->state = 0; }

template <class EQT>
void MarkovChain<EQT>::update(double ctime, size_t, const abstractBinding &,
                              EQT &EQ, timeGen &TG) {
  EQ.reset();
  for (int i = 0; i < 10; i++) {
    generateEvent(ctime, F, i, ab, TG);
    EQ.insert(F);
  }
}

template <class EQT>
void MarkovChain<EQT>::generateEvent(double ctime, Event &E, size_t Id,
                                     const abstractBinding &b, timeGen &TG) {
  double t = ctime;
  ParamDistr[0] = 1 / Transition.size(); // equiprobable

  t += fmax(
      TG.GenerateTime(Transition[Id].DistTypeIndex, ParamDistr, customDistr),
      0.0);
  if (P.verbose > 4) {
    cerr << "Sample " << Transition[Id].label << ": ";
    cerr << TG.string_of_dist(Transition[Id].DistTypeIndex, ParamDistr,
                              customDistr);
    cerr << endl;
  }

  E.transition = Id;
  E.time = t;
  E.priority = 1;
  E.weight = 1;
  E.binding = b;
}

// template class MarkovChain<EventsQueue>;
// #include "Simulator.cpp"
// template class SimulatorBase<Simulator<EventsQueue,MarkovChain<EventsQueue>>,
// EventsQueue,MarkovChain<EventsQueue>>;
