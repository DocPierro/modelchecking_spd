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
 * file MultiModel.hpp created by Benoit Barbot on 06/04/17.                   *
 *******************************************************************************
 */

#ifndef MultiModel_hpp
#define MultiModel_hpp

#include <iostream>
#include <vector>

#include "marking.hpp"
#include "timeGen.hpp"

template <typename EQT, typename M1, typename M2> class MultiModel {
  M1 m1;
  M2 m2;

public:
  using stateType = abstractMarking;

  //! Current marking
  // abstractMarking Marking;
  //! contains all the transitions of the Petri net
  // std::vector<spn_trans> Transition;
  size_t lastTransition; //! store the last fired transition

  MultiModel(M1 nm1, M2 nm2)
      : m1(nm1), m2(nm2) /*,Transition(m1.Transition)*/ {};

  const stateType &getState() const { return m1.getState(); }

  void printSedCmd(std::ostream &of) const {
    m1.printSedCmd(of);
    m2.printSedCmd(of);
  }

  void printHeader(std::ostream &of) const {
    m1.printHeader(of);
    m2.printHeader(of);
  }

  void printState(std::ostream &of, double et) const {
    m1.printState(of, et);
    m2.printState(of, et);
  }

  void initialEventsQueue(EQT &EQ, timeGen &tg) {
    m1.initialEventsQueue(EQ, tg);
    m2.initialEventsQueue(EQ, tg);
  }

  void reset() {
    m1.reset();
    m2.reset();
  }

  void synchronize_fire(size_t t);
  void synchronize_update(double ctime, size_t tr, const abstractBinding &lb,
                          EQT &EQ, timeGen &TG);

  void fire(size_t tr, const abstractBinding &b, double time) {
    if (tr >= m2.shift) {
      m2.fire(tr - m2.shift, b, time);
    } else {
      m1.fire(tr, b, time);
    }
    // cerr << "synchronize("<< tr <<")"<< endl;
    synchronize_fire(tr);
  }

  void update(double ctime, size_t tr, const abstractBinding &lb, EQT &EQ,
              timeGen &TG) {
    if (tr >= m2.shift) {
      m2.update(ctime, tr - m2.shift, lb, EQ, TG);
    } else {
      m1.update(ctime, tr, lb, EQ, TG);
    }
    synchronize_update(ctime, tr, lb, EQ, TG);
  }

  const std::vector<size_t> getNbTransition() const {
    auto m1v = m1.getNbTransition();
    for (let t : m2.getNbTransition())
      m1v.push_back(t);
    return m1v;
  }

  const std::vector<std::string> getTransitionLabels() const {
    auto m1v = m1.getTransitionLabels();
    for (let t : m2.getTransitionLabels())
      m1v.push_back(t);
    return m1v;
  }

  const std::string getTransitionLabel(size_t i) const {
    if (i < m1.tr)
      return m1.getTransitionLabel(i);
    return m2.getTransitionLabel(i - m1.tr);
  }
};

#endif /* MultiModel_hpp */
