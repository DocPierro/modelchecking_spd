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
 * file numericalSolverBB.cpp created by Benoit Barbot on 06/01/12.            *
 *******************************************************************************
 */

#include "numSolverBB.hpp"

void numSolverBB::initVect(int nT) {
  T = nT;
  u = T;
  l = (int)sqrt(T);
  time_t start, endt;
  time(&start);

  minT = -1;

  circularvect = new vector<boostmat::vector<double>>(
      l + 1, boostmat::zero_vector<double>(finalVector->size()));
  checkPoint = new vector<boostmat::vector<double>>(T / l + 1, *finalVector);

  time(&endt);
  cerr << "time for allocation:" << difftime(endt, start) << endl;

  lastCP = (T / l) * l;
  boostmat::vector<double> itervect = *finalVector;
  boostmat::vector<double> itervect2 =
      boostmat::zero_vector<double>(finalVector->size());
  // We suppose here that the initial state is the first of the vector
  if (itervect(0) != 0)
    minT = 0;

  for (int i = 1; i <= lastCP; i++) {
    itervect2.clear();
    sparseProd(&itervect2, &itervect, transitionsMatrix);
    itervect = itervect2;

    if ((itervect(0) != 0) && (minT == -1))
      minT = i;
    // cerr << "i:" << i << "v0:\t" << itervect (0) << "mint:\t" << minT<< endl;

    // itervect = boostmat::prod ((*transitionsMatrix), itervect);
    if (i % l == 0)
      (*checkPoint)[i / l] = itervect;
  }

  time(&endt);
  cerr << "time for precalculation:" << difftime(endt, start) << endl;

  cerr << "Starting the simulation" << endl;
}

void numSolverBB::reset() {
  time_t start, endt;
  time(&start);

  (*circularvect)[0] = (*checkPoint)[lastCP / l];

  /*for(int i=1; i<=T-lastCP ; i++){
   (*circularvect)[i] = boostmat::prod ((*transitionsMatrix),
   (*circularvect)[i-1]);
   }*/
  for (int i = 1; i <= T - lastCP; i++) {
    ((*circularvect)[i]).clear();
    sparseProd(&((*circularvect)[i]), &((*circularvect)[i - 1]),
               transitionsMatrix);
    // cerr << "i:" << i << "v0:\t" << (*circularvect)[i] (0) << "mint:\t" <<
    // minT<< endl;
    if (((*circularvect)[i](0) != 0.0) && (minT == -1))
      minT = i + lastCP;

    // cerr << "itervect:" << i << ":"<< (*circularvect)[i] << endl;
  }
  time(&endt);
  cerr << "time for reset:" << difftime(endt, start) << endl << endl;

  u = T;
  matOffset = T - lastCP;
}

void numSolverBB::stepVect() {
  // cerr << "step:" << u << endl;
  if (matOffset > 0 || u == 0) {
    matOffset--;
    u--;
  } else {
    u--;
    (*circularvect)[l] = (*circularvect)[0];
    (*circularvect)[0] = (*checkPoint)[u / l];
    for (int i = 1; i < l; i++) {
      (*circularvect)[i].clear();
      sparseProd(&((*circularvect)[i]), &((*circularvect)[i - 1]),
                 transitionsMatrix);
      //(*circularvect)[i] = boostmat::prod ((*transitionsMatrix),
      //(*circularvect)[i-1]);
    }
    matOffset = l - 1;
  }
}

void numSolverBB::previousVect() {
  u++;
  matOffset++;
}

void numSolverBB::printState() { cerr << u; }

int numSolverBB::currentRound() { return T - u; }
