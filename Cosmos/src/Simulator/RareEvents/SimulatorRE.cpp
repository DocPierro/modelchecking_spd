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
 * file SimulatorRE.cpp created by Benoit Barbot on 09/11/11.                  *
 *******************************************************************************
 */

#include "EventsQueue.hpp"
#include "marking.hpp"
#include <float.h>
#include <iostream>
#include <math.h>
#include <set>
#include <time.h>
// #include "RareEvent.hpp"

#include "SimulatorRE.hpp"

using namespace std;

template <class DEDS>
void generateEvent(double ctime, Event &E, size_t Id, const abstractBinding &b,
                   timeGen &TG, DEDS &N) {
  double t = ctime;
  if (N.Transition[Id].DistTypeIndex != IMMEDIATE) {
    N.getParams(Id, b);
    t += TG.GenerateTime(N.Transition[Id].DistTypeIndex, Id, N.ParamDistr,
                         N.customDistr);

    N.Rate_Table[Id] = N.ParamDistr[0];
    N.Origine_Rate_Table[Id] = N.ParamDistr[1];
    N.Rate_Sum = N.Rate_Sum + N.ParamDistr[0];
    N.Origine_Rate_Sum = N.Origine_Rate_Sum + N.ParamDistr[1];
  }
  double w = 0.0;
  if (N.Transition[Id].DistTypeIndex > 2) {
    N.ParamDistr[0] = N.GetWeight(Id, b);
    w = TG.GenerateTime(EXPONENTIAL, Id, N.ParamDistr, N.customDistr);
    // vector<double> wParam(1, N.GetWeight(Id));
    // w = GenerateTime(2, wParam);
  }

  E.transition = Id;
  E.time = t;
  E.priority = N.GetPriority(Id, b);
  E.weight = w;
  E.binding = b;
}

template <class S, class DEDS>
SimulatorREBase<S, DEDS>::SimulatorREBase(DEDS &N,
                                          LHA_orig<typename DEDS::stateType> &A)
    : SimulatorBase<S, EventsQueue, DEDS>(N, A){};

template <class S>
SPNBaseRE<S>::SPNBaseRE(bool doubleIS)
    : SPNBase<S, EventsQueue>(0), doubleIS_mode(doubleIS) {
  rareEventEnabled = false;
  Rate_Sum = 0;
  Origine_Rate_Sum = 0;
  Rate_Table = vector<double>(this->tr, 0.0);
  Origine_Rate_Table = vector<double>(this->tr, 0.0);
}

template <class S> void SPNBaseRE<S>::initialize(stateSpace *muprob) {
  this->muprob = muprob;
}

template <class S, class DEDS> void SimulatorREBase<S, DEDS>::initVect() {
  muprob = new stateSpace();
  muprob->inputVect();
  this->N.initialize(muprob);
}

template <class S>
void SPNBaseRE<S>::InitialEventsQueue(EventsQueue &EQ, timeGen &TG) {
  Rate_Sum = 0;
  Origine_Rate_Sum = 0;
  SPNBase<S, EventsQueue>::initialEventsQueue(EQ, TG);
}

template <class S, class DEDS>
void SimulatorREBase<S, DEDS>::returnResultTrue() {
  this->A.getFinalValues(this->N.getState(), this->Result.quantR,
                         this->Result.qualR);
  this->Result.accept = true;
  for (size_t i = 0; i < this->A.FormulaVal.size(); i++)
    this->Result.quantR[i] *= this->A.Likelihood;
  if (P.verbose > 3)
    cerr << "---------------\n TRUE: Likelyhood: " << this->A.Likelihood
         << " \n------\n";
}

