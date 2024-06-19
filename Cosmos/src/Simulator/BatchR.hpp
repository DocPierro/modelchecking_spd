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
 * file BatchR.hpp created by Benoit Barbot.                                   *
 *******************************************************************************
 */

#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>
// #include <boost/archive/text_oarchive.hpp>
// #include <boost/archive/text_iarchive.hpp>
// #include <boost/serialization/vector.hpp>

#ifndef _BATCHR_HPP
#define _BATCHR_HPP

struct SimOutput {
  bool accept;
  std::vector<double> quantR;
  std::vector<bool> qualR;
};

/**
 * This file implement a structure for the result of a batch of
 * Simulation it also implement input/output on it.
 * It is used both by the simulator and the main program.
 */
class BatchR {

public:
  BatchR(size_t, size_t);

  //! Number of simulation.
  unsigned long int I;

  //! Number of succesfull simulation.
  unsigned long int Isucc;

  //! Is a variable a boolean.
  std::vector<bool> IsBernoulli;

  //! The mean value of each formula.
  std::vector<double> Mean;

  //! The second moment of each formula.
  std::vector<double> M2;

  //! The third moment of each formula.
  std::vector<double> M3;

  //! The fourth moment of each formula.
  std::vector<double> M4;

  //! The Min value of each formula.
  std::vector<double> Min;

  //! The Max moment of each formula.
  std::vector<double> Max;

  //! Bernouilly variable vector
  std::vector<unsigned long int> bernVar;

  //! Time of simulation to produce the result
  double simTime;

  //! Add the result of one simulation to the Batch.
  void addSim(const SimOutput &);

  //! Merge the result of two batch of simulation.
  void unionR(const BatchR &);

  void outputR(std::ostream &f);

  bool inputR(FILE *f);
  // void inputR(std::istream &f);
  friend std::ostream &
  operator<<(std::ostream &os,
             const BatchR &dt); //! Print human readable version of batch.

  /*
  private:
      friend class boost::serialization::access;
      template<class Archive>
      void serialize(Archive & ar, const unsigned int version);*/
};

#endif /* _BATCHR_HPP */
