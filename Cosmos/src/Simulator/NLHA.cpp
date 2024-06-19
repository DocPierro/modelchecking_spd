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
 * file NLHA.cpp created by Benoit Barbot on 05/08/15.                         *
 *******************************************************************************
 */

#include <float.h>

#include "NLHA.hpp"

using namespace std;

template <class DEDState>
void NLHA<DEDState>::setInitLocation(const DEDState &Marking) {
  for (const auto &l : this->InitLoc) {
    if (this->CheckLocation(l, Marking)) {
      powerSetState->emplace(l, *(this->Vars));
    }
  }
}

template <class DEDState> void NLHA<DEDState>::reset(const DEDState &Marking) {
  fullState fs;
  this->Vars = fs.var;
  this->resetVariables();
  powerSet[0].clear();
  powerSet[1].clear();
  powerSetState = &powerSet[0];
  selectPS = 0;
  setInitLocation(Marking);
  this->CurrentTime = 0;
}

template <class DEDState> void NLHA<DEDState>::printState(ostream &s) {
  s << "{";
  for (let fs : *powerSetState) {
    this->Vars = fs.var;
    this->CurrentLocation = fs.loc;
    s << "(";
    LHA<DEDState>::printState(s);
    s << "), ";
  }
  s << "} ";
}

template <class DEDState>
void NLHA<DEDState>::updateLHA(double DeltaT, const DEDState &Marking) {
  for (let fs : *powerSetState) {
    this->Vars = fs.var;
    this->CurrentLocation = fs.loc;
    this->DoElapsedTimeUpdate(DeltaT, Marking);
  }

  this->CurrentTime += DeltaT;
}

/*
 In Non deterninistic LHA this function actually compute the new state. Fire
 will do nothing.
 */
template <class DEDState>
int NLHA<DEDState>::synchroniseWith(size_t PetriNetTransition,
                                    const DEDState &NextMarking,
                                    const abstractBinding &binding) {
  auto &newPS = powerSet[1 - selectPS];
  for (let fs : *powerSetState) {
    const auto fs2 = fs;
    this->Vars = fs.var;
    this->CurrentLocation = fs.loc;

    const size_t mult = this->NbLoc * this->NbTrans;
    for (int i = 1;
         i <= this->ActionEdgesAr[this->NbTrans * this->CurrentLocation +
                                  PetriNetTransition];
         i++) {
      // cerr << i << endl;
      const int it = this->ActionEdgesAr[this->NbTrans * this->CurrentLocation +
                                         PetriNetTransition + i * mult];
      // cerr << it << endl;
      if ((this->CheckLocation(this->Edge[it].Target, NextMarking))) {
        if (this->CheckEdgeContraints(it, PetriNetTransition, binding,
                                      NextMarking)) {
          // Valid transition compute new state
          this->DoEdgeUpdates(it, NextMarking, binding);
          this->CurrentLocation = this->Edge[it].Target;
          // Add it to the set of next state
          newPS.emplace(this->CurrentLocation, *(this->Vars));
          // Restore the current state.
          this->Vars = fs2.var;
          this->CurrentLocation = fs2.loc;
        }
      }
    }
  }
  if (newPS.empty()) {
    return (-1);
  } else {
    selectPS = 1 - selectPS;
    powerSetState = &powerSet[selectPS];
    powerSet[1 - selectPS].clear();
    return 1;
  }
}

template <class DEDState>
void NLHA<DEDState>::getFinalValues(const DEDState &m, vector<double> &,
                                    vector<bool> &v2) {
  for (size_t i = 0; i < this->FormulaValQual.size(); i++) {
    v2[i] = false;
  }
  for (let fs : *powerSetState) {
    this->Vars = fs.var;
    this->CurrentLocation = fs.loc;
    this->UpdateFormulaVal(m);
    for (size_t i = 0; i < this->FormulaValQual.size(); i++) {
      v2[i] = v2[i] || this->FormulaValQual[i];
    }
  }
}

template <class DEDState>
AutEdge NLHA<DEDState>::GetEnabled_A_Edges(const DEDState &Marking) {
  // TODO Check for bugs Not working due to infinit loop
  AutEdge Ed;
  Ed.Index = -1;
  Ed.FiringTime = DBL_MAX;
  for (let fs : *powerSetState) {
    this->Vars = fs.var;
    this->CurrentLocation = fs.loc;
    let I = LHA_orig<DEDState>::GetEnabled_A_Edges(Marking);
    if (I.FiringTime < Ed.FiringTime) {
      cerr << "Autonomous: " << I.Index << "\tTime:" << I.FiringTime << endl;
      Ed.Index = I.Index;
      Ed.FiringTime = I.FiringTime;
      assert(false); // Does not work currently
    }
  }
  return Ed;
}

template <class DEDState> bool NLHA<DEDState>::isFinal() const {
  for (let fs : *powerSetState) {
    if (this->FinalLoc[fs.loc])
      return true;
  }
  return false;
}

extern template class LHA<abstractMarking>;
template class NLHA<abstractMarking>;
