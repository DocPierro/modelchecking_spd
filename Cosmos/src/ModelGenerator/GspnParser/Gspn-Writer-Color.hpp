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
 * file Gspn-Writer-Color.hpp                                                  *
 * Created by Benoit Barbot on 17/04/15.                                       *
 *******************************************************************************
 */


#ifndef __Cosmos__Gspn_Writer_Color__
#define __Cosmos__Gspn_Writer_Color__

#include <stdio.h>

#include "Gspn-Writer.hpp"

class Gspn_Writer_Color: public Gspn_Writer{
public:
    Gspn_Writer_Color(GspnType& mgspn,parameters& Q);

    void writeMarkingClasse(std::ofstream &, std::ofstream &)const;
    void printloot(std::ofstream& sf, const colorDomain &domain, size_t nesting )const;
    void writeEnabledDisabledBinding(std::ofstream &)const;
    void writeEnabledDisabledBindingSet(std::ofstream &)const;
    void writeDomainTable(std::ofstream &cpp, std::ofstream &hpp, const colorDomain &d)const ;
    void writeDomainToken(std::ofstream &header, const colorDomain & it)const;
    void writeDomainSet(std::ofstream &SpnCppFile , std::ofstream &header, const colorDomain & it)const;
    void writeNextBindingSet(std::ofstream &SpnCppFile , std::ofstream &header)const;
};

#endif /* defined(__Cosmos__Gspn_Writer_Color__) */
