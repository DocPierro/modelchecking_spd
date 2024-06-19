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
 * file timeGen.cpp created by Benoit Barbot on 25/01/12.                      *
 *******************************************************************************
 */

#include <array>
#include <boost/math/special_functions/erf.hpp>
#include <boost/math/special_functions/prime.hpp>
#include <boost/math/tools/roots.hpp>
#include <float.h>
#include <limits>
#include <math.h>
#include <random>
#include <tuple>

#include "timeGen.hpp"

/*
  Copy from Boost: boost/math/tools/roots.hpp
  //  (C) Copyright John Maddock 2006.
  //  Use, modification and distribution are subject to the
  //  Boost Software License, Version 1.0. (See accompanying file
  //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
  */
/*template <class F>
double boost_newton_raphson_iterate(F f, double guess, double min, double max,
int max_iter)
{
  int digits = 16;
  double f0(0), f1;
  double result = guess;

  double factor = static_cast<double>(ldexp(1.0, 1 - digits));
  double delta = 1;
  double delta1 = std::numeric_limits<double>::max();
  double delta2 = std::numeric_limits<double>::max();

  unsigned int count(max_iter);

  do{
    delta2 = delta1;
    delta1 = delta;
    std::tie(f0, f1) = f(result);
    if(0 == f0)
      break;
    if(f1 == 0)
      {
        std::cerr << "Zero derivative in newton raphson iterate" << std::endl;
        exit(EXIT_FAILURE);
      }
    else
      {
        delta = f0 / f1;
      }
    if(fabs(delta * 2) > fabs(delta2))
      {
        // last two steps haven't converged, try bisection:
        delta = (delta > 0) ? (result - min) / 2 : (result - max) / 2;
      }
    guess = result;
    result -= delta;
    if(result <= min)
      {
        delta = 0.5F * (guess - min);
        result = guess - delta;
        if((result == min) || (result == max))
          break;
      }
    else if(result >= max)
      {
        delta = 0.5F * (guess - max);
        result = guess - delta;
        if((result == min) || (result == max))
          break;
      }
    // update brackets:
    if(delta > 0)
      max = guess;
    else
      min = guess;
  }while(--count && (fabs(result * factor) < fabs(delta)));

  max_iter -= count;

  return result;
}
*/

using namespace std;

VanDerCorputState::VanDerCorputState() {
  it = 20;
  baseId = 0;
}

void VanDerCorputState::seed(unsigned long seed) {
  it = seed % 10000223 /* large prime */;
}

double VanDerCorputState::sample() {
  long n = it;
  unsigned int base = boost::math::prime(baseId);
  baseId = baseId + 1;
  //(5437*baseId + 3671) % 9973; // A simple random (I choose the parameters
  //randomly) linear congruantial pseudo random number generator.
  double vdc = 0, denom = 1;
  while (n) {
    vdc += fmod(n, base) / (denom *= base);
    n /= base; // note: conversion from 'double' to 'int'
  }
  if (P.verbose > 4)
    cerr << "Sampling VDC; base: " << base << "\tn: " << it << " ->" << vdc
         << endl;

  return vdc;
}

void VanDerCorputState::newTrajectory() {
  baseId = 0;
  it++;
}

void timeGen::initRandomGenerator(unsigned int seed) {
  RandomNumber.seed(seed);
  vanDerCorputSampler.seed(RandomNumber.operator()());
  kroneckerSampler.seed(seed);
}

KroneckerState::KroneckerState() {
  it = 0;
  baseId = 1;
  phi_d = 1.0 / compute_gamma((unsigned long)P.horizon);
  if (P.verbose > 4)
    cerr << "Initializing Kronecker Sampler: horizon: " << P.horizon
         << "\tgamma: " << 1.0 / phi_d << endl;
}

double unif01ofint(unsigned long v) {
  double res = 1.0;
  while (v != 0) {
    if (v % 2) {
      res /= 2.0;
    } else {
      res = (0.5 + res) / 2.0;
    }
    v /= 2;
  }
  return res;
}

void KroneckerState::seed(unsigned long seed) {
  // double * tmp =  reinterpret_cast<double*>(&seed);
  seed_val = unif01ofint(seed);
}

double KroneckerState::sample() {
  double alpha = pow(phi_d, baseId);
  // double alpha = sqrt(boost::math::prime(baseId-1));

  double d;
  double sample = modf(seed_val + (double)it * alpha, &d);
  if (P.verbose > 4)
    cerr << "Sampling Kronecker; seed: " << seed_val << " base: " << baseId
         << "\tn: " << it << "\tbasealpha: " << phi_d << "\talpha: " << alpha
         << "->" << sample << endl;
  baseId = baseId + 1;
  return sample;
}

double KroneckerState::compute_gamma(unsigned long d) {
  double x = 1.0;
  for (int i = 0; i < 30; i++) {
    x = x - (pow(x, d + 1) - x - 1) / ((d + 1) * pow(x, d) - 1);
  }
  return x;
}

