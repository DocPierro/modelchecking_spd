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
 * file Gspn-Writer.hpp created by Benoit Barbot on 14/01/14.                  *
 *******************************************************************************
 */

#ifndef __Cosmos__Gspn_Writer__
#define __Cosmos__Gspn_Writer__

#include <iostream>
#include "Gspn-model.hpp"
#include "../parameters.hpp"

class Gspn_Writer {
public:
	Gspn_Writer(GspnType& mgspn,const parameters& Q);
    
	GspnType MyGspn;
	const parameters P;
	
	void writeFile();
    void writeDotFile(const std::string &file)const;

    

protected:
    
    size_t markingSize;

    std::string trId= "TR_PL_ID t, const abstractBinding &b";
    std::string objName= "SPN::";

    void writeFunT(std::ostream &s,const std::string &rtype,const std::string &name,const std::string &extraArg, std::function< void(unsigned int,std::stringstream &)>,std::string) const;

	int varMultiplier(size_t var)const;
    void writeTok(std::ostream &SpnF,const coloredToken&,const colorDomain&)const;
    void generateStringVal(arcStore&);
	virtual void writeMarkingClasse(std::ofstream &, std::ofstream &)const;
	void writeEnabledDisabled(std::ofstream &)const;
	virtual void writeEnabledDisabledBinding(std::ofstream &)const=0;
        virtual void writeEnabledDisabledBindingSet(std::ofstream &)const=0;
	void writeUpdateVect(std::ofstream &,const std::string &name,const std::vector< std::set<int> > &vect)const;
	void writeTransition(std::ofstream &)const;
	void writeVariable(std::ofstream & spnF)const;
    void writeOverrideParamater(std::ofstream & spnF)const;
	
    void error(const std::string& m);
    void view();
	
	
	virtual void printloot(std::ofstream& sf, const colorDomain &domain, size_t nesting )const=0;
	
	void EnabledDisabledTr(std::vector< std::set<int> >&,
						   std::vector< std::set<int> >&,
						   std::vector< std::set<int> >&)const;

    void writeMacro(std::ofstream &)const;

    void writeMarkingUpdate(std::stringstream &f, size_t t,const place &p,const arcStore &as2,bool direct)const;
    void writeMarkingUpdateIn(std::stringstream &f,const arcStore &as, size_t t,const place &p , size_t t2, bool pos,const arcStore &as2,bool directionxs)const;
    void writeFire(std::ofstream &f)const;
    void writeIsEnabled(std::ofstream &f)const;
    void writeSynchronisation(std::ofstream &f)const;
    void writeSetConditionsVector(std::ofstream &f)const;
    void writeGetDistParameters(std::ofstream &f)const;
    void writeGetPriority(std::ofstream &f)const;
    void writeGetWeight(std::ofstream &f)const;
    void writeUserDefineDistr(std::ofstream &f)const;
    void writePolynome(std::ofstream &f)const;

};



#endif /* defined(__Cosmos__Gspn_Writer__) */
