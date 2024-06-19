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
 * file SimulatorRE.hpp created by Benoit Barbot on 09/11/11.                  *
 *******************************************************************************
 */

#ifndef _SIMULATOR_RE_HPP
#define _SIMULATOR_RE_HPP

#include "SPNBase.hpp"
#include "Simulator.hpp"
#include "stateSpace.hpp"

template <class S>
class SPNBaseRE : public SPNBase<S, EventsQueue>, public REHandling {
public:
  SPNBaseRE(bool doubleIS);

  bool rareEventEnabled;

  void initialize(stateSpace *muprob);
  void update(double ctime, size_t, const abstractBinding &, EventsQueue &,
              timeGen &);
  void InitialEventsQueue(EventsQueue &, timeGen &);

  double mu();
  const bool doubleIS_mode;

  double Rate_Sum;
  double Origine_Rate_Sum;
  std::vector<double> Rate_Table;
  std::vector<double> Origine_Rate_Table;

  /* private */
  stateSpace *muprob;

  void getParams(size_t, const abstractBinding &);
  double ComputeDistr(size_t i, const abstractBinding &, double origin_rate);
};

template <class S, class DEDS>
class SimulatorREBase : public SimulatorBase<S, EventsQueue, DEDS> {
public:
  SimulatorREBase(DEDS &N, LHA_orig<typename DEDS::stateType> &);

  void initVect();

  /* private */

  // TAB muprob;  // mu(s) table
  stateSpace *muprob;

  void SimulateSinglePath();
  void returnResultTrue();
  // virtual void GenerateDummyEvent(Event &, size_t);
  void updateLikelihood(size_t);
  bool transitionSink(size_t);
  void reset();
};

class SPN_RE : public SPNBaseRE<SPN_RE> {
public:
  SPN_RE(bool doubleIS) : SPNBaseRE<SPN_RE>(doubleIS){};
};

template <class DEDS>
class SimulatorRE : public SimulatorREBase<SimulatorRE<DEDS>, DEDS> {
public:
  SimulatorRE(DEDS &deds, LHA_orig<typename DEDS::stateType> &lha)
      : SimulatorREBase<SimulatorRE, DEDS>(deds, lha){};
};

#endif /* _SIMULATOR_RE_HPP */