void KroneckerState::newTrajectory() {
  baseId = 1;
  it++;
}

double timeGen::sampleQuasiRandom(size_t) {
  double gentime = 0.0;
  switch (P.randomGen) {
  case VDC:
    gentime = vanDerCorputSampler.sample();
    break;
  case Kronecker:
    gentime = vanDerCorputSampler.sample();
    break;
  case MT19937:
    gentime = 0.0;
  }
  // cerr << gentime << "\t";
  return gentime;
}

void timeGen::reset() {
  switch (P.randomGen) {
  case VDC:
    vanDerCorputSampler.newTrajectory();
    break;
  case Kronecker:
    kroneckerSampler.newTrajectory();
  default: {
  }
  }
  // cerr << endl;
}

string timeGen::string_of_dist(DistributionType d,
                               const array<double, PARAM_TBL_SIZE> &param,
                               const CustomDistr &cd) const {
  // use for debuging
  switch (d) {
  case NORMAL:
    return "Normal(" + to_string(get<0>(param)) + "," +
           to_string(get<1>(param)) + ")";
  case GAMMA:
    return "Gamma(" + to_string(get<0>(param)) + "," +
           to_string(get<1>(param)) + ")";
  case UNIFORM:
    return "Uniform(" + to_string(get<0>(param)) + "," +
           to_string(get<1>(param)) + ")";
  case EXPONENTIAL:
    return "Exponential(" + to_string(get<0>(param)) + ")";
  case DETERMINISTIC:
    return "Deterministic(" + to_string(get<0>(param)) + ")";
  case TRUNKNORMAL:
    return "TrunkNormal(" + to_string(get<0>(param)) + "," +
           to_string(get<1>(param)) + ")";
  case LOGNORMAL:
    return "LogNormal";
  case TRIANGLE:
    return "Triangle";
  case GEOMETRIC:
    return "Geometric";
  case ERLANG:
    return "Erlang(" + to_string(get<0>(param)) + "," +
           to_string(get<1>(param)) + ")";
  case PARETO:
    return "Pareto(" + to_string(get<0>(param)) + "," +
           to_string(get<1>(param)) + ")";
  case WEIBULL:
    return "Weibull(" + to_string(get<0>(param)) + "," +
           to_string(get<1>(param)) + ")";
  case DISCRETEUNIF:
    return "DiscreteUnif(" + to_string(get<0>(param)) + "," +
           to_string(get<1>(param)) + ")";
  case MASSACTION:
    return "MassAction(" + to_string(get<0>(param)) + ")";
  case IMMEDIATE:
  case IMDT:
    return "Immediate";

    //    return "Userdefine("+ to_string(param[0]) +","+
    //    to_string(param[1])+","+ to_string(param[2])+","+
    //    to_string(param[3])+")";
  case DISCRETEUSERDEFINE:
    return "DiscreteUserDefine(" + to_string(get<0>(param)) + "," +
           to_string(get<1>(param)) + ")";
  case USERDEFINE:
  case USERDEFINEPOLYNOMIAL: {
    string s = "UserdefinePolynomial(" + to_string(get<0>(param)) + "," +
               to_string(get<1>(param)) + "," + to_string(get<2>(param)) + "," +
               to_string(get<3>(param)) + ")";
    return s + cd.print_poly((int)get<0>(param));
  }
  case SYNC:
    return "Synchronisation";
  default:
    return "Unknown distribution";
  }
}

/**
 * Call the random generator to generate fire time.
 * @param distribution is the type of distribution
 * @param param is a vector of parameters of the distribution.
 */