template <class S>
void SPNBaseRE<S>::update(double ctime, size_t t, const abstractBinding &b,
                          EventsQueue &EQ, timeGen &TG) {
  // If rareevent not require yet call the parent function

  if (!rareEventEnabled) {
    if (precondition(this->Marking)) {
      rareEventEnabled = true;
      // A.Likelihood = 1.0;
    } else {
      SPNBase<S, EventsQueue>::update(ctime, t, b, EQ, TG);
      return;
    }
  }

  Event F;

  Rate_Sum = 0;
  Origine_Rate_Sum = 0;

  // Run over all transitions
  for (const auto &tr : this->Transition) {
    if (tr.Id != this->tr - 1) {
      for (const auto &bindex : tr.bindingList) {
        if (this->IsEnabled(tr.Id, bindex)) {
          if (EQ.isScheduled(tr.Id, bindex.id())) {
            generateEvent(ctime, F, tr.Id, bindex, TG, *this);
            EQ.replace(F);
          } else {
            generateEvent(ctime, F, tr.Id, bindex, TG, *this);
            EQ.insert(F);
          }
        } else {
          if (EQ.isScheduled(tr.Id, bindex.id()))
            EQ.remove(tr.Id, bindex.id());
        }
      }
    }
  }

  abstractBinding bpuit;
  generateEvent(ctime, F, (this->tr - 1), bpuit, TG, *this);
  if (!doubleIS_mode) {
    EQ.replace(F);
  }

  /*
   //In Debug mode check that transition are scheduled iff they are enabled
  for (const auto &tr : N.Transition) {
          for(const auto &bindex : tr.bindingList){
                  if (N.IsEnabled(tr.Id, bindex) !=
                          EQ->isScheduled(tr.Id, bindex.id())){
                          cerr << "N.IsEnabled(" << tr.label << ",";
                          bindex.print();
                          cerr <<")" << endl;
                          if(EQ->isScheduled(tr.Id, bindex.id())){
                                  cerr << "Scheduled and not enabled!"<< endl;
                          }else{
                                  cerr << "Enabled and not scheduled!" << endl;
                          }
                          assert(N.IsEnabled(tr.Id, bindex) ==
                                     EQ->isScheduled(tr.Id, bindex.id()));
                  }
          }
  }
   */
};

template <class S, class DEDS>
void SimulatorREBase<S, DEDS>::updateLikelihood(size_t E1_transitionNum) {

  if (P.verbose > 4) {
    cerr << "initialised?:\t" << E1_transitionNum << "\t" << this->A.Likelihood
         << endl;
    cerr << this->N.Rate_Sum << "\t" << this->N.Origine_Rate_Sum << "\t[";
    for (let cv : this->N.Rate_Table)
      cerr << cv << ", ";
    cerr << "]\t[";
    for (let cv : this->N.Origine_Rate_Table)
      cerr << cv << ", ";
    cerr << "]" << endl;
  }

  if (this->N.doubleIS_mode) {
    this->A.Likelihood =
        this->A.Likelihood *
        (this->N.Origine_Rate_Table[E1_transitionNum] /
         this->N.Origine_Rate_Sum) *
        ((this->N.Rate_Sum - this->N.Rate_Table[this->N.tr - 1]) /
         this->N.Rate_Table[E1_transitionNum]);
  } else {
    this->A.Likelihood =
        this->A.Likelihood *
        //
        //(N.Origine_Rate_Table[E1_transitionNum] / 1.0) *
        (this->N.Origine_Rate_Table[E1_transitionNum] /
         this->N.Origine_Rate_Sum) *
        (this->N.Rate_Sum / this->N.Rate_Table[E1_transitionNum]);
  }
}

template <class S, class DEDS>
bool SimulatorREBase<S, DEDS>::transitionSink(size_t i) {
  return (i == this->N.tr - 1);
}

/*
 Useless for the moment
 void SimulatorRE::GenerateDummyEvent(Event& E, size_t Id) {
 E.transition = Id;
 E.time = 0.0;
 E.priority = N.GetPriority(Id);
 E.weight = 0.0;
 }*/

template <class S, class DEDS> void SimulatorREBase<S, DEDS>::reset() {
  this->A.Likelihood = 1.0;
  this->N.reset();
  this->A.reset(this->N.getState());
  this->EQ->reset();
}

/**
 * Simulate a whole trajectory in the system. Result is store in SimOutput
 */
