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
 * file SimulatorBoundedRE.cpp created by Benoit Barbot on 06/12/11.           *
 *******************************************************************************
 */

#include <list>
#include <sys/resource.h>

#include "SimulatorBoundedRE.hpp"

#include "numSolverBB.hpp"
#include "numSolverSH.hpp"

using namespace std;

template <class S, class DEDS>
SimulatorBoundedREBase<S, DEDS>::SimulatorBoundedREBase(
    DEDS &N, LHA_orig<typename DEDS::stateType> &A, int m)
    : SimulatorREBase<S, DEDS>(N, A) {
  switch (m) {
  case 1:
    numSolv = new numericalSolver();
    break;

  case 2:
    numSolv = new numSolverBB();
    break;

  case 3:
    numSolv = new numSolverSH();
    break;
  }
  this->muprob = numSolv;
  delete this->EQ;
  this->N.initialize(this->muprob);
  // numSolv->initVect(T);
}

template <class S, class DEDS>
void SimulatorBoundedREBase<S, DEDS>::initVect(int T) {
  lambda = numSolv->uniformizeMatrix();
  cerr << "lambda:" << lambda << endl;
  numSolv->initVect(T);
}

template <class S, class DEDS>
BatchR SimulatorBoundedREBase<S, DEDS>::RunBatch() {

  // cerr << "test(";
  numSolv->reset();
  // cerr << ")" << endl;

  BatchR batchResult(1, 0);

  list<simulationState<DEDS, EventsQueue>> statevect(this->BatchSize);
  // delete EQ;

  if (P.verbose >= 1) {
    cerr << "Initial round:";
    numSolv->printState();
    cerr << "\tremaining trajectories: " << statevect.size() << "\tInit Prob:";
    cerr << numSolv->getVect()[0] << endl;
  }
  for (auto &it : statevect) {
    this->N.Origine_Rate_Table = vector<double>(this->N.tr, 0.0);
    this->N.Rate_Table = vector<double>(this->N.tr, 0.0);
    this->EQ = new EventsQueue(this->N.getNbTransition());
    this->reset();

    this->N.initialEventsQueue(*(this->EQ), *this);

    // AE = A.GetEnabled_A_Edges( N.Marking);

    it.saveState(&(this->N), &(this->A), &(this->EQ));
  }

  // cout << "new batch" << endl;
  while (!statevect.empty()) {
    numSolv->stepVect();
    if (P.verbose >= 1) {
      cerr << "new round:";
      numSolv->printState();
      cerr << "\tremaining trajectories: " << statevect.size()
           << "\tInit Prob:";
      cerr << numSolv->getVect()[0] << endl;
    }

    for (auto it = statevect.begin(); it != statevect.end(); it++) {

      it->loadState(&(this->N), &(this->A), &(this->EQ));

      // cerr << A.Likelihood << endl;
      // cerr << "mu:\t" << mu() << " ->\t";
      bool continueb = this->SimulateOneStep();
      // cerr << mu() << endl;
      if (numSolv->getVect().size() <= 1) {
        continueb = false;
        this->Result.accept = false;
      }

      if ((!this->EQ->isEmpty()) && continueb) {
        it->saveState(&(this->N), &(this->A), &(this->EQ));
      } else {
        batchResult.addSim(this->Result);
        delete this->EQ;
        it = statevect.erase(it);
        it--;

        // log the result
        if (this->Result.accept && this->logResult) {
          for (size_t i = 0; i < this->Result.quantR.size(); i++) {
            if (i > 0)
              this->logvalue << "\t";
            this->logvalue << this->Result.quantR[i];
          }
          this->logvalue << endl;
        }
      }
    }
  }

  // cerr << "test" << endl;
  // exit(0);

  rusage ruse;
  getrusage(RUSAGE_SELF, &ruse);
  cerr << endl
       << endl
       << "Total Time: "
       << ruse.ru_utime.tv_sec + ruse.ru_utime.tv_usec / 1000000.
       << "\tTotal Memory: " << ruse.ru_maxrss << "ko" << endl
       << endl;

  return (batchResult);
}

template <class S> double SPNBaseBoundedRE<S>::mu() {

  vector<int> vect(this->muprob->S.begin()->first->size(), 0);

  static_cast<S *>(this)->lumpingFun(this->Marking, vect);
  int stateN = this->muprob->findHash(&vect);

  if (stateN < 0) {
    // cerr << numSolv->getVect()<< endl
    cerr << "statevect(";
    for (size_t i = 0; i < vect.size(); i++)
      cerr << vect[i] << ",";
    cerr << ")" << endl << "state not found" << endl;
    this->print_state(vect);
    return 0.0;
    // exit(EXIT_FAILURE);
  }

  return (this->muprob->getMu(stateN));
}

