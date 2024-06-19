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
 * file clientsim.hpp created by Benoît Barbot on 16/01/2017.                  *
 *******************************************************************************
 */

#ifndef clientsim_h
#define clientsim_h

#include <iostream>

#include "BatchR.hpp"
#include "MarkovChain.hpp"
#include "Polynome.hpp"
#include "RareEvents/SimulatorBoundedRE.hpp"
#include "RareEvents/SimulatorContinuousBounded.hpp"
#include "RareEvents/SimulatorRE.hpp"
#include "Simulator.hpp"
#include "parameters.hpp"

// Handler for interuption of the server
void signalHandler(int);

template <class SIM>
void setSimulator(SIM &sim, int argc, char *argv[], parameters &P) {
  if (argc == 0) {
    sim.initRandomGenerator(0);
    return;
  }
  if (P.interactive)
    P.verbose = 6;

  P.seed = atoi(argv[2]);
  sim.initRandomGenerator(P.seed);

  sim.logValue(P.dataraw);
  sim.logTrace(P.datatrace, P.sampleResol);
  sim.dotFile = P.dotfile;
}

void readParameters(std::ifstream &i);

#endif /* clientsim_h */