template <class S, class DEDS>
void SimulatorREBase<S, DEDS>::SimulateSinglePath() {
  this->N.rareEventEnabled = this->N.precondition(this->N.getState());
  static_cast<S *>(this)->reset();
  this->N.InitialEventsQueue(*(this->EQ), *this);

  if (this->logtrace.is_open())
    this->logtrace << "New Path" << endl;

  bool continueb = true;
  while ((!this->EQ->isEmpty()) && continueb) {
    // cerr << "continue path"<< endl;
    if (this->logtrace.is_open()) {
      this->logtrace << this->A.CurrentTime << "\t";
      this->N.getState().print(this->logtrace, 0.0);
      this->A.printState(this->logtrace);
      this->logtrace << endl;
    }
    if (P.verbose > 3) {
      // Print marking and location of the automata
      // Usefull to track a simulation
      this->N.getState().printHeader(cerr);
      this->A.printHeader(cerr);
      cerr << endl;
      this->N.getState().print(cerr, 0.0);
      this->A.printState(cerr);
      cerr << "\t" << this->A.Likelihood;
      cerr << endl;
      if (P.verbose > 4)
        this->EQ->view(this->N.getTransitionLabels());
      if (P.verbose == 6)
        this->interactiveSimulation();
    }

    continueb = static_cast<S *>(this)->SimulateOneStep();
    if (!this->N.rareEventEnabled)
      this->N.rareEventEnabled = this->N.precondition(this->N.getState());
  }
  // cerr << "finish path"<< endl;
}

template <class S>
void SPNBaseRE<S>::getParams(size_t Id, const abstractBinding &b) {
  // If rareevent not require yet call the parent function
  if (!rareEventEnabled) {
    this->GetDistParameters(Id, b);
    return;
  }
  this->GetDistParameters(Id, b);
  this->ParamDistr[1] = this->ParamDistr[0];
  this->ParamDistr[0] = ComputeDistr(Id, b, this->ParamDistr[0]);
  // N.ParamDistr[0]= N.ParamDistr[1];
  // /////////////////////////////////////////////////to remove
}

template <class S> double SPNBaseRE<S>::mu() {

  double mud = direct_computation(this->Marking);

  if (mud >= 0.0) {
    if (P.verbose > 3)
      cerr << "muValue direct comp: " << mud << endl;
    return mud;
  }
  vector<int> vect(muprob->S.begin()->first->size(), 0);
  lumpingFun(this->Marking, vect);
  // cerr << "test(";
  int i = muprob->findHash(&vect);
  if (i < 0 || P.verbose > 4) {
    cerr << "state:(";
    for (size_t j = 0; j < vect.size(); j++) {
      cerr << vect[j] << ",";
    }
    cerr << ") ->" << i << endl;
    this->Marking.printHeader(cerr);
    cerr << endl;
    this->Marking.print(cerr, 0.0);
    cerr << endl;
    print_state(vect);
    if (i < 0)
      exit(EXIT_FAILURE);
  }
  if (P.verbose > 3)
    cerr << "muValue: " << muprob->getMu(i) << endl;
  return muprob->getMu(i);
}

template <class S>
double SPNBaseRE<S>::ComputeDistr(size_t t, const abstractBinding &b,
                                  double origin_rate) {
  if (P.verbose > 4)
    cerr << "trans: " << this->Transition[t].label << " mu origine:";
  double mux = mu();
  if (mux == 0.0 || mux == 1.0)
    return (origin_rate);

  if (t == this->tr - 1) {
    if (Origine_Rate_Sum >= Rate_Sum) {
      return (Origine_Rate_Sum - Rate_Sum);
    } else {
      if (P.verbose > 3 && (Origine_Rate_Sum < 0.99 * Rate_Sum)) {
        cerr << "Reduce model does not guarantee variance" << endl;
        cerr << "Initial sum of rate: " << Origine_Rate_Sum
             << " Reduce one: " << Rate_Sum
             << " difference: " << Origine_Rate_Sum - Rate_Sum << endl;
        // exit(EXIT_FAILURE);
      }

      return 0.0;
    };
  };
  if (P.verbose > 4)
    cerr << "mu target : ";
  double distr;
  this->fire(t, b, 0.0);
  distr = origin_rate * (mu() / mux);
  this->unfire(t, b);
  if (P.verbose > 4)
    cerr << endl;
  return (distr);
}
