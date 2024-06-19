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
 * file numericalSolverSH.cpp created by Benoit Barbot on 06/01/12.            *
 *******************************************************************************
 */

#include "numSolverSH.hpp"
#include <time.h>

bool numSolverSH::readbit(int a, int b) {
  int c = a & (1 << b);
  return (0 != c);
}

void numSolverSH::initVect(int nT) {
  time_t start, endt;
  time(&start);

  T = nT;
  u = T;
  l = (int)log2(T);

  // cerr << "T: " <<T << " l: " << l << endl;

  // powTVect = new vector< boostmat::vector<double> > (l+1, *finalVector);
  lastOne = new vector<boostmat::vector<double>>(l + 2, *finalVector);
  ktable = new vector<double>(l + 2, 0);
  lastPowT = 1 << l;

  // cerr << "lastPow" << lastPowT << " log(T) " << l << endl;

  boostmat::vector<double> itervect = *finalVector;
  boostmat::vector<double> itervect2 =
      boostmat::zero_vector<double>(finalVector->size());

  time(&endt);
  cerr << "time for allocation:" << difftime(endt, start) << endl;

  // We suppose here that the initial state is the first of the vector
  if (itervect(0) != 0.0)
    minT = 0;

  for (int i = 1; i <= lastPowT; i++) {
    // cerr << "currPow " << currPow << " newPow " << nextPow << endl;
    itervect2.clear();
    sparseProd(&itervect2, &itervect, transitionsMatrix);
    itervect = itervect2;

    if ((itervect(0) != 0.0) && (minT == -1))
      minT = i;
    // cerr << "i: " <<i << " v: " <<itervect << endl;
    // itervect = boostmat::prod ((*transitionsMatrix), itervect);
  }
  (*lastOne)[l] = itervect;
  (*ktable)[l] = lastPowT;
  (*ktable)[l + 1] = 0;

  time(&endt);
  cerr << "time for precalculation:" << difftime(endt, start) << endl;

  cerr << "Starting the simulation" << endl;
  // cerr << "finish init" << endl;
}

void numSolverSH::compPow(int kp, int up) {
  // cerr << "compPow("<< kp << "," << up << ")" << endl;

  int k = kp - 1;
  boostmat::vector<double> itervect = (*lastOne)[kp];
  boostmat::vector<double> itervect2 =
      boostmat::zero_vector<double>(finalVector->size());

  while (k > 0 && readbit(up, k) == 0) {
    // cerr << itervect<< endl;
    (*lastOne)[k] = itervect;
    (*ktable)[k] = (*ktable)[kp];
    // cerr << "k--" << endl;
    k--;
  }

  int m = 1 << k; // (m = 2^k
  for (int i = (int)(*ktable)[kp] + 1; i <= up; i++) {
    // cerr << "i: " << i << " k: " << k << " m: " << m << endl;
    itervect2.clear();
    sparseProd(&itervect2, &itervect, transitionsMatrix);
    itervect = itervect2;

    if ((itervect(0) != 0.0) && (minT == -1))
      minT = i;

    // itervect = boostmat::prod ((*transitionsMatrix), itervect);
    m--;
    if (m == 0) {
      (*lastOne)[k] = itervect;
      (*ktable)[k] = i;
      k--;
      // cerr << "k--1" << endl;
      while (k > 0 && readbit(up, k) == 0) {
        (*lastOne)[k] = itervect;
        (*ktable)[k] = i;
        // cerr << "k--" << endl;
        k--;
      }

      m = 1 << k;
    }
  }
  // cerr << "finish";
  // cerr << itervect << endl;
  previous_vect = current_vect;
  current_vect = itervect;
  is_previous = false;
  /*current_vect= boostmat::zero_vector<double> (itervect.size());
       sparseProd(&current_vect, &itervect, transitionsMatrix);*/
  // current_vect = boostmat::prod ((*transitionsMatrix), itervect);
  // cerr << "finish reset" << endl;
}

void numSolverSH::reset() {
  time_t start, endt;
  time(&start);

  u = T;
  compPow(l, T);

  time(&endt);
  cerr << "time for reset:" << difftime(endt, start) << endl << endl;

  // cerr << "finish reset" << endl;
}

void numSolverSH::switchOff() { u = -1; }

void numSolverSH::stepVect() {
  // cerr << "step vect u:" << u << "->" << u-1 << "(";
  u--;
  if (is_previous) {
    is_previous = false;
  } else {
    if (u == 0) {
      previous_vect = current_vect;
      current_vect = *finalVector;
    } else if (u > 0) {
      int kp = (int)log2((u ^ (u + 1)) + 1);
      // cerr << "u: " << u<< ": kp: " << kp << endl;
      compPow(kp, u);
    }
  }

  // cerr << ")" << endl;
}

void numSolverSH::previousVect() {
  u++;
  if (is_previous) {
    cerr << "fail to compute previous vect" << endl;
  } else {
    is_previous = true;
  }
}

boostmat::vector<double> numSolverSH::getVect() {
  if (is_previous) {
    return previous_vect;
  } else
    return current_vect;
}

double numSolverSH::getMu(int i) const {
  if (u < 0)
    return 1.0;

  if (is_previous) {
    return previous_vect[i];
  } else
    return current_vect[i];
}

int numSolverSH::currentRound() { return T - u; }

void numSolverSH::printState() { cerr << u; }
