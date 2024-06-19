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

/**
 * This classe is the interface for SPN
 */

#ifndef _SPN_HPP
#define _SPN_HPP

#include <array>
#include <assert.h>
#include <iostream>
#include <limits.h>
#include <map>
#include <math.h>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

#include "DistributionDef.hpp"
#include "marking.hpp"

#define TR_PL_ID unsigned long
#define REAL_TYPE double

/**
 * Datatype for transition of the SPN
 */
struct _trans {
  _trans(){};

  //! transition constructor
  _trans(unsigned int id, DistributionType dti, bool MD, size_t nbb, bool am)
      : Id(id), DistTypeIndex(dti), MarkingDependent(MD), AgeMemory(am),
        bindingLinkTable(nbb, std::string::npos){};
  _trans(unsigned int id, DistributionType dti, bool MD, size_t nbb, bool am,
         std::string l)
      : Id(id), label(l), DistTypeIndex(dti), MarkingDependent(MD),
        AgeMemory(am), bindingLinkTable(nbb, std::string::npos){};

  //! number of the transition
  unsigned int Id;

  //! Name of the transition, can be empty
  std::string label;
  DistributionType DistTypeIndex;

  bool MarkingDependent;
  //! true if the memory policy of the transition is age memory
  bool AgeMemory;
  //! List of alowed binding for this transition.
  std::vector<abstractBinding> bindingList;
  //! Table to access bindings of the transition.
  std::vector<size_t> bindingLinkTable;
};
typedef struct _trans spn_trans;

template <typename T, typename C>
bool greaterIndex(std::vector<T *> v, C c, int i, int j) {
  for (let d : v) {
    int r = d->compare(c, i, j);
    if (r > 0)
      return true;
    if (r < 0)
      return false;
  }
  return false;
}

/**
 * DataType for place of the SPN
 */
struct _place {
  _place() : isTraced(true){};

  //! name of the place, can be empty
  std::string label;

  //! set to true if the place should appear in outputted trace
  bool isTraced;
};
typedef struct _place spn_place;

/**
 * \brief Class of the SPN.
 *
 * All the implementation of this class are generated at runtime.
 */
class SPN {
public:
  using stateType = abstractMarking;

  //! Current marking
  stateType Marking;

  //! Initialize all the data
  SPN();

  const CustomDistr &customDistr;

  //! Number of places
  const size_t pl;
  //! Number of transitions
  const size_t tr;

  //! contains all the transitions of the Petri net
  std::vector<spn_trans> Transition;
  //! contains all the places of the Petri net
  std::vector<spn_place> Place;

  const abstractMarking &getState() const { return Marking; };
  void setState(const abstractMarking &s) { Marking = s; };
  void setState(const std::vector<int> &v) { Marking.setVector(v); };
  void symmetrize() { Marking.Symmetrize(); };

  //! set the marking to the initial marking
  void reset();

  /**
   * \brief fire a given transition.
   * The implementation of this function is generated
   * at runtime.
   * @param tr a transition of the SPN
   * @param b a binding of the transition of the SPN
   * @param time, the current time of the simulation, only used when link
   * with external code.
   */
  void fire(size_t tr, const abstractBinding &b, double time);

  /**
   * \brief unfire a given transition.
   * The implementation of this function is generated
   * at runtime.
   * This function is only used for rare event
   * @param tr a transition of the SPN
   * @param b a binding of the transition of the SPN
   */
  void unfire(size_t tr, const abstractBinding &b);

  /**
   * \brief A vector use to store temporary parameters value.
   * This vector is used to to store parameter of distribution
   * When the simulator ask for parameters the function GetDistParameter
   * store them inside this vector.
   * This is done to avoid allocating a new vector too frequently.
   */
  mutable std::array<double, PARAM_TBL_SIZE> ParamDistr;

  /**
   * \brief Check if a given transition is enabled.
   * The implementation of this function is generated
   * at runtime.
   * @param tr a transition of the SPN
   * @param b a binding of the transition of the SPN
   */
  bool IsEnabled(size_t tr, const abstractBinding &b) const;

  /**
   * \brief compute the the parameters value of a given
   * transition.
   * The implementation of this function is generated
   * at runtime.
   * @param tr a transition of the SPN
   * @param b a binding of the transition of the SPN
   */
  void GetDistParameters(size_t tr, const abstractBinding &b) const;

  /**
   *  Selected Place for state space exploration
   */
  std::vector<int> Msimpletab;

  size_t lastTransition;     //! store the last fired transition
  double lastTransitionTime; //! store the last fired transition time for hybrid
                             //! part.

  /*private */

  void setConditionsVector();

  //! compute the the weight value of a given transition
  double GetWeight(size_t, const abstractBinding &) const;

  //! compute the the priority value of a given transition
  double GetPriority(size_t, const abstractBinding &) const;

  //! A table of set of transitions that may be enabled after firing the last
  //! transition
  static const int *PossiblyEnabled[];

  //! A table of set of transitions that may be disabled after firing the last
  //! transition
  static const int *PossiblyDisabled[];

  //! A table of set of transition without constrain but marking dependant
  static const int *FreeMarkDepT[];

  const abstractBinding *nextPossiblyEnabledBinding(size_t tr,
                                                    const abstractBinding &b,
                                                    size_t *) const;
  const abstractBinding *nextPossiblyDisabledBinding(size_t tr,
                                                     const abstractBinding &b,
                                                     size_t *) const;

  void overrideParameter(std::string &, double) const;

private:
  //! The path of the file use to generate the implementation
  std::string Path;

  void Msimple();

  //------------------------- On the fly enabling disabling transition--------
  std::vector<int> TransitionConditions;
  std::vector<int> initTransitionConditions;
  //-------------------------/On the fly enabling disabling transition--------
};

class REHandling {
public:
  void print_state(const std::vector<int> &);
  double direct_computation(const abstractMarking &);
  void lumpingFun(const abstractMarking &, std::vector<int> &);
  bool precondition(const abstractMarking &);
};

#endif /* _SPN_HPP */
