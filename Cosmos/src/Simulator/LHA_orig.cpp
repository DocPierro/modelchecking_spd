/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2012 LSV & LACL                                          *
 * Authors: Paolo Ballarini & Hilal Djafri                                     *
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
 * with this program; if not, write to the Free Software Foundation, Inc., *· 51
 *Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
 *******************************************************************************
 */

#include "LHA_orig.hpp"
#include "LHA.hpp"

#include <float.h>
#include <map>
#include <math.h>

using namespace std;

template <class DEDState> void LHA_orig<DEDState>::copyState(LHA_orig *A) {
  this->Vars = A->Vars;
  this->LinForm.swap(A->LinForm);
  this->OldLinForm.swap(A->OldLinForm);
  this->LhaFunc.swap(A->LhaFunc);
  this->Likelihood = A->Likelihood;
  this->CurrentTime = A->CurrentTime;
  this->CurrentLocation = A->CurrentLocation;
}

/**
 * This function makes the automaton takes an autonomous edge.
 * @param EdgeIndex the number of the edge of the LHA
 * @param M is the marking of the SPN
 */
template <class DEDState>
void LHA_orig<DEDState>::fireAutonomous(int EdgeIndex, const DEDState &M) {
  static const abstractBinding dummyBinding;
  this->DoEdgeUpdates(EdgeIndex, M, dummyBinding);
  this->CurrentLocation = this->Edge[EdgeIndex].Target;
}

/**
 * Find a suitable syncronized transitions in the LHA
 * @param tr a SPN transiiton index
 * @param M is the marking of the SPN
 * @param b a binding of the colored variable of the SPN for the transition.
 */
template <class DEDState>
int LHA_orig<DEDState>::synchroniseWith(size_t tr, const DEDState &m,
                                        const abstractBinding &b) {
  // Check if there exist a valid transition in the automata.

  int SE = GetEnabled_S_Edges(tr, m, b);

  if (SE >= 0) {
    // If synchronisation is possible fire it
    fireLHA(SE, m, b);
  }
  return SE;
}

/**
 *	Return the next autonomous edge.
 *	@param Marking is the current marking of the Petri net.
 *	@return the most urgent autonomous edge
 */
template <class DEDState>
AutEdge LHA_orig<DEDState>::GetEnabled_A_Edges(const DEDState &Marking) {
  AutEdge Ed;
  Ed.Index = -1;
  Ed.FiringTime = DBL_MAX;
  static const abstractBinding dummyBinding;
  for (auto it : this->Out_A_Edges[this->CurrentLocation]) {
    if (this->CheckLocation(this->Edge[it].Target, Marking) &&
        this->CheckEdgeContraints(it, 0, dummyBinding, Marking)) {
      t_interval I = this->GetEdgeEnablingTime(it, Marking);
      if (I.first <= I.second) {
        if (I.first < Ed.FiringTime) {
          Ed.Index = it;
          Ed.FiringTime = I.first;
        }
      }
    }
  }

  return Ed;
}

/**
 * This function makes time elapse in the automaton.
 * @param DeltaT the ammout of time the automaton should wait.
 * @param Marking is the Marking of the SPN.
 */

template <class DEDState>
void LHA_orig<DEDState>::updateLHA(double DeltaT, const DEDState &Marking) {
  this->DoElapsedTimeUpdate(DeltaT, Marking);
  this->UpdateLinForm(Marking);
  this->UpdateLhaFunc(DeltaT);
  this->CurrentTime += DeltaT;
}

/**
 *	Reset the whole automaton to its initial state for
 * the given Marking.
 */
template <class DEDState>
void LHA_orig<DEDState>::reset(const DEDState &Marking) {
  Likelihood = 1.0;
  resetLinForms();
  this->resetVariables();
  setInitLocation(Marking);
  this->CurrentTime = 0;
}

/**
 *	This function is called when the automaton reach a final state.
 *	The result of path formula is stored in vector v
 */
template <class DEDState>
void LHA_orig<DEDState>::getFinalValues(const DEDState &m, vector<double> &v,
                                        vector<bool> &v2) {
  this->UpdateLinForm(m);
  this->UpdateFormulaVal(m);
  v = this->FormulaVal;
  v2 = this->FormulaValQual;
}

/**
 * This function makes the automaton takes an edge.
 * The edge can be either a autonomous or a synchronize on.
 * @param EdgeIndex the number of the edge of the LHA
 * @param M is the marking of the SPN
 * @param b a binding of the colored variable of the SPN for the transition.
 */
template <class DEDState>
void LHA_orig<DEDState>::fireLHA(int EdgeIndex, const DEDState &M,
                                 const abstractBinding &b) {
  this->DoEdgeUpdates(EdgeIndex, M, b);
  this->CurrentLocation = this->Edge[EdgeIndex].Target;
}

/**
 *	Set the automaton to its initial location.
 * If the automaton contain several initial state only one should satisfie
 * its invarient with the given Marking due to determinicity.
 * @param Marking, A marking of the Petri net in principle the initial marking.
 */
template <class DEDState>
void LHA_orig<DEDState>::setInitLocation(const DEDState &Marking) {
  for (const auto &l : this->InitLoc) {
    if (this->CheckLocation(l, Marking)) {
      this->CurrentLocation = l;
      return;
    }
  }

  cerr << "There is no initial location enabled with the initial marking"
       << endl;
  exit(EXIT_FAILURE);
}

/**
 *	Return a sychronized edge index with a transition.
 * The edge is such that the location invariant will holds in the marking of the
 * Petri net after the transition.
 * @param PetriNetTransition, a Petri net transition index.
 * @param bindin, the color binding of the transition.
 * @param NextMarking, The marking in with the Petri net will be after the
 *transition.
 * @return an index of synchronized edge or -1 if there is no suitable
 *synchronized edge.
 */
template <class DEDState>
int LHA_orig<DEDState>::GetEnabled_S_Edges(size_t PetriNetTransition,
                                           const DEDState &NextMarking,
                                           const abstractBinding &binding) {
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
        // assert(it==oracle);
        return it;
      }
    }
  }
  return (-1);
}

/**
 * Reset all linear form.
 */
template <class DEDState> void LHA_orig<DEDState>::resetLinForms() {
  for (size_t i = 0; i < this->LinForm.size(); i++) {
    this->LinForm[i] = 0;
    this->OldLinForm[i] = 0;
  }
  for (size_t i = 0; i < this->LhaFunc.size(); i++)
    this->LhaFunc[i] = this->LhaFuncDefaults[i];
}

template <class DEDState>
double LHA<DEDState>::Min(double a, double b, double c) {
  double x = min(b, c);
  return min(a, x);
}

template <class DEDState>
double LHA<DEDState>::Max(double a, double b, double c) {
  double x = max(b, c);
  return max(a, x);
}

template <class DEDState>
double LHA<DEDState>::Integral(double OldInt, double, double Delta, double x,
                               double y) {
  return (OldInt + Delta * (x + y) / 2);
}

template <class DEDState>
double LHA<DEDState>::BoxedIntegral(double OldInt, double t, double Delta,
                                    double x, double y, double t1, double t2) {
  if (t >= t2 || t + Delta <= t1)
    return OldInt;
  double slope = (y - x) / Delta;
  if (t1 > t) {
    x += (t1 - t) * slope;
  } else
    t1 = t;
  if (t2 < t + Delta) {
    y -= (t + Delta - t2) * slope;
  } else
    t2 = t + Delta;
  return (OldInt + (t2 - t1) * (x + y) / 2);
}

// #include "MarkovChain.hpp"
// template class LHA<State>;
// template class LHA_orig<State>;
