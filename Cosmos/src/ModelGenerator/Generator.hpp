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
 * file Generator.hpp                                                          *
 * Created by Benoit Barbot on 21/01/2014.                                     *
 *******************************************************************************
 */

#ifndef __Cosmos__Generator__
#define __Cosmos__Generator__

#include <iostream>

#include "GspnParser/Gspn-Reader.hpp"

/**
 * Parse the input file and build the simulator
 * Return true iff the parsing was successfull
 * input file are read as Grml file or .gspn and .lha file or
 * directly .cpp for LHA
 * according to the P.GMLinput parameters or extension
 * If require by some option modify the SPN or the LHA on the fly.
 * @return a boolean equal to true if everything run correctly
 */

std::shared_ptr<GspnType> ParseGSPN();
bool ParseLHA(GspnType &);
bool ParseLHA();


void generateLoopLHA(GspnType &);
void generateSamplingLHA(GspnType &);
void generateEmptyLHA();
void generateMain();

bool compileSource(const std::vector<std::string> &sources);
bool build();

#endif /* defined(__Cosmos__Generator__) */
