/**
 *	WARNING: Do Not Remove This Section
 *
 *       $LastChangedRevision: 415 $
 *       $LastChangedDate: 2010-12-18 17:21:05 +0100 (Sa, 18. Dez 2010) $
 *       $LastChangedBy: davidjansen $
 *
 *   Modified by Benoît Barbot on 25/01/2014 to integrate it into Cosmos
 *
 *	MRMC is a model checker for discrete-time and continuous-time Markov
 *	reward models. It supports reward extensions of PCTL and CSL (PRCTL
 *	and CSRL), and allows for the automated verification of properties
 *	concerning long-run and instantaneous rewards as well as cumulative
 *	rewards.
 *
 *	Copyright (C) The University of Twente, 2004-2008.
 *	Copyright (C) RWTH Aachen, 2008-2009.
 *	Authors: Ivan Zapreev
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version 2
 *	of the License, or (at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *	MA  02110-1301, USA.
 *
 *	Main contact:
 *		Lehrstuhl für Informatik 2, RWTH Aachen University
 *		Ahornstrasse 55, 52074 Aachen, Germany
 *		E-mail: info@mrmc-tool.org
 *
 *       Old contact:
 *		Formal Methods and Tools Group, University of Twente,
 *		P.O. Box 217, 7500 AE Enschede, The Netherlands,
 *		Phone: +31 53 4893767, Fax: +31 53 4893247,
 *		E-mail: mrmc@cs.utwente.nl
 *
 *	Source description: Calculate left/right truncation pts. and
 *		weights based on Fox&Glynn algorithm.
 *	Uses: Definition of foxglynn - foxglynn.h
 *	Reference: "Computing Poisson Probabilities", Bennett L. Fox and
 *			Peter W. Glynn,
 *                       Communications of the ACM , vol. 31, pp. 440-445
 *								(1988 ).
 */

#include "foxglynn.hpp"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*The pi constant*/
static const double pi = 3.14159265358979323846264;
static const double lambda_25 = 25.0;
static const double lambda_400 = 400.0;

