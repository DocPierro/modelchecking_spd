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
 * file casesWriter.hpp                                                        *
 * Created by Benoit Barbot on 11/09/13.                                       *
 *******************************************************************************
 */

#ifndef Cosmos_casesWriter_h
#define Cosmos_casesWriter_h

#include <string>
#include <vector>
#include <ostream>
#include <map>
#include <stdio.h>
#include <string.h>
#include <functional>

struct cmp_str
{
	bool operator()(const std::string a,const std::string b)const{
		return a.compare(b) < 0;
	}
};

/**
 * \brief A class allowing to generate efficient switch case pattern in the
 * generated code. It allows to merge identical case together and thus
 * reduce the size of the generated code
 */

class casesHandler {
public:
	//! Build a new cases handler over a given variable.
	casesHandler(std::string svar);
	void addCase(int c,const std::string &st,const std::string &comment="");
    void addDefault(const std::string &st);
	void writeCases(std::ostream &s);
private:
	int maxc;
    std::string defaultvalue;
	const std::string scase;
    bool hasDefault;
	std::map<const std::string,int,cmp_str> cases;
	std::map<int,std::string> mapping;
	std::map<int,std::string> mapcomment;
};

std::ostream& operator<<(std::ostream& os, casesHandler& obj);




#endif
