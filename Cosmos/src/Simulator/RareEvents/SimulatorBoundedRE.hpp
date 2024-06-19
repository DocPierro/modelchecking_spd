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
 * file SimulatorBoundedRE.hpp created by Benoit Barbot on 06/12/11.           *
 *******************************************************************************
 */

/* This class inherit the class of Simulator and modify
 * several intern function to implement rare event acceleration.
 */

#include "SimulatorRE.hpp"
#include "marking.hpp"
#include "numericalSolver.hpp"
#include <vector>

#ifndef _SIMULATOR_BOUNDED_RE_HPP
#define _SIMULATOR_BOUNDED_RE_HPP

template <class S, class EQT> class simulationState {
private:
  abstractMarking marking;
  EQT *EQ;
  LHA_orig<typeof marking> lhaState;

  // rare event variable
  vector<double> Rate_Table;
  vector<double> Origine_Rate_Table;
  double Rate_Sum;
  double Origine_Rate_Sum;

public:
  int maxStep;
  simulationState() {
    // EQ = new EventsQueue(n);
    maxStep = 0;
  };
  ~simulationState(){};

  void saveState(S *N, LHA_orig<typeof marking> *A, EQT **EQsim) {
    marking.swap(N->Marking);
    // AE = *AEsim;
    EQ = *EQsim; // new EventsQueue(*EQsim);

    lhaState.copyState(A);

    Rate_Table.swap(N->Rate_Table);
    Origine_Rate_Table.swap(N->Origine_Rate_Table);
    Rate_Sum = N->Rate_Sum;
    Origine_Rate_Sum = N->Origine_Rate_Sum;
  };
  void loadState(S *N, LHA_orig<typeof marking> *A, EQT **EQsim) {

    N->Marking.swap(marking);
    //*AEsim = AE;
    *EQsim = EQ;

    A->copyState(&lhaState);

    N->Rate_Table.swap(Rate_Table);
    N->Origine_Rate_Table.swap(Origine_Rate_Table);
    N->Rate_Sum = Rate_Sum;
    N->Origine_Rate_Sum = Origine_Rate_Sum;
  };
};

template <class S> class SPNBaseBoundedRE : public SPNBaseRE<S> {
public:
  SPNBaseBoundedRE(bool doubleIS) : SPNBaseRE<S>(doubleIS){};

  void update(double ctime, size_t, const abstractBinding &, EventsQueue &,
              timeGen &);
  void getParams(size_t, const abstractBinding &);
  double mu();
  double ComputeDistr(size_t i, const abstractBinding &, double origin_rate);
};

template <class S, class DEDS>
class SimulatorBoundedREBase : public SimulatorREBase<S, DEDS> {
public:
  // SimulatorBoundedRE();
  SimulatorBoundedREBase(DEDS &N, LHA_orig<typename DEDS::stateType> &, int m);
  BatchR RunBatch();
  using SimulatorREBase<S, DEDS>::initVect;
  void initVect(int T);

  /* private */
  numericalSolver *numSolv;
  double lambda;
};

class SPN_BoundedRE : public SPNBaseBoundedRE<SPN_BoundedRE> {
public:
  SPN_BoundedRE(bool doubleIS) : SPNBaseBoundedRE<SPN_BoundedRE>(doubleIS){};
};

template <class DEDS>
class SimulatorBoundedRE
    : public SimulatorBoundedREBase<SimulatorBoundedRE<DEDS>, DEDS> {
public:
  SimulatorBoundedRE(DEDS &deds, LHA_orig<typename DEDS::stateType> &lha, int m)
      : SimulatorBoundedREBase<SimulatorBoundedRE<DEDS>, DEDS>(deds, lha, m){};
};

#endif /* _SIMULATOR_BOUNDED_RE_HPP */
