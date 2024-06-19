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
 * file Gspn-model.hpp                                                         *
 * Created by Benoit Barbot on 09/06/15.                                       *
 *******************************************************************************
 */

#include <fstream>

#include "Gspn-model.hpp"

using namespace std;


size_t userDefineDistribution::nbparam = 1;
string userDefineDistribution::polyfile;


bool operator< (const coloredToken& lhs, const coloredToken& rhs){
  if(lhs.field.size() < rhs.field.size())return true;
  for(size_t i=0; i<lhs.field.size(); i++ ){
    if(lhs.field[i] < rhs.field[i])return true;
    if(lhs.Flags[i] < rhs.Flags[i])return true;
    if(lhs.varIncrement[i] < rhs.varIncrement[i])return true;
  }
  return false;
}

bool operator== (const coloredToken& lhs, const coloredToken& rhs){
  if(lhs.field.size() != rhs.field.size())return false;
  for(size_t i=0; i<lhs.field.size(); i++ ){
    if(lhs.field[i] != rhs.field[i])return false;
    if(lhs.Flags[i] != rhs.Flags[i])return false;
    if(lhs.varIncrement[i] != rhs.varIncrement[i])return false;
  }
  return true;
}

set<size_t> coloredToken::varSet()const{
    set<size_t> s;
    for(size_t i=0; i<field.size(); i++ )
        if( Flags[i] == CT_VARIABLE )
            s.insert(field[i].intVal);
    return s;
}

void searchreplace(const string &in,const string &motif,const string &rep,string &out){
  out = in;
  size_t pos = out.find(motif);
  while (pos != string::npos) {
    out.replace(pos, motif.size(), rep);
    pos = out.find(motif,pos);
  }
}


void GspnType::iterateDom(const string &s,const string &sop, const string &sclos ,const string &s2,const string &sop2, const string &sclos2 ,const colorDomain & cd, size_t prof,function<void (const string&,const string&)> func){
  if(prof == cd.colorClassIndex.size() ){func(s,s2);}
  else{
    for(const auto &cc : colClasses[cd.colorClassIndex[prof]].colors )
      iterateDom(s+ sop + cc.name + sclos, sop, sclos,
                 s2+sop2+ "Color_"+colClasses[cd.colorClassIndex[prof]].name+"_"+cc.name+sclos2,sop2, sclos2, cd, prof+1, func);
  }

}

void GspnType::iterateDomVec(vector<color> &v, const colorDomain & cd, size_t prof,function<void (const vector<color>&)> func){
  if(prof == cd.colorClassIndex.size() ){func(v);}
  else{
    for(const auto &cc : colClasses[cd.colorClassIndex[prof]].colors ){
      v.push_back(cc);
      iterateDomVec(v, cd, prof+1, func);
      v.pop_back();
    }
  }

}

void GspnType::iterateVars(const string &s,const string &sop, const string &sclos ,const set<size_t> &vd, size_t prof,function<void (const string&)> func){
  if(prof == colVars.size() ){func(s);}
  else{
    if (vd.count(prof)>0) {
      for(const auto &cc : colClasses[colDoms[colVars[prof].type].colorClassIndex[0]].colors )
        iterateVars(s+ sop + cc.name + sclos, sop, sclos, vd, prof+1, func);
    }else{
      iterateVars(s, sop, sclos,vd, prof+1, func);
    }
  }
}

void GspnType::iterateVars(vector<color> &v,const set<size_t> &vd, size_t prof,function<void (const vector<color>&)> func){
  if(prof == colVars.size() ){func(v);}
  else{
    if (vd.count(prof)>0) {
      for(const auto &cc : colClasses[colDoms[colVars[prof].type].colorClassIndex[0]].colors ){
        v.push_back(cc);
        iterateVars(v, vd, prof+1, func);
        v.pop_back();
      }
    }else{
      iterateVars(v,vd, prof+1, func);
    }
  }
}

bool GspnType::isInVar(size_t var, size_t tr)const {
  for(const auto &arcmap : inArcsStruct )if( get_t(arcmap.first) ==tr  ){
      if(arcmap.second.containsVar(var))return true;
    }
  return false;
}

bool GspnType::isInVar(const coloredToken &var, size_t tr)const {
  for(const auto &arcmap : inArcsStruct )if( get_t(arcmap.first) ==tr  ){
      for(const auto &tok:arcmap.second.coloredVal)
        if( tok ==  var  )return true;
    }
  return false;
}


map<coloredToken,pair<size_t,size_t>> GspnType::computeMacroVar()const{
    map<coloredToken,pair<size_t,size_t>> macroVar;
    size_t id = 0;
    for(const auto &arc: inArcsStruct){
        const auto &domain = placeStruct[get_p(arc.first)].colorDom;
        if(!colDoms[domain].isUncolored())
            for(const auto &tok: arc.second.coloredVal){
            if( macroVar.count(tok)==0 ){
                macroVar.insert(make_pair(tok, make_pair(id++, domain)));
            }
        }
  }
  return macroVar;
}

