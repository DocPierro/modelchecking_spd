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
 * file SimulatorContinuousBounded.cpp created by Benoit Barbot on 31/01/12.   *
 *******************************************************************************
 */

#include "SimulatorContinuousBounded.hpp"
#include "float.h"
#include "math.h"
#include <algorithm>
#include <iostream>
#include <sys/resource.h>
// #include <boost/math/distributions/normal.hpp>

#include <boost/math/distributions/binomial.hpp>
#include <boost/math/distributions/normal.hpp>
#include <limits>

template <class DEDS>
SimulatorContinuousBounded<DEDS>::SimulatorContinuousBounded(
    DEDS &N, LHA_orig<typename DEDS::stateType> &A, int m, double e, int js)
    : SimulatorBoundedREBase<SimulatorContinuousBounded, DEDS>(N, A, m) {
  epsilon = e;
  if (js > 0) {
    jumpsize = js;
    singleIS = false;
  } else {
    jumpsize = -js;
    singleIS = true;
  }
  // boost::math::normal norm;
  // Normal_quantile = quantile(norm, 0.5 + 0.99 / 2.0);
}

template <class DEDS>
void SimulatorContinuousBounded<DEDS>::initVectCo(double t) {

  this->lambda = this->numSolv->uniformizeMatrix();
  cerr << "lambda:" << this->lambda << endl;
  fg = unique_ptr<FoxGlynn>(
      new FoxGlynn((this->lambda * t), DBL_EPSILON, 1 / DBL_EPSILON, epsilon));
  if (fg->isValid) {
    cerr << "fox_glyn:" << fg->left << "," << fg->right
         << " Total weigts:" << fg->total_weight << endl;
    /*for(int i = 0; i<= fg->right - fg->left; i++){
             cerr << fg->left+i << " " << fg->weights[i]/ fg->total_weight <<
       endl;
             }*/
  }
  // fg->right = 10;
  double lambda2 = this->lambda;
  this->initVect(fg->right + 1);
  this->lambda = lambda2;
}

