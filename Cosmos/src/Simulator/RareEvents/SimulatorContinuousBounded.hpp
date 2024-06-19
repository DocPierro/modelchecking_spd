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
 * file SimulatorContinuousBounded.hpp created by Benoit Barbot on 31/01/12.   *
 *******************************************************************************
 */

#ifndef Cosmos_SimulatorContinuousBounded_h
#define Cosmos_SimulatorContinuousBounded_h

#include "SimulatorBoundedRE.hpp"
#include "foxglynn.hpp"

template <class DEDS>
class SimulatorContinuousBounded
    : public SimulatorBoundedREBase<SimulatorContinuousBounded<DEDS>, DEDS> {
public:
  SimulatorContinuousBounded(DEDS &deds, LHA_orig<typename DEDS::stateType> &A,
                             int m, double e, int js);
  void initVectCo(double t);
  double epsilon;
  int jumpsize;
  bool singleIS;
  // double Normal_quantile;
  BatchR RunBatch();

protected:
  unique_ptr<FoxGlynn> fg;
};

#endif
