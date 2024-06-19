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
 * with this program; if not, write to the Free Software Foundation, Inc.,     *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
 *******************************************************************************
 */

#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "marking.hpp"
#include "spn.hpp"

#include <stdlib.h>

#ifndef _LHA_HPP
#define _LHA_HPP

typedef std::pair<double, double> t_interval;

using namespace std;

//!< A simple type to know if an edge is synchronize with the SPN.
enum EdgeType { Auto, Synch };

/**
 * Type for edge of the LHA.
 */
struct LhaEdge {
  unsigned int Index;
  unsigned int Source;
  unsigned int Target;
  EdgeType Type;
  LhaEdge() : Index(0), Source(0), Target(0), Type(Auto){};
  LhaEdge(unsigned int i, unsigned int s, unsigned int t, const EdgeType &ty)
      : Index(i), Source(s), Target(t), Type(ty){};
};

/**
 * Auto edge need a firing time
 */
struct _AutEdge {
  int Index;
  double FiringTime;
};
typedef struct _AutEdge AutEdge;

/**
 * Strucuture for the variable of the automaton.
 * Implementation is provide only at runtime.
 */
struct Variables;
/**
 Strict order on valuation set provide at runtime.
 */
bool varOrder(const Variables &, const Variables &);

extern bool IsLHADeterministic;

/**
 * Class implementing the Linear Hybrid Automaton.
 * Part of the implementation is generated at runtime for efficiency.
 */
template <class DEDState> class LHA {
public:
  LHA();

  // number of locations
  const unsigned int NbLoc;

  /**
   * Current location of the LHA
   */
  int CurrentLocation;

  /**
   * Current time of the LHA
   */
  double CurrentTime;

  // Result of Computation
  vector<double> FormulaVal;
  vector<bool> FormulaValQual;

  // Pretty printing
  void printHeader(ostream &) const;
  virtual void printState(ostream &);

  // Topologie of automata
  const size_t NbTrans;
  const size_t NbVar;

protected:
  vector<LhaEdge> Edge;
  vector<int> EdgeCounter;
  set<int> InitLoc;             // initial locations
  vector<bool> FinalLoc;        // final locations
  vector<set<int>> Out_A_Edges; // for a given location l returns the set of
                                // autonomous edges  starting from l
  static const int ActionEdgesAr[];

  // Labels for pretty printing, empty when verbose < 4
  vector<string> VarLabel;
  string label;
  vector<string> LocLabel;

  // Runtime variables
  Variables *Vars;     // Var[i] value of the variable indexed by i
  Variables *tempVars; // Temporary compy of vars
  vector<double> LinForm;
  vector<double> OldLinForm;
  vector<double> LhaFunc;
  vector<double> LhaFuncDefaults;

  void resetVariables();
  void DoElapsedTimeUpdate(double, const DEDState &);
  double GetFlow(int, const DEDState &) const;
  bool CheckLocation(int, const DEDState &) const;
  bool CheckEdgeContraints(int, size_t, const abstractBinding &,
                           const DEDState &) const;
  t_interval GetEdgeEnablingTime(int, const DEDState &) const;
  void DoEdgeUpdates(int, const DEDState &, const abstractBinding &);
  void UpdateLinForm(const DEDState &);
  void UpdateLhaFunc(double &);
  void UpdateFormulaVal(const DEDState &);

  double Min(double, double, double);
  double Max(double, double, double);
  double Integral(double, double, double, double, double);
  double BoxedIntegral(double OldInt, double t, double Delta, double x,
                       double y, double t1, double t2);
};

class fullState {
public:
  int loc;
  Variables *var;
  inline bool operator<(const fullState &other) const {
    return this->loc < other.loc ||
           (this->loc == other.loc && varOrder(*(this->var), *(other.var)));
  }
  fullState();
  fullState(int l, const Variables &v);
  fullState(const fullState &);
  ~fullState();
};
#endif /* _LHA_HPP */