/*****************************************************************************
Name		: finder
Role		: The FINDER function from the Fox-Glynn algorithm
@param		: int m: the middle point from weighter.
@param		: double lambda: (rate of uniformization)*(mission time)
@param		: double tau: underflow
@param		: double omega: overflow
@param		: double epsilon: error bound
@param		: double * pw_m: pointer to the initial value in the array of
sets, will be set here
@param		: FoxGlynn *: return by reference
@return		: TRUE if everything is fine, otherwise FALSE.
              This is the F parameter of Fox-Glynn finder function.
remark      : This implementation is based on the original Fox-Glynn paper with
              small modifications.
              The most remarkable alterations are the removal of terminating
indices for the search for the truncation points. This will result in higher
accuracy when needed. It can be shown that the introduced error converges to a
value below the requested error level (for epsilon > 0). Hence the algorithm
will always terminate.
******************************************************************************/
bool FoxGlynn::finder(const int m, const double lambda, const double tau,
                      const double omega, const double epsilon, double *pw_m) {
  const double sqrt_2_pi = sqrt(2.0 * pi);
  const double sqrt_2 = sqrt(2.0);
  const double sqrt_lambda = sqrt(lambda);
  double lambda_max, k, k_rtp = HUGE_VAL, k_prime, c_m_inf, result, al, dkl, bl;

  /*Simple bad cases, when we quit*/
  if (lambda == 0.0) {
    fprintf(stderr,
            "ERROR: Fox-Glynn: lambda = 0, terminating the algorithm\n");
    return false;
  }
  /* The requested error level must not be smaller than the minimum machine
     precision (needed to guarantee the convergence of the error conditions) */
  if (epsilon < tau) {
    fprintf(stderr, "ERROR: Fox-Glynn: epsilon < tau, invalid error level, "
                    "terminating the algorithm\n");
    fprintf(stderr, "epsilon %e, tau %e\n", epsilon, tau);
    return false;
  }
  /* zero is used as left truncation point for lambda <= 25 */
  left = 0;
  lambda_max = lambda;

  /* for lambda below 25 the exponential can be smaller than tau */
  /* if that is the case we expect underflows and warn the user */
  if (0.0 < lambda && lambda <= lambda_25) {
    if (exp(-lambda) <= tau) {
      fprintf(stderr, "ERROR: Fox-Glynn: 0 < lambda < 25, underflow. The "
                      "results are UNRELIABLE.\n");
    }
  }

  bl = (1.0 + 1.0 / lambda) * exp(1.0 / (8.0 * lambda));

  /****Compute pFG->right truncation point****/
  /*According to Fox-Glynn, if lambda < 400 we should take lambda = 400,
    otherwise use the original value. This is for computing the right truncation
    point*/
  if (lambda < lambda_400)
    lambda_max = lambda_400;
  k = 4;
  al = (1.0 + 1.0 / lambda_max) * exp(1.0 / 16.0) * sqrt_2;
  dkl = exp(-2.0 / 9.0 * (k * sqrt(2.0 * lambda_max) + 1.5));
  dkl = 1.0 / (1.0 - dkl);
  /* find right truncation point */

  /* This loop is a modification to the original Fox-Glynn paper.
     The search for the right truncation point is only terminated by
     the error condition and not by the stop index from the FG paper.
     This can yield more accurate results if neccesary.*/
  while ((epsilon / 2.0) <
         ((al * dkl * exp(-(k * k) / 2.0)) / (k * sqrt_2_pi))) {
    k++;
    dkl = exp(-2.0 / 9.0 * (k * sqrt_2 * sqrt(lambda_max) + 1.5));
    dkl = 1.0 / (1.0 - dkl);
  }
  k_rtp = k;
  right = (int)ceil(m + k_rtp * sqrt_2 * sqrt(lambda_max) + 1.5);

  /****Compute pFG->left truncation point****/
  /* compute the left truncation point for lambda > 25 */
  /* for lambda <= 25 we use zero as left truncation point */
  if (lambda > lambda_25) {
    /*Start looking for the left truncation point*/
    /* start search at k=4 (taken from original Fox-Glynn paper */
    k = 4;
    /* increase the left truncation point as long as we fulfill the error
     * condition */

    /* This loop is a modification to the original Fox-Glynn paper.
       The search for the left truncation point is only terminated by
       the error condition and not by the stop index from the FG paper.
       This can yield more accurate results if neccesary.*/
    while ((epsilon / 2.0) < ((bl * exp(-(k * k) / 2.0)) / (k * sqrt_2_pi)))
      k++;
    /*Finally the left truncation point is found*/
    left = (int)floor(m - k * sqrt(lambda) - 1.5);
    /* for small lambda the above calculation can yield negative truncation
     * points, crop them here */
    if (left < 0)
      left = 0;
    /* perform underflow check */
    k_prime = k + 3.0 / (2.0 * sqrt_lambda);
    /*We take the c_m_inf = 0.02935 / sqrt( m ), as for lambda >= 25
     c_m = 1 / ( sqrt( 2.0 * pi * m ) ) * exp( m - lambda - 1 / ( 12.0 * m ) )
     => c_m_inf*/
    c_m_inf = 0.02935 / sqrt((double)m);
    result = 0.0;
    if (0.0 < k_prime && k_prime <= sqrt_lambda / 2.0) {
      result = c_m_inf * exp(-pow(k_prime, 2.0) / 2.0 -
                             pow(k_prime, 3.0) / (3.0 * sqrt_lambda));
    } else {
      if (k_prime <= sqrt(m + 1.0) / m) {
        double result_1 = c_m_inf * pow(1.0 - k_prime / sqrt((double)(m + 1)),
                                        k_prime * sqrt((double)(m + 1)));
        double result_2 = exp(-lambda);
        /*Take the maximum*/
        result = (result_1 > result_2 ? result_1 : result_2);
      } else {
        /*NOTE: It will be an underflow error*/;
        fprintf(stderr, "ERROR: Fox-Glynn: lambda >= 25, underflow. The "
                        "results are UNRELIABLE.\n");
      }
    }
    if (result * omega / (1.0e+10 * (right - left)) <= tau) {
      fprintf(stderr, "ERROR: Fox-Glynn: lambda >= 25, underflow. The results "
                      "are UNRELIABLE.\n");
    }
  }

  /*We still have to perform an underflow check for the right truncation point
   * when lambda >= 400*/
  if (lambda >= lambda_400) {
    k_prime = k_rtp * sqrt_2 + 3.0 / (2.0 * sqrt_lambda);
    /*We take the c_m_inf = 0.02935 / sqrt( m ), as for lambda >= 25
     c_m = 1 / ( sqrt( 2.0 * pi * m ) ) * exp( m - lambda - 1 / ( 12.0 * m ) )
     => c_m_inf*/
    c_m_inf = 0.02935 / sqrt((double)m);
    result = c_m_inf * exp(-pow(k_prime + 1.0, 2.0) / 2.0);
    if (result * omega / (1.0e+10 * (right - left)) <= tau) {
      fprintf(stderr, "ERROR: Fox-Glynn: lambda >= 400, underflow. The results "
                      "are UNRELIABLE.\n");
    }
  }
  /*Time to set the initial value for weights*/
  *pw_m = omega / (1.0e+10 * (right - left));

  return true;
}

