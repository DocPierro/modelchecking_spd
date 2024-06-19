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
 * file DistributionDef.hpp created by Benoit Barbot on 14/12/2016.            *
 *******************************************************************************
 */

#ifndef DistributionDef_h
#define DistributionDef_h

#include <array>

#define PARAM_TBL_SIZE 10

/**
 * Type of probability distribution
 */
enum DistributionType {
  NORMAL,
  GAMMA,
  UNIFORM,
  EXPONENTIAL,
  DETERMINISTIC,
  LOGNORMAL,
  TRUNKNORMAL,
  TRIANGLE,
  GEOMETRIC,
  ERLANG,
  WEIBULL,
  PARETO,
  DISCRETEUNIF,
  MASSACTION,
  IMMEDIATE,
  IMDT,
  USERDEFINE,
  DISCRETEUSERDEFINE,
  USERDEFINEPOLYNOMIAL,
  PLAYER1,
  SYNC
};

#define TRIANGULAR TRIANGLE

/**
 * Class handling custom distribution
 */
class CustomDistr {
public:
  virtual double userDefineCDF(const std::array<double, PARAM_TBL_SIZE> &,
                               double) const {
    return 0.0;
  };
  virtual double userDefinePDF(const std::array<double, PARAM_TBL_SIZE> &,
                               double) const {
    return 0.0;
  };
  virtual double
  userDefineLowerBound(const std::array<double, PARAM_TBL_SIZE> &) const {
    return 0.0;
  };
  virtual double
  userDefineUpperBound(const std::array<double, PARAM_TBL_SIZE> &) const {
    return 0.0;
  };
  virtual double
  userDefineDiscreteDistr(const std::array<double, PARAM_TBL_SIZE> &,
                          unsigned int) const {
    return 0.0;
  };
  virtual double evalPoly(unsigned long,
                          const std::array<double, PARAM_TBL_SIZE> &) const {
    return 0.0;
  };
  virtual const std::string print_poly(unsigned long) const { return ""; };
};

#endif /* DistributionDef_h */
