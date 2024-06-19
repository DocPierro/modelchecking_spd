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
 * file unfolder.cpp created by Benoit Barbot on 10/01/14.                     *
 *******************************************************************************
 */

#include <stdlib.h>

#include "unfolder.hpp"

using namespace std;
using namespace xml_output;

string unfolder::str_of_vect(const vector<color> &v,const string &smid)const {
	string acc;
    for( let s : v){
		acc += smid;
		acc += s.name;
	}
	return acc;
}

string unfolder::cleanstr(const string &str)const {
    if(str[0] == '(' && str[str.length()-1]==')') return str.substr(1,str.length()-2);
    return str;
}

void unfolder::export_grml(ofstream& fout){
	fout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
	fout << "<model formalismUrl=\"http://formalisms.cosyverif.org/sptgd-net.fml\" xmlns=\"http://cosyverif.org/ns/model\">" << endl;
	
    fout << "\t<attribute name=\"declaration\"><attribute name=\"constants\">" << endl;
    
    fout << "\t\t<attribute name=\"intConsts\">" <<endl;
    for( let ci : IntConstant){
        fout << "\t\t\t<attribute name=\"intConst\"><attribute name=\"name\">"<< endl;
        fout << "\t\t\t\t" << ci.first << endl;
        fout << "\t\t\t</attribute><attribute name=\"expr\"><attribute name=\"numValue\">" << endl;
        fout << "\t\t\t\t" << ci.second << endl;
        fout << "\t\t\t</attribute></attribute></attribute>" << endl;
    }
    fout << "\t\t</attribute>" << endl;
    
    fout << "\t\t<attribute name=\"realConsts\">" <<endl;
    for( let ci : RealConstant){
        fout << "\t\t\t<attribute name=\"realConst\"><attribute name=\"name\">"<< endl;
        fout << "\t\t\t\t" << ci.first << endl;
        fout << "\t\t\t</attribute><attribute name=\"expr\"><attribute name=\"numValue\">" << endl;
        fout << "\t\t\t\t" << ci.second << endl;
        fout << "\t\t\t</attribute></attribute></attribute>" << endl;
    }
    fout << "\t\t</attribute>" << endl;

    fout << "\t</attribute></attribute>" << endl;
    
	for(let p: placeStruct)export_place_grml(fout,p);
	
	for(let t: transitionStruct)export_transition_grml(fout,t);
	
	for(let t: transitionStruct)export_arc_grml(fout, t);
	
	fout << "</model>" << endl;
    cout << "Number of Places:" << nbPlace << "\tNumber of Transitions:" << nbTrans << "\tNumber of Arcs:" << nbArc << endl;
}

void unfolder::export_place_grml(ofstream &fout,const place &p){
	iterateDom(p.name, "_", "", "", "_", "", colDoms[p.colorDom], 0, [&](const string& str,const string&){
        nbPlace++;
		fout << "\t<node id=\"" << get_uid("place"+str) << "\" nodeType=\"place\">"<< endl;
		fout << "\t\t<attribute name=\"name\">" << str << "</attribute>" << endl;
		fout << "\t\t<attribute name=\"marking\"><attribute name=\"expr\"><attribute name=\"numValue\">" << endl;
		if((p.Marking.t == Int && p.Marking.intVal==0) || p.initMarking.empty()){
			fout << "\t\t\t0" << endl;
		} else if(p.colorDom != UNCOLORED_DOMAIN) {
			fout << "\t\t\t1" << endl;
		} else {
            fout << "\t\t\t" << p.Marking;
        }
            fout << "\t\t</attribute></attribute></attribute>" << endl;
		fout << "\t</node>" << endl;
		
	});
}

void unfolder::export_transition_grml(ofstream &fout, const transition &t){
		iterateVars(t.name , "_", "", t.varDomain , 0, [&](const string& str){
            nbTrans++;
			fout << "\t<node id=\"" << get_uid("transition"+str) << "\" nodeType=\"transition\">"<< endl;
			fout << "\t\t<attribute name=\"name\">" << str << "</attribute>" << endl;
			fout << "\t\t<attribute name=\"distribution\">" << endl;
            fout << "\t\t\t<attribute name=\"type\">" << t.dist.name ;
            fout << "</attribute>" << endl;
            for(auto &sparam : t.dist.Param){
                fout << "\t\t\t<attribute name=\"param\">";
                fout << sparam;
                fout << "\t\t\t</attribute>"<< endl;
            }
            fout << "\t\t</attribute>" << endl;
			fout << "\t</node>" << endl;
		});
}