double timeGen::GenerateTime(DistributionType distribution, size_t trid,
                             const array<double, PARAM_TBL_SIZE> &param,
                             const CustomDistr &cd) {
  // cerr << "sampling " << string_of_dist(distribution,param) << endl;;
  switch (distribution) {
  case UNIFORM: { // UNIF
    if (get<0>(param) == get<1>(param))
      return get<0>(param);
    if (P.randomGen != MT19937) {
      return get<0>(param) +
             (get<1>(param) - get<0>(param)) * sampleQuasiRandom(trid);
    } else {
      std::uniform_real_distribution<> unif(get<0>(param), get<1>(param));
      return unif(RandomNumber);
    }
  }

  case MASSACTION:
  case EXPONENTIAL: { // EXP
    // Exponential distribution is the only marking dependent parameters. Check
    // of validity is required

    //-------------- Rare Event -----------------
    // cout << "rate:" << param[0] << endl;
    if (get<0>(param) <= 0) {
      return 1e200;
    };
    //------------- /Rare Event -----------------

    if (get<0>(param) <= 0) {
      cout << "Exponential ditribution should be with rate > 0 not "
           << get<0>(param) << "\n End of Simulation" << endl;
      exit(1);
    }

    if (P.randomGen != MT19937) {
      return -log(1.0 - sampleQuasiRandom(trid)) / get<0>(param);
    } else {
      std::exponential_distribution<> exp(get<0>(param));
      return exp(RandomNumber);
    }
  }

  case SYNC:
  case PLAYER1:
  case IMDT:
  case IMMEDIATE:
    return 0;

  case DETERMINISTIC: { // DETERMINISTIC
    return get<0>(param);
  }

  case NORMAL: {
    std::normal_distribution<> normal(get<0>(param), get<1>(param));
    return fmax(0.0, normal(RandomNumber));
  }

  case TRUNKNORMAL: {
    double mu = get<0>(param);
    double s = get<1>(param);

    // integral over [0;+\infty]
    double v = 0.5 * (erfc(-mu / (s * sqrt(2))));

    // translate the mean by half the probability weight
    double mu2 = mu - s * sqrt(2) * boost::math::erfc_inv(v);

    // Using rejection sampling
    std::normal_distribution<> normal(mu2, s);
    double r;
    do {
      r = normal(RandomNumber);
    } while (r <= 0);
    return r;
  }

  case LOGNORMAL: {           // LogNormal
    double m = get<0>(param); // location
    double s = get<1>(param); // scale
    // double mean = exp(m + s*s/2);
    // double variance = (exp(s*s)-1)*exp(2*m+s*s);
    std::lognormal_distribution<> lognormal(m, s);
    return lognormal(RandomNumber);
  }

  case TRIANGLE: { // Triangle
    std::uniform_real_distribution<> unif(0, 1);
    double p = unif(RandomNumber);
    double a = get<0>(param);      // lower
    double b = get<2>(param);      // upper
    double c = get<1>(param);      // mode
    double p0 = (c - a) / (b - a); // inflection point
    if (p < p0)
      return sqrt((b - a) * (c - a) * p) + a;
    if (p > p0)
      return b - sqrt((b - a) * (b - c) * (1.0 - p));
    if (p == p0)
      return c; // Prob of this case is zero
  }
  case GEOMETRIC: { // GEOMETRIC
    std::uniform_real_distribution<> unif(0, 1);
    double p = unif(RandomNumber);
    return (get<1>(param) * (ceil(log(p) / log(1 - get<0>(param)))));
    /*if (p >= param[0]){
      return param[1];
      } else {
      return param[1] * ceil(log(p / param[0]) / log(1 - param[0]) + 1);
      }*/
  }
  case ERLANG: { // ERLANG
    std::uniform_real_distribution<> unif(0, 1);
    double sum = 0.0;
    for (int i = 0; i < get<0>(param); i++)
      sum -= log(unif(RandomNumber));
    return sum / get<1>(param);
  }
  case PARETO: {
    std::uniform_real_distribution<> unif(0, 1);
    return get<0>(param) / pow(unif(RandomNumber), 1 / get<1>(param));
  }

  case WEIBULL: {
    std::weibull_distribution<> weibull(get<0>(param), get<1>(param));
    return weibull(RandomNumber);
  }

  case GAMMA: { // GAMMA
    std::gamma_distribution<> gamma(get<0>(param));
    return get<1>(param) * gamma(RandomNumber);
  }
  case DISCRETEUNIF: { // DISCRETEUNIF
    std::uniform_int_distribution<> unif((int)get<0>(param),
                                         (int)get<1>(param));
    return unif(RandomNumber);
    break;
  }

  case DISCRETEUSERDEFINE: {
    std::uniform_int_distribution<> unif(0, 100000);
    unsigned int i = unif(RandomNumber);
    return cd.userDefineDiscreteDistr(param, i);
  }

  case USERDEFINEPOLYNOMIAL:
  case USERDEFINE: {
    double gentime;
    if (P.randomGen != MT19937) {
      gentime = sampleQuasiRandom(trid);
    } else {
      std::uniform_real_distribution<> unif(0.0, 1.0);
      gentime = unif(RandomNumber);
    }

    // cerr << gentime << "\t";

    double lower = cd.userDefineLowerBound(param);
    double upper = cd.userDefineUpperBound(param);

    /*//Isotrop Hack
    std::uniform_real_distribution<> unif(lower, upper);
    gentime = unif(RandomNumber);
    //gentime = sampleQuasiRandom(trid);
    //gentime = (upper-lower)*gentime + lower;
      cerr << gentime << "\t";
    //cerr << cd.userDefineCDF(param,gentime) << "\t";
    return gentime;
    //Isotrop Hack
    */

    // cerr << "sample(" << gentime << ",[" << lower << "," << upper << "]):"
    // <<endl;
    double initialpt = (lower + upper) / 2.0;
    double pt = boost::math::tools::newton_raphson_iterate(
        [&](double x) {
          const auto cdf = cd.userDefineCDF(param, x);
          const auto pdf = cd.userDefinePDF(param, x);
          //      cerr << "it:" << x << endl;
          return make_tuple(cdf - gentime, pdf);
        },
        initialpt, lower, upper, 100);
    // cerr << pt << "\t";
    return pt;
    break;
  }
  }
  return DBL_MIN;
}
