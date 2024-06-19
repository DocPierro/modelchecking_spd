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
 * file Gspn-Grml-Output.hpp                                                   *
 * Created by Benoit Barbot on 02/11/2015.                                     *
 *******************************************************************************
 */

#ifndef Gspn_Grml_Output_hpp
#define Gspn_Grml_Output_hpp

#include <stdio.h>

#include "Gspn-model.hpp"
#include "Gspn-Reader.hpp"

namespace xml_output {

class GspnGrmlOutput: public GspnType {

public:
    GspnGrmlOutput(GspnType& g):GspnType(g){};

    friend std::ostream& operator<<(std::ostream & os, GspnGrmlOutput& gspn);
    friend std::ostream& operator<<(std::ostream & os, const Distribution& d);
    void printPlace(std::ostream&,const place &);
    void printTransition(std::ostream&,const transition &);
    void printToken(std::ostream& os, const coloredToken& c, size_t);
    void printArc(std::ostream& os, const std::pair<size_t,size_t> &p, const arc& a,bool,size_t);
};
}

#endif /* Gspn_Grml_Output_hpp */