void unfolder::print_arc(ofstream &fout,size_t arcuid,size_t truid, size_t pluid, bool dir, const expr &val){
    fout << "\t<arc id=\"" << arcuid;
	fout << "\" arcType=\"arc\" source=\"";
	if(dir){
		fout << pluid << "\" target=\"" << truid << "\">";
	}else fout << truid << "\" target=\"" << pluid << "\">";
	fout << "<attribute name=\"valuation\">" << endl;
	fout << "\t\t" << val << endl;
	fout << "\t</attribute></arc>" << endl;
}


void unfolder::export_coltoken(ofstream &fout,const vector<color> &vec,
							   const coloredToken &coltoken,const transition &t,const place &p,bool dir){
	size_t truid = get_uid("transition"+t.name+str_of_vect(vec, "_"));
	vector<color> vec2;
	for ( size_t i =0 ; i != coltoken.field.size(); ++i) {
		colorClass cc = colClasses[vec[coltoken.field[i].intVal].cc];
        size_t col;
        if(coltoken.Flags[i] == CT_SINGLE_COLOR){
            col = coltoken.field[i].intVal;
        }else col = (vec[coltoken.field[i].intVal].id + coltoken.varIncrement[i] + cc.colors.size()) % cc.colors.size();
		vec2.push_back(cc.colors[col]);
	}
	size_t pluid = get_uid("place"+p.name+str_of_vect(vec2, "_"));
	nbArc++;
    size_t arcuid = get_uid("arcpre_"+t.name+str_of_vect(vec, "_")+"_"+p.name);

	print_arc(fout, arcuid, truid, pluid, dir, coltoken.mult);
}

void unfolder::export_multcoltok(ofstream &fout,const vector<color> &vec,const transition &t,const place &p, bool dir, const arc inoutarc){
    if (!inoutarc.isColored && !inoutarc.isEmpty) {
        size_t truid = get_uid("transition"+t.name+str_of_vect(vec, "_"));
        size_t pluid = get_uid("place"+p.name);
        size_t arcuid = get_uid("arcpre_"+t.name+str_of_vect(vec, "_")+"_"+p.name);
        print_arc(fout,arcuid,truid,pluid,dir,to_string(inoutarc.exprVal.intVal));
        return;
    }
    auto toklist = inoutarc.coloredVal;
    if(toklist.size() == 1 && !toklist[0].hasAll){
        export_coltoken(fout,vec,toklist[0],t,p,dir);
    } else if(toklist.size()>0){
        vector<color> iteratevec;
        iterateDomVec(iteratevec, colDoms[p.colorDom], 0, [&](const vector<color> &v){
            expr mult(0);
            for (auto coltoken: toklist) {
                bool match = true;
                for ( size_t i =0 ; i != coltoken.field.size(); ++i) {
                    if(coltoken.Flags[i]!=CT_ALL){
                        if(coltoken.Flags[i] != CT_SINGLE_REAL){
                            colorClass cc = colClasses[vec[coltoken.field[i].intVal].cc];
                            size_t col;
                            if(coltoken.Flags[i] == CT_SINGLE_COLOR){
                                col = coltoken.field[i].intVal;
                            }else col = (vec[coltoken.field[i].intVal].id + coltoken.varIncrement[i] + cc.colors.size()) % cc.colors.size();
                            if(col != v[i].id)match=false;
                        }else{
                            cerr << "Cannot unfold unbounded domain";
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                if(match)mult = expr(Plus,mult,coltoken.mult);
            }
            mult.eval();
            if(!mult.empty() || (mult.t == Int && mult.intVal != 0) ){
                nbArc++;
                size_t truid = get_uid("transition"+t.name+str_of_vect(vec, "_"));
                size_t pluid = get_uid("place"+p.name+str_of_vect(v, "_"));
                size_t arcuid = get_uid("arcpre_"+t.name+str_of_vect(vec, "_")+"_"+p.name);
                print_arc(fout,arcuid,truid,pluid,dir,mult);
            }
        });
    }
}


void unfolder::export_arc_grml(ofstream &fout, const transition &t){
	vector<color> iteratevec;
	iterateVars(iteratevec , t.varDomain , 0, [&](const vector<color> &vec){
		for(const auto &p: placeStruct){
            auto a = access(inArcsStruct,t.id,p.id);
            if (!a.isEmpty)export_multcoltok(fout,vec,t,p,true,a);
            auto b = access(outArcsStruct,t.id,p.id);
            if (!b.isEmpty)export_multcoltok(fout,vec,t,p,false,b);
		}
	});
}