template <class S>
void SPNBaseBoundedRE<S>::update(double ctime, size_t, const abstractBinding &,
                                 EventsQueue &EQ, timeGen &TG) {
  Event F;
  // check if the current transition is still enabled

  this->Rate_Sum = 0;
  this->Origine_Rate_Sum = 0;

  // Run over all transition
  for (size_t it = 0; it < SPN::tr - 2; it++) {
    for (auto bindex = this->Transition[it].bindingList.begin();
         bindex != this->Transition[it].bindingList.end(); ++bindex) {
      if (this->IsEnabled(it, *bindex)) {
        if (EQ.isScheduled(it, bindex->id())) {
          generateEvent(ctime, F, it, *bindex, TG, *(static_cast<S *>(this)));
          EQ.replace(F);
        } else {
          generateEvent(ctime, F, it, *bindex, TG, *(static_cast<S *>(this)));
          EQ.insert(F);
        }
      } else {
        if (EQ.isScheduled(it, bindex->id()))
          EQ.remove(it, bindex->id());
      }
    }
  }

  abstractBinding bpuit;
  generateEvent(ctime, F, (SPN::tr - 2), bpuit, TG, *(static_cast<S *>(this)));
  if (!this->doubleIS_mode) {
    EQ.replace(F);
  }

  generateEvent(ctime, F, (SPN::tr - 1), bpuit, TG, *(static_cast<S *>(this)));
  if (!this->doubleIS_mode) {
    EQ.replace(F);
  }
};

template <class S>
void SPNBaseBoundedRE<S>::getParams(size_t Id, const abstractBinding &b) {

  static_cast<S *>(this)->GetDistParameters(Id, b);
  double origin_rate = this->ParamDistr[0];
  if (Id == SPN::tr - 2) {
    origin_rate = this->muprob->maxRate - this->Origine_Rate_Sum;
    // cerr << "lambda:\t" << lambda << "\tselfloop:\t" << origin_rate << endl;
  }
  this->ParamDistr[0] =
      static_cast<S *>(this)->ComputeDistr(Id, b, origin_rate);
  this->ParamDistr[1] = origin_rate;
}

template <class S>
double SPNBaseBoundedRE<S>::ComputeDistr(size_t t, const abstractBinding &b,
                                         double origin_rate) {

  // cerr << endl<< "mux" << endl;
  double mux = mu();

  if (t == SPN::tr - 1) {
    if (mux == 0.0)
      return 1E200;

    if (P.verbose > 3) {
      this->Marking.printHeader(cerr);
      cerr << endl;
      this->Marking.print(cerr, 0.0);
      cerr << endl;
      vector<int> vect(this->muprob->S.begin()->first->size(), 0);
      static_cast<S *>(this)->lumpingFun(this->Marking, vect);
      static_cast<S *>(this)->print_state(vect);
    }

    if (this->Origine_Rate_Sum >= this->Rate_Sum) {
      if (P.verbose > 3)
        cerr << "trans:sink distr: " << this->Origine_Rate_Sum - this->Rate_Sum
             << " origine_rate:" << this->Origine_Rate_Sum
             << " Rate: " << this->Rate_Sum << endl;
      // cerr << "strange !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
      return ((this->Origine_Rate_Sum - this->Rate_Sum));
    } else {
      if (P.verbose > 3 && (this->Origine_Rate_Sum < 0.99 * this->Rate_Sum)) {
        cerr << "Reduce model does not guarantee variance" << endl;
        cerr << "Initial sum of rate: " << this->Origine_Rate_Sum
             << " Reduce one: " << this->Rate_Sum
             << " difference: " << this->Origine_Rate_Sum - this->Rate_Sum
             << endl;
        // exit(EXIT_FAILURE);
      }
      // cerr << "trans:sink distr: 0 " << endl;
      return 0.0;
    };
  };
  if (mux == 0.0 || mux == 1.0)
    return (origin_rate);

  double distr;
  SPN::fire(t, b, 0.0);
  static_cast<numericalSolver *>(this->muprob)->stepVect();
  distr = origin_rate * (mu() / mux);
  if (P.verbose > 3)
    cerr << "trans: " << this->Transition[t].label << "\tdistr: " << distr
         << "\torigin Rate: " << origin_rate << "\tmu: " << mu()
         << "\tmu prec: " << mux << endl;

  static_cast<numericalSolver *>(this->muprob)->previousVect();
  SPN::unfire(t, b);
  return (distr);
}
