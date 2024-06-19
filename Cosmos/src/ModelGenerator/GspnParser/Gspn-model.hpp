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

#ifndef __Cosmos__Gspn_model__
#define __Cosmos__Gspn_model__

#include <stdio.h>
#include <string>
#include <map>
#include <set>
#include <functional>
#include <unordered_map>
#include <vector>

#include "../expressionStruct.hpp"

//! A type to discriminate timed and untimmed transition.
enum TransType {
  Timed, unTimed
};

//! a structure for probability distribution
struct ProbabiliteDistribution {
  std::string name;
  std::vector<expr> Param;
  ProbabiliteDistribution(){};
  ProbabiliteDistribution(const expr &s):name("EXPONENTIAL"),Param(1,s){};
};
typedef struct ProbabiliteDistribution Distribution;

struct userDefineDistribution{
  static std::string polyfile;
  static size_t nbparam;
  std::string name;
  std::string var;
  std::string cdf;
  std::string pdf;
  std::string norm;
  std::string lowerBound;
  std::string upperBound;
};

struct color {
  std::string name;
  size_t id;
  size_t cc;
  color(){};
  color(const std::string s,size_t i,size_t c){ name =s;id = i; cc=c;}
};

enum ColorClassType {
    Asymmetric, Cyclic, Continuous
};
struct colorStaticSubclass {
    std::string name;
    size_t id;
    std::vector<color> colors;
};
struct colorClass {
    std::string name;
    bool isCircular;
    ColorClassType type;
    std::vector<color> colors;
    std::vector<colorStaticSubclass> staticSubclasses;
    std::string intIntervalName;
    inline std::string cname()const {
        return name+ "_Color_Classe";
    }
    inline unsigned int size()const {
        return colors.size();
    }
};


#define UNCOLORED_DOMAIN (0)

struct colorDomain {
    std::string name;
    std::vector<size_t> colorClassIndex;
    bool isBounded;
    inline bool isUncolored()const {
        return name.size() ==0;
    }
    inline std::string cname()const {
        if(isUncolored())return "int";
        return name+ "_Domain";
    }
    inline std::string ccname()const {
        if(isUncolored())return "int";
        if (colorClassIndex.size()==1) return name+"_Color_Classe";
        else {
            std::cerr << "The function ccname only apply to domains with exactly one color, to get the corresponding color class name. An error may occur." << std::endl;
            return name+"_Color_Classe";
        }
    }
    inline std::string tokname()const {
        if(isUncolored())return " ";
        return name+ "_Token";
    }

};

struct colorVariable {
  std::string name;
  size_t type;
};

enum varType {
    CT_SINGLE_COLOR,
    CT_SINGLE_REAL,
    CT_VARIABLE,
    CT_ALL,
    CT_ALL_SUBCLASS,
    CV_CLOCK,
    CV_REAL,
    CV_INT
};

struct hybridVariable {
  std::string name;
  varType type;
  bool isTraced;
  std::string initialValue;
  hybridVariable():type(CV_CLOCK),isTraced(true) {}
};

struct coloredToken {
    expr mult;
    std::shared_ptr<colorDomain> coldom;
    std::vector<expr> field;
    std::vector<varType> Flags;
    std::vector<int> varIncrement;
    bool hasAll;
    coloredToken():hasAll(false){};
    coloredToken(expr st):mult(st),hasAll(false){};
    coloredToken(int i):mult(i),hasAll(false){};

    std::set<size_t> varSet()const;
    friend bool operator< (const coloredToken& lhs, const coloredToken& rhs);
    friend bool operator== (const coloredToken& lhs, const coloredToken& rhs);
};

struct transition {
  size_t id;
  std::string name;
  bool isTraced;
  TransType type;
  Distribution dist;
  expr priority;
  expr weight;
  bool singleService;
  bool markingDependant;
  int nbServers;
  bool ageMemory;
  std::set<size_t> varDomain;
  expr guard;
  std::string update;
  transition():isTraced(true),guard(true){}
  transition(size_t i,const std::string &n,const expr &p,bool md):id(i),name(n),isTraced(true),
                                 type(Timed),dist(p),priority(expr(1)),weight(expr(1.0)),singleService(true),
                                 markingDependant(md),nbServers(1),ageMemory(false),guard(true){};
};

struct InterfaceSRG {
  bool isInterface;
  unsigned int index;
  InterfaceSRG():isInterface(false),index(0){};
};

struct place {
  size_t id;
  std::string name;
  bool isTraced;
  size_t colorDom;
  std::vector<coloredToken> initMarking;
  expr Marking;
  InterfaceSRG isrg;
  place(){ colorDom = UNCOLORED_DOMAIN; isTraced = true; }
  bool operator < (const place &p)const{ return id < p.id ;}
  bool operator > (const place &p)const{ return id > p.id ;}
  bool operator == (const place &p)const{ return id == p.id ;}
};