template <class DEDS> BatchR SimulatorContinuousBounded<DEDS>::RunBatch() {
  // cerr << "test(";
  this->numSolv->reset();

  // cerr << ")" << endl;
  int left = (int)max(this->numSolv->getMinT(), fg->left);
  int Nmax = (int)ceil(((double)fg->right - left) / jumpsize);
  // cerr << "minT:\t" << numSolv->getMinT() << endl;
  cerr << "Number of batch:\t" << Nmax + 1 << "\tleft: " << left
       << "\tright: " << fg->right << "\tjumpsize:" << jumpsize << endl;
  // cerr << "left:\t" << left << endl;

  int n = -1;

  BatchR batchResult(2 * (Nmax + 1), 0);

  // Copy and sum Poisson Weight
  for (int i = 0; i <= Nmax; i++)
    batchResult.Min[2 * i] = 0.0;
  for (int i = left; i <= fg->right; i++) {
    int j = (int)ceil((double)(i - left) / jumpsize);
    batchResult.Min[2 * j] += fg->weights[i - fg->left];
  }
  for (int i = 0; i <= Nmax; i++)
    batchResult.Min[2 * i] /= fg->total_weight;

  list<simulationState<DEDS, EventsQueue>> statevect((Nmax + 1) *
                                                     this->BatchSize);

  int c = 0;
  if (P.verbose >= 1)
    cerr << "new round:" << n
         << "\tremaining trajectories: " << statevect.size() << endl;
  for (auto it = statevect.begin(); it != statevect.end(); it++) {
    this->N.Origine_Rate_Table = vector<double>(this->N.tr, 0.0);
    this->N.Rate_Table = vector<double>(this->N.tr, 0.0);

    bool continueb = false;
    // we try to find a trajectory reaching the precondition.
    while (!continueb) {
      // we build a new trajectory from the initial state.
      continueb = true;
      this->EQ = new EventsQueue(this->N.getNbTransition());
      this->reset();
      // We simulate until either the condition is satisfied or the trajectory
      // reach a deadend.
      while (!this->N.precondition(this->N.getState()) && continueb) {
        continueb = this->SimulateOneStep();
      }
    }

    if (singleIS) {
      it->maxStep = fg->right;
    } else {
      it->maxStep = (int)fmin(fg->right,
                              left + (Nmax - (c / this->BatchSize)) * jumpsize);
    }

    // cerr << "new path:\t" << it->maxStep << endl;

    c++;

    it->saveState(&(this->N), &(this->A), &(this->EQ));
  }

  // cout << "new batch" << endl;
  while (!statevect.empty()) {
    this->numSolv->stepVect();
    if (P.verbose >= 3)
      cerr << this->numSolv->getVect() << endl;
    n++;
    if (P.verbose >= 1)
      cerr << "new round:" << n
           << "\tremaining trajectories: " << statevect.size() << endl;

    for (auto it = statevect.begin(); it != statevect.end(); it++) {
      if (it->maxStep >= fg->right - n) {

        // cerr << "vect:\t" << it->maxStep;
        it->loadState(&(this->N), &(this->A), &(this->EQ));

        // cerr << A.Likelihood << endl;

        // cerr << "mu:\t" << mu() << " -> ";

        if (it->maxStep == fg->right - n) {
          // We first need to initialise the trajectory
          this->N.InitialEventsQueue(*(this->EQ), *this);
          if (P.verbose >= 2)
            // cerr << "new Path: " << it->maxStep << "\tmuinit: " << mu() <<
            // endl;
            it->saveState(&(this->N), &(this->A), &(this->EQ));
        } else {

          bool continueb = this->SimulateOneStep();
          // cerr << "\t" << mu() << endl;

          if ((!this->EQ->isEmpty()) && continueb) {
            it->saveState(&(this->N), &(this->A), &(this->EQ));
          } else {
            int i = (int)ceil((double)(it->maxStep - left) / jumpsize);
            int i2 = (int)fmax(0.0, ceil((double)(n - left) / jumpsize));
            // cerr << "Successfull trajectory: "<< it->maxStep << " -> " << i
            // << endl;
            if (this->Result.accept) {

              if (this->Result.quantR[0] * (1 - this->Result.quantR[0]) != 0)
                batchResult.IsBernoulli[0] = false;

              batchResult.Isucc += 1;
              if (singleIS)
                for (int j = i2; j <= Nmax; j++) {
                  batchResult.Mean[2 * j] += 1;
                  batchResult.Mean[2 * j + 1] += this->Result.quantR[0];
                  batchResult.M2[2 * j + 1] += pow(this->Result.quantR[0], 2);
                  batchResult.M3[2 * j + 1] += pow(this->Result.quantR[0], 3);
                  batchResult.M4[2 * j + 1] += pow(this->Result.quantR[0], 4);
                  batchResult.Min[2 * j + 1] =
                      fmin(batchResult.Min[2 * j + 1], this->Result.quantR[0]);
                  batchResult.Max[2 * j + 1] =
                      fmax(batchResult.Max[2 * j + 1], this->Result.quantR[0]);
                }
              else {
                batchResult.Mean[2 * i] += 1;
                batchResult.Mean[2 * i + 1] += this->Result.quantR[0];
                batchResult.M2[2 * i + 1] += pow(this->Result.quantR[0], 2);
                batchResult.M3[2 * i + 1] += pow(this->Result.quantR[0], 3);
                batchResult.M4[2 * i + 1] += pow(this->Result.quantR[0], 4);
                batchResult.Min[2 * i + 1] =
                    fmin(batchResult.Min[2 * i + 1], this->Result.quantR[0]);
                batchResult.Max[2 * i + 1] =
                    fmax(batchResult.Max[2 * i + 1], this->Result.quantR[0]);
              }
            }
            if (singleIS) {
              for (int j = 0; j <= Nmax; j++)
                batchResult.M2[2 * j] += 1;
            } else
              batchResult.M2[2 * i] += 1;
            batchResult.I++;

            delete this->EQ;
            it = statevect.erase(it);

            it--;
          }
        }
      }
    }
  }

  return (batchResult);
}
