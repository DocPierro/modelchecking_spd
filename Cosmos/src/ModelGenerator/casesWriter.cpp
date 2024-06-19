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
 * file casesWriter.cpp                                                        *
 * Created by Benoit Barbot on 11/09/13.                                       *
 *******************************************************************************
 */


#include <iostream>
#include <sstream>

#include "casesWriter.hpp"




using namespace std;

casesHandler::casesHandler(string svar):maxc(0),scase(svar),hasDefault(false){};

void casesHandler::addCase(int c,const string &st,const string &comment){
	const char* cst = st.c_str();
	map<const string,int>::iterator it = cases.find(cst);
	if(it == cases.end()){
		cases[st]=1;
		mapping[c]=st;
	}else{
		it->second++;
		mapping[c]=st;
	}
	mapcomment[c]=comment;
	maxc = max(maxc,cases[st]);
}

void casesHandler::addDefault(const string &st){
    defaultvalue =st;
    hasDefault =true;
}

void casesHandler::writeCases(ostream &s){
    if(cases.size()==0)return;
	if(cases.size()==1) {
		s << cases.begin()->first << endl;
	}else{
		s << "\tswitch (" << scase << "){" << endl;
		map<const string,int>::iterator itmax = cases.begin();
		for(map<const string,int>::iterator it = cases.begin(); it != cases.end(); ++it){
			if(it->second != maxc || maxc ==1){
				for(map<int,std::string>::iterator it2 = mapping.begin(); it2 != mapping.end(); ++it2){
					if(it2->second.compare(it->first)==0)
						s << "\t\tcase " << it2->first << ":\t//" << mapcomment[it2->first] << endl;
				}
				s << it->first << endl;
				s << "\t\tbreak;" << endl;
			}else{
				itmax = it;
				maxc++;
			}
		}
		if(itmax->first.compare("")!=0 && maxc != 1 && !hasDefault){
			s<< "\t\tdefault:\t//";
			for(map<int,std::string>::iterator it2 = mapping.begin();
				it2 != mapping.end(); ++it2)
				if(it2->second.compare(itmax->first)==0)
					s << mapcomment[it2->first] << ",";
			
			s << endl << itmax->first << endl;
            s << "\t\tbreak;" << endl;
        } else if(hasDefault){
            s<< "\t\tdefault:"<< endl;
            s<< defaultvalue;
            s << "\t\tbreak;" << endl;
        }
        
		s << "\t}" << endl;
	}
}

std::ostream& operator<<(std::ostream& os, casesHandler& obj){
    obj.writeCases(os);
    return os;
}