struct arc {
    arc(const expr& val):isEmpty(false),isMarkDep(val.is_markDep()),isColored(false),exprVal(val){};
    arc(size_t v):isEmpty(false),isMarkDep(false),isColored(false),exprVal((int)v){};
    arc():isEmpty(true),isMarkDep(false),isColored(false),exprVal(0){};
    arc(const expr &val,const std::vector<coloredToken> &vct):isEmpty(false),isMarkDep(val.is_markDep()),exprVal(val),coloredVal(vct){
        isColored = val.t == ColouredPlaceHolder ;
    }
    bool isEmpty;
    bool isMarkDep;
    bool isColored;
    expr exprVal;
/*    size_t intVal;
    std::string stringVal;*/
    std::vector<coloredToken> coloredVal;

    bool containsVar(size_t v) const {
        if (isEmpty) { return false; }
        if (not isColored) { return false; }
        for (const auto& tok : coloredVal) {
            for (size_t i = 0; i < tok.field.size();i++) {
                if (tok.Flags[i] == CT_VARIABLE and (size_t)tok.field[i].intVal == v) { return true; }
            }
        }
        return false;
    }
};

//first is transition, second is place.
struct classcomp {
  bool operator() (const std::pair<size_t,size_t> &lhs,const std::pair<size_t,size_t> &rhs) const
  {
    if(lhs.first == rhs.first){
      return lhs.second< rhs.second;
    }
    else return lhs.first<rhs.first;
  }
};

//maps place,transition to an arc
typedef std::map< std::pair<size_t,size_t>, arc, classcomp>  arcStore;

struct GspnType {
  size_t tr;
  size_t pl;
  size_t nbpass;

  std::set<std::string> TransList;
  std::vector<colorClass> colClasses;
  std::vector<colorDomain> colDoms;
  std::vector<colorVariable> colVars;
  std::vector<hybridVariable> hybridVars;

  std::map<std::string, int> PlacesId;
  std::map<std::string, int> TransId;

  std::vector<transition> transitionStruct;
  std::vector<place> placeStruct;
  std::vector<userDefineDistribution> distribStruct;

  inline static std::pair<size_t, size_t> arckey(size_t t,size_t p) { return std::make_pair(t,p); };
  inline static size_t get_t(std::pair<size_t, size_t> key){return key.first;};
  inline static size_t get_p(std::pair<size_t, size_t> key){return key.second;};
  inline static const arc access(const arcStore &h,size_t t, size_t p){
    static const arc emptyarc;
    auto a = h.find(arckey(t,p));
    if(a == h.end()){return emptyarc;}else return a->second;
  };
  arcStore inArcsStruct;
  arcStore outArcsStruct;
  arcStore inhibArcsStruct;

  std::map <std::string, int> IntConstant;
  std::map <std::string, double> RealConstant;
  std::set <std::string> ExternalConstant;

  inline bool isColored()const{
    return colClasses.size()>0;
  }

  GspnType(){
    nbpass=0;
    colDoms.push_back(colorDomain());
  }

  void iterateDom(const std::string &s,const std::string &sop, const std::string &sclos ,const std::string &s2,const std::string &sop2, const std::string &sclos2 ,const colorDomain & cd, size_t prof,std::function<void (const std::string&,const std::string&)> func);
  void iterateDomVec(std::vector<color> &v, const colorDomain & cd, size_t prof,std::function<void (const std::vector<color>&)> func);


  void iterateVars(const std::string &s,const std::string &sop, const std::string &sclos ,const std::set<size_t> &varDom, size_t prof,std::function<void (const std::string&)> func);
  void iterateVars(std::vector<color> &v, const std::set<size_t> &varDom, size_t prof,std::function<void (const std::vector<color>&)> func);

  bool isInVar(size_t var, size_t tr)const;
  bool isInVar(const coloredToken &var, size_t tr)const;

  //Build a set of all token shape. The mapped value is a pair of an id and the domain.
  std::map<coloredToken, std::pair<size_t,size_t>> computeMacroVar()const;

protected:
  std::map<std::string,size_t> uid;
public:
  size_t get_uid(const std::string &str);
  size_t new_uid(const std::string &str);
};
void searchreplace(const std::string &in,const std::string &motif,const std::string &rep,std::string &out);

namespace text_output {
  std::ostream& operator<<(std::ostream& os, const color& obj);
  std::ostream& operator<<(std::ostream& os, const colorClass& obj);
  std::ostream& operator<<(std::ostream& os, const ProbabiliteDistribution& obj);
  std::ostream& operator<<(std::ostream& os, const userDefineDistribution& obj);
  std::ostream& operator<<(std::ostream& os, const transition& obj);
  std::ostream& operator<<(std::ostream& os, const coloredToken& tok);
  std::ostream& operator<<(std::ostream& os, const place& obj);
  std::ostream& operator<<(std::ostream& st, const arcStore& as);
  std::ostream& operator<<(std::ostream& st, const GspnType& tr);
}


#endif /* defined(__Cosmos__Gspn_model__) */