/*****************************************************************************
Name		: weighter
Role		: The WEIGHTER function from the Fox-Glynn algorithm
@param		: double lambda: (rate of uniformization)*(mission time)
@param		: double tau: underflow
@param		: double omega: overflow
@param		: double epsilon: error bound
@param		: FoxGlynn *: return by reference
@return		: TRUE if everything is fine, otherwise FALSE.
              This is the F parameter of Fox-Glynn finder function.
remark	    :
******************************************************************************/
bool FoxGlynn::weighter(const double lambda, const double tau,
                        const double omega, const double epsilon) {
  /*The magic m point*/
  const int m = (int)floor(lambda);
  double w_m = 0;
  int j, s, t;

  if (!finder(m, lambda, tau, omega, epsilon, &w_m))
    return false;

  /*Allocate space for weights*/
  weights = (double *)calloc((size_t)(right - left + 1), sizeof(double));
  /*Set an initial weight*/
  weights[m - left] = w_m;

  /*Fill the left side of the array*/
  for (j = m; j > left; j--)
    weights[(j - left) - 1] = (j / lambda) * weights[j - left];

  /*Fill the right side of the array, have two cases lambda < 400 & lambda >=
   * 400*/
  if (lambda < lambda_400) {
    /*Perform the underflow check, according to Fox-Glynn*/
    if (right > 600) {
      fprintf(stderr,
              "ERROR: Fox-Glynn: pFG->right > 600, underflow is possible\n");
      return false;
    }
    /*Compute weights*/
    for (j = m; j < right; j++) {
      double q = lambda / (j + 1);
      if (weights[j - left] > tau / q) {
        weights[(j - left) + 1] = q * weights[j - left];
      } else {
        right = j;
        break; /*It's time to compute W*/
      }
    }
  } else {
    /*Compute weights*/
    for (j = m; j < right; j++)
      weights[(j - left) + 1] = (lambda / (j + 1)) * weights[j - left];
  }

  /*It is time to compute the normalization weight W*/
  total_weight = 0.0;
  s = left;
  t = right;

  while (s < t) {
    if (weights[s - left] <= weights[t - left]) {
      total_weight += weights[s - left];
      s++;
    } else {
      total_weight += weights[t - left];
      t--;
    }
  }
  total_weight += weights[s - left];

  /* printf("Fox-Glynn: ltp = %d, rtp = %d, w = %10.15le \n", pFG->left,
   * pFG->right, pFG->total_weight); */

  return true;
}

/*****************************************************************************
Name		: fox_glynn
Role		: get poisson probabilities.
@param		: double lambda: (rate of uniformization)*(mission time)
@param		: double tau: underflow
@param		: double omega: overflow
@param		: double epsilon: error bound
@param		: FoxGlynn **: return a new FoxGlynn structure by reference
@return	: TRUE if it worked fine, otherwise false
remark		:
******************************************************************************/
FoxGlynn::FoxGlynn(const double lambda, const double tau, const double omega,
                   const double epsilon)
    : isValid(false), left(0), right(0), total_weight(0.0), weights(NULL) {
  /* printf("Fox-Glynn: lambda = %3.3le, epsilon = %1.8le\n",lambda, epsilon);
   */
  isValid = weighter(lambda, tau, omega, epsilon);
}

/**
 * Fries the memory allocated for the FoxGlynn structure
 * @param fg the structure to free
 */
FoxGlynn::~FoxGlynn() {
  if (weights)
    free(weights);
}
