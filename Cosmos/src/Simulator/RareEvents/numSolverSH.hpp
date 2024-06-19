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
 * file numericalSolverSH.hpp created by Benoit Barbot on 06/01/12.            *
 *******************************************************************************
 */

#include "numericalSolver.hpp"

class numSolverSH : public numericalSolver {
public:
  void initVect(int) override;
  void previousVect() override;
  void stepVect() override;
  void reset() override;
  void switchOff() override;
  void printState() override;
  virtual double getMu(int) const override;
  boostmat::vector<double> getVect() override;
  int currentRound() override;

protected:
  bool readbit(int a, int b);
  // vector<boostmat::vector<double> >* powTVect;
  vector<boostmat::vector<double>> *lastOne;
  vector<double> *ktable;
  int l;
  int u;
  int lastPowT;
  void compPow(int, int);

  bool is_previous;

  boostmat::vector<double> current_vect;
  boostmat::vector<double> previous_vect;
};
