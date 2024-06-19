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
 * file timeGen.hpp created by Benoit Barbot on 25/01/12.            *
 *******************************************************************************
 */

#ifndef __Cosmos__File__
#define __Cosmos__File__

#include <iostream>
#include <random>
#include <unordered_map>

#include "DistributionDef.hpp"
#include "Event.hpp"
#include "parameters.hpp"

class VanDerCorputState {
public:
  VanDerCorputState();
  void newTrajectory();
  double sample();
  void seed(unsigned long seed);

private:
  unsigned long it;
  unsigned int baseId;
};

class KroneckerState {
public:
  KroneckerState();
  void newTrajectory();
  double sample();
  void seed(unsigned long seed);

private:
  double compute_gamma(unsigned long d);
  double phi_d;

  double seed_val;
  unsigned long it;
  unsigned int baseId;
};

double unif01ofint(unsigned long v);

class timeGen {
public:
  //! generate a time acording to the distribution d with parameters p
  double GenerateTime(DistributionType distribution, size_t trid,
                      const std::array<double, PARAM_TBL_SIZE> &param,
                      const CustomDistr &);

  /**
   * \brief Initialize the random number generator with the given seed
   * @param seed is an unsigned integer to be used as seed.
   */
  void initRandomGenerator(unsigned int seed);
  void reset();

  std::string string_of_dist(DistributionType d,
                             const std::array<double, PARAM_TBL_SIZE> &param,
                             const CustomDistr &) const;

  double current_weight = 0.0;

private:
  //! The random Generator Mersenne Twister from std librairy
  std::mt19937_64 RandomNumber;
  double sampleQuasiRandom(size_t);
  VanDerCorputState vanDerCorputSampler;
  KroneckerState kroneckerSampler;
};

// template<class DEDS>
// void generateEvent(double ctime,Event& E,size_t Id,const abstractBinding&
// b,timeGen &,DEDS &);

extern parameters P;

#endif /* defined(__Cosmos__File__) */
