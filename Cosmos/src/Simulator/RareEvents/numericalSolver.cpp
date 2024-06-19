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
 * file numericalSolver.cpp created by Benoit Barbot on 05/12/11.              *
 *******************************************************************************
 */

#include "numericalSolver.hpp"

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/operation_sparse.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <fstream>
#include <iostream>
#include <time.h>

#define BOOST_UBLAS_NDEBUG

using namespace std;

numericalSolver::numericalSolver() {
  cerr << "Initialising the simulator" << endl;
  cerr << "Reading Matrix" << endl;
  time_t start, endt;
  time(&start);
  inputMat();
  time(&endt);
  cerr << "time for input:" << endt - start << endl;
}

void numericalSolver::sparseProd(boostmat::vector<double> *result,
                                 boostmat::vector<double> *vect,
                                 boostmat::compressed_matrix<double> *mat) {
  //*result = boostmat::zero_vector<double> (vect->size());

  // boostmat::axpy_prod((const
  // boostmat::compressed_matrix<double,boostmat::row_major>) *mat,(const
  // boostmat::vector<double>) *vect,*result,true);

  // boostmat::axpy_prod(*mat, *vect, *result, boostmat::row_major );

  // boostmat::sparse_prod<double>(mat, vect);

  // boostmat::sparse_prod(mat, vect, result);

  for (boostmat::compressed_matrix<double>::iterator1 it = mat->begin1();
       it != mat->end1(); it++) {
    for (boostmat::compressed_matrix<double>::iterator2 it2 = it.begin();
         it2 != it.end(); it2++) {
      // cerr << "iteration: " << it.index1() << ":" << it2.index2() << endl;
      (*result)(it.index1()) += ((*vect)(it2.index2()) * (*it2));
    };
  };
}

void numericalSolver::initVect(int nT) {
  time_t start, endt;
  time(&start);
  minT = -1;

  T = nT;
  circularvect = new vector<boostmat::vector<double>>(
      nT + 1, boostmat::zero_vector<double>(finalVector->size()));
  (*circularvect)[0] = *finalVector;
  time(&endt);
  cerr << "time for allocation:" << difftime(endt, start) << endl;

  // cerr << "itervect:" << 0 << ":"<< (*circularvect)[0] << endl;

  // We suppose here that the initial state is the first of the vector
  if (!((*circularvect)[0](0) == 0))
    minT = 0;

  // int test=0;
  boostmat::vector<double> itervect = boostmat::vector<double>(*finalVector);
  // boostmat::vector<double> itervect2(finalVector->size());
  for (int i = 1; i <= nT; i++) {

    sparseProd(&((*circularvect)[i]), &((*circularvect)[i - 1]),
               transitionsMatrix);

    if (((*circularvect)[i](0) != 0) && (minT == -1))
      minT = i;
    // cerr << "i:" << i<< "\t"<<(*circularvect)[i] (0) << endl;

    // boostmat::sparse_prod(transitionsMatrix, (*circularvect)[i] ,
    // (*circularvect)[i-1] );

    // cerr << "itervect:" << i << ":"<< (*circularvect)[i] << endl;
  }
  nbVect = nT;
  matOffset = nT;
  time(&endt);
  cerr << "time for precalculation:" << difftime(endt, start) << endl;

  cerr << "Starting the simulation" << endl;
}

int numericalSolver::currentRound() { return T - matOffset; }

void numericalSolver::switchOff() { matOffset = -1; }

void numericalSolver::reset() {
  matOffset = T;
  // cerr << (*circularvect)[matOffset] << endl;
}

boostmat::vector<double> numericalSolver::getVect() {
  if (matOffset < 0) {
    return boostmat::vector<double>(1, 0.0);
  }
  return (*circularvect)[matOffset];
}

double numericalSolver::getMu(int i) const {
  // cerr << "indice" << matOffset << endl;
  if (matOffset >= 0)
    return (*circularvect)[matOffset][i];
  else
    return 1.0;
}

void numericalSolver::stepVect() {
  matOffset--;
  // cerr << " # "<< matOffset <<" # ";
}

void numericalSolver::previousVect() { matOffset++; }

void numericalSolver::printState() { cerr << matOffset; }