size_t GspnType::get_uid(const string &str){
  //cout << "debug:\t" << str;
  if(uid.count(str)>0){
    //cout << " -> " << uid[str] << endl;
    return uid[str];
  }
  size_t it = uid.size();
  uid[str] = it;
  //cout << " -> " << uid[str] << endl;
  return it;
}

size_t GspnType::new_uid(const string &str){
  const auto i = uid.size();
  return get_uid(str+"U"+ to_string(i));
}

namespace text_output{
  ostream& operator<< (ostream& st, const Distribution& d){
    st << d.name << "(";
    for(const auto &e:d.Param)st << e << ", ";
    st << ")";
    return st;
  }

    ostream& operator<<(ostream& os, const coloredToken &tok){
        os << tok.mult << "<";
        for(size_t i=0; i< tok.field.size();i++ ){
            switch(tok.Flags[i]){
                case CT_SINGLE_COLOR: os << "Col_classe_" << tok.field[i] << ", ";continue;
                case CT_ALL: os << "ALL , ";continue;
                case CT_ALL_SUBCLASS: os << "StaticSubClass_" << tok.field[i] << ", ";continue;
                case CT_VARIABLE: os << "var_" << tok.field[i] << " + " <<tok.varIncrement[i] << ", "; continue;
                case CT_SINGLE_REAL: os << tok.field[i]; continue;
                default: os << "badly formed";
            }
        }
        os << ">";
        return os;
    }
    
    ostream& operator<<(ostream& os, const place& obj){
        os << "place " << obj.name << "(" << obj.id << "){" << endl;
        os << "\tmarking:" << obj.Marking;
        for(const auto &tok: obj.initMarking)os << tok << ",";
        os << endl;
        return os;
    }

    
    
  ostream& operator<< (ostream& st, const color& c){
    st << c.name;
    return st;
  }

  ostream& operator<< (ostream& st, const colorClass& cc){
    st << cc.name << "{\ncontent:{";
    for( const auto& c: cc.colors){
      st << c << ", ";
    }
    st << "}"<<endl;
    for( const auto& css: cc.staticSubclasses){
      st<< css.name << "{";
      for (const auto& c: css.colors){
	st << c << ", ";
      }
      st<< "}"<< endl;
    }
    st << "}"<< endl;
    return st;
  }
    
  
  ostream& operator<< (ostream& st, const transition& tr){
    st << "transition " << tr.name << "{" << endl;
    st << "\tid:\t" << tr.id << endl;
    st << "\tisTraced:\t" << tr.isTraced << endl;
    st << "\tdist:\t" << tr.dist << endl;
    st << "\tpriority:\t" << tr.priority << endl;
    st << "\tservice:\t" << tr.nbServers << endl;
    st << "\tweight:\t" << tr.weight << endl;

    st << "\tvarDomain:\t{";
    for(auto v: tr.varDomain)st << v << ", ";
    st << "}"<< endl;
    st << "\tguard:\t" << tr.guard << endl;
    st << "\tupdate:\t" << tr.update << endl;


    st << "}" << endl;
    return st;
  }

  ostream& operator<<(ostream& os, const userDefineDistribution& obj){
    os << "UserDefineDistribution " << obj.name << "{" << endl;
    os << "\tvar:" << obj.var << endl;
    os << "\tcdf:" << obj.cdf << endl;
    os << "\tpdf:" << obj.pdf << endl;
    os << "\tlowerBound:" << obj.lowerBound << endl << "}" << endl;
    return os;
  }

  ostream& operator<<(ostream& os, const arcStore& obj){
    for(const auto& arc:obj){
      os << "(" << GspnType::get_p(arc.first) << ")-- ";
      if(arc.second.isColored){
        for(const auto& tok: arc.second.coloredVal)
          os << tok;
      } else os << arc.second.exprVal;
      os << " --(" << GspnType::get_t(arc.first)<< ")"<< endl;
    }
    return os;
  }

  ostream& operator<< (ostream& st, const GspnType& gspn){
    st << "#### Internal Memory Model of GSPN #######" << endl;
    st << "colClasses{"<< endl;
    for(const auto& cc: gspn.colClasses) st << cc;
    st << "}\n" << endl;
    st << "transitionStruct{" << endl;
    for(const auto& tr: gspn.transitionStruct) st<< tr;
    st << "}\n";
    st << "placeStruct{" << endl;
    for(const auto& tr: gspn.placeStruct) st<< tr;
    st << "}\n";
    st << "inArcs{" << endl << gspn.inArcsStruct << "}"<<endl;
    st << "outArcs{" << endl << gspn.outArcsStruct << "}"<<endl;
    st << "inhibArcs{" << endl << gspn.inhibArcsStruct << "}"<<endl;
    st << "##########################################" << endl;
    return st;
  }
}
