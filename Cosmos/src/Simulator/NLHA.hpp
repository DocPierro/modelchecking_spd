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
 * file NLHA.hpp created by Benoit Barbot on 05/08/15.                         *
 *******************************************************************************
 */

#ifndef __Cosmos__NLHA__
#define __Cosmos__NLHA__

#include <set>
#include <stdio.h>

#include "LHA_orig.hpp"

template <class DEDState> class NLHA : public LHA_orig<DEDState> {

  std::set<fullState, std::less<fullState>> powerSet[2];
  std::set<fullState, std::less<fullState>> *powerSetState;
  int selectPS;

public:
  NLHA() : powerSetState(&powerSet[0]), selectPS(0){};
  virtual void updateLHA(double DeltaT, const DEDState &) override;
  virtual int synchroniseWith(size_t, const DEDState &,
                              const abstractBinding &) override;
  virtual AutEdge GetEnabled_A_Edges(const DEDState &Marking) override;

  virtual void printState(ostream &) override;
  virtual bool isFinal() const override;
  virtual void reset(const DEDState &) override;
  virtual void getFinalValues(const DEDState &m, vector<double> &,
                              vector<bool> &) override;

private:
  virtual void setInitLocation(const DEDState &Marking) override;
};

#endif /* defined(__Cosmos__NLHA__) */
